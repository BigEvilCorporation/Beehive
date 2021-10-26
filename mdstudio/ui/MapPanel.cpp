///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#include "MapPanel.h"
#include "MainWindow.h"
#include "SpriteAnimEditorDialog.h"
#include "EditStampCollisionDialog.h"

#include <wx/Menu.h>
#include <wx/msgdlg.h>

#include <ion/maths/Bounds.h>
#include <ion/core/utils/STL.h>

#if defined BEEHIVE_PLUGIN_LUMINARY
#include <luminary/BeehiveToLuminary.h>
#endif

MapPanel::MapPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, wxGLAttributes& glAttributes, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, project, renderer, glContext, glAttributes, renderResources, parent, winid, pos, size, style, name)
	, m_gizmo(renderResources)
{
	m_currentTool = eToolSelectTiles;
	m_cursorOrigin = eCursorTopLeft;
	m_tempStamp = NULL;
	m_stampPreviewPrimitive = NULL;
	m_primitiveBezierPoints = NULL;
	m_primitiveBezierHandles = NULL;
	m_highlightedBezier = NULL;
	m_currentBezier = NULL;
	m_currentBezierControlIdx = -1;
	m_stampPastePos.x = -1;
	m_stampPastePos.y = -1;
	m_previewGameObjectType = InvalidGameObjectTypeId;
	m_moveGameObjByPixel = false;
	m_boxSelection = false;
	m_editingPrefabInstanceId = InvalidGameObjectId;

	m_cursorHorizontal = new wxCursor(wxCURSOR_SIZEWE);
	m_cursorVertical = new wxCursor(wxCURSOR_SIZENS);

	ResetToolData();

	Map& map = project.GetEditingMap();
	CollisionMap& collisionMap = project.GetEditingCollisionMap();
	Tileset& tileset = project.GetTileset();
	int mapWidth = map.GetWidth();
	int mapHeight = map.GetHeight();

	//Create canvas
	CreateCanvas(mapWidth, mapHeight);
	CreateCollisionCanvas(collisionMap.GetWidth(), collisionMap.GetHeight());

	//Create grid
	CreateGrid(mapWidth, mapHeight, mapWidth / project.GetGridSize(), mapHeight / project.GetGridSize());

	if (!mainWindow->IsRefreshLocked())
	{
		//Redraw map
		PaintMap(project.GetEditingMap());

		//Redraw collision map
		PaintCollisionMap(project.GetEditingMap(), project.GetEditingCollisionMap());
	}
}

MapPanel::~MapPanel()
{
	delete m_cursorHorizontal;
	delete m_cursorVertical;

	ResetToolData();
}

void MapPanel::OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta)
{
	ViewPanel::OnMouse(event, mouseDelta);
}

void MapPanel::OnKeyboard(wxKeyEvent& event)
{
	if(event.GetKeyCode() == WXK_ESCAPE)
	{
		ResetToolData();
		m_currentTool = eToolSelectTiles;
		Refresh();
	}

	if (m_currentTool == eToolPaintStamp)
	{
		if(m_previewTileFlipX != event.ShiftDown())
		{
			//SHIFT held, set H flip and refresh preview tile
			m_previewTileFlipX = event.ShiftDown();
			Refresh();
		}

		if(m_previewTileFlipY != event.ControlDown())
		{
			//CTRL held, set V flip and refresh preview tile
			m_previewTileFlipY = event.ControlDown();
			Refresh();
		}
	}

	if(m_currentTool == eToolSelectGameObject
		|| m_currentTool == eToolPlaceGameObject
		|| m_currentTool == eToolDrawGameObject)
	{
		m_moveGameObjByPixel = event.ShiftDown();
	}

#if !BEEHIVE_FIXED_STAMP_MODE //Fixed grid placement only in fixed mode
	if(m_currentTool == eToolSelectTiles)
	{
		//Store CTRL held state for multiple selection
		m_multipleSelection = event.ControlDown();
	}
#endif

	ViewPanel::OnKeyboard(event);
}

void MapPanel::OnResize(wxSizeEvent& event)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		ViewPanel::OnResize(event);
	}
}

void MapPanel::BucketFill(Map& map, ion::Vector2i position, ion::Vector2i prevPosition, TileId originalTile, TileId newTile)
{
	std::vector<ion::Vector2i> fillQueue;

	//Best guess at max queue size
	fillQueue.reserve((map.GetWidth() * map.GetHeight()) / 8);

	//Add this node to queue
	fillQueue.push_back(position);

	for(int i = 0; i < fillQueue.size(); i++)
	{
		//Stop if already been here
		if(fillQueue[i] == prevPosition)
		{
			continue;
		}

		//Get tile at position
		TileId currentTile = map.GetTile(fillQueue[i].x, fillQueue[i].y);

		//Stop if original tiles differ
		if(currentTile != originalTile)
		{
			continue;
		}

		//Stop if stamp
		ion::Vector2i stampPos;
		u32 stampFlags = 0;
		u32 mapEntryIndex = 0;
		StampId stampId = map.FindStamp(fillQueue[i].x, fillQueue[i].y, stampPos, stampFlags, mapEntryIndex);

		if(stampId)
		{
			continue;
		}

		//Set this tile
		map.SetTile(fillQueue[i].x, fillQueue[i].y, newTile);

		//Paint to canvas
		PaintTile(newTile, fillQueue[i].x, map.GetHeight() - 1 - fillQueue[i].y, 0);

		//Recurse up
		if(fillQueue[i].y > 0)
		{
			fillQueue.push_back(ion::Vector2i(fillQueue[i].x, fillQueue[i].y - 1));
		}

		//Recurse down
		if(fillQueue[i].y < (map.GetHeight() - 1))
		{
			fillQueue.push_back(ion::Vector2i(fillQueue[i].x, fillQueue[i].y + 1));
		}

		//Recurse left
		if(fillQueue[i].x > 0)
		{
			fillQueue.push_back(ion::Vector2i(fillQueue[i].x - 1, fillQueue[i].y));
		}

		//Recurse right
		if(fillQueue[i].x < (map.GetWidth() - 1))
		{
			fillQueue.push_back(ion::Vector2i(fillQueue[i].x + 1, fillQueue[i].y));
		}

		prevPosition = fillQueue[i];
	}
}

void MapPanel::OnMouseTileEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int x, int y)
{
	Map& map = m_project.GetEditingMap();
	Tileset& tileset = m_project.GetTileset();

	const int mapWidth = map.GetWidth();
	const int mapHeight = map.GetHeight();
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	//Invert for OpenGL
	int y_inv = (mapHeight - 1 - y);

	//Check in map range
	bool inMapRange = ((x >= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight));

	//if (inMaprange)
	//{
	//	std::stringstream tipStr;
	//	tipStr << "Cursor: " << x << ", " << y << std::endl;
	//	SetToolTip(tipStr.str().c_str());
	//}
	//else
	//{
	//	UnsetToolTip();
	//}

	switch(m_currentTool)
	{
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
		case eToolPaintTile:
		{
			if(inMapRange)
			{
				//Cannot paint over a stamp
				ion::Vector2i stampTopLeft;
				u32 stampFlags = 0;
				u32 mapEntryIndex = 0;
				if(map.FindStamp(x, y, stampTopLeft, stampFlags, mapEntryIndex) == InvalidStampId)
				{
					//Update paint preview tile
					m_previewTile = m_project.GetPaintTile();

					//Update preview tile pos
					m_previewTilePos.x = x;
					m_previewTilePos.y = y;

					//If clicking/dragging, paint tile
					if(buttonBits & eMouseLeft)
					{
						//Get tile ID to paint
						TileId tileId = InvalidTileId;
						if(buttonBits & eMouseLeft)
							tileId = m_project.GetPaintTile();
						else if(buttonBits & eMouseRight)
							tileId = m_project.GetEraseTile();

						if(tileId != InvalidTileId)
						{
							//Set on map
							map.SetTile(x, y, tileId);

							//Set V/H flip flags
							u32 tileFlags = 0;
							if(m_previewTileFlipX)
								tileFlags |= Map::eFlipX;
							if(m_previewTileFlipY)
								tileFlags |= Map::eFlipY;

							//Invert Y from OpenGL back to map space
							map.SetTileFlags(x, y, tileFlags);

							//Paint to canvas
							PaintTile(tileId, x, y_inv, tileFlags);
						}
					}
				}
				else
				{
					m_previewTile = InvalidTileId;
				}
			}
				
			break;
		}

		case eToolFill:
		{
			if(buttonBits & eMouseLeft)
			{
				if(inMapRange)
				{
					if(map.GetTile(x, y) != m_project.GetPaintTile())
					{
						BucketFill(map, ion::Vector2i(x, y), ion::Vector2i(-1, -1), map.GetTile(x, y), m_project.GetPaintTile());
					}
				}
			}
			break;
		}

		case eToolPaintTerrainTile:
		case eToolDeleteTerrainTile:
		{
			if(inMapRange)
			{
				//If clicking/dragging, paint collision tile
				if(buttonBits & eMouseLeft)
				{
					//Get tile ID to paint
					TerrainTileId tileId = (m_currentTool == eToolPaintTerrainTile) ? m_project.GetPaintTerrainTile() : InvalidTerrainTileId;

					//Get collision map
					CollisionMap& collisionMap = m_project.GetEditingCollisionMap();

					//Set on map
					collisionMap.SetTerrainTile(x, y, tileId);

					//Paint to canvas
					PaintCollisionTile(tileId, x, y_inv, collisionMap.GetCollisionTileFlags(x, y));
				}
			}

			break;
		}

		case eToolPaintCollisionSolid:
		{
			if(inMapRange)
			{
				//If clicking/dragging, paint solid collision tile
				if(buttonBits & eMouseLeft)
				{
					//Get collision map
					CollisionMap& collisionMap = m_project.GetEditingCollisionMap();

					//Set solid tile
					u16 collisionTileFlags = collisionMap.GetCollisionTileFlags(x, y) | eCollisionTileFlagSolid;
					collisionMap.SetCollisionTileFlags(x, y, collisionTileFlags);

					//Paint to canvas
					PaintCollisionTile(collisionMap.GetTerrainTile(x, y), x, y_inv, collisionTileFlags);
				}
				else if(buttonBits & eMouseRight)
				{
					//Get collision map
					CollisionMap& collisionMap = m_project.GetEditingCollisionMap();

					//Clear solid tile
					u16 collisionTileFlags = collisionMap.GetCollisionTileFlags(x, y) & ~eCollisionTileFlagSolid;
					collisionMap.SetCollisionTileFlags(x, y, collisionTileFlags);

					//Paint to canvas
					PaintCollisionTile(collisionMap.GetTerrainTile(x, y), x, y_inv, collisionTileFlags);
				}
			}

			break;
		}

		case eToolPaintCollisionHole:
		{
			if(inMapRange)
			{
				//If clicking/dragging, paint collision hole
				if(buttonBits & eMouseLeft)
				{
					//Get collision map
					CollisionMap& collisionMap = m_project.GetEditingCollisionMap();

					//Set hole tile
					u16 collisionTileFlags = collisionMap.GetCollisionTileFlags(x, y) | eCollisionTileFlagHole;
					collisionMap.SetCollisionTileFlags(x, y, collisionTileFlags);

					//Paint to canvas
					PaintCollisionTile(collisionMap.GetTerrainTile(x, y), x, y_inv, collisionTileFlags);
				}
				else if(buttonBits & eMouseRight)
				{
					//Get collision map
					CollisionMap& collisionMap = m_project.GetEditingCollisionMap();

					//Clear hole tile
					u16 collisionTileFlags = collisionMap.GetCollisionTileFlags(x, y) & ~eCollisionTileFlagHole;
					collisionMap.SetCollisionTileFlags(x, y, collisionTileFlags);

					//Paint to canvas
					PaintCollisionTile(collisionMap.GetTerrainTile(x, y), x, y_inv, collisionTileFlags);
				}
			}

			break;
		}
#endif

		case eToolSelectTiles:
		{
			if(inMapRange)
			{
				if(buttonBits & eMouseLeft)
				{
					if(!(m_prevMouseBits & eMouseLeft))
					{
						//Single left-click, pick tiles one by one
						if(m_boxSelectEnd.x > 0 && m_boxSelectEnd.y > 0)
						{
							//Previous selection was box selection, clear it
							m_selectedTiles.clear();

							//Reset box end
							m_boxSelectEnd.x = -1;
							m_boxSelectEnd.y = -1;
						}

						//If CTRL not held
						if(!m_multipleSelection)
						{
							//Clear selection, start again
							m_selectedTiles.clear();

							//eStart box selection, in case next event is dragging
							m_boxSelectStart.x = x * tileWidth;
							m_boxSelectStart.y = y * tileHeight;
						}

						//Single click - add tile at cursor to selection
						m_selectedTiles.push_back(ion::Vector2i(x, y));
					}
					else if(!m_multipleSelection)
					{
						//Dragging, set box end
						m_boxSelectEnd.x = x * tileWidth;
						m_boxSelectEnd.y = y * tileHeight;

						//Clear current selection
						m_selectedTiles.clear();

						//Sanitise loop order
						int top = ion::maths::Min(m_boxSelectStart.y * tileHeight, m_boxSelectEnd.y * tileHeight);
						int left = ion::maths::Min(m_boxSelectStart.x * tileWidth, m_boxSelectEnd.x * tileWidth);
						int bottom = ion::maths::Max(m_boxSelectStart.y * tileHeight, m_boxSelectEnd.y * tileHeight);
						int right = ion::maths::Max(m_boxSelectStart.x * tileWidth, m_boxSelectEnd.x * tileWidth);

						//Add all tiles in box
						for(int tileX = left; tileX <= right; tileX++)
						{
							for(int tileY = top; tileY <= bottom; tileY++)
							{
								m_selectedTiles.push_back(ion::Vector2i(tileX, tileY));
							}
						}
					}

					//Refresh to draw box selection
					Refresh();
				}
			}
			break;
		}

		case eToolSelectStamp:
		case eToolStampPicker:
		case eToolRemoveStamp:
		{
#if BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
			if (inMapRange)
#endif
			{
				//Find stamp under cursor
				ion::Vector2i stampPos;
				u32 stampFlags = 0;
				u32 mapEntryIndex = 0;
				StampId stampId = map.FindStamp(x, y, stampPos, stampFlags, mapEntryIndex);

				m_hoverStamp = stampId;
				m_hoverStampPos = stampPos;
				m_hoverStampFlags = stampFlags;

				if (buttonBits == 0)
				{
					if (const Stamp* stamp = m_project.GetStamp(stampId))
					{
						if (x >= 0 && y >= 0)
						{
							int stampX = x % stamp->GetWidth();
							int stampY = y % stamp->GetHeight();
							TileId tileId = stamp->GetTile(stampX, stampY);

#if BEEHIVE_FIXED_STAMP_MODE
							TerrainTileId terrainTileId = stamp->GetTerrainTile(stampX, stampY);
							u16 collisionFlags = stamp->GetCollisionTileFlags(stampX, stampY);
#else
							TerrainTileId terrainTileId = inMapRange ? m_project.GetEditingCollisionMap().GetTerrainTile(x, y) : InvalidTerrainTileId;
							u16 collisionFlags = inMapRange ? m_project.GetEditingCollisionMap().GetCollisionTileFlags(x, y) : 0;
#endif

							u8 terrainAngleByte = 0;
							ion::Vector2 terrainNormal;
							if (const TerrainTile* terrainTile = m_project.GetTerrainTileset().GetTerrainTile(terrainTileId))
							{
								terrainAngleByte = terrainTile->GetAngleByte();
								terrainNormal = terrainTile->GetNormal();
							}

							std::stringstream tipStr;
							tipStr << "Cursor: 0x" << SSTREAM_HEX4(x) << ", 0x" << SSTREAM_HEX4(y) << " (" << x << ", " << y << ")" << std::endl;
							tipStr << "Stamp: 0x" << SSTREAM_HEX4(stampId) << " (" << stampId << ")" << std::endl;
							tipStr << "Tile: 0x" << SSTREAM_HEX4(tileId) << " (" << tileId << ")" << std::endl;
							tipStr << "Terrain: 0x" << SSTREAM_HEX4(terrainTileId) << " (" << terrainTileId << ")" << " collisionFlags: 0x" << SSTREAM_HEX2(collisionFlags) << " angleByte: 0x" << SSTREAM_HEX2(terrainAngleByte) << " normal: " << terrainNormal.x << "," << terrainNormal.y << std::endl;
							tipStr << "Stamp pos: 0x" << SSTREAM_HEX4(stampPos.x) << ", 0x" << SSTREAM_HEX4(stampPos.y) << " (" << stampPos.x << ", " << stampPos.y << ")" << std::endl;
							tipStr << "Size: " << stamp->GetWidth() << ", " << stamp->GetHeight() << std::endl;
							tipStr << "Addr: 0x" << SSTREAM_HEX8(stampId * tileWidth * tileHeight * 4) << std::endl;
							SetToolTip(tipStr.str().c_str());
						}
					}
					else
					{
						UnsetToolTip();
					}
				}

				if (buttonBits & eMouseLeft && !(m_prevMouseBits & eMouseLeft))
				{
					m_selectedStamp = m_hoverStamp;

					if (m_currentTool == eToolSelectStamp)
					{
						//Reset selection box
						m_boxSelectStart.x = -1;
						m_boxSelectStart.y = -1;
						m_boxSelectEnd.x = -1;
						m_boxSelectEnd.y = -1;

						if (Stamp* stamp = m_project.GetStamp(stampId))
						{
							//Set box selection
							m_boxSelectStart.x = stampPos.x * tileWidth;
							m_boxSelectStart.y = stampPos.y * tileHeight;
							m_boxSelectEnd.x = stampPos.x + ((stamp->GetWidth() - 1) * tileWidth);
							m_boxSelectEnd.y = stampPos.y + ((stamp->GetHeight() - 1) * tileHeight);
						}
					}

					if (m_currentTool == eToolStampPicker)
					{
						//Set as paint stamp
						m_project.SetPaintStamp(stampId);

						//Set stamp tool
						SetTool(eToolPaintStamp);

						//TODO: Update tileset panel selection + toolbox button state
					}

					if (m_currentTool == eToolRemoveStamp)
					{
						if (Stamp* stamp = m_project.GetStamp(stampId))
						{
							//Remove stamp under cursor
							map.RemoveStamp(stampId, x, y);

							//Clear hover stamp
							m_hoverStamp = InvalidStampId;

							//Repaint area underneath stamp
							for (int tileX = stampPos.x; tileX < stampPos.x + stamp->GetWidth(); tileX++)
							{
								for (int tileY = stampPos.y; tileY < stampPos.y + stamp->GetHeight(); tileY++)
								{
									//Invert Y for OpenGL
									int y_inv = map.GetHeight() - 1 - tileY;

									//Stamps can be placed outside map boundaries, only remove tiles that are inside
									if (tileX >= 0 && tileX < mapWidth && y_inv >= 0 && y_inv < mapHeight)
									{
										TileId tileId = InvalidTileId;

										//Find stamp under cursor first
										ion::Vector2i stampPos;
										u32 flags = 0;
										u32 mapEntryIndex = 0;
										StampId stampId = map.FindStamp(tileX, tileY, stampPos, flags, mapEntryIndex);

										if (stampId)
										{
											//Get from stamp
											if (Stamp* stamp = m_project.GetStamp(stampId))
											{
												ion::Vector2i offset = ion::Vector2i(tileX, tileY) - stampPos;

												int sourceX = (flags & Map::eFlipX) ? (stamp->GetWidth() - 1 - offset.x) : offset.x;
												int sourceY = (flags & Map::eFlipY) ? (stamp->GetHeight() - 1 - offset.y) : offset.y;

												tileId = stamp->GetTile(sourceX, sourceY);
												flags ^= stamp->GetTileFlags(sourceX, sourceY);
											}
										}
										else
										{
											//Pick tile
											tileId = map.GetTile(tileX, tileY);
											flags = map.GetTileFlags(tileX, tileY);
										}

										PaintTile(tileId, tileX, y_inv, flags);
									}
								}
							}
						}
					}
				}

				if (buttonBits & eMouseRight)
				{
					if (m_hoverStamp != InvalidStampId)
					{
						//Right-click menu
						wxMenu contextMenu;

						contextMenu.Append(eContextMenuDeleteStamp, wxString("Delete stamp"));
						contextMenu.Append(eContextMenuEditStampCollision, wxString("Edit stamp collision"));
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
						contextMenu.Append(eContextMenuBakeStamp, wxString("Bake stamp"));
						contextMenu.Append(eContextMenuStampBringToFront, wxString("Bring to front"));
						contextMenu.Append(eContextMenuStampSendToBack, wxString("Send to back"));
#endif
						contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MapPanel::OnContextMenuClick, NULL, this);
						PopupMenu(&contextMenu);
					}
				}

				Refresh();
			}
		}
		break;

		case eToolTilePicker:
		{
			if(inMapRange)
			{
				if(buttonBits & eMouseLeft)
				{
					TileId tileId = InvalidTileId;

					//Find stamp under cursor first
					ion::Vector2i stampPos;
					u32 stampFlags = 0;
					u32 mapEntryIndex = 0;
					StampId stampId = map.FindStamp(x, y, stampPos, stampFlags, mapEntryIndex);

					if(stampId)
					{
						//Get from stamp
						if(Stamp* stamp = m_project.GetStamp(stampId))
						{
							ion::Vector2i offset = ion::Vector2i(x, y) - stampPos;
							tileId = stamp->GetTile(offset.x, offset.y);
						}
					}
					else
					{
						//Pick tile
						tileId = map.GetTile(x, y);
					}

					//Set as paint tile
					m_project.SetPaintTile(tileId);

					//Set as preview tile
					m_previewTile = tileId;

#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
					//Set paint tool
					SetTool(eToolPaintTile);
#endif

					//Refresh tile edit panel
					m_mainWindow->RedrawPanel(MainWindow::ePanelTileEditor);

					//Refresh collision tile edit panel
					m_mainWindow->RedrawPanel(MainWindow::ePanelTerrainTileEditor);
				}
			}

			//TODO: Update tileset panel selection + toolbox button state
			break;
		}

#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
		case eToolFlipX:
		{
			if(inMapRange)
			{
				//Flip preview opposite of current tile
				m_previewTile = map.GetTile(x, y);
				m_previewTilePos.x = x;
				m_previewTilePos.y = y;
				m_previewTileFlipX = (map.GetTileFlags(x, y) & Map::eFlipX) == 0;

				if(buttonBits & eMouseLeft)
				{
					//Flip tile X
					u32 tileFlags = map.GetTileFlags(x, y);
					tileFlags ^= Map::eFlipX;
					map.SetTileFlags(x, y, tileFlags);

					//Redraw on canvas
					TileId tileId = map.GetTile(x, y);
					PaintTile(tileId, x, y_inv, tileFlags);
					Refresh();
				}
			}

			break;
		}

		case eToolFlipY:
		{
			if(inMapRange)
			{
				//Flip preview opposite of current tile
				m_previewTile = map.GetTile(x, y);
				m_previewTilePos.x = x;
				m_previewTilePos.y = y;
				m_previewTileFlipY = (map.GetTileFlags(x, y) & Map::eFlipY) == 0;

				if(buttonBits & eMouseLeft)
				{
					//Flip tile Y
					u32 tileFlags = map.GetTileFlags(x, y);
					tileFlags ^= Map::eFlipY;
					map.SetTileFlags(x, y, tileFlags);

					//Redraw on canvas
					TileId tileId = map.GetTile(x, y);
					PaintTile(tileId, x, y_inv, tileFlags);
					Refresh();
				}
			}
			break;
		}
#endif

		case eToolPaintStamp:
		{
#if BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
			if (inMapRange)
#endif
			{
				//Paint temp cloning stamp, else Paint current paint stamp
				Stamp* stamp = m_tempStamp ? m_tempStamp : m_project.GetStamp(m_project.GetPaintStamp());
				if (stamp)
				{
					if ((buttonBits & eMouseRight) && !(m_prevMouseBits & eMouseRight))
					{
						//Switch cursor origin
						int cursorOrigin = (int)m_cursorOrigin;
						m_cursorOrigin = (CursorOrigin)(++cursorOrigin % eCursor_MAX);
					}

					//If fixed stamp size, snap to grid
					int fixedStampWidth = m_project.GetPlatformConfig().stampWidth;
					int fixedStampHeight = m_project.GetPlatformConfig().stampHeight;

					if (fixedStampWidth > 0)
					{
						x = ion::maths::RoundDownToNearest(x, fixedStampWidth);
					}

					if (fixedStampHeight > 0)
					{
						y = ion::maths::RoundDownToNearest(y, fixedStampHeight);
					}

					//Update paste pos
					m_stampPastePos.x = x;
					m_stampPastePos.y = y;

					//Adjust for cursor origin
					switch (m_cursorOrigin)
					{
					case eCursorCentred:
						m_stampPastePos.x -= stamp->GetWidth() / 2;
						m_stampPastePos.y -= stamp->GetHeight() / 2;
						break;
					case eCursorTopRight:
						m_stampPastePos.x -= stamp->GetWidth();
						break;
					case eCursorBottomLeft:
						m_stampPastePos.y -= stamp->GetHeight();
						break;
					case eCursorBottomRight:
						m_stampPastePos.x -= stamp->GetWidth();
						m_stampPastePos.y -= stamp->GetHeight();
						break;
					default:
						break;
					}

					//Redraw
					Refresh();

					if ((buttonBits & eMouseLeft) && !(m_prevMouseBits & eMouseLeft))
					{
						u32 flipFlags = (m_previewTileFlipX ? Map::eFlipX : 0) | (m_previewTileFlipY ? Map::eFlipY : 0);

						//Set on map
						map.SetStamp(m_stampPastePos.x, m_stampPastePos.y, *stamp, flipFlags);

						//Paint on canvas
						PaintStamp(*stamp, m_stampPastePos.x, m_stampPastePos.y, flipFlags);
					}
				}
				break;
			}
		}

		case eToolPlaceGameObject:
		{
			m_previewGameObjectType = m_project.GetPaintGameObjectType();
			m_previewGameObjectArchetype = m_project.GetPaintGameObjectArchetype();
			m_previewGameObjectPos.x = x;
			m_previewGameObjectPos.y = y;

			if((buttonBits & eMouseLeft) && !(m_prevMouseBits & eMouseLeft))
			{
				GameObjectTypeId gameObjectTypeId = m_project.GetPaintGameObjectType();
				if(GameObjectType* gameObjectType = m_project.GetGameObjectType(gameObjectTypeId))
				{
					m_project.GetEditingMap().PlaceGameObject(x, y, *gameObjectType, m_previewGameObjectArchetype);
					m_mainWindow->RedrawPanel(MainWindow::ePanelGameObjectTypes);
					m_mainWindow->RefreshPanel(MainWindow::ePanelSceneExplorer);
					Refresh();
				}
			}

			break;
		}

		case eToolDrawGameObject:
		{
			if(buttonBits & eMouseLeft)
			{
				if(m_boxSelectStart.x == -1)
				{
					//Mouse first down, take start pos
					m_boxSelectStart.x = x * tileWidth;
					m_boxSelectStart.y = y * tileHeight;
				}
				else
				{
					//Start pos already taken, take end pos
					m_boxSelectEnd.x = x * tileWidth;
					m_boxSelectEnd.y = y;
				}
			}
			else
			{
				if(m_boxSelectStart.x != -1 && m_boxSelectEnd.x != -1)
				{
					//Mouse up, box drawn
					GameObjectTypeId gameObjectTypeId = m_project.GetPaintGameObjectType();
					if(GameObjectType* gameObjectType = m_project.GetGameObjectType(gameObjectTypeId))
					{
						int boxX = ion::maths::Min(m_boxSelectStart.x * tileWidth, m_boxSelectEnd.x * tileWidth);
						int boxY = ion::maths::Min(m_boxSelectStart.y * tileHeight, m_boxSelectEnd.y * tileHeight);
						int boxWidth = ion::maths::Abs(m_boxSelectEnd.x - m_boxSelectStart.x) + 1;
						int boxHeight = ion::maths::Abs(m_boxSelectEnd.y - m_boxSelectStart.y) + 1;

						m_project.GetEditingMap().PlaceGameObject(boxX, boxY, boxWidth, boxHeight, *gameObjectType, m_project.GetPaintGameObjectArchetype());
						m_mainWindow->RefreshPanel(MainWindow::ePanelSceneExplorer);
						Refresh();
					}
				}

				//Reset box
				m_boxSelectStart.x = -1;
				m_boxSelectStart.y = -1;
				m_boxSelectEnd.x = -1;
				m_boxSelectEnd.y = -1;
			}

			break;
		}
	}

	if(!inMapRange && m_currentTool != eToolPaintStamp)
	{
		//Mouse of of map range, invalidate preview tile
		m_previewTile = InvalidTileId;
		m_stampPastePos.x = -1;
		m_stampPastePos.y = -1;
	}

	//Refresh
	Refresh();
}

void MapPanel::OnContextMenuClick(wxCommandEvent& event)
{
	Map& map = m_project.GetEditingMap();

	if(event.GetId() == eContextMenuDeleteStamp)
	{
		if(Stamp* stamp = m_project.GetStamp(m_hoverStamp))
		{
			//Remove stamp under cursor
			map.RemoveStamp(m_hoverStamp, m_hoverStampPos.x, m_hoverStampPos.y);

			//Clear hover stamp
			m_hoverStamp = InvalidStampId;

			//Repaint area underneath stamp
			for(int tileX = m_hoverStampPos.x; tileX < m_hoverStampPos.x + stamp->GetWidth(); tileX++)
			{
				for(int tileY = m_hoverStampPos.y; tileY < m_hoverStampPos.y + stamp->GetHeight(); tileY++)
				{
					//Invert Y for OpenGL
					int y_inv = m_project.GetEditingMap().GetHeight() - 1 - tileY;
					PaintTile(map.GetTile(tileX, tileY), tileX, y_inv, map.GetTileFlags(tileX, tileY));
				}
			}
		}
	}
	else if (event.GetId() == eContextMenuEditStampCollision)
	{
		//Show collision editor dialog
		if (Stamp* stamp = m_project.GetStamp(m_hoverStamp))
		{
			DialogEditStampCollision dialog(*m_mainWindow, *stamp, m_project, m_renderer, *m_glContext, m_renderResources);
			dialog.ShowModal();
		}
	}
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
	else if(event.GetId() == eContextMenuBakeStamp)
	{
		if(Stamp* stamp = m_project.GetStamp(m_hoverStamp))
		{
			//Remove stamp under cursor
			map.RemoveStamp(m_hoverStamp, m_hoverStampPos.x, m_hoverStampPos.y);

			//Bake stamp to map
			map.BakeStamp(m_hoverStampPos.x, m_hoverStampPos.y, *stamp, m_hoverStampFlags);

			//Clear hover stamp
			m_hoverStamp = InvalidStampId;

			//Repaint area underneath stamp
			const int mapWidth = map.GetWidth();
			const int mapHeight = map.GetHeight();

			for(int tileX = m_hoverStampPos.x; tileX < m_hoverStampPos.x + stamp->GetWidth(); tileX++)
			{
				for(int tileY = m_hoverStampPos.y; tileY < m_hoverStampPos.y + stamp->GetHeight(); tileY++)
				{
					//Can place stamps outside map boundaries, only bake tiles that are inside
					if(tileX >= 0 && tileX < mapWidth && tileY >= 0 && tileY < mapHeight)
					{
						//Invert Y for OpenGL
						int y_inv = mapHeight - 1 - tileY;
						PaintTile(map.GetTile(tileX, tileY), tileX, y_inv, map.GetTileFlags(tileX, tileY));
					}
				}
			}
		}
	}
	else if(event.GetId() == eContextMenuStampBringToFront)
	{
		m_project.GetEditingMap().StampBringToFront(m_hoverStampPos.x, m_hoverStampPos.y, m_hoverStamp);

		m_project.InvalidateMap(true);
		m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
		m_project.InvalidateMap(false);
	}
	else if(event.GetId() == eContextMenuStampSendToBack)
	{
		m_project.GetEditingMap().StampSendToBack(m_hoverStampPos.x, m_hoverStampPos.y, m_hoverStamp);

		m_project.InvalidateMap(true);
		m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
		m_project.InvalidateMap(false);
	}
#endif
	else if (event.GetId() == eContextMenuGameObjCreatePrefab)
	{
		if (m_selectedGameObjects.size() > 0)
		{
			//Create new game object
			GameObjectTypeId prefabId = m_project.AddGameObjectType();

			if (GameObjectType* prefab = m_project.GetGameObjectType(prefabId))
			{
				//Ask name
				wxTextEntryDialog dialog(m_mainWindow, "Prefab Name");
				if (dialog.ShowModal() == wxID_OK)
				{
					//Determine bounds
					ion::maths::Bounds2i bounds;

					for (auto gameObjId : m_selectedGameObjects)
					{
						if (const GameObject* gameObj = m_project.GetEditingMap().GetGameObject(gameObjId))
						{
							if (const GameObjectType* gameObjType = m_project.GetGameObjectType(gameObj->GetTypeId()))
							{
								ion::Vector2i position = gameObj->GetPosition();
								bounds.AddPoint(position);
								bounds.AddPoint(position + gameObjType->GetDimensions());
							}
						}
					}

					//Add all object types to prefab, remove original objects from scene
					for (auto gameObjId : m_selectedGameObjects)
					{
						if (const GameObject* gameObj = m_project.GetEditingMap().GetGameObject(gameObjId))
						{
							if (const GameObjectType* gameObjType = m_project.GetGameObjectType(gameObj->GetTypeId()))
							{
								//Add at relative position
								GameObjectType::PrefabChild child;

								child.name = gameObj->GetName();
								child.typeId = gameObj->GetTypeId();
								child.instanceId = gameObj->GetId();
#if BEEHIVE_GAMEOBJ_ORIGIN_CENTRE
								child.relativePos = gameObj->GetPosition() - bounds.GetCentre();
#else
								child.relativePos = gameObj->GetPosition() - bounds.GetMin();
#endif
								child.dimensions = gameObj->GetDimensions();
								child.spriteActorId = gameObj->GetSpriteActorId() == InvalidActorId ? gameObjType->GetSpriteActorId() : gameObj->GetSpriteActorId();
								child.spriteSheetId = gameObj->GetSpriteSheetId() == InvalidSpriteSheetId ? gameObjType->GetSpriteSheetId() : gameObj->GetSpriteSheetId();
								child.spriteAnimId = gameObj->GetSpriteAnim() == InvalidSpriteAnimId ? gameObjType->GetSpriteAnim() : gameObj->GetSpriteAnim();
								child.variables = gameObj->GetVariables();

								prefab->AddPrefabChild(child);

								//Remove original
								m_project.GetEditingMap().RemoveGameObject(gameObj->GetId());
							}
						}
					}

					//Configure prefab
					prefab->SetPrefab(true);
					prefab->SetDimensions(bounds.GetSize());
					prefab->SetPrefabName(dialog.GetValue().c_str().AsChar());

#if defined BEEHIVE_PLUGIN_LUMINARY
					//Configure variables needed for Luminary type
					luminary::beehive::CreatePrefabType(*prefab);
#endif

					//Place prefab in scene
					GameObjectId instanceId = m_project.GetEditingMap().PlaceGameObject(0, 0, prefabId, InvalidGameObjectArchetypeId);

					if (GameObject* prefabInstance = m_project.GetEditingMap().GetGameObject(instanceId))
					{
						//PlaceGameObject() takes tile coords, need to set world pos manually
						m_project.GetEditingMap().MoveGameObject(instanceId, bounds.GetMin().x, bounds.GetMin().y);

						//Set instance name
						prefabInstance->SetName(prefab->GetPrefabName());

						//Set as new selection, update gizmo
						m_selectedGameObjects.clear();
						m_selectedGameObjects.push_back(instanceId);
						SetGizmoCentre(bounds.GetCentre());
					}

					//Refresh relevant panels
					m_mainWindow->RedrawPanel(MainWindow::ePanelGameObjectTypes);
					m_mainWindow->RefreshPanel(MainWindow::ePanelSceneExplorer);
					Refresh();
				}
			}
		}
	}
	else if(event.GetId() == eContextMenuGameObjAddToAnim)
	{
		if(m_hoverGameObject != InvalidGameObjectId)
		{
			AnimationId animId = m_mainWindow->GetSelectedAnimation();
			Animation* anim = m_project.GetAnimation(animId);
			if(anim && !anim->GetActor(m_hoverGameObject))
			{
				if (GameObject* gameObject = m_project.GetEditingMap().GetGameObject(m_hoverGameObject))
				{
					anim->AddActor(m_hoverGameObject);

					//Match keyframe count of existing actors
					if (AnimationActor* animActor = anim->GetActor(m_hoverGameObject))
					{
						for (int i = 0; i < (int)anim->GetLength(); i++)
						{
							animActor->m_trackPosition.InsertKeyframe(AnimKeyframePosition((float)i, gameObject->GetPosition()));
							animActor->m_trackSpriteAnim.InsertKeyframe(AnimKeyframeSpriteAnim((float)i, std::make_pair(InvalidSpriteSheetId, InvalidSpriteAnimId)));
						}
					}

					m_mainWindow->RefreshAnimActors();
					m_mainWindow->SetSelectedAnimObject(m_hoverGameObject);
				}
			}
		}
	}
	else if(event.GetId() == eContextMenuSetTerrainFlagSpecial)
	{
		u16 currentFlags = m_project.GetEditingCollisionMap().GetTerrainBezierFlags(m_highlightedBezierIdx);
		m_project.GetEditingCollisionMap().SetTerrainBezierFlags(m_highlightedBezierIdx, currentFlags ^ eCollisionTileFlagSpecial);
	}
	else if(event.GetId() == eContextMenuSetTerrainFlagWater)
	{
		u16 currentFlags = m_project.GetEditingCollisionMap().GetTerrainBezierFlags(m_highlightedBezierIdx);
		m_project.GetEditingCollisionMap().SetTerrainBezierFlags(m_highlightedBezierIdx, currentFlags ^ eCollisionTileFlagWater);
	}
}

void MapPanel::OnMousePixelEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int tileX, int tileY)
{
	Map& map = m_project.GetEditingMap();
	Tileset& tileset = m_project.GetTileset();

	const int mapWidth = map.GetWidth();
	const int mapHeight = map.GetHeight();

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	//Invert for OpenGL
	ion::Vector2 mousePosF((float)mousePos.x, (float)(mapHeight * tileHeight) - (float)mousePos.y);
	int y_inv = (mapHeight - 1 - tileY);

	//Update Gizmo mouse cursor
	ion::Vector2i mapSizePx(m_project.GetEditingMap().GetWidth() * m_project.GetPlatformConfig().tileWidth, m_project.GetEditingMap().GetHeight() * m_project.GetPlatformConfig().tileHeight);
	m_gizmo.OnMouse(mousePos, mouseDelta, buttonBits, m_cameraZoom, mapSizePx);
	ion::Vector2i gizmoDelta = m_gizmo.GetLastDelta();

	switch (m_gizmo.GetCurrentConstraint())
	{
	case Gizmo::Constraint::Vertical:
		SetCursor(*m_cursorVertical);
		break;
	case Gizmo::Constraint::Horizontal:
		SetCursor(*m_cursorHorizontal);
		break;
	default:
		SetCursor(*wxSTANDARD_CURSOR);
		break;
	}

	switch(m_currentTool)
	{
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
		case eToolDrawTerrainBezier:
		{
			const float boxHalfExtents = 2.0f;

			//Check if touching an existing control handle
			bool redraw = false;
			if(m_currentBezier)
			{
				ion::Vector2 position;
				ion::Vector2 controlA;
				ion::Vector2 controlB;

				if(!(m_prevMouseBits & eMouseLeft))
				{
					//Not previously clicking, determine control point under cursor
					m_currentBezierControlIdx = -1;
					const int numPoints = m_currentBezier->GetNumPoints();

					for(int i = 0; i < numPoints && m_currentBezierControlIdx == -1; i++)
					{
						//Ignore first and last control handles (not drawn)
						bool modifyHandleA = (i != 0);
						bool modifyHandleB = (i != (numPoints - 1));

						m_currentBezier->GetPoint(i, position, controlA, controlB);

						const ion::Vector2 boxMinP(position.x - boxHalfExtents, position.y - boxHalfExtents);
						const ion::Vector2 boxMaxP(position.x + boxHalfExtents, position.y + boxHalfExtents);
						const ion::Vector2 boxMinA(controlA.x + position.x - boxHalfExtents, controlA.y + position.y - boxHalfExtents);
						const ion::Vector2 boxMaxA(controlA.x + position.x + boxHalfExtents, controlA.y + position.y + boxHalfExtents);
						const ion::Vector2 boxMinB(controlB.x + position.x - boxHalfExtents, controlB.y + position.y - boxHalfExtents);
						const ion::Vector2 boxMaxB(controlB.x + position.x + boxHalfExtents, controlB.y + position.y + boxHalfExtents);

						if(ion::maths::PointInsideBox(mousePosF, boxMinP, boxMaxP))
						{
							//Touching position handle
							m_currentBezierControlPos = position;
							m_currentBezierControlIdx = i;
							m_currentBezierControlHndl = eBezierPosition;
						}
						else if(modifyHandleA && ion::maths::PointInsideBox(mousePosF, boxMinA, boxMaxA))
						{
							//Touching control A handle
							m_currentBezierControlPos = controlA + position;
							m_currentBezierControlIdx = i;
							m_currentBezierControlHndl = eBezierControlA;
						}
						else if(modifyHandleB && ion::maths::PointInsideBox(mousePosF, boxMinB, boxMaxB))
						{
							//Touching control B handle
							m_currentBezierControlPos = controlB + position;
							m_currentBezierControlIdx = i;
							m_currentBezierControlHndl = eBezierControlB;
						}

						//Refresh this panel to redraw preview
						Refresh();
					}
				}

				if((m_currentBezierControlIdx != -1))
				{
					if(buttonBits & eMouseLeft)
					{
						//Control point is under cursor, and left button is held
						m_currentBezier->GetPoint(m_currentBezierControlIdx, position, controlA, controlB);

						//Set new point/control handle position
						switch(m_currentBezierControlHndl)
						{
						case eBezierPosition:
							position = mousePosF;
							break;
						case eBezierControlA:
							controlA = mousePosF - position;
							break;
						case eBezierControlB:
							controlB = mousePosF - position;
							break;
						}

						//Set new preview pos
						m_currentBezierControlPos = mousePosF;

						//Set new point
						m_currentBezier->SetPoint(m_currentBezierControlIdx, position, controlA, controlB);
						redraw = true;
					}
					else if((buttonBits & eMouseRight) && !(m_prevMouseBits & eMouseRight))
					{
						//Control point is under cursor, and right button was clicked
						if(m_currentBezierControlHndl == eBezierPosition)
						{
							//Remove point
							m_currentBezier->RemovePoint(m_currentBezierControlIdx);
							redraw = true;
						}
					}
				}
			}

			//If not handling a control point
			if(m_currentBezierControlIdx == -1)
			{
				if((buttonBits & eMouseLeft) && !(m_prevMouseBits & eMouseLeft))
				{
					//If no current bezier, create new
					if(!m_currentBezier)
					{
						m_currentBezier = m_project.GetEditingCollisionMap().AddTerrainBezier();
					}

					//If new point is to the right of the prev, point handles -1,1 on X axis, else 1,-1
					ion::Vector2 prevPosition(0.0f, 0.0f);
					ion::Vector2 prevControlA(0.0f, 0.0f);
					ion::Vector2 prevControlB(0.0f, 0.0f);

					int numExistingPoints = m_currentBezier->GetNumPoints();

					if(numExistingPoints > 0)
					{
						m_currentBezier->GetPoint(numExistingPoints - 1, prevPosition, prevControlA, prevControlB);
					}

					const float defaultControlLen = (mousePos.x > prevPosition.x) ? 10.0f : -10.0f;

					m_currentBezier->AddPoint(ion::Vector2(mousePos.x, (mapHeight * tileHeight) - mousePos.y), ion::Vector2(-defaultControlLen, 0.0f), ion::Vector2(defaultControlLen, 0.0f));
					redraw = true;
				}
				else if((buttonBits & eMouseRight) && !(m_prevMouseBits & eMouseRight))
				{
					//Finalise bezier
					m_currentBezier = NULL;
					redraw = true;
				}
			}

			if(redraw)
			{
				//Invalidate beziers and refresh this panel
				m_project.InvalidateTerrainBeziers(true);
				Refresh();
				m_project.InvalidateTerrainBeziers(false);
			}

			break;
		}

		case eToolSelectTerrainBezier:
		case eToolDeleteTerrainBezier:
		{
			m_currentBezier = NULL;
			m_highlightedBezier = NULL;

			//Find bezier(s) under cursor
			ion::gamekit::BezierPath* smallestBezier = NULL;
			float smallestBezierSize = ion::maths::FLOAT_MAX;
			int smallestBezierIndex = 0;

			for(int i = 0; i < m_project.GetEditingCollisionMap().GetNumTerrainBeziers(); i++)
			{
				ion::gamekit::BezierPath* bezier = m_project.GetEditingCollisionMap().GetTerrainBezier(i);

				ion::Vector2 boundsMin;
				ion::Vector2 boundsMax;

				bezier->GetBounds(boundsMin, boundsMax);

				//Ensure bounds are at least 2 tiles thick
				if((ion::maths::Abs(boundsMax.x - boundsMin.x) / (float)tileWidth) < 2.0f)
				{
					boundsMin.x -= (float)tileWidth;
					boundsMax.x += (float)tileWidth;
				}

				if((ion::maths::Abs(boundsMax.y - boundsMin.y) / (float)tileHeight) < 2.0f)
				{
					boundsMin.y -= (float)tileHeight;
					boundsMax.y += (float)tileHeight;
				}

				if(ion::maths::PointInsideBox(mousePosF, boundsMin, boundsMax))
				{
					//Found a bezier, check if smallest
					float size = (boundsMax - boundsMin).GetLength();

					if(size < smallestBezierSize)
					{
						smallestBezier = bezier;
						smallestBezierSize = size;
						smallestBezierIndex = i;
					}
				}
			}

			if(smallestBezier)
			{
				if(buttonBits & eMouseLeft)
				{
					if(m_currentTool == eToolSelectTerrainBezier)
					{
						//Set current bezier
						m_currentBezier = smallestBezier;

						//Set bezier draw tool
						m_currentTool = eToolDrawTerrainBezier;
					}
					else if(m_currentTool == eToolDeleteTerrainBezier)
					{
						//Get collision map
						CollisionMap& collisionMap = m_project.GetEditingCollisionMap();

						//Delete bezier
						collisionMap.RemoveTerrainBezier(smallestBezierIndex);
					}

					//Invalidate beziers and refresh this panel
					m_project.InvalidateTerrainBeziers(true);
					Refresh();
					m_project.InvalidateTerrainBeziers(false);
				}
				else if(buttonBits & eMouseRight)
				{
					//Right-click menu
					wxMenu contextMenu;

					wxMenuItem* item = contextMenu.Append(eContextMenuSetTerrainFlagSpecial, wxString("Special terrain"));
					item->SetCheckable(true);
					item->Check(m_project.GetEditingCollisionMap().GetTerrainBezierFlags(m_highlightedBezierIdx) & eCollisionTileFlagSpecial);

					item = contextMenu.Append(eContextMenuSetTerrainFlagWater, wxString("Water terrain"));
					item->SetCheckable(true);
					item->Check(m_project.GetEditingCollisionMap().GetTerrainBezierFlags(m_highlightedBezierIdx) & eCollisionTileFlagWater);

					contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MapPanel::OnContextMenuClick, NULL, this);
					PopupMenu(&contextMenu);
				}
				else
				{
					//Set highlighted bezier
					m_highlightedBezier = smallestBezier;
					m_highlightedBezierIdx = smallestBezierIndex;
				}
			}

			break;
		}

		case eToolPaintCollisionTerrain:
		{
			if((buttonBits & eMouseLeft) || (buttonBits & eMouseRight))
			{
				//Get collision map
				CollisionMap& collisionMap = m_project.GetEditingCollisionMap();

				//Get terrain tileset
				TerrainTileset& terrainTileset = m_project.GetTerrainTileset();

				//Get pixel tile under cursor
				TerrainTileId tileId = collisionMap.GetTerrainTile(tileX, tileY);

				if(tileId == InvalidTerrainTileId)
				{
					//Create new collision tile
					tileId = terrainTileset.AddTerrainTile();

					if(tileId == InvalidTerrainTileId)
					{
						wxMessageBox("Max terrain tiles reached", "Error", wxOK, this);
					}
					else
					{
						//Set on map
						collisionMap.SetTerrainTile(tileX, tileY, tileId);

						//Paint to canvas
						PaintCollisionTile(tileId, tileX, y_inv, collisionMap.GetCollisionTileFlags(tileX, tileY));

						//Invalidate terrain tileset
						m_project.InvalidateTerrainTiles(true);

						//Refresh terrain tileset texture
						m_mainWindow->RefreshTerrainTileset();
					}
				}

				//Get collision tile
				if(TerrainTile* terrainTile = terrainTileset.GetTerrainTile(tileId))
				{
					//Get pixel offset into tile
					int pixelX = mousePos.x - (tileX * tileWidth);
					int pixelY = mousePos.y - (tileY * tileHeight);

					if(buttonBits & eMouseLeft)
					{
						//Set height at X
						const int height = tileHeight - pixelY;
						terrainTile->SetHeight(pixelX, height);

						//Set height on collision tile texture
						m_renderResources.SetTerrainTileHeight(tileId, pixelX, height);
					}
					else
					{
						//Clear height at X
						terrainTile->ClearHeight(pixelX);

						//Clear height on collision tile texture
						m_renderResources.SetTerrainTileHeight(tileId, pixelX, 0);
					}
					

					//Refresh this panel
					Refresh();

					//Refresh collision panels
					m_mainWindow->RedrawPanel(MainWindow::ePanelTerrainTiles);
					m_mainWindow->RedrawPanel(MainWindow::ePanelTerrainTileEditor);
				}

				m_project.InvalidateTerrainTiles(false);
			}

			break;
		}
#endif

		case eToolMoveStamp:
		{
			if(buttonBits & eMouseLeft)
			{
				if(!(m_prevMouseBits & eMouseLeft))
				{
					ion::Vector2i topLeft;
					m_hoverStamp = m_project.GetEditingMap().FindStamp(tileX, tileY, topLeft, m_hoverStampFlags, m_hoverStampMapEntry);

					m_hoverStampPos = topLeft;
				}

				//If fixed stamp size, snap to grid
				int fixedStampWidth = m_project.GetPlatformConfig().stampWidth;
				int fixedStampHeight = m_project.GetPlatformConfig().stampHeight;

				if (fixedStampWidth > 0)
				{
					tileX = ion::maths::RoundDownToNearest(tileX, fixedStampWidth);
				}

				if (fixedStampHeight > 0)
				{
					tileY = ion::maths::RoundDownToNearest(tileY, fixedStampHeight);
				}

				if(tileX != m_hoverStampPos.x || tileY != m_hoverStampPos.y)
				{
					if(m_hoverStamp != InvalidStampId)
					{
						if(Stamp* stamp = m_project.GetStamp(m_hoverStamp))
						{
							int originalX = 0;
							int originalY = 0;

							m_project.GetEditingMap().MoveStamp(m_hoverStamp, m_hoverStampMapEntry, tileX, tileY, originalX, originalY);

							//Repaint area underneath original pos
							for(int x = originalX; x < originalX + stamp->GetWidth(); x++)
							{
								for(int y = originalY; y < originalY + stamp->GetHeight(); y++)
								{
									//Invert Y for OpenGL
									int y_inv = map.GetHeight() - 1 - y;

									//Stamps can be placed outside map boundaries, only paint tiles that are inside
									if(x >= 0 && x < mapWidth && y_inv >= 0 && y_inv < mapHeight)
									{
										TileId tileId = InvalidTileId;

										//Find stamp under cursor first
										ion::Vector2i stampPos;
										u32 flags = 0;
										u32 mapEntryIndex = 0;

										//TODO: Very slow
										StampId stampId = map.FindStamp(x, y, stampPos, flags, mapEntryIndex);

										if(stampId)
										{
											//Get from stamp
											if(Stamp* stamp = m_project.GetStamp(stampId))
											{
												ion::Vector2i offset = ion::Vector2i(x, y) - stampPos;

												int sourceX = (flags & Map::eFlipX) ? (stamp->GetWidth() - 1 - offset.x) : offset.x;
												int sourceY = (flags & Map::eFlipY) ? (stamp->GetHeight() - 1 - offset.y) : offset.y;

												if(sourceX >= 0 && sourceX < stamp->GetWidth() && sourceY >= 0 && sourceY < stamp->GetHeight())
												{
													tileId = stamp->GetTile(sourceX, sourceY);
													flags ^= stamp->GetTileFlags(sourceX, sourceY);
												}
											}
										}
										else
										{
											//Pick tile
											tileId = map.GetTile(x, y);
											flags = map.GetTileFlags(x, y);
										}

										PaintTile(tileId, x, y_inv, flags);
									}
								}
							}

							//Repaint stamp
							for(int x = 0; x < stamp->GetWidth(); x++)
							{
								for(int y = 0; y < stamp->GetHeight(); y++)
								{
									int mapX = x + tileX;
									int mapY = y + tileY;

									//Invert Y for OpenGL
									int y_inv = map.GetHeight() - 1 - mapY;

									//Stamps can be placed outside map boundaries, only paint tiles that are inside
									if(mapX >= 0 && mapX < mapWidth && y_inv >= 0 && y_inv < mapHeight)
									{
										int sourceX = (m_hoverStampFlags & Map::eFlipX) ? (stamp->GetWidth() - 1 - x) : x;
										int sourceY = (m_hoverStampFlags & Map::eFlipY) ? (stamp->GetHeight() - 1 - y) : y;

										PaintTile(stamp->GetTile(sourceX, sourceY), mapX, y_inv, stamp->GetTileFlags(sourceX, sourceY) ^ m_hoverStampFlags);
									}
								}
							}

							m_hoverStampPos.x = tileX;
							m_hoverStampPos.y = tileY;
						}
					}
				}
			}

			break;
		}

		case eToolSelectGameObject:
		case eToolAnimateGameObject:
		{
			ion::Vector2i topLeft;
			m_hoverGameObject = FindGameObject(mousePos.x, mousePos.y, topLeft);

			ion::Vector2i moveDelta = m_gizmo.IsEnabled() ? gizmoDelta : mouseDelta;
			bool mouseOverGizmo = m_gizmo.IsEnabled() && (m_gizmo.GetCurrentConstraint() != Gizmo::Constraint::None);

			if (!mouseOverGizmo || m_boxSelection)
			{
				//Mouse selection
				if (buttonBits & eMouseLeft)
				{
					if (!(m_prevMouseBits & eMouseLeft))
					{
						//First click
						m_boxSelectStart.x = -1;
						m_boxSelectStart.y = -1;
						m_boxSelectEnd.x = -1;
						m_boxSelectEnd.y = -1;

						m_selectedGameObjects.clear();
						m_selectedGameObjects.push_back(m_hoverGameObject);

						if (GameObject* gameObject = m_project.GetEditingMap().GetGameObject(m_hoverGameObject))
						{
							m_mainWindow->SetSelectedGameObject(gameObject);
							m_mainWindow->SetSelectedAnimObject(m_hoverGameObject);
						}
					}
					else
					{
						//Dragging
						if (m_boxSelectStart.x == -1)
						{
							m_boxSelectStart = mousePos;
							m_boxSelection = true;
						}

						m_boxSelectEnd = mousePos;

						//Sanitise box size
						int top = ion::maths::Min(m_boxSelectStart.y, m_boxSelectEnd.y);
						int left = ion::maths::Min(m_boxSelectStart.x, m_boxSelectEnd.x);
						int bottom = ion::maths::Max(m_boxSelectStart.y, m_boxSelectEnd.y);
						int right = ion::maths::Max(m_boxSelectStart.x, m_boxSelectEnd.x);

						std::vector<const GameObjectMapEntry*> gameObjects;
						FindGameObjects(left, top, (right - left), (bottom - top), gameObjects);

						m_selectedGameObjects.clear();

						for (auto gameObj : gameObjects)
						{
							m_selectedGameObjects.push_back(gameObj->m_gameObject.GetId());
						}
					}

					Refresh();
				}
				else
				{
					m_boxSelectStart.x = -1;
					m_boxSelectStart.y = -1;
					m_boxSelectEnd.x = -1;
					m_boxSelectEnd.y = -1;
					m_boxSelection = false;
				}
			}
			
			if (buttonBits & eMouseRight)
			{
				if (m_hoverGameObject != InvalidGameObjectId)
				{
					//Right-click menu
					wxMenu contextMenu;

					contextMenu.Append(eContextMenuGameObjCreatePrefab, wxString("Create Prefab"));
					contextMenu.Append(eContextMenuGameObjAddToAnim, wxString("Add to animation"));
					contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MapPanel::OnContextMenuClick, NULL, this);
					PopupMenu(&contextMenu);
				}
			}

			//Transform using gizmo
			if (!m_boxSelection && m_selectedGameObjects.size() > 0)
			{
				//Objects selected
				m_gizmo.SetEnabled(true);

				if(buttonBits & eMouseLeft)
				{
					const int tileWidth = m_project.GetPlatformConfig().tileWidth;
					const int tileHeight = m_project.GetPlatformConfig().tileHeight;

					if(m_moveGameObjByPixel || m_gizmo.IsEnabled())
					{
						for (auto gameObjectId : m_selectedGameObjects)
						{
							if (GameObject* gameObject = m_project.GetEditingMap().GetGameObject(gameObjectId))
							{
								m_project.GetEditingMap().MoveGameObject(gameObjectId, gameObject->GetPosition().x + moveDelta.x, gameObject->GetPosition().y + moveDelta.y);
							}
						}

						Refresh();
					}
					else if(!m_gizmo.IsEnabled())
					{
						for (auto gameObjectId : m_selectedGameObjects)
						{
							if (GameObject* gameObject = m_project.GetEditingMap().GetGameObject(gameObjectId))
							{
								m_project.GetEditingMap().MoveGameObject(gameObjectId, gameObject->GetPosition().x + (tileDelta.x * tileWidth), gameObject->GetPosition().y + (tileDelta.y * tileHeight));
							}
						}

						Refresh();
					}
				}

				//Determine gizmo centre
				ion::maths::Bounds2i bounds;

				for (auto gameObjectId : m_selectedGameObjects)
				{
					if (const GameObject* gameObject = m_project.GetEditingMap().GetGameObject(gameObjectId))
					{
						if (const GameObjectType* gameObjectType = m_project.GetGameObjectType(gameObject->GetTypeId()))
						{
							bounds.AddPoint(gameObject->GetPosition());
							bounds.AddPoint(gameObject->GetPosition() + (gameObject->GetDimensions().x == 0 ? gameObjectType->GetDimensions() : gameObject->GetDimensions()));
						}
					}
				}

				//Update gizmo pos
				SetGizmoCentre(bounds.GetCentre());
			}
			else
			{
				//No objects selected
				m_gizmo.SetEnabled(false);
			}

			break;
		}
	}
}

void MapPanel::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	//Render map
	RenderCanvas(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	//Render reference image
	RenderReferenceImage(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	//Render game objects
	RenderGameObjects(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	if(m_project.GetShowCollision())
	{
		//Render collision map
		RenderCollisionCanvas(renderer, cameraInverseMtx, projectionMtx, z);

		z += zOffset;

		//Render terrain map
		RenderTerrainCanvas(renderer, cameraInverseMtx, projectionMtx, z);

		z += zOffset;
	}

	//Render paint preview tile
	RenderPaintPreview(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	//Render selection box
	RenderBoxSelection(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	//Render selected stamp
	RenderStampSelection(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	//Render stamp preview tiles
	RenderStampPreview(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	//Render game object preview
	RenderGameObjectPreview(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	if(m_project.GetShowCollision())
	{
		//Render collision bezier paths
		RenderCollisionBeziers(renderer, cameraInverseMtx, projectionMtx, z);

		z += zOffset;
	}

	//Render grid
	if(m_project.GetShowGrid())
	{
		RenderGrid(renderer, cameraInverseMtx, projectionMtx, z);

		z += zOffset;

		if (m_project.GetPlatformConfig().stampWidth > 0)
		{
			RenderStampGrid(renderer, cameraInverseMtx, projectionMtx, z);
		}
	}

	//Render stamp outlines
	if(m_project.GetShowStampOutlines())
	{
		RenderStampOutlines(renderer, cameraInverseMtx, projectionMtx, z);
		z += zOffset;
	}

	//Render physics world outline
	//if(m_project.GetShowPhysicsworldOutline())
	{
		RenderPhysicsWorldOutline(renderer, cameraInverseMtx, projectionMtx, z);
		z += zOffset;
	}

	//Render display frame
	if(m_project.GetShowDisplayFrame())
	{
		RenderDisplayFrame(renderer, cameraInverseMtx, projectionMtx, z);
		z += zOffset;
	}

	//Render gizmo
	ion::Vector2i mapSizePx(m_project.GetEditingMap().GetWidth() * m_project.GetPlatformConfig().tileWidth, m_project.GetEditingMap().GetHeight() * m_project.GetPlatformConfig().tileHeight);
	m_gizmo.OnRender(renderer, cameraInverseMtx, projectionMtx, z, mapSizePx);
}

void MapPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		//If map invalidated
		if(m_project.MapIsInvalidated())
		{
			Map& map = m_project.GetEditingMap();
			CollisionMap& collisionMap = m_project.GetEditingCollisionMap();
			Tileset& tileset = m_project.GetTileset();
			int mapWidth = map.GetWidth();
			int mapHeight = map.GetHeight();

			//Recreate canvas
			CreateCanvas(mapWidth, mapHeight);
			CreateCollisionCanvas(collisionMap.GetWidth(), collisionMap.GetHeight());

			//Recreate grid
			CreateGrid(mapWidth, mapHeight, mapWidth / m_project.GetGridSize(), mapHeight / m_project.GetGridSize());

			//Redraw map
			PaintMap(map);
		}

		//If collision tilset invalidated
		if(m_project.TerrainTilesAreInvalidated())
		{
			//Redraw collision map
			PaintCollisionMap(m_project.GetEditingMap(), m_project.GetEditingCollisionMap());
		}

		//If terrain beziers invalidated
		if(m_project.TerrainBeziersAreInvalidated())
		{
			PaintTerrainBeziers(m_project);
		}

		//If camera invalidated
		if(m_project.CameraIsInvalidated())
		{
			CentreCamera();
		}

		ViewPanel::Refresh(eraseBackground, rect);
	}
}

void MapPanel::CameraCentreOnObject(const GameObject& gameObject)
{
	ion::Vector2 centre(gameObject.GetPosition().x + gameObject.GetDimensions().x / 2.0f, gameObject.GetPosition().y + gameObject.GetDimensions().y / 2.0f);

	const Map& map = m_project.GetEditingMap();
	const float mapWidth = map.GetWidth();
	const float mapHeight = map.GetHeight();
	const float tileWidth = m_project.GetPlatformConfig().tileWidth;
	const float tileHeight = m_project.GetPlatformConfig().tileHeight;
	const float mapWidthPixels = mapWidth * tileWidth;
	const float mapHeightPixels = mapHeight * tileHeight;

	//Reset zoom to identity
	float zoom = m_cameraZoom;
	SetCameraZoom(1.0f);

	//Centre camera
	ion::Vector3 cameraPos(-(m_panelSize.x / 2.0f) - (mapWidthPixels / 2.0f) + centre.x, -(m_panelSize.y / 2.0f) - (mapHeightPixels / 2.0f) + (mapHeightPixels - centre.y), 0.0f);
	m_camera.SetPosition(cameraPos);

	//Re-apply zoom
	SetCameraZoom(zoom);
}

ion::Vector2 MapPanel::GetCameraPos() const
{
	float prevZoom = m_cameraZoom;

	//Reset camera zoom
	ion::render::Camera camera = m_camera;
	camera.SetZoom(ion::Vector3(1.0f, 1.0f, 1.0f));

	//Compensate camera pos
	ion::Vector2 originalViewportSize((float)m_panelSize.x / prevZoom, (float)m_panelSize.y / prevZoom);
	ion::Vector2 newViewportSize((float)m_panelSize.x, (float)m_panelSize.y);
	ion::Vector3 cameraPos = camera.GetPosition();
	cameraPos.x -= (newViewportSize.x - originalViewportSize.x) / 2.0f;
	cameraPos.y -= (newViewportSize.y - originalViewportSize.y) / 2.0f;
	
	return ion::Vector2(cameraPos.x, cameraPos.y);
}

void MapPanel::SelectGameObject(GameObjectId gameObjectId)
{
	m_selectedGameObjects.clear();
	m_selectedGameObjects.push_back(gameObjectId);

	if (GameObject* gameObject = m_project.GetEditingMap().GetGameObject(gameObjectId))
	{
		m_mainWindow->SetSelectedGameObject(gameObject);
		m_mainWindow->SetSelectedAnimObject(gameObjectId);
	}
}

void MapPanel::SetEditingPrefab(GameObjectId instanceId)
{

	m_editingPrefabInstanceId = instanceId;
}

GameObjectId MapPanel::FindGameObject(int x, int y, ion::Vector2i& topLeft) const
{
	GameObjectId gameObjectId = InvalidGameObjectId;
	ion::Vector2i size;
	ion::Vector2i bottomRight;

	const int x_px = x;
	const int y_px = y;

	std::vector<const GameObjectMapEntry*> foundObjects;

	//Find all game objects at position
	const TGameObjectPosMap& gameObjs = m_project.GetEditingMap().GetGameObjects();
	for (TGameObjectPosMap::const_iterator itMap = gameObjs.begin(), endMap = gameObjs.end(); itMap != endMap && !gameObjectId; ++itMap)
	{
		for (std::vector<GameObjectMapEntry>::const_reverse_iterator itVec = itMap->second.rbegin(), endVec = itMap->second.rend(); itVec != endVec && !gameObjectId; ++itVec)
		{
			const int objWidth = (itVec->m_gameObject.GetDimensions().x > 0) ? itVec->m_gameObject.GetDimensions().x : itVec->m_size.x;
			const int objHeight = (itVec->m_gameObject.GetDimensions().y > 0) ? itVec->m_gameObject.GetDimensions().y : itVec->m_size.y;

			ion::Vector2i size(objWidth, objHeight);

#if BEEHIVE_GAMEOBJ_ORIGIN_CENTRE
			topLeft.x = itVec->m_gameObject.GetPosition().x - (size.x / 2);
			topLeft.y = itVec->m_gameObject.GetPosition().y - (size.y / 2);
#else
			topLeft.x = itVec->m_gameObject.GetPosition().x;
			topLeft.y = itVec->m_gameObject.GetPosition().y;
#endif

			ion::Vector2i bottomRight = topLeft + size;

			if (x_px >= topLeft.x && y_px >= topLeft.y
				&& x_px < bottomRight.x && y_px < bottomRight.y)
			{
				foundObjects.push_back(&(*itVec));
			}
		}
	}

	//Pick the smallest
	int smallestSize = ion::maths::S32_MAX;
	for (int i = 0; i < foundObjects.size(); i++)
	{
		const int objWidth = (foundObjects[i]->m_gameObject.GetDimensions().x > 0) ? foundObjects[i]->m_gameObject.GetDimensions().x : foundObjects[i]->m_size.x;
		const int objHeight = (foundObjects[i]->m_gameObject.GetDimensions().y > 0) ? foundObjects[i]->m_gameObject.GetDimensions().y : foundObjects[i]->m_size.y;

		int size = objWidth * objHeight;
		if (size < smallestSize)
		{
			smallestSize = size;
			gameObjectId = foundObjects[i]->m_gameObject.GetId();
		}
	}

	return gameObjectId;
}

int MapPanel::FindGameObjects(int x, int y, int width, int height, std::vector<const GameObjectMapEntry*>& gameObjects) const
{
	ion::Vector2i boundsMin(x, y);
	ion::Vector2i boundsMax(x + width, y + height);

	const TGameObjectPosMap& gameObjs = m_project.GetEditingMap().GetGameObjects();
	for (TGameObjectPosMap::const_iterator it = gameObjs.begin(), end = gameObjs.end(); it != end; ++it)
	{
		const std::vector<GameObjectMapEntry>& gameObjectsOfType = it->second;

		for (int i = 0; i < gameObjectsOfType.size(); i++)
		{
			const GameObjectMapEntry& gameObject = gameObjectsOfType[i];

			const int objWidth = (gameObject.m_gameObject.GetDimensions().x > 0) ? gameObject.m_gameObject.GetDimensions().x : gameObject.m_size.x;
			const int objHeight = (gameObject.m_gameObject.GetDimensions().y > 0) ? gameObject.m_gameObject.GetDimensions().y : gameObject.m_size.y;

#if BEEHIVE_GAMEOBJ_ORIGIN_CENTRE
			ion::Vector2i position = gameObject.m_gameObject.GetPosition() - (ion::Vector2i(objWidth, objHeight) / 2);
			if (ion::maths::BoxIntersectsBox(boundsMin, boundsMax, position, position + ion::Vector2i(objWidth, objHeight)))
#else
			if (ion::maths::BoxIntersectsBox(boundsMin, boundsMax, gameObject.m_gameObject.GetPosition(), gameObject.m_gameObject.GetPosition() + ion::Vector2i(objWidth, objHeight)))
#endif
			{
				gameObjects.push_back(&gameObject);
			}
		}
	}

	return gameObjects.size();
}

void MapPanel::SetTool(ToolType tool)
{
	ToolType previousTool = m_currentTool;
	m_currentTool = tool;

	Map& map = m_project.GetEditingMap();
	CollisionMap& collisionMap = m_project.GetEditingCollisionMap();

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	switch(tool)
	{
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
	case eToolFill:
		//If previous tool was selectTiles, fill selection and leave previous tool data
		if(m_selectedTiles.size() > 0)
		{
			if(previousTool == eToolSelectTiles)
			{
				if(m_project.GetPaintTile() != InvalidTileId)
				{
					//Set on map
					map.FillTiles(m_project.GetPaintTile(), m_selectedTiles);

					//Draw to canvas
					FillTiles(m_project.GetPaintTile(), m_selectedTiles);

					//Refresh
					Refresh();
				}

				//Set back to select tool, leave tool data intact
				m_currentTool = previousTool;
			}
			else
			{
				ResetToolData();
			}
		}
		break;

	case eToolGenerateTerrain:
		m_project.GenerateTerrain(m_selectedTiles);
		m_mainWindow->RefreshTerrainTileset();
		m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
		m_mainWindow->RefreshPanel(MainWindow::ePanelTerrainTiles);
		break;


	case eToolPaintTile:
		ResetToolData();
		m_previewTile = m_project.GetPaintTile();
		break;
#endif

	case eToolPaintStamp:
	{
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
		//If coming from stamp clone tool, don't wipe preview data
		if(previousTool != eToolClone)
		{
			ResetToolData();
		}
#endif

		//Get temp cloning stamp, else get current painting stamp
		Stamp* stamp = m_tempStamp ? m_tempStamp : m_project.GetStamp(m_project.GetPaintStamp());
		if(stamp)
		{
			//Create preview primitive
			CreateStampPreview(stamp);
		}
		else
		{
			//No stamp, revert to selection tool
			m_currentTool = eToolSelectTiles;
		}

		break;
	}

#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
	case eToolCopyToNewMap:
	{
		//Must previously have been in Select mode
		if(previousTool == eToolSelectTiles)
		{
			//and have data to work with
			if(m_selectedTiles.size() > 0)
			{
				//Query for new map name
				DialogNewMap dialog(m_mainWindow);

				if(dialog.ShowModal() == wxID_OK)
				{
					//Get min/max width/height
					int left, top, right, bottom;
					FindBounds(m_selectedTiles, left, top, right, bottom);
					left = ion::maths::Clamp(left, 0, map.GetWidth());
					right = ion::maths::Clamp(right, 0, map.GetWidth());
					top = ion::maths::Clamp(top, 0, map.GetHeight());
					bottom = ion::maths::Clamp(bottom, 0, map.GetHeight());
					int width = abs(right - left) + 1;
					int height = abs(bottom - top) + 1;

					//Create new map
					MapId newMapId = m_project.CreateMap();
					CollisionMapId collisionMapId = m_project.CreateCollisionMap(newMapId);
					Map& newMap = m_project.GetMap(newMapId);
					CollisionMap& newCollisionMap = m_project.GetCollisionMap(collisionMapId);

					//Resize
					newMap.SetName(dialog.m_textMapName->GetValue().GetData().AsChar());
					newMap.Resize(width, height, false, false);
					newCollisionMap.Resize(width, height, false, false);
					
					//Populate tiles
					for(int y = top; y < bottom; y++)
					{
						for(int x = left; x < right; x++)
						{
							int newX = x - left;
							int newY = y - top;

							if(x >= 0 && x < map.GetWidth() && y >= 0 && y < map.GetHeight())
							{
								//Grab tile
								newMap.SetTile(newX, newY, map.GetTile(x, y));
								newMap.SetTileFlags(newY, newY, map.GetTileFlags(x, y));

								//Grab terrain tile
								newCollisionMap.SetTerrainTile(newX, newY, collisionMap.GetTerrainTile(x, y));
								newCollisionMap.SetCollisionTileFlags(newX, newY, collisionMap.GetCollisionTileFlags(x, y));
							}
						}
					}

					//Populate stamps
					std::vector<const StampMapEntry*> stamps;
					map.FindStamps(left, top, width, height, stamps);
					for(int i = 0; i < stamps.size(); i++)
					{
						if(const Stamp* stamp = m_project.GetStamp(stamps[i]->m_id))
						{
							newMap.SetStamp(stamps[i]->m_position.x - left, stamps[i]->m_position.y - top, *stamp, stamps[i]->m_flags);
						}
					}

					//Populate terrain splines
					std::vector<const ion::gamekit::BezierPath*> beziers;
					collisionMap.FindTerrainBeziers(left, top, width, height, beziers);
					for(int i = 0; i < beziers.size(); i++)
					{
						ion::gamekit::BezierPath bezier = *beziers[i];
						bezier.Move(ion::Vector2(-left * tileWidth, -(map.GetHeight() - bottom - 1) * tileHeight));
						newCollisionMap.AddTerrainBezier(bezier);
					}

					//Populate game objects
					std::vector<const GameObjectMapEntry*> gameObjects;
					FindGameObjects(left, top, width, height, gameObjects);
					for(int i = 0; i < gameObjects.size(); i++)
					{
						if(const GameObjectType* gameObjectType = m_project.GetGameObjectType(gameObjects[i]->m_gameObject.GetTypeId()))
						{
							newMap.PlaceGameObject(gameObjects[i]->m_gameObject.GetPosition().x - (left * tileWidth), gameObjects[i]->m_gameObject.GetPosition().y - (top * tileHeight), gameObjects[i]->m_gameObject, *gameObjectType, InvalidGameObjectArchetypeId);
						}
					}

					//Set as editing map
					m_project.SetEditingMap(newMapId);
					m_project.SetEditingCollisionMap(newMapId);

					//Refresh
					m_project.InvalidateMap(true);
					m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
					m_mainWindow->RefreshPanel(MainWindow::ePanelMapList);
					m_project.InvalidateMap(false);
				}
			}
		}

		break;
	}

	case eToolClone:
	case eToolCreateStamp:
	{
		//Must previously have been in Select mode
		if(previousTool == eToolSelectTiles)
		{
			//and have data to work with
			if(m_selectedTiles.size() > 0)
			{
				//Get min/max width/height
				int left, top, right, bottom;
				FindBounds(m_selectedTiles, left, top, right, bottom);
				int width = abs(right - left) + 1;
				int height = abs(bottom - top) + 1;

				Stamp* stamp = NULL;

				if(tool == eToolClone)
				{
					//Cloning - create temp stamp
					stamp = new Stamp(InvalidStampId, width, height);
					m_tempStamp = stamp;
				}
				else if(tool == eToolCreateStamp)
				{
					//Creating - create real stamp
					StampId stampId = m_project.AddStamp(width, height);
					stamp = m_project.GetStamp(stampId);
				}

				//Populate stamp, set primitive UV coords
				ion::render::TexCoord coords[4];

				for(int i = 0; i < m_selectedTiles.size(); i++)
				{
					int mapX = m_selectedTiles[i].x;
					int mapY = m_selectedTiles[i].y;
					int stampX = mapX - left;
					int stampY = mapY - top;
					int y_inv = height - 1 - stampY;

					TileId tileId = InvalidTileId;

					//Find stamp under cursor first
					ion::Vector2i stampPos;
					u32 stampFlags = 0;
					u32 mapEntryIndex = 0;
					StampId stampId = map.FindStamp(mapX, mapY, stampPos, stampFlags, mapEntryIndex);

					//TODO - optimise for known stamp
					if(stampId)
					{
						//Get from stamp
						if(Stamp* stamp = m_project.GetStamp(stampId))
						{
							ion::Vector2i offset = ion::Vector2i(mapX, mapY) - stampPos;
							tileId = stamp->GetTile(offset.x, offset.y);
						}
					}
					else
					{
						//Pick tile
						tileId = map.GetTile(mapX, mapY);
					}

					u32 tileFlags = map.GetTileFlags(mapX, mapY);
					stamp->SetTile(stampX, stampY, tileId);
					stamp->SetTileFlags(stampX, stampY, tileFlags);
				}

				if(tool == eToolClone)
				{
					//Cloning - switch straight to paste tool
					SetTool(eToolPaintStamp);
					return;
				}
				else if(tool == eToolCreateStamp)
				{
					//Creating - switch back to selection mode
					m_currentTool = eToolSelectTiles;

					//Place new stamp to map
					ion::Vector2i stampPos = (m_boxSelectStart.x <= m_boxSelectEnd.x && m_boxSelectStart.y <= m_boxSelectEnd.y) ? m_boxSelectStart : m_boxSelectEnd;
					map.SetStamp(stampPos.x, stampPos.y, *stamp, 0);

					//Paint on canvas
					PaintStamp(*stamp, stampPos.x, stampPos.y, 0);

					//Refresh stamp panel to draw new stamp
					m_mainWindow->RefreshPanel(MainWindow::ePanelStamps);
				}
			}
		}
		else
		{
			ResetToolData();
		}

		break;
	}

	case eToolSelectTerrainBezier:
	case eToolDeleteTerrainBezier:
	{
		//Calc bezier bounds
		for(int i = 0; i < m_project.GetEditingCollisionMap().GetNumTerrainBeziers(); i++)
		{
			ion::gamekit::BezierPath* bezier = m_project.GetEditingCollisionMap().GetTerrainBezier(i);
			bezier->CalculateBounds();
		}

		break;
	}
#endif

	default:
		ResetToolData();
	}

	Refresh();
}

void MapPanel::SetGizmoCentre(const ion::Vector2i& centre)
{
	ion::Vector2i mapSizePx(m_project.GetEditingMap().GetWidth() * m_project.GetPlatformConfig().tileWidth, m_project.GetEditingMap().GetHeight() * m_project.GetPlatformConfig().tileHeight);
	ion::Vector2i position(centre.x, mapSizePx.y - centre.y);
	m_gizmo.SetPosition(position);
}

void MapPanel::ResetToolData()
{
	//Disable gizmo
	m_gizmo.SetEnabled(false);

	//Invalidate preview tile
	m_previewTile = InvalidTileId;
	m_previewTileFlipX = false;
	m_previewTileFlipY = false;

	//Invalidate box/multiple selection
	m_selectedTiles.clear();
	m_multipleSelection = false;
	m_boxSelection = false;
	m_boxSelectStart.x = -1;
	m_boxSelectStart.y = -1;
	m_boxSelectEnd.x = -1;
	m_boxSelectEnd.y = -1;

	//Invalidate selected stamp
	m_hoverStamp = InvalidStampId;
	m_selectedStamp = InvalidStampId;

	//Invalidate game object preview
	m_hoverGameObject = InvalidGameObjectId;
	m_selectedGameObjects.clear();
	m_previewGameObjectType = InvalidGameObjectTypeId;

	//Invalidate bezier
	m_currentBezier = NULL;
	m_highlightedBezier = NULL;
	m_currentBezierControlIdx = -1;

	//Delete clipboard stamp
	if(m_tempStamp)
	{
		delete m_tempStamp;
		m_tempStamp = NULL;
	}

	if(m_stampPreviewPrimitive)
	{
		delete m_stampPreviewPrimitive;
		m_stampPreviewPrimitive = NULL;
	}

	if(m_primitiveBezierPoints)
	{
		delete m_primitiveBezierPoints;
		m_primitiveBezierPoints = NULL;
	}

	if(m_primitiveBezierHandles)
	{
		delete m_primitiveBezierHandles;
		m_primitiveBezierHandles = NULL;
	}
}

void MapPanel::RenderCollisionCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//No depth test (stops grid cells Z fighting)
	renderer.SetDepthTest(ion::render::Renderer::DepthTest::Always);

	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialCollisionTypes);

	//Draw map
	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	renderer.BindMaterial(*material, ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_collisionCanvasPrimitive->GetVertexBuffer(), m_collisionCanvasPrimitive->GetIndexBuffer());
	renderer.UnbindMaterial(*material);
	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);

	renderer.SetDepthTest(ion::render::Renderer::DepthTest::LessOrEqual);
}

void MapPanel::RenderCollisionBeziers(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//No depth test
	renderer.SetDepthTest(ion::render::Renderer::DepthTest::Always);

	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);

	const Map& map = m_project.GetEditingMap();
	const float mapWidth = map.GetWidth();
	const float mapHeight = map.GetHeight();
	const float tileWidth = m_project.GetPlatformConfig().tileWidth;
	const float tileHeight = m_project.GetPlatformConfig().tileHeight;

	ion::Matrix4 bezierMatrix;
	bezierMatrix.SetTranslation(ion::Vector3(-(mapWidth * tileWidth) / 2.0f, -(mapHeight * tileHeight) / 2.0f, z));

	//Draw curves
	if(m_primitiveBeziers.size() > 0)
	{
		material->SetDiffuseColour(ion::Colour(1.0f, 0.4f, 0.4f, 1.0f));
		renderer.BindMaterial(*material, bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(3.0f);

		for(int i = 0; i < m_primitiveBeziers.size(); i++)
		{
			renderer.DrawVertexBuffer(m_primitiveBeziers[i]->GetVertexBuffer());
		}

		renderer.SetLineWidth(1.0f);
		renderer.UnbindMaterial(*material);
	}

	//Draw curve points
	if(m_primitiveBezierPoints)
	{
		material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
		renderer.BindMaterial(*material, bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(2.0f);
		renderer.DrawVertexBuffer(m_primitiveBezierPoints->GetVertexBuffer());
		renderer.SetLineWidth(1.0f);
		renderer.UnbindMaterial(*material);
	}

	//Draw curve control handles
	if(m_primitiveBezierHandles)
	{
		material->SetDiffuseColour(ion::Colour(1.0f, 0.6f, 0.6f, 1.0f));
		renderer.BindMaterial(*material, bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(2.0f);
		renderer.DrawVertexBuffer(m_primitiveBezierHandles->GetVertexBuffer());
		renderer.SetLineWidth(1.0f);
		renderer.UnbindMaterial(*material);
	}

	//Draw selected handle
	if(m_currentBezierControlIdx != -1)
	{
		ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileQuad);
		const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourSelected);

		renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
		material->SetDiffuseColour(colour);

		const float x = m_currentBezierControlPos.x;
		const float y = m_currentBezierControlPos.y;
		const float width = m_currentBezierControlHndl == eBezierPosition ? 1.0f : 0.5f;
		const float height = m_currentBezierControlHndl == eBezierPosition ? 1.0f : 0.5f;

		ion::Vector3 previewScale(width, height, 1.0f);
		ion::Matrix4 previewMtx;
		ion::Vector3 previewPos(x, y, z);
		previewMtx.SetTranslation(previewPos);
		previewMtx.SetScale(previewScale);

		renderer.BindMaterial(*material, previewMtx * bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		renderer.UnbindMaterial(*material);

		renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);
	}

	//Draw selected bezier
	if(m_highlightedBezier)
	{
		ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileQuad);
		const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourSelected);

		renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
		material->SetDiffuseColour(colour);

		ion::Vector2 boundsMin;
		ion::Vector2 boundsMax;

		m_highlightedBezier->GetBounds(boundsMin, boundsMax);

		//Ensure bounds are at least 2 tiles thick
		if((ion::maths::Abs(boundsMax.x - boundsMin.x) / (float)tileWidth) < 2.0f)
		{
			boundsMin.x -= (float)tileWidth;
			boundsMax.x += (float)tileWidth;
		}

		if((ion::maths::Abs(boundsMax.y - boundsMin.y) / (float)tileHeight) < 2.0f)
		{
			boundsMin.y -= (float)tileHeight;
			boundsMax.y += (float)tileHeight;
		}

		ion::Vector2 size = boundsMax - boundsMin;

		const float x = boundsMin.x + (size.x / 2.0f);
		const float y = boundsMin.y + (size.y / 2.0f);

		ion::Vector3 previewScale(size.x / (float)tileWidth, size.y / (float)tileWidth, 1.0f);
		ion::Matrix4 previewMtx;
		ion::Vector3 previewPos(x, y, z);
		previewMtx.SetTranslation(previewPos);
		previewMtx.SetScale(previewScale);

		renderer.BindMaterial(*material, previewMtx * bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		renderer.UnbindMaterial(*material);

		renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);
	}

	renderer.SetDepthTest(ion::render::Renderer::DepthTest::LessOrEqual);
}

void MapPanel::RenderTerrainCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//No depth test (stops grid cells Z fighting)
	renderer.SetDepthTest(ion::render::Renderer::DepthTest::Always);

	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTerrainTilesetHeight);

	//Draw map
	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	renderer.BindMaterial(*material, ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_terrainCanvasPrimitive->GetVertexBuffer(), m_terrainCanvasPrimitive->GetIndexBuffer());
	renderer.UnbindMaterial(*material);
	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);

	renderer.SetDepthTest(ion::render::Renderer::DepthTest::LessOrEqual);
}

void RenderGameObject(
	Project& project,
	ion::render::Renderer& renderer,
	RenderResources& renderResources,
	const ion::Matrix4& cameraInverseMtx,
	const ion::Matrix4& projectionMtx,
	float z,
	int mapWidth,
	int mapHeight,
	int tileWidth,
	int tileHeight,
	ion::render::Primitive* primitive,
	const GameObjectType& gameObjectType,
	const GameObject* gameObject,
	const ion::Vector2i& position,
	const ion::Vector2i& dimensions,
	ActorId spriteActorId,
	SpriteSheetId spriteSheetId,
	SpriteAnimId spriteAnimId,
	ActorId previewSpriteActorId,
	bool selected,
	bool hovering)
{
	const float x = position.x;
	const float y_inv = (mapHeight * tileHeight) - 1 - position.y;
	const float width = (dimensions.x > 0) ? dimensions.x : (gameObject && gameObject->GetDimensions().x > 0) ? gameObject->GetDimensions().x : gameObjectType.GetDimensions().x;
	const float height = (dimensions.y > 0) ? dimensions.y : (gameObject && gameObject->GetDimensions().y > 0) ? gameObject->GetDimensions().y : gameObjectType.GetDimensions().y;
	const float height_inv = -height;
	const bool customSize = gameObject && gameObject->GetDimensions().x > 0;

	//Find a usable sprite actor, sheet, anim from editor preview, object, or type
	const SpriteSheet* spriteSheet = nullptr;
	int spriteFrame = 0;
	ion::Vector3 animPosOffset;

	//Get sprite actor
	const Actor* spriteActor = project.GetActor(spriteActorId);

	if (!spriteActor)
	{
		//Find sprite actor from game object
		spriteActorId = gameObject ? gameObject->GetSpriteActorId() : InvalidActorId;
		spriteActor = project.GetActor(spriteActorId);
	}

	if (!spriteActor)
	{
		//Find sprite actor from game object type
		spriteActorId = gameObjectType.GetSpriteActorId();
		spriteActor = project.GetActor(spriteActorId);
	}

	if (!spriteActor)
	{
		//Find sprite actor from editor preview
		spriteActorId = previewSpriteActorId;
		spriteActor = project.GetActor(spriteActorId);
	}

	if (spriteActor)
	{
		//Get sprite sheet
		spriteSheet = spriteActor->GetSpriteSheet(spriteSheetId);

		if (!spriteSheet)
		{
			//Find sprite sheet from editor preview
			spriteSheetId = gameObjectType.GetPreviewSpriteSheetId();
			spriteSheet = spriteActor->GetSpriteSheet(spriteSheetId);
		}

		if (!spriteSheet)
		{
			//Find sprite sheet from game object
			spriteSheetId = gameObject ? gameObject->GetSpriteSheetId() : InvalidSpriteSheetId;
			spriteSheet = spriteActor->GetSpriteSheet(spriteSheetId);
		}

		//Find sprite sheet from game object type
		if (!spriteSheet)
		{
			spriteSheetId = gameObjectType.GetSpriteSheetId();
			spriteSheet = spriteActor->GetSpriteSheet(spriteSheetId);
		}

		//Default to first sprite sheet
		if (!spriteSheet && spriteActor->GetSpriteSheetCount() > 0)
		{
			spriteSheet = &spriteActor->GetSpriteSheets().begin()->second;
			spriteSheetId = spriteActor->GetSpriteSheets().begin()->first;
		}
	}

	//Use editor preview sprite sheet if available
	if (!spriteSheet && gameObjectType.GetPreviewSpriteSheetId() != InvalidSpriteSheetId)
	{
		spriteSheetId = gameObjectType.GetPreviewSpriteSheetId();
		spriteSheet = &gameObjectType.GetPreviewSpriteSheet();
	}

	if (spriteSheet)
	{
		//Get sprite anim
		const SpriteAnimation* spriteAnim = spriteSheet->GetAnimation(spriteAnimId);

		if (!spriteAnim)
		{
			//Find sprite anim from game object
			spriteAnimId = gameObject ? gameObject->GetSpriteAnim() : InvalidSpriteAnimId;
			spriteAnim = spriteSheet->GetAnimation(spriteAnimId);
		}

		if (!spriteAnim)
		{
			//Find sprite anim from game object type
			spriteAnimId = gameObjectType.GetSpriteAnim();
			spriteAnim = spriteSheet->GetAnimation(spriteAnimId);
		}

		if (spriteAnim)
		{
			ion::Vector2i position = spriteAnim->m_trackPosition.GetValue(spriteAnim->GetFrame());
			spriteFrame = spriteAnim->m_trackSpriteFrame.GetValue(spriteAnim->GetFrame());

			animPosOffset.x = (float)position.x;
			animPosOffset.y = (float)position.y;
		}
	}

	//Render outline
	if (project.GetShowStampOutlines())
	{
		ion::Colour colour(1.0f, 1.0f, 1.0f, 0.0f);

		if (selected)
			colour = renderResources.GetColour(RenderResources::eColourSelected);
		else if (hovering)
			colour = renderResources.GetColour(RenderResources::eColourOutline);
		else
			colour = renderResources.GetColour(RenderResources::eColourUnselected);

		ion::render::Material* material = renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
		material->SetDiffuseColour(colour);

		ion::Vector3 scale(width / tileWidth, height / tileHeight, 1.0f);
		ion::Matrix4 mtx;

#if BEEHIVE_GAMEOBJ_ORIGIN_CENTRE
		ion::Vector3 pos(floor((x - ((mapWidth * tileWidth) / 2.0f))),
			floor((y_inv - ((mapHeight * tileHeight) / 2.0f))), z);
#else
		ion::Vector3 pos(floor((x - ((mapWidth * tileWidth) / 2.0f) + (width / 2.0f))),
			floor((y_inv - ((mapHeight * tileHeight) / 2.0f) + ((height_inv / 2.0f) + 1.0f))), z);
#endif

		mtx.SetTranslation(pos + animPosOffset);
		mtx.SetScale(scale);

		renderer.BindMaterial(*material, mtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		renderer.UnbindMaterial(*material);
	}

	if (spriteSheetId != InvalidSpriteSheetId)
	{
		//Render spriteSheet
		if (RenderResources::SpriteSheetRenderResources* spriteSheetResources = renderResources.GetSpriteSheetResources(spriteSheetId))
		{
			ion::debug::Assert(spriteSheetResources, "MapPanel::RenderGameObjects() - Missing spriteSheet render resources");
			ion::debug::Assert(spriteSheetResources->m_frames.size() > 0, "MapPanel::RenderGameObjects() - SpriteSheet contains no frames");

			ion::render::Primitive* spriteSheetPrimitive = spriteSheetResources->m_primitive;
			ion::render::Material* spriteSheetMaterial = spriteSheetResources->m_frames[spriteFrame].material;
			spriteSheetMaterial->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));

			ion::Matrix4 spriteSheetMtx;

#if BEEHIVE_GAMEOBJ_ORIGIN_CENTRE
			ion::Vector3 pos(floor((x - ((mapWidth * tileWidth) / 2.0f))),
				floor((y_inv - ((mapHeight * tileHeight) / 2.0f))), z);
#else
			ion::Vector3 pos(floor((x - ((mapWidth * tileWidth) / 2.0f) + (width / 2.0f))),
				floor((y_inv - ((mapHeight * tileHeight) / 2.0f) + ((height_inv / 2.0f) + 1.0f))), z);
#endif

			spriteSheetMtx.SetTranslation(pos + animPosOffset);

			if (customSize)
			{
				spriteSheetPrimitive = renderResources.GetPrimitive(RenderResources::ePrimitiveUnitQuad);
				spriteSheetMtx.SetScale(ion::Vector3(width, height_inv, 1.0f));
			}

			renderer.BindMaterial(*spriteSheetMaterial, spriteSheetMtx, cameraInverseMtx, projectionMtx);
			renderer.SetFaceCulling(ion::render::Renderer::CullingMode::None);
			renderer.DrawVertexBuffer(spriteSheetPrimitive->GetVertexBuffer(), spriteSheetPrimitive->GetIndexBuffer());
			renderer.SetFaceCulling(ion::render::Renderer::CullingMode::CounterClockwise);
			renderer.UnbindMaterial(*spriteSheetMaterial);
		}
	}
}

void MapPanel::RenderGameObjects(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	const TGameObjectPosMap& gameObjects = m_project.GetEditingMap().GetGameObjects();

	const Map& map = m_project.GetEditingMap();
	const float mapWidth = map.GetWidth();
	const float mapHeight = map.GetHeight();
	const float tileWidth = m_project.GetPlatformConfig().tileWidth;
	const float tileHeight = m_project.GetPlatformConfig().tileHeight;

	ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileQuad);

	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);

	for(TGameObjectPosMap::const_iterator itMap = gameObjects.begin(), endMap = gameObjects.end(); itMap != endMap; ++itMap)
	{
		for(std::vector<GameObjectMapEntry>::const_iterator itVec = itMap->second.begin(), endVec = itMap->second.end(); itVec != endVec; ++itVec)
		{
			const GameObject& gameObject = itVec->m_gameObject;

			if(const GameObjectType* gameObjectType = m_project.GetGameObjectType(gameObject.GetTypeId()))
			{
				bool selected = ion::utils::stl::Find(m_selectedGameObjects, gameObject.GetId());
				bool hovering = gameObject.GetId() == m_hoverGameObject;

				//Draw root game object
				ion::Vector2i position = gameObject.GetPosition();

				RenderGameObject(
					m_project,
					renderer,
					m_renderResources,
					cameraInverseMtx,
					projectionMtx,
					z,
					mapWidth,
					mapHeight,
					tileWidth,
					tileHeight,
					primitive,
					*gameObjectType,
					&gameObject,
					position,
					ion::Vector2i(),
					InvalidActorId,
					InvalidSpriteSheetId,
					InvalidSpriteAnimId,
					gameObjectType->GetPreviewSpriteSheetId(),
					selected,
					hovering);

				if (gameObjectType->IsPrefabType())
				{
					//Draw all prefab children
					for (auto prefabChild : gameObjectType->GetPrefabChildren())
					{
						if (const GameObjectType* prefabChildType = m_project.GetGameObjectType(prefabChild.typeId))
						{
							RenderGameObject(
								m_project,
								renderer,
								m_renderResources,
								cameraInverseMtx,
								projectionMtx,
								z,
								mapWidth,
								mapHeight,
								tileWidth,
								tileHeight,
								primitive,
								*prefabChildType,
								nullptr,
								position + prefabChild.relativePos,
								prefabChild.dimensions,
								prefabChild.spriteActorId,
								prefabChild.spriteSheetId,
								prefabChild.spriteAnimId,
								InvalidSpriteSheetId,
								selected,
								hovering);
						}
					}
				}
			}
		}
	}

	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);
}

void MapPanel::RenderReferenceImage(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveUnitQuad);
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialReferenceImage);

	if (material->GetDiffuseMap(0)->GetWidth() > 0)
	{
		const Map& map = m_project.GetEditingMap();
		const float mapWidth = map.GetWidth();
		const float mapHeight = map.GetHeight();

		const float x = 0.0f;
		const float y = 0.0f;
		const float y_inv = mapHeight - 1 - y;
		const float width = material->GetDiffuseMap(0)->GetWidth();
		const float height = material->GetDiffuseMap(0)->GetHeight();

		ion::Matrix4 matrix;
		ion::Vector3 scale(width, height, 1.0f);
		ion::Vector3 position(x, y, z);
		matrix.SetTranslation(position);
		matrix.SetScale(scale);

		renderer.BindMaterial(*material, matrix, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		renderer.UnbindMaterial(*material);
	}
}

void MapPanel::RenderGameObjectPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(const GameObjectType* gameObjectType = m_project.GetGameObjectType(m_previewGameObjectType))
	{
		ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileQuad);
		ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
		const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourSelected);

		renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
		material->SetDiffuseColour(colour);

		const Map& map = m_project.GetEditingMap();
		const float mapWidth = map.GetWidth();
		const float mapHeight = map.GetHeight();
		const float tileWidth = m_project.GetPlatformConfig().tileWidth;
		const float tileHeight = m_project.GetPlatformConfig().tileHeight;

		const float x = m_previewGameObjectPos.x;
		const float y_inv = mapHeight - 1 - m_previewGameObjectPos.y;
		const float width = gameObjectType->GetDimensions().x / tileWidth;
		const float height_inv = -gameObjectType->GetDimensions().y / tileHeight;

		ion::Vector3 previewScale(gameObjectType->GetDimensions().x / tileWidth, gameObjectType->GetDimensions().y / tileHeight, 1.0f);
		ion::Matrix4 previewMtx;

#if BEEHIVE_GAMEOBJ_ORIGIN_CENTRE
		ion::Vector3 previewPos(floor((x - (mapWidth / 2.0f)) * tileWidth),
			floor((y_inv - (mapHeight / 2.0f)) * tileHeight), z);
#else
		ion::Vector3 previewPos(floor((x - (mapWidth / 2.0f) + (width / 2.0f)) * tileWidth),
								floor((y_inv - (mapHeight / 2.0f) + ((height_inv / 2.0f) + 1.0f)) * tileHeight), z);
#endif

		previewMtx.SetTranslation(previewPos);
		previewMtx.SetScale(previewScale);

		renderer.BindMaterial(*material, previewMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		renderer.UnbindMaterial(*material);

		SpriteSheetId spriteSheetId = InvalidSpriteSheetId;

		//Find sprite sheet from archetype
		if (const GameObjectArchetype* archetype = gameObjectType->GetArchetype(m_previewGameObjectArchetype))
		{
			spriteSheetId = archetype->spriteSheetId;
		}
		
		//Find sprite sheet from game object type
		if (spriteSheetId == InvalidSpriteSheetId)
			spriteSheetId = gameObjectType->GetPreviewSpriteSheetId();

		//Default to first sprite sheet in actor
		if (spriteSheetId == InvalidSpriteSheetId)
		{
			if (const Actor* spriteActor = m_project.GetActor(gameObjectType->GetSpriteActorId()))
			{
				if (!spriteActor->GetSpriteSheet(spriteSheetId) && spriteActor->GetSpriteSheetCount() > 0)
				{
					spriteSheetId = spriteActor->GetSpriteSheets().begin()->first;
				}
			}
		}

		if(spriteSheetId != InvalidSpriteSheetId)
		{
			//Render spriteSheet
			RenderResources::SpriteSheetRenderResources* spriteSheetResources = m_renderResources.GetSpriteSheetResources(spriteSheetId);
			ion::debug::Assert(spriteSheetResources, "MapPanel::RenderGameObjects() - Missing spriteSheet render resources");
			ion::debug::Assert(spriteSheetResources->m_frames.size() > 0, "MapPanel::RenderGameObjects() - SpriteSheet contains no frames");

			ion::render::Primitive* spriteSheetPrimitive = spriteSheetResources->m_primitive;
			ion::render::Material* spriteSheetMaterial = spriteSheetResources->m_frames[0].material;
			spriteSheetMaterial->SetDiffuseColour(colour);

			ion::Matrix4 spriteSheetMtx;
			spriteSheetMtx.SetTranslation(previewPos);

			renderer.BindMaterial(*material, spriteSheetMtx, cameraInverseMtx, projectionMtx);
			renderer.DrawVertexBuffer(spriteSheetPrimitive->GetVertexBuffer(), spriteSheetPrimitive->GetIndexBuffer());
			renderer.UnbindMaterial(*material);
		}

		renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);
	}
}

void MapPanel::RenderPaintPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_previewTile != InvalidTileId)
	{
		const Map& map = m_project.GetEditingMap();
		const float mapWidth = map.GetWidth();
		const float mapHeight = map.GetHeight();
		const float tileWidth = m_project.GetPlatformConfig().tileWidth;
		const float tileHeight = m_project.GetPlatformConfig().tileHeight;

		ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileQuad);
		ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTileset);
		const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourHighlight);

		//Set preview quad texture coords
		ion::render::TexCoord coords[4];
		u32 flipFlags = (m_previewTileFlipX ? Map::eFlipX : 0) | (m_previewTileFlipY ? Map::eFlipY : 0);
		m_renderResources.GetTileTexCoords(m_previewTile, coords, flipFlags);
		((ion::render::Quad*)primitive)->SetTexCoords(coords);

		ion::Matrix4 previewQuadMtx;
		ion::Vector3 previewQuadPos(((m_previewTilePos.x - (mapWidth / 2) + 0.5f) * tileWidth),
			((mapHeight - 1 - m_previewTilePos.y - (mapHeight / 2) + 0.5f) * tileHeight), z);
		previewQuadMtx.SetTranslation(previewQuadPos);

		material->SetDiffuseColour(colour);
		renderer.BindMaterial(*material, previewQuadMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		renderer.UnbindMaterial(*material);
	}
}

void MapPanel::RenderBoxSelection(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	const Map& map = m_project.GetEditingMap();
	const float mapWidth = map.GetWidth();
	const float mapHeight = map.GetHeight();
	const float tileWidth = m_project.GetPlatformConfig().tileWidth;
	const float tileHeight = m_project.GetPlatformConfig().tileHeight;

	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
	const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourSelected);

	if(m_boxSelectStart.x >= 0 && m_boxSelectEnd.x >= 0)
	{
		//Draw overlay over box selection
		ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveUnitQuad);

		const float x = ion::maths::Min(m_boxSelectStart.x, m_boxSelectEnd.x);
		const float y = ion::maths::Min(m_boxSelectStart.y, m_boxSelectEnd.y);
		const float y_inv = (mapHeight * tileHeight) - 1 - y;
		const float width = (float)abs(m_boxSelectEnd.x - m_boxSelectStart.x) + 1.0f;
		const float height = (float)abs(m_boxSelectEnd.y - m_boxSelectStart.y) + 1.0f;
		const ion::Vector2 mapCentre((mapWidth * tileWidth) / 2.0f, (mapHeight * tileHeight) / 2.0f);
		const ion::Vector2 boxCentre(width / 2.0f, height / 2.0f);

		ion::Vector3 boxScale(width, height, 0.0f);
		ion::Vector3 boxPos(floor((x - mapCentre.x + boxCentre.x)), floor((y_inv - mapCentre.y - boxCentre.y)), z);

		ion::Matrix4 boxMtx;
		boxMtx.SetTranslation(boxPos);
		boxMtx.SetScale(boxScale);

		renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
		material->SetDiffuseColour(colour);
		renderer.BindMaterial(*material, boxMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		renderer.UnbindMaterial(*material);
		renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);
	}
	else if(m_selectedTiles.size() > 0)
	{
		//Draw overlay over selected tiles
		ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileQuad);

		ion::Matrix4 selectionMtx;
		ion::Matrix4 worldViewProjMtx;
		ion::render::Shader* shader = m_renderResources.GetShader(RenderResources::eShaderFlatColour);
		ion::render::Shader::ParamHndl<ion::Matrix4> worldViewProjParamV = shader->CreateParamHndl<ion::Matrix4>("gWorldViewProjectionMatrix");

		renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
		material->SetDiffuseColour(colour);
		renderer.BindMaterial(*material, selectionMtx, cameraInverseMtx, projectionMtx);

		for(int i = 0; i < m_selectedTiles.size(); i++)
		{
			const float x = m_selectedTiles[i].x;
			const float y = m_selectedTiles[i].y;
			const float y_inv = mapHeight - 1 - y;

			ion::Vector3 selectedQuadPos(((x - (mapWidth / 2) + 0.5f) * tileWidth),
				((y_inv - (mapHeight / 2) + 0.5f) * tileHeight), z);

			selectionMtx.SetTranslation(selectedQuadPos);
			worldViewProjMtx = selectionMtx * cameraInverseMtx * projectionMtx;
			worldViewProjParamV.SetValue(worldViewProjMtx);

			renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		}

		renderer.UnbindMaterial(*material);
		renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);
	}
}

void MapPanel::RenderStampPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_stampPreviewPrimitive)
	{
		//Draw temp cloning stamp, else draw current paint stamp
		Stamp* stamp = m_tempStamp ? m_tempStamp : m_project.GetStamp(m_project.GetPaintStamp());
		if(stamp)
		{
			const Map& map = m_project.GetEditingMap();
			const float mapWidth = map.GetWidth();
			const float mapHeight = map.GetHeight();
			const float tileWidth = m_project.GetPlatformConfig().tileWidth;
			const float tileHeight = m_project.GetPlatformConfig().tileHeight;

			//Draw clipboard tiles
			float x = m_stampPastePos.x;
			float y_inv = mapHeight - 1 - m_stampPastePos.y;
			float width = stamp->GetWidth();
			float height_inv = -stamp->GetHeight();

			ion::Vector3 clonePreviewScale(m_previewTileFlipX ? -1.0f : 1.0f, m_previewTileFlipY ? -1.0f : 1.0f, 1.0f);
			ion::Matrix4 clonePreviewMtx;
			ion::Vector3 clonePreviewPos(floor((x - (mapWidth / 2.0f) + (width / 2.0f)) * tileWidth),
				floor((y_inv - (mapHeight / 2.0f) + ((height_inv / 2.0f) + 1.0f)) * tileHeight), z);
			clonePreviewMtx.SetTranslation(clonePreviewPos);
			clonePreviewMtx.SetScale(clonePreviewScale);

			renderer.SetFaceCulling(ion::render::Renderer::CullingMode::None);
			renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
			renderer.SetDepthTest(ion::render::Renderer::DepthTest::Always);

			ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTileset);
			const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourPreview);

			material->SetDiffuseColour(colour);
			renderer.BindMaterial(*material, clonePreviewMtx, cameraInverseMtx, projectionMtx);
			renderer.DrawVertexBuffer(m_stampPreviewPrimitive->GetVertexBuffer(), m_stampPreviewPrimitive->GetIndexBuffer());
			renderer.UnbindMaterial(*material);

			renderer.SetDepthTest(ion::render::Renderer::DepthTest::LessOrEqual);
			renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);
			renderer.SetFaceCulling(ion::render::Renderer::CullingMode::CounterClockwise);
		}
	}
}

void MapPanel::RenderStampOutlines(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	ion::Matrix4 worldViewProjMtx;
	ion::Matrix4 outlineMtx;
	ion::render::Shader* shader = m_renderResources.GetShader(RenderResources::eShaderFlatColour);
	ion::render::Shader::ParamHndl<ion::Matrix4> worldViewProjParamV = shader->CreateParamHndl<ion::Matrix4>("gWorldViewProjectionMatrix");

	ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileLineQuad);
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
	const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourOutline);

	const Map& map = m_project.GetEditingMap();
	const ion::Vector2i mapSize(map.GetWidth(), map.GetHeight());

	material->SetDiffuseColour(colour);
	renderer.BindMaterial(*material, outlineMtx, cameraInverseMtx, projectionMtx);

	for(TStampPosMap::const_iterator it = m_project.GetEditingMap().StampsBegin(), end = m_project.GetEditingMap().StampsEnd(); it != end; ++it)
	{
		Stamp* stamp = m_project.GetStamp(it->m_id);
		if(stamp)
		{
			outlineMtx = m_renderResources.CalcBoxMatrix(it->m_position, ion::Vector2i(stamp->GetWidth(), stamp->GetHeight()), mapSize, z);
			worldViewProjMtx = outlineMtx * cameraInverseMtx * projectionMtx;
			worldViewProjParamV.SetValue(worldViewProjMtx);

			renderer.DrawVertexBuffer(primitive->GetVertexBuffer());
		}
	}

	renderer.UnbindMaterial(*material);
}

void MapPanel::RenderPhysicsWorldOutline(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	ion::Matrix4 worldViewProjMtx;
	ion::Matrix4 outlineMtx;
	ion::render::Shader* shader = m_renderResources.GetShader(RenderResources::eShaderFlatColour);
	ion::render::Shader::ParamHndl<ion::Matrix4> worldViewProjParamV = shader->CreateParamHndl<ion::Matrix4>("gWorldViewProjectionMatrix");

	ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileLineQuad);
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
	const ion::Colour colourTiles(0.0f, 0.0f, 1.0f, 1.0f);
	const ion::Colour colourBlocks(1.0f, 0.0f, 0.0f, 1.0f);

	const Map& map = m_project.GetEditingMap();
	const CollisionMap& collisionMap = m_project.GetEditingCollisionMap();
	const ion::Vector2i mapSize(map.GetWidth(), map.GetHeight());

	ion::Vector2i physicsWorldTopLeftTiles;
	ion::Vector2i physicsWorldSizeTiles;
	ion::Vector2i physicsWorldTopLeftBlocks;
	ion::Vector2i physicsWorldSizeBlocks;
	collisionMap.GetPhysicsWorldBounds(physicsWorldTopLeftTiles, physicsWorldSizeTiles);
	collisionMap.GetPhysicsWorldBoundsBlocks(physicsWorldTopLeftBlocks, physicsWorldSizeBlocks);
	physicsWorldTopLeftBlocks = physicsWorldTopLeftBlocks * ion::Vector2i(m_project.GetPlatformConfig().blockWidth, m_project.GetPlatformConfig().blockHeight);
	physicsWorldSizeBlocks = physicsWorldSizeBlocks * ion::Vector2i(m_project.GetPlatformConfig().blockWidth, m_project.GetPlatformConfig().blockHeight);

	//Tiles
	material->SetDiffuseColour(colourTiles);
	renderer.BindMaterial(*material, outlineMtx, cameraInverseMtx, projectionMtx);

	outlineMtx = m_renderResources.CalcBoxMatrix(physicsWorldTopLeftTiles, physicsWorldSizeTiles, mapSize, z);
	worldViewProjMtx = outlineMtx * cameraInverseMtx * projectionMtx;
	worldViewProjParamV.SetValue(worldViewProjMtx);

	renderer.DrawVertexBuffer(primitive->GetVertexBuffer());

	renderer.UnbindMaterial(*material);

	//Blocks
	material->SetDiffuseColour(colourBlocks);
	renderer.BindMaterial(*material, outlineMtx, cameraInverseMtx, projectionMtx);

	outlineMtx = m_renderResources.CalcBoxMatrix(physicsWorldTopLeftBlocks, physicsWorldSizeBlocks, mapSize, z);
	worldViewProjMtx = outlineMtx * cameraInverseMtx * projectionMtx;
	worldViewProjParamV.SetValue(worldViewProjMtx);

	renderer.DrawVertexBuffer(primitive->GetVertexBuffer());

	renderer.UnbindMaterial(*material);
}

void MapPanel::RenderDisplayFrame(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveScreenLineQuad);
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
	const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourDisplayFrame);

	const int screenWidth = m_project.GetPlatformConfig().screenWidth;
	const int screenHeight = m_project.GetPlatformConfig().screenHeight;

	renderer.SetLineWidth(5.0f);

	ion::Matrix4 gridMtx;
	gridMtx.SetTranslation(ion::Vector3(m_panelSize.x / 2, m_panelSize.y / 2, 0.0f));
	gridMtx.SetScale(ion::Vector3(m_cameraZoom, m_cameraZoom, 1.0f));
	material->SetDiffuseColour(colour);
	renderer.BindMaterial(*material, gridMtx, ion::Matrix4(), projectionMtx);
	renderer.DrawVertexBuffer(primitive->GetVertexBuffer());
	renderer.UnbindMaterial(*material);

	renderer.SetLineWidth(1.0f);
}

void MapPanel::RenderStampSelection(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_hoverStamp)
	{
		Stamp* stamp = m_project.GetStamp(m_hoverStamp);
		if(stamp)
		{
			const Map& map = m_project.GetEditingMap();
			const float mapWidth = map.GetWidth();
			const float mapHeight = map.GetHeight();
			const float tileWidth = m_project.GetPlatformConfig().tileWidth;
			const float tileHeight = m_project.GetPlatformConfig().tileHeight;

			ion::Vector2i hoverStampEnd = m_hoverStampPos + ion::Vector2i(stamp->GetWidth() - 1, stamp->GetHeight() - 1);

			float bottom = ion::maths::Min(mapHeight - 1 - m_hoverStampPos.y, mapHeight - 1 - hoverStampEnd.y);
			float left = ion::maths::Min(m_hoverStampPos.x, hoverStampEnd.x);

			ion::Matrix4 boxMtx;
			ion::Vector3 boxScale((float)abs(hoverStampEnd.x - m_hoverStampPos.x) + 1.0f, (float)abs(hoverStampEnd.y - m_hoverStampPos.y) + 1.0f, 0.0f);
			ion::Vector3 boxPos(floor((left - (mapWidth / 2.0f) + (boxScale.x / 2.0f)) * tileWidth),
				floor((bottom - (mapHeight / 2.0f) + (boxScale.y / 2.0f)) * tileHeight), z);

			boxMtx.SetTranslation(boxPos);
			boxMtx.SetScale(boxScale);

			ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileQuad);
			ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
			const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourHighlight);

			renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
			material->SetDiffuseColour(colour);
			renderer.BindMaterial(*material, boxMtx, cameraInverseMtx, projectionMtx);
			renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
			renderer.UnbindMaterial(*material);
			renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);
		}
	}
}

void MapPanel::RenderStampGrid(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//Draw grid
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
	const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourOutline);

	ion::Matrix4 gridMtx;
	gridMtx.SetTranslation(ion::Vector3(0.0f, 0.0f, z));
	gridMtx.SetScale(ion::Vector3((float)m_project.GetPlatformConfig().stampWidth, (float)m_project.GetPlatformConfig().stampHeight, 1.0f));
	material->SetDiffuseColour(colour);
	renderer.BindMaterial(*material, gridMtx, cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_gridPrimitive->GetVertexBuffer());
	renderer.UnbindMaterial(*material);
}

void MapPanel::PaintMap(const Map& map)
{
	int mapWidth = map.GetWidth();
	int mapHeight = map.GetHeight();

	//Paint all tiles
	for(int y = 0; y < mapHeight; y++)
	{
		for(int x = 0; x < mapWidth; x++)
		{
			//Get tile
			TileId tileId = map.GetTile(x, y);

			//Invert Y for OpenGL
			int yInv = mapHeight - 1 - y;

			//Get V/H flip
			u32 tileFlags = map.GetTileFlags(x, y);

			//Paint tile
			PaintTile(tileId, x, yInv, tileFlags);
		}
	}

	//Paint all stamps
	for(TStampPosMap::const_iterator it = map.StampsBegin(), end = map.StampsEnd(); it != end; ++it)
	{
		if(Stamp* stamp = m_project.GetStamp(it->m_id))
		{
			PaintStamp(*stamp, it->m_position.x, it->m_position.y, it->m_flags);
		}
	}
}

void MapPanel::PaintCollisionMap(const Map& map, const CollisionMap& collisionMap)
{
	int mapWidth = map.GetWidth();
	int mapHeight = map.GetHeight();

	//Paint all collision tiles
	for(int y = 0; y < mapHeight; y++)
	{
		for(int x = 0; x < mapWidth; x++)
		{
			//Get terrain tile and collision flags
			TerrainTileId terrainTileId = collisionMap.GetTerrainTile(x, y);
			u16 collisionFlags = collisionMap.GetCollisionTileFlags(x, y);

			//Invert Y for OpenGL
			int yInv = mapHeight - 1 - y;

			//Paint tile
			PaintCollisionTile(terrainTileId, x, yInv, collisionFlags);
		}
	}

#if BEEHIVE_FIXED_STAMP_MODE
	//Paint all stamps
	for (TStampPosMap::const_iterator it = map.StampsBegin(), end = map.StampsEnd(); it != end; ++it)
	{
		if (Stamp* stamp = m_project.GetStamp(it->m_id))
		{
			PaintStampCollision(*stamp, it->m_position.x, it->m_position.y, it->m_flags);
		}
	}
#endif
}

void MapPanel::PaintTerrainBeziers(Project& project)
{
	for(int i = 0; i < m_primitiveBeziers.size(); i++)
	{
		delete m_primitiveBeziers[i];
	}

	m_primitiveBeziers.clear();

	if(m_primitiveBezierPoints)
	{
		delete m_primitiveBezierPoints;
		m_primitiveBezierPoints = NULL;
	}

	if(m_primitiveBezierHandles)
	{
		delete m_primitiveBezierHandles;
		m_primitiveBezierHandles = NULL;
	}

	for(int i = 0; i < project.GetEditingCollisionMap().GetNumTerrainBeziers(); i++)
	{
		ion::gamekit::BezierPath* bezier = project.GetEditingCollisionMap().GetTerrainBezier(i);
		m_primitiveBeziers.push_back(m_renderResources.CreateBezierPrimitive(*bezier));
	}

	if(m_currentBezier && m_currentBezier->GetNumPoints() > 0)
	{
		m_primitiveBezierPoints = m_renderResources.CreateBezierPointsPrimitive(*m_currentBezier, 2.0f);
		m_primitiveBezierHandles = m_renderResources.CreateBezierHandlesPrimitive(*m_currentBezier, 1.0f);
	}
}

void MapPanel::CreateStampPreview(Stamp* stamp)
{
	if(m_stampPreviewPrimitive)
		delete m_stampPreviewPrimitive;

	const int width = stamp->GetWidth();
	const int height = stamp->GetHeight();
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	m_stampPreviewPrimitive = new ion::render::Chessboard(ion::render::Chessboard::Axis::xy, ion::Vector2(width * (tileWidth / 2.0f), height * (tileHeight / 2.0f)), width, height, true);

	//Set primitive UV coords
	ion::render::TexCoord coords[4];

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			TileId tileId = stamp->GetTile(x, y);
			u32 tileFlags = stamp->GetTileFlags(x, y);
			m_renderResources.GetTileTexCoords(tileId, coords, tileFlags);
			int y_inv = height - 1 - y;
			m_stampPreviewPrimitive->SetTexCoords((y_inv * width) + x, coords);
		}
	}

	m_stampPreviewPrimitive->GetVertexBuffer().CommitBuffer();
}