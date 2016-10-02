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
#include "StampAnimEditorDialog.h"

#include <wx/Menu.h>
#include <wx/msgdlg.h>

MapPanel::MapPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, project, renderer, glContext, renderResources, parent, winid, pos, size, style, name)
{
	m_currentTool = eToolPaintTile;
	m_cursorOrigin = eCursorTopLeft;
	m_tempStamp = NULL;
	m_terrainCanvasPrimitive = NULL;
	m_collisionCanvasPrimitive = NULL;
	m_stampPreviewPrimitive = NULL;
	m_primitiveBezierPoints = NULL;
	m_primitiveBezierHandles = NULL;
	m_highlightedBezier = NULL;
	m_currentBezier = NULL;
	m_currentBezierControlIdx = -1;
	m_stampPastePos.x = -1;
	m_stampPastePos.y = -1;
	m_previewGameObjectType = InvalidGameObjectTypeId;

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

	//Redraw map
	PaintMap(project.GetEditingMap());

	//Redraw collision map
	PaintCollisionMap(project.GetEditingCollisionMap());
}

MapPanel::~MapPanel()
{
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

	if(m_currentTool == eToolPaintTile || m_currentTool == eToolPaintStamp)
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

	if(m_currentTool == eToolSelectTiles)
	{
		//Store CTRL held state for multiple selection
		m_multipleSelection = event.ControlDown();
	}

	ViewPanel::OnKeyboard(event);
}

void MapPanel::OnResize(wxSizeEvent& event)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		ViewPanel::OnResize(event);
	}
}

void MapPanel::OnMouseTileEvent(int buttonBits, int x, int y)
{
	Map& map = m_project.GetEditingMap();
	Tileset& tileset = m_project.GetTileset();

	const int mapWidth = map.GetWidth();
	const int mapHeight = map.GetHeight();

	//Invert for OpenGL
	int y_inv = (mapHeight - 1 - y);

	//Check in map range
	bool inMaprange = ((x >= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight));

	switch(m_currentTool)
	{
		case eToolPaintTile:
		{
			if(inMaprange)
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

		case eToolPaintTerrainTile:
		case eToolDeleteTerrainTile:
		{
			if(inMaprange)
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
					PaintCollisionTile(tileId, collisionMap.GetCollisionTileFlags(x, y), x, y_inv);
				}
			}

			break;
		}

		case eToolPaintCollisionSolid:
		{
			if(inMaprange)
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
					PaintCollisionTile(collisionMap.GetTerrainTile(x, y), collisionTileFlags, x, y_inv);
				}
				else if(buttonBits & eMouseRight)
				{
					//Get collision map
					CollisionMap& collisionMap = m_project.GetEditingCollisionMap();

					//Clear solid tile
					u16 collisionTileFlags = collisionMap.GetCollisionTileFlags(x, y) & ~eCollisionTileFlagSolid;
					collisionMap.SetCollisionTileFlags(x, y, collisionTileFlags);

					//Paint to canvas
					PaintCollisionTile(collisionMap.GetTerrainTile(x, y), collisionTileFlags, x, y_inv);
				}
			}

			break;
		}

		case eToolPaintCollisionHole:
		{
			if(inMaprange)
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
					PaintCollisionTile(collisionMap.GetTerrainTile(x, y), collisionTileFlags, x, y_inv);
				}
				else if(buttonBits & eMouseRight)
				{
					//Get collision map
					CollisionMap& collisionMap = m_project.GetEditingCollisionMap();

					//Clear hole tile
					u16 collisionTileFlags = collisionMap.GetCollisionTileFlags(x, y) & ~eCollisionTileFlagHole;
					collisionMap.SetCollisionTileFlags(x, y, collisionTileFlags);

					//Paint to canvas
					PaintCollisionTile(collisionMap.GetTerrainTile(x, y), collisionTileFlags, x, y_inv);
				}
			}

			break;
		}

		case eToolSelectTiles:
		{
			if(inMaprange)
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
							m_boxSelectStart.x = x;
							m_boxSelectStart.y = y;
						}

						//Single click - add tile at cursor to selection
						m_selectedTiles.push_back(ion::Vector2i(x, y));
					}
					else if(!m_multipleSelection)
					{
						//Dragging, set box end
						m_boxSelectEnd.x = x;
						m_boxSelectEnd.y = y;

						//Clear current selection
						m_selectedTiles.clear();

						//Sanitise loop order
						int top = min(m_boxSelectStart.y, m_boxSelectEnd.y);
						int left = min(m_boxSelectStart.x, m_boxSelectEnd.x);
						int bottom = max(m_boxSelectStart.y, m_boxSelectEnd.y);
						int right = max(m_boxSelectStart.x, m_boxSelectEnd.x);

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
			//Find stamp under cursor
			ion::Vector2i stampPos;
			u32 stampFlags = 0;
			u32 mapEntryIndex = 0;
			StampId stampId = map.FindStamp(x, y, stampPos, stampFlags, mapEntryIndex);

			m_hoverStamp = stampId;
			m_hoverStampPos = stampPos;
			m_hoverStampFlags = stampFlags;

			if(buttonBits & eMouseLeft && !(m_prevMouseBits & eMouseLeft))
			{
				m_selectedStamp = m_hoverStamp;

				if(m_currentTool == eToolSelectStamp)
				{
					//Reset selection box
					m_boxSelectStart.x = -1;
					m_boxSelectStart.y = -1;
					m_boxSelectEnd.x = -1;
					m_boxSelectEnd.y = -1;

					if(Stamp* stamp = m_project.GetStamp(stampId))
					{
						//Set box selection
						m_boxSelectStart.x = stampPos.x;
						m_boxSelectStart.y = stampPos.y;
						m_boxSelectEnd.x = stampPos.x + stamp->GetWidth() - 1;
						m_boxSelectEnd.y = stampPos.y + stamp->GetHeight() - 1;
					}
				}

				if(m_currentTool == eToolStampPicker)
				{
					//Set as paint stamp
					m_project.SetPaintStamp(stampId);

					//Set stamp tool
					SetTool(eToolPaintStamp);

					//TODO: Update tileset panel selection + toolbox button state
				}

				if(m_currentTool == eToolRemoveStamp)
				{
					if(Stamp* stamp = m_project.GetStamp(stampId))
					{
						//Remove stamp under cursor
						map.RemoveStamp(x, y);

						//Clear hover stamp
						m_hoverStamp = InvalidStampId;

						//Repaint area underneath stamp
						for(int tileX = stampPos.x; tileX < stampPos.x + stamp->GetWidth(); tileX++)
						{
							for(int tileY = stampPos.y; tileY < stampPos.y + stamp->GetHeight(); tileY++)
							{
								//Invert Y for OpenGL
								int y_inv = map.GetHeight() - 1 - tileY;

								//Stamps can be placed outside map boundaries, only remove tiles that are inside
								if(tileX >= 0 && tileX < mapWidth && y_inv >= 0 && y_inv < mapHeight)
								{
									TileId tileId = InvalidTileId;

									//Find stamp under cursor first
									ion::Vector2i stampPos;
									u32 flags = 0;
									u32 mapEntryIndex = 0;
									StampId stampId = map.FindStamp(tileX, tileY, stampPos, flags, mapEntryIndex);

									if(stampId)
									{
										//Get from stamp
										if(Stamp* stamp = m_project.GetStamp(stampId))
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

			if(buttonBits & eMouseRight)
			{
				if(m_hoverStamp != InvalidStampId)
				{
					//Right-click menu
					wxMenu contextMenu;

					contextMenu.Append(eContextMenuDeleteStamp, wxString("Delete stamp"));
					contextMenu.Append(eContextMenuBakeStamp, wxString("Bake stamp"));
					contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MapPanel::OnContextMenuClick, NULL, this);
					PopupMenu(&contextMenu);
				}
			}

			Refresh();
		}
		break;

		case eToolTilePicker:
		{
			if(inMaprange)
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

					//Set paint tool
					SetTool(eToolPaintTile);

					//Refresh tile edit panel
					m_mainWindow->RedrawPanel(MainWindow::ePanelTileEditor);

					//Refresh collision tile edit panel
					m_mainWindow->RedrawPanel(MainWindow::ePanelTerrainTileEditor);
				}
			}

			//TODO: Update tileset panel selection + toolbox button state
			break;
		}

		case eToolFlipX:
		{
			if(inMaprange)
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
			if(inMaprange)
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

		case eToolPaintStamp:
		{
			//Paint temp cloning stamp, else Paint current paint stamp
			Stamp* stamp = m_tempStamp ? m_tempStamp : m_project.GetStamp(m_project.GetPaintStamp());
			if(stamp)
			{
				if((buttonBits & eMouseRight) && !(m_prevMouseBits & eMouseRight))
				{
					//Switch cursor origin
					int cursorOrigin = (int)m_cursorOrigin;
					m_cursorOrigin = (CursorOrigin)(++cursorOrigin % eCursor_MAX);
				}

				//Update paste pos
				m_stampPastePos.x = x;
				m_stampPastePos.y = y;

				//Adjust for cursor origin
				switch(m_cursorOrigin)
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

				if((buttonBits & eMouseLeft) && !(m_prevMouseBits & eMouseLeft))
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

		case eToolSelectGameObject:
		case eToolAnimateGameObject:
		{
			if(inMaprange)
			{
				ion::Vector2i topLeft;
				m_hoverGameObject = m_project.GetEditingMap().FindGameObject(x, y, topLeft);

				if((buttonBits & eMouseLeft) && !(m_prevMouseBits & eMouseLeft))
				{
					m_selectedGameObject = m_hoverGameObject;

					if(GameObject* gameObject = m_project.GetEditingMap().GetGameObject(m_hoverGameObject))
					{
						m_mainWindow->SetSelectedGameObject(gameObject);

						if(m_currentTool == eToolAnimateGameObject)
						{
							m_mainWindow->SetSelectedAnimObject(m_selectedGameObject);
						}

						m_previewGameObjectType = gameObject->GetTypeId();
						m_previewGameObjectPos.x = topLeft.x;
						m_previewGameObjectPos.y = topLeft.y;
					}
				}

				if(m_currentTool == eToolAnimateGameObject)
				{
					if(buttonBits & eMouseRight)
					{
						if(m_hoverGameObject != InvalidGameObjectId)
						{
							//Right-click menu
							wxMenu contextMenu;

							contextMenu.Append(eContextMenuGameObjAddToAnim, wxString("Add to animation"));
							contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MapPanel::OnContextMenuClick, NULL, this);
							PopupMenu(&contextMenu);
						}
					}
				}
			}
			break;
		}

		case eToolPlaceGameObject:
		{
			if(inMaprange)
			{
				m_previewGameObjectType = m_project.GetPaintGameObjectType();
				m_previewGameObjectPos.x = x;
				m_previewGameObjectPos.y = y;

				if((buttonBits & eMouseLeft) && !(m_prevMouseBits & eMouseLeft))
				{
					GameObjectTypeId gameObjectTypeId = m_project.GetPaintGameObjectType();
					if(GameObjectType* gameObjectType = m_project.GetGameObjectType(gameObjectTypeId))
					{
						m_project.GetEditingMap().PlaceGameObject(x, y, *gameObjectType);
						Refresh();
					}
				}
			}
			break;
		}

		case eToolRemoveGameObject:
		{
			if(inMaprange)
			{
				if((buttonBits & eMouseLeft) && !(m_prevMouseBits & eMouseLeft))
				{
					m_project.GetEditingMap().RemoveGameObject(x, y);
					Refresh();
				}
			}
			break;
		}
	}

	if(!inMaprange && m_currentTool != eToolPaintStamp)
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
			map.RemoveStamp(m_hoverStampPos.x, m_hoverStampPos.y);

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
	else if(event.GetId() == eContextMenuBakeStamp)
	{
		if(Stamp* stamp = m_project.GetStamp(m_hoverStamp))
		{
			//Remove stamp under cursor
			map.RemoveStamp(m_hoverStampPos.x, m_hoverStampPos.y);

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
	else if(event.GetId() == eContextMenuGameObjAddToAnim)
	{
		if(m_hoverGameObject != InvalidGameObjectId)
		{
			AnimationId animId = m_mainWindow->GetSelectedAnimation();
			Animation* anim = m_project.GetAnimation(animId);
			if(anim)
			{
				anim->AddActor(m_hoverGameObject);
			}
		}
	}
}

void MapPanel::OnMousePixelEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, int buttonBits, int tileX, int tileY)
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

	switch(m_currentTool)
	{
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

					const float defaultControlLen = (mousePos.x > prevPosition.x )? 10.0f : -10.0f;

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
		{
			m_currentBezier = NULL;
			m_highlightedBezier = NULL;

			//Find bezier under cursor
			for(int i = 0; i < m_project.GetEditingCollisionMap().GetNumTerrainBeziers() && m_currentBezier == NULL && m_highlightedBezier == NULL; i++)
			{
				ion::gamekit::BezierPath* bezier = m_project.GetEditingCollisionMap().GetTerrainBezier(i);

				ion::Vector2 boundsMin;
				ion::Vector2 boundsMax;

				bezier->GetBounds(boundsMin, boundsMax);

				if(ion::maths::PointInsideBox(mousePosF, boundsMin, boundsMax))
				{
					if(buttonBits & eMouseLeft)
					{
						//Set current bezier
						m_currentBezier = bezier;

						//Set bezier draw tool
						m_currentTool = eToolDrawTerrainBezier;

						//Invalidate beziers and refresh this panel
						m_project.InvalidateTerrainBeziers(true);
						Refresh();
						m_project.InvalidateTerrainBeziers(false);
					}
					else
					{
						//Set highlighted bezier
						m_highlightedBezier = bezier;
					}
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
						PaintCollisionTile(tileId, collisionMap.GetCollisionTileFlags(tileX, tileY), tileX, y_inv);

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
												
												tileId = stamp->GetTile(sourceX, sourceY);
												flags ^= stamp->GetTileFlags(sourceX, sourceY);
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

		case eToolMoveGameObject:
		{
			if(buttonBits & eMouseLeft)
			{
				if(!(m_prevMouseBits & eMouseLeft))
				{
					ion::Vector2i topLeft;
					m_hoverGameObject = m_project.GetEditingMap().FindGameObject(tileX, tileY, topLeft);
				}

				if(m_hoverGameObject != InvalidGameObjectId)
				{
					if(GameObject* gameObject = m_project.GetEditingMap().GetGameObject(m_hoverGameObject))
					{
						const int tileWidth = m_project.GetPlatformConfig().tileWidth;
						const int tileHeight = m_project.GetPlatformConfig().tileHeight;

						m_project.GetEditingMap().MoveGameObject(m_hoverGameObject, tileX, tileY);
						gameObject->SetPosition(ion::Vector2i(tileX * tileWidth, tileY * tileHeight));
					}
				}
			}

			break;
		}

		//case eToolMoveGameObject:
		//{
		//	if(buttonBits & eMouseLeft)
		//	{
		//		ion::Vector2i topLeft;
		//		m_hoverGameObject = m_project.GetEditingMap().FindGameObject(x, y, topLeft);
		//
		//		if(m_hoverGameObject != InvalidGameObjectId)
		//		{
		//			GameObject* gameObject = m_project.GetEditingMap().GetGameObject(m_hoverGameObject);
		//			if(gameObject)
		//			{
		//				gameObject->SetAnimDrawOffset(gameObject->GetAnimDrawOffset() + mouseDelta);
		//				Refresh();
		//			}
		//		}
		//	}
		//	
		//	break;
		//}

		case eToolAnimateGameObject:
		{
			if(buttonBits & eMouseLeft)
			{
				ion::Vector2i topLeft;
				m_hoverGameObject = m_project.GetEditingMap().FindGameObject(tileX, tileY, topLeft);

				if(m_hoverGameObject != InvalidGameObjectId)
				{
					GameObject* gameObject = m_project.GetEditingMap().GetGameObject(m_hoverGameObject);
					if(gameObject)
					{
						gameObject->SetAnimDrawOffset(gameObject->GetAnimDrawOffset() + mouseDelta);
						Refresh();
					}
				}
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

	//Render selected tiles
	RenderTileSelection(renderer, cameraInverseMtx, projectionMtx, z);

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
	}

	//Render stamp outlines
	if(m_project.GetShowStampOutlines())
	{
		RenderStampOutlines(renderer, cameraInverseMtx, projectionMtx, z);
	}

	//Render display frame
	if(m_project.GetShowDisplayFrame())
	{
		RenderDisplayFrame(renderer, cameraInverseMtx, projectionMtx, z);
	}
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
			PaintCollisionMap(m_project.GetEditingCollisionMap());
		}

		//If terrain beziers invalidated
		if(m_project.TerrainBeziersAreInvalidated())
		{
			PaintTerrainBeziers(m_project);
		}

		ViewPanel::Refresh(eraseBackground, rect);
	}
}

void MapPanel::CreateCollisionCanvas(int width, int height)
{
	//Create rendering primitives
	if(m_terrainCanvasPrimitive)
		delete m_terrainCanvasPrimitive;

	if(m_collisionCanvasPrimitive)
		delete m_collisionCanvasPrimitive;

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	m_terrainCanvasPrimitive = new ion::render::Chessboard(ion::render::Chessboard::xy, ion::Vector2((float)(width * tileWidth) / 2.0f, (float)(height * tileHeight) / 2.0f), width, height, true);
	m_collisionCanvasPrimitive = new ion::render::Chessboard(ion::render::Chessboard::xy, ion::Vector2((float)(width * tileWidth) / 2.0f, (float)(height * tileHeight) / 2.0f), width, height, true);
}

void MapPanel::SetTool(ToolType tool)
{
	ToolType previousTool = m_currentTool;
	m_currentTool = tool;

	Map& map = m_project.GetEditingMap();

	switch(tool)
	{
	case eToolFill:
		//If previous tool was selectTiles, fill selection and leave previous tool data
		//TODO: Should this really be a tool? Doesn't follow the same rules as the others
		//(it's a single action, rather than a state which requires interaction from the user via the map)
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

	case eToolPaintStamp:
	{
		//If coming from stamp clone tool, don't wipe preview data
		if(previousTool != eToolClone)
		{
			ResetToolData();
		}

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

	case eToolClone:
	case eToolCreateStamp:
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

	default:
		ResetToolData();
	}
}

void MapPanel::ResetToolData()
{
	//Invalidate preview tile
	m_previewTile = InvalidTileId;
	m_previewTileFlipX = false;
	m_previewTileFlipY = false;

	//Invalidate box/multiple selection
	m_selectedTiles.clear();
	m_multipleSelection = false;
	m_boxSelectStart.x = -1;
	m_boxSelectStart.y = -1;
	m_boxSelectEnd.x = -1;
	m_boxSelectEnd.y = -1;

	//Invalidate selected stamp
	m_hoverStamp = InvalidStampId;
	m_selectedStamp = InvalidStampId;

	//Invalidate game object preview
	m_hoverGameObject = InvalidGameObjectId;
	m_selectedGameObject = InvalidGameObjectId;
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
	renderer.SetDepthTest(ion::render::Renderer::eAlways);

	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialCollisionTypes);

	//Draw map
	renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	material->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_collisionCanvasPrimitive->GetVertexBuffer(), m_collisionCanvasPrimitive->GetIndexBuffer());
	material->Unbind();
	renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);

	renderer.SetDepthTest(ion::render::Renderer::eLessEqual);
}

void MapPanel::RenderCollisionBeziers(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//No depth test
	renderer.SetDepthTest(ion::render::Renderer::eAlways);

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
		material->Bind(bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(3.0f);

		for(int i = 0; i < m_primitiveBeziers.size(); i++)
		{
			renderer.DrawVertexBuffer(m_primitiveBeziers[i]->GetVertexBuffer());
		}

		renderer.SetLineWidth(1.0f);
		material->Unbind();
	}

	//Draw curve points
	if(m_primitiveBezierPoints)
	{
		material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
		material->Bind(bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(2.0f);
		renderer.DrawVertexBuffer(m_primitiveBezierPoints->GetVertexBuffer());
		renderer.SetLineWidth(1.0f);
		material->Unbind();
	}

	//Draw curve control handles
	if(m_primitiveBezierHandles)
	{
		material->SetDiffuseColour(ion::Colour(1.0f, 0.6f, 0.6f, 1.0f));
		material->Bind(bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(2.0f);
		renderer.DrawVertexBuffer(m_primitiveBezierHandles->GetVertexBuffer());
		renderer.SetLineWidth(1.0f);
		material->Unbind();
	}

	//Draw selected handle
	if(m_currentBezierControlIdx != -1)
	{
		ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileQuad);
		const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourSelected);

		renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
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

		material->Bind(previewMtx * bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		material->Unbind();

		renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);
	}

	//Draw selected bezier
	if(m_highlightedBezier)
	{
		ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileQuad);
		const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourSelected);

		renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
		material->SetDiffuseColour(colour);

		ion::Vector2 boundsMin;
		ion::Vector2 boundsMax;

		m_highlightedBezier->GetBounds(boundsMin, boundsMax);

		ion::Vector2 size = boundsMax - boundsMin;

		const float x = boundsMin.x + (size.x / 2.0f);
		const float y = boundsMin.y + (size.y / 2.0f);

		ion::Vector3 previewScale(size.x / (float)tileWidth, size.y / (float)tileWidth, 1.0f);
		ion::Matrix4 previewMtx;
		ion::Vector3 previewPos(x, y, z);
		previewMtx.SetTranslation(previewPos);
		previewMtx.SetScale(previewScale);

		material->Bind(previewMtx * bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		material->Unbind();

		renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);
	}

	renderer.SetDepthTest(ion::render::Renderer::eLessEqual);
}

void MapPanel::RenderTerrainCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//No depth test (stops grid cells Z fighting)
	renderer.SetDepthTest(ion::render::Renderer::eAlways);

	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTerrainTileset);

	//Draw map
	renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	material->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_terrainCanvasPrimitive->GetVertexBuffer(), m_terrainCanvasPrimitive->GetIndexBuffer());
	material->Unbind();
	renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);

	renderer.SetDepthTest(ion::render::Renderer::eLessEqual);
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
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
	const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourSelected);

	renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
	material->SetDiffuseColour(colour);

	for(TGameObjectPosMap::const_iterator itMap = gameObjects.begin(), endMap = gameObjects.end(); itMap != endMap; ++itMap)
	{
		for(std::vector<GameObjectMapEntry>::const_iterator itVec = itMap->second.begin(), endVec = itMap->second.end(); itVec != endVec; ++itVec)
		{
			if(const GameObjectType* gameObjectType = m_project.GetGameObjectType(itVec->m_gameObject.GetTypeId()))
			{
				const float x = itVec->m_position.x;
				const float y_inv = mapHeight - 1 - itVec->m_position.y;
				const float width = gameObjectType->GetDimensions().x / tileWidth;
				const float height_inv = -gameObjectType->GetDimensions().y / tileHeight;

				//Render coloured box
				ion::Vector3 scale(gameObjectType->GetDimensions().x / tileWidth, gameObjectType->GetDimensions().y / tileHeight, 1.0f);
				ion::Matrix4 mtx;
				ion::Vector3 pos(floor((x - (mapWidth / 2.0f) + (width / 2.0f)) * tileWidth),
					floor((y_inv - (mapHeight / 2.0f) + ((height_inv / 2.0f) + 1.0f)) * tileHeight), z);

				pos.x += itVec->m_gameObject.GetAnimDrawOffset().x;
				pos.y -= itVec->m_gameObject.GetAnimDrawOffset().y;

				mtx.SetTranslation(pos);
				mtx.SetScale(scale);

				material->Bind(mtx, cameraInverseMtx, projectionMtx);
				renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
				material->Unbind();

				if(gameObjectType->GetPreviewSpriteSheet() != InvalidSpriteSheetId)
				{
					//Render spriteSheet
					RenderResources::SpriteSheetRenderResources* spriteSheetResources = m_renderResources.GetSpriteSheetResources(gameObjectType->GetPreviewSpriteSheet());
					ion::debug::Assert(spriteSheetResources, "MapPanel::RenderGameObjects() - Missing spriteSheet render resources");
					ion::debug::Assert(spriteSheetResources->m_frames.size() > 0, "MapPanel::RenderGameObjects() - SpriteSheet contains no frames");

					ion::render::Primitive* spriteSheetPrimitive = spriteSheetResources->m_primitive;
					ion::render::Material* spriteSheetMaterial = spriteSheetResources->m_frames[0].material;
					spriteSheetMaterial->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));

					ion::Matrix4 spriteSheetMtx;
					spriteSheetMtx.SetTranslation(pos);

					spriteSheetMaterial->Bind(spriteSheetMtx, cameraInverseMtx, projectionMtx);
					renderer.DrawVertexBuffer(spriteSheetPrimitive->GetVertexBuffer(), spriteSheetPrimitive->GetIndexBuffer());
					spriteSheetMaterial->Unbind();
				}
			}
		}
	}

	renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);
}

void MapPanel::RenderGameObjectPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(const GameObjectType* gameObjectType = m_project.GetGameObjectType(m_previewGameObjectType))
	{
		ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileQuad);
		ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
		const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourSelected);

		renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
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
		ion::Vector3 previewPos(floor((x - (mapWidth / 2.0f) + (width / 2.0f)) * tileWidth),
								floor((y_inv - (mapHeight / 2.0f) + ((height_inv / 2.0f) + 1.0f)) * tileHeight), z);
		previewMtx.SetTranslation(previewPos);
		previewMtx.SetScale(previewScale);

		material->Bind(previewMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		material->Unbind();

		if(gameObjectType->GetPreviewSpriteSheet() != InvalidSpriteSheetId)
		{
			//Render spriteSheet
			RenderResources::SpriteSheetRenderResources* spriteSheetResources = m_renderResources.GetSpriteSheetResources(gameObjectType->GetPreviewSpriteSheet());
			ion::debug::Assert(spriteSheetResources, "MapPanel::RenderGameObjects() - Missing spriteSheet render resources");
			ion::debug::Assert(spriteSheetResources->m_frames.size() > 0, "MapPanel::RenderGameObjects() - SpriteSheet contains no frames");

			ion::render::Primitive* spriteSheetPrimitive = spriteSheetResources->m_primitive;
			ion::render::Material* spriteSheetMaterial = spriteSheetResources->m_frames[0].material;
			spriteSheetMaterial->SetDiffuseColour(colour);

			ion::Matrix4 spriteSheetMtx;
			spriteSheetMtx.SetTranslation(previewPos);

			spriteSheetMaterial->Bind(spriteSheetMtx, cameraInverseMtx, projectionMtx);
			renderer.DrawVertexBuffer(spriteSheetPrimitive->GetVertexBuffer(), spriteSheetPrimitive->GetIndexBuffer());
			spriteSheetMaterial->Unbind();
		}

		renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);
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
		material->Bind(previewQuadMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		material->Unbind();
	}
}

void MapPanel::RenderTileSelection(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	const Map& map = m_project.GetEditingMap();
	const float mapWidth = map.GetWidth();
	const float mapHeight = map.GetHeight();
	const float tileWidth = m_project.GetPlatformConfig().tileWidth;
	const float tileHeight = m_project.GetPlatformConfig().tileHeight;

	ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileQuad);
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
	const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourSelected);

	if(m_boxSelectStart.x >= 0 && m_boxSelectEnd.x >= 0)
	{
		//Draw overlay over box selection
		const float bottom = min(mapHeight - 1 - m_boxSelectStart.y, mapHeight - 1 - m_boxSelectEnd.y);
		const float left = min(m_boxSelectStart.x, m_boxSelectEnd.x);

		ion::Vector3 boxScale((float)abs(m_boxSelectEnd.x - m_boxSelectStart.x) + 1.0f, (float)abs(m_boxSelectEnd.y - m_boxSelectStart.y) + 1.0f, 0.0f);
		ion::Vector3 boxPos(floor((left - (mapWidth / 2.0f) + (boxScale.x / 2.0f)) * tileWidth),
			floor((bottom - (mapHeight / 2.0f) + (boxScale.y / 2.0f)) * tileHeight), z);

		ion::Matrix4 boxMtx;
		boxMtx.SetTranslation(boxPos);
		boxMtx.SetScale(boxScale);

		renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
		material->SetDiffuseColour(colour);
		material->Bind(boxMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		material->Unbind();
		renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);
	}
	else if(m_selectedTiles.size() > 0)
	{
		//Draw overlay over selected tiles
		ion::Matrix4 selectionMtx;
		ion::Matrix4 worldViewProjMtx;
		ion::render::Shader* vertexShader = m_renderResources.GetVertexShader(RenderResources::eShaderFlatColour);
		ion::render::Shader::ParamHndl<ion::Matrix4> worldViewProjParamV = vertexShader->CreateParamHndl<ion::Matrix4>("gWorldViewProjectionMatrix");

		renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
		material->SetDiffuseColour(colour);
		material->Bind(selectionMtx, cameraInverseMtx, projectionMtx);

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

		material->Unbind();
		renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);
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

			renderer.SetFaceCulling(ion::render::Renderer::eNoCull);
			renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
			renderer.SetDepthTest(ion::render::Renderer::eAlways);

			ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTileset);
			const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourPreview);

			material->SetDiffuseColour(colour);
			material->Bind(clonePreviewMtx, cameraInverseMtx, projectionMtx);
			renderer.DrawVertexBuffer(m_stampPreviewPrimitive->GetVertexBuffer(), m_stampPreviewPrimitive->GetIndexBuffer());
			material->Unbind();

			renderer.SetDepthTest(ion::render::Renderer::eLessEqual);
			renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);
			renderer.SetFaceCulling(ion::render::Renderer::eCounterClockwise);
		}
	}
}

void MapPanel::RenderStampOutlines(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	ion::Matrix4 worldViewProjMtx;
	ion::Matrix4 outlineMtx;
	ion::render::Shader* vertexShader = m_renderResources.GetVertexShader(RenderResources::eShaderFlatColour);
	ion::render::Shader::ParamHndl<ion::Matrix4> worldViewProjParamV = vertexShader->CreateParamHndl<ion::Matrix4>("gWorldViewProjectionMatrix");

	ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveUnitLineQuad);
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
	const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourOutline);

	const Map& map = m_project.GetEditingMap();
	const ion::Vector2i mapSize(map.GetWidth(), map.GetHeight());

	material->SetDiffuseColour(colour);
	material->Bind(outlineMtx, cameraInverseMtx, projectionMtx);

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

	material->Unbind();
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
	material->Bind(gridMtx, ion::Matrix4(), projectionMtx);
	renderer.DrawVertexBuffer(primitive->GetVertexBuffer());
	material->Unbind();

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

			float bottom = min(mapHeight - 1 - m_hoverStampPos.y, mapHeight - 1 - hoverStampEnd.y);
			float left = min(m_hoverStampPos.x, hoverStampEnd.x);

			ion::Matrix4 boxMtx;
			ion::Vector3 boxScale((float)abs(hoverStampEnd.x - m_hoverStampPos.x) + 1.0f, (float)abs(hoverStampEnd.y - m_hoverStampPos.y) + 1.0f, 0.0f);
			ion::Vector3 boxPos(floor((left - (mapWidth / 2.0f) + (boxScale.x / 2.0f)) * tileWidth),
				floor((bottom - (mapHeight / 2.0f) + (boxScale.y / 2.0f)) * tileHeight), z);

			boxMtx.SetTranslation(boxPos);
			boxMtx.SetScale(boxScale);

			ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileQuad);
			ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
			const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourHighlight);

			renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
			material->SetDiffuseColour(colour);
			material->Bind(boxMtx, cameraInverseMtx, projectionMtx);
			renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
			material->Unbind();
			renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);
		}
	}
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

void MapPanel::PaintCollisionMap(const CollisionMap& map)
{
	int mapWidth = map.GetWidth();
	int mapHeight = map.GetHeight();

	//Paint all collision tiles
	for(int y = 0; y < mapHeight; y++)
	{
		for(int x = 0; x < mapWidth; x++)
		{
			//Get terrain tile and collision flags
			TerrainTileId terrainTileId = map.GetTerrainTile(x, y);
			u16 collisionFlags = map.GetCollisionTileFlags(x, y);

			//Invert Y for OpenGL
			int yInv = mapHeight - 1 - y;

			//Paint tile
			PaintCollisionTile(terrainTileId, collisionFlags, x, yInv);
		}
	}
}

void MapPanel::PaintCollisionTile(TerrainTileId terrainTileId, u16 collisionFlags, int x, int y)
{
	//Set texture coords for terrain cell
	ion::render::TexCoord coords[4];
	m_renderResources.GetTerrainTileTexCoords(terrainTileId, coords);
	m_terrainCanvasPrimitive->SetTexCoords((y * m_canvasSize.x) + x, coords);

	//Set texture coords for collision cell
	m_renderResources.GetCollisionTypeTexCoords(collisionFlags, coords);
	m_collisionCanvasPrimitive->SetTexCoords((y * m_canvasSize.x) + x, coords);
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

	m_stampPreviewPrimitive = new ion::render::Chessboard(ion::render::Chessboard::xy, ion::Vector2(width * (tileWidth / 2.0f), height * (tileHeight / 2.0f)), width, height, true);

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
}