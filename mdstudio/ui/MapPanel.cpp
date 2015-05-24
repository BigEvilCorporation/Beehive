///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "MapPanel.h"
#include "MainWindow.h"
#include "TileRendering.h"

MapPanel::MapPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, ion::render::Texture* tilesetTexture, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, renderer, glContext, tilesetTexture, parent, winid, pos, size, style, name)
{
	
	m_currentTool = eToolPaintTile;
	m_tempStamp = NULL;
	m_stampPreviewPrimitive = NULL;
	m_stampPastePos.x = -1;
	m_stampPastePos.y = -1;

	ResetToolData();

	//Colours
	m_previewColour = ion::Colour(1.0f, 1.0f, 1.0f, 1.0f);
	m_boxSelectColour = ion::Colour(0.1f, 0.5f, 0.7f, 0.8f);
	m_clonePreviewColour = ion::Colour(0.7f, 0.7f, 0.7f, 1.0f);

	//Load shaders
	m_selectionVertexShader = ion::render::Shader::Create();
	if(!m_selectionVertexShader->Load("shaders/flat_v.ion.shader"))
	{
		ion::debug::Error("Error loading vertex shader");
	}

	m_selectionPixelShader = ion::render::Shader::Create();
	if(!m_selectionPixelShader->Load("shaders/flat_p.ion.shader"))
	{
		ion::debug::Error("Error loading pixel shader");
	}

	//Create selection material
	m_selectionMaterial = new ion::render::Material();
	m_selectionMaterial->SetDiffuseColour(m_boxSelectColour);
	m_selectionMaterial->SetVertexShader(m_selectionVertexShader);
	m_selectionMaterial->SetPixelShader(m_selectionPixelShader);

	//Create preview quad
	m_previewPrimitive = new ion::render::Quad(ion::render::Quad::xy, ion::Vector2(4.0f, 4.0f));
}

MapPanel::~MapPanel()
{
	ResetToolData();

	delete m_selectionMaterial;
	delete m_previewPrimitive;
	delete m_selectionVertexShader;
	delete m_selectionPixelShader;
}

void MapPanel::OnMouse(wxMouseEvent& event, const ion::Vector2& mouseDelta)
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
	ViewPanel::OnResize(event);
}

void MapPanel::OnMouseTileEvent(ion::Vector2 mouseDelta, int buttonBits, int x, int y)
{
	Map& map = m_project->GetMap();
	Tileset& tileset = m_project->GetTileset();

	const int mapWidth = map.GetWidth();
	const int mapHeight = map.GetHeight();

	//Invert for OpenGL
	int y_inv = (mapHeight - 1 - y);

	//Check in map range
	if((x >= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight))
	{
		switch(m_currentTool)
		{
			case eToolPaintTile:
			{
				//Cannot paint over a stamp
				ion::Vector2i stampTopLeft;
				u32 stampFlags = 0;
				if(map.FindStamp(x, y, stampTopLeft, stampFlags) == InvalidStampId)
				{
					//Update paint preview tile
					m_previewTile = m_project->GetPaintTile();

					//Update preview tile pos
					m_previewTilePos.x = x;
					m_previewTilePos.y = y;

					//If clicking/dragging, paint tile
					if(buttonBits)
					{
						//Get tile ID to paint
						TileId tileId = InvalidTileId;
						if(buttonBits & eMouseLeft)
							tileId = m_project->GetPaintTile();
						else if(buttonBits & eMouseRight)
							tileId = m_project->GetEraseTile();

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
				
				break;
			}

			case eToolSelectTiles:
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

							//Start box selection, in case next event is dragging
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
				break;
			}

			case eToolSelectStamp:
			case eToolStampPicker:
				{
					//Find stamp under cursor
					ion::Vector2i stampPos;
					u32 stampFlags = 0;
					StampId stampId = map.FindStamp(x, y, stampPos, stampFlags);

					m_hoverStamp = stampId;
					m_hoverStampPos = stampPos;

					if(buttonBits & eMouseLeft)
					{
						m_selectedStamp = m_hoverStamp;

						if(m_currentTool == eToolSelectStamp)
						{
							//Clear current selection and reset box
							m_selectedTiles.clear();
							m_boxSelectStart.x = -1;
							m_boxSelectStart.y = -1;
							m_boxSelectEnd.x = -1;
							m_boxSelectEnd.y = -1;

							if(Stamp* stamp = m_project->GetStamp(stampId))
							{
								//Set box selection
								m_boxSelectStart.x = stampPos.x;
								m_boxSelectStart.y = stampPos.y;
								m_boxSelectEnd.x = stampPos.x + stamp->GetWidth() - 1;
								m_boxSelectEnd.y = stampPos.y + stamp->GetHeight() - 1;

								//Add all tiles in stamp to selection
								for(int tileX = stampPos.x; tileX < stamp->GetWidth(); tileX++)
								{
									for(int tileY = stampPos.y; tileY < stamp->GetHeight(); tileY++)
									{
										m_selectedTiles.push_back(ion::Vector2i(tileX, tileY));
									}
								}
							}
						}

						if(m_currentTool == eToolStampPicker)
						{
							//Set as paint stamp
							m_project->SetPaintStamp(stampId);

							//Set stamp tool
							SetTool(eToolPaintStamp);

							//TODO: Update tileset panel selection + toolbox button state
						}
					}

					Refresh();
				}
				break;

			case eToolTilePicker:
			{
				if(buttonBits & eMouseLeft)
				{
					//Pick tile
					TileId tile = map.GetTile(x, y);

					//Set as paint tile
					m_project->SetPaintTile(tile);

					//Set as preview tile
					m_previewTile = tile;

					//Set paint tool
					SetTool(eToolPaintTile);
				}

				//TODO: Update tileset panel selection + toolbox button state
				break;
			}

			case eToolFlipX:
			{
				//Flip preview opposite of current tile
				m_previewTile = map.GetTile(x, y);
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

				break;
			}

			case eToolFlipY:
			{
				//Flip preview opposite of current tile
				m_previewTile = map.GetTile(x, y);
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
				break;
			}

			case eToolPaintStamp:
			{
				//Paint temp cloning stamp, else Paint current paint stamp
				Stamp* stamp = m_tempStamp ? m_tempStamp : m_project->GetStamp(m_project->GetPaintStamp());
				if(stamp)
				{
					//Update paste pos
					m_stampPastePos.x = x;
					m_stampPastePos.y = y;

					//Clamp to stamp size
					if(m_stampPastePos.x + stamp->GetWidth() > mapWidth)
						m_stampPastePos.x = mapWidth - stamp->GetWidth();
					if(m_stampPastePos.y + stamp->GetHeight() > mapHeight)
						m_stampPastePos.y = mapHeight - stamp->GetHeight();

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
			}
		}
	}
	else
	{
		//Mouse of of map range, invalidate preview tile
		m_previewTile = InvalidTileId;
		m_stampPastePos.x = -1;
		m_stampPastePos.y = -1;
	}

	//Refresh
	Refresh();
}

void MapPanel::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	//Render map
	RenderCanvas(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

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

	//Render grid
	if(m_project->GetShowGrid())
	{
		RenderGrid(m_renderer, cameraInverseMtx, projectionMtx, z);
	}
}

void MapPanel::SetProject(Project* project)
{
	//Creates tileset texture
	ViewPanel::SetProject(project);

	Map& map = project->GetMap();
	Tileset& tileset = project->GetTileset();
	int mapWidth = map.GetWidth();
	int mapHeight = map.GetHeight();

	//Create canvas
	CreateCanvas(mapWidth, mapHeight);

	//Create grid
	CreateGrid(mapWidth, mapHeight, mapWidth / m_project->GetGridSize(), mapHeight / m_project->GetGridSize());

	//Redraw map
	PaintMap(m_project->GetMap());
}

void MapPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(m_project)
	{
		//If map invalidated
		if(m_project->MapIsInvalidated())
		{
			Map& map = m_project->GetMap();
			Tileset& tileset = m_project->GetTileset();
			int mapWidth = map.GetWidth();
			int mapHeight = map.GetHeight();

			//Recreate canvas
			CreateCanvas(mapWidth, mapHeight);

			//Recreate grid
			CreateGrid(mapWidth, mapHeight, mapWidth / m_project->GetGridSize(), mapHeight / m_project->GetGridSize());

			//Redraw map
			PaintMap(map);
		}
	}

	ViewPanel::Refresh(eraseBackground, rect);
}

void MapPanel::SetTool(Tool tool)
{
	Tool previousTool = m_currentTool;
	m_currentTool = tool;

	if(m_project)
	{
		Map& map = m_project->GetMap();

		switch(tool)
		{
		case eToolFill:
			//If previous tool was selectTiles or selectStamp, fill selection and leave previous tool data
			//TODO: Should this really be a tool? Doesn't follow the same rules as the others
			//(it's a single action, rather than a state which requires interaction from the user via the map)
			if(previousTool == eToolSelectTiles || previousTool == eToolSelectStamp)
			{
				if(m_project->GetPaintTile() != InvalidTileId)
				{
					//Fill selection
					FillTiles(m_project->GetPaintTile(), m_selectedTiles);

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

		case eToolPaintTile:
			ResetToolData();
			m_previewTile = m_project->GetPaintTile();
			break;

		case eToolPaintStamp:
		{
			//If coming from stamp clone tool, don't wipe preview data
			if(previousTool != eToolClone)
			{
				ResetToolData();
			}

			//Get temp cloning stamp, else get current painting stamp
			Stamp* stamp = m_tempStamp ? m_tempStamp : m_project->GetStamp(m_project->GetPaintStamp());
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
						StampId stampId = m_project->AddStamp(width, height);
						stamp = m_project->GetStamp(stampId);
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
						TileId tileId = map.GetTile(mapX, mapY);
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
}

void MapPanel::RenderPaintPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_previewTile)
	{
		const Map& map = m_project->GetMap();
		const int mapWidth = map.GetWidth();
		const int mapHeight = map.GetHeight();
		const int tileWidth = 8;
		const int tileHeight = 8;
		const int quadHalfExtentsX = 4;
		const int quadHalfExtentsY = 4;

		//Set preview quad texture coords
		ion::render::TexCoord coords[4];
		u32 flipFlags = (m_previewTileFlipX ? Map::eFlipX : 0) | (m_previewTileFlipY ? Map::eFlipY : 0);
		m_mainWindow->GetTileTexCoords(m_previewTile, coords, flipFlags);
		m_previewPrimitive->SetTexCoords(coords);

		ion::Matrix4 previewQuadMtx;
		ion::Vector3 previewQuadPos(((m_previewTilePos.x - (mapWidth / 2)) * tileWidth) + quadHalfExtentsX,
			((mapHeight - 1 - m_previewTilePos.y - (mapHeight / 2)) * tileHeight) + quadHalfExtentsY, z);
		previewQuadMtx.SetTranslation(previewQuadPos);

		m_canvasMaterial->SetDiffuseColour(m_previewColour);
		m_canvasMaterial->Bind(previewQuadMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(m_previewPrimitive->GetVertexBuffer(), m_previewPrimitive->GetIndexBuffer());
		m_canvasMaterial->Unbind();
	}
}

void MapPanel::RenderTileSelection(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	const Map& map = m_project->GetMap();
	const int mapWidth = map.GetWidth();
	const int mapHeight = map.GetHeight();
	const int tileWidth = 8;
	const int tileHeight = 8;
	const int quadHalfExtentsX = 4;
	const int quadHalfExtentsY = 4;

	if(m_boxSelectStart.x >= 0 && m_boxSelectEnd.x >= 0)
	{
		//Draw overlay over box selection
		float bottom = min(mapHeight - 1 - m_boxSelectStart.y, mapHeight - 1 - m_boxSelectEnd.y);
		float left = min(m_boxSelectStart.x, m_boxSelectEnd.x);

		ion::Matrix4 boxMtx;
		ion::Vector3 boxScale((float)abs(m_boxSelectEnd.x - m_boxSelectStart.x) + 1.0f, (float)abs(m_boxSelectEnd.y - m_boxSelectStart.y) + 1.0f, 0.0f);
		ion::Vector3 boxPos(floor((left - (mapWidth / 2.0f) + (boxScale.x / 2.0f)) * tileWidth),
			floor((bottom - (mapHeight / 2.0f) + (boxScale.y / 2.0f)) * tileHeight), z);

		boxMtx.SetTranslation(boxPos);
		boxMtx.SetScale(boxScale);

		renderer.SetAlphaBlending(ion::render::Renderer::Translucent);
		m_selectionMaterial->Bind(boxMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(m_previewPrimitive->GetVertexBuffer(), m_previewPrimitive->GetIndexBuffer());
		m_selectionMaterial->Unbind();
		renderer.SetAlphaBlending(ion::render::Renderer::NoBlend);
	}
	else if(m_selectedTiles.size() > 0)
	{
		//Draw overlay over selected tiles
		ion::Matrix4 selectionMtx;
		ion::Matrix4 worldViewProjMtx;
		ion::render::Shader::ParamHndl<ion::Matrix4> worldViewProjParamV = m_selectionVertexShader->CreateParamHndl<ion::Matrix4>("gWorldViewProjectionMatrix");

		renderer.SetAlphaBlending(ion::render::Renderer::Translucent);
		m_selectionMaterial->Bind(selectionMtx, cameraInverseMtx, projectionMtx);

		for(int i = 0; i < m_selectedTiles.size(); i++)
		{
			int x = m_selectedTiles[i].x;
			int y = m_selectedTiles[i].y;
			int y_inv = mapHeight - 1 - y;

			ion::Vector3 selectedQuadPos(((x - (mapWidth / 2)) * tileWidth) + quadHalfExtentsX,
				((y_inv - (mapHeight / 2)) * tileHeight) + quadHalfExtentsY, z);

			selectionMtx.SetTranslation(selectedQuadPos);
			worldViewProjMtx = selectionMtx * cameraInverseMtx * projectionMtx;
			worldViewProjParamV.SetValue(worldViewProjMtx);

			renderer.DrawVertexBuffer(m_previewPrimitive->GetVertexBuffer(), m_previewPrimitive->GetIndexBuffer());
		}

		m_selectionMaterial->Unbind();
		renderer.SetAlphaBlending(ion::render::Renderer::NoBlend);
	}
}

void MapPanel::RenderStampPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_stampPreviewPrimitive && m_stampPastePos.x >= 0 && m_stampPastePos.y >= 0)
	{
		//Draw temp cloning stamp, else draw current paint stamp
		Stamp* stamp = m_tempStamp ? m_tempStamp : m_project->GetStamp(m_project->GetPaintStamp());
		if(stamp)
		{
			const Map& map = m_project->GetMap();
			const int mapWidth = map.GetWidth();
			const int mapHeight = map.GetHeight();
			const int tileWidth = 8;
			const int tileHeight = 8;

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

			renderer.SetFaceCulling(ion::render::Renderer::NoCull);
			renderer.SetAlphaBlending(ion::render::Renderer::Translucent);
			renderer.SetDepthTest(ion::render::Renderer::Always);

			m_canvasMaterial->SetDiffuseColour(m_clonePreviewColour);
			m_canvasMaterial->Bind(clonePreviewMtx, cameraInverseMtx, projectionMtx);
			renderer.DrawVertexBuffer(m_stampPreviewPrimitive->GetVertexBuffer(), m_stampPreviewPrimitive->GetIndexBuffer());
			m_canvasMaterial->Unbind();
			m_canvasMaterial->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));

			renderer.SetDepthTest(ion::render::Renderer::LessEqual);
			renderer.SetAlphaBlending(ion::render::Renderer::NoBlend);
			renderer.SetFaceCulling(ion::render::Renderer::CounterClockwise);
		}
	}
}

void MapPanel::RenderStampSelection(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_hoverStamp)
	{
		Stamp* stamp = m_project->GetStamp(m_hoverStamp);
		if(stamp)
		{
			const Map& map = m_project->GetMap();
			const int mapWidth = map.GetWidth();
			const int mapHeight = map.GetHeight();
			const int tileWidth = 8;
			const int tileHeight = 8;

			ion::Vector2i hoverStampEnd = m_hoverStampPos + ion::Vector2i(stamp->GetWidth() - 1, stamp->GetHeight() - 1);

			float bottom = min(mapHeight - 1 - m_hoverStampPos.y, mapHeight - 1 - hoverStampEnd.y);
			float left = min(m_hoverStampPos.x, hoverStampEnd.x);

			ion::Matrix4 boxMtx;
			ion::Vector3 boxScale((float)abs(hoverStampEnd.x - m_hoverStampPos.x) + 1.0f, (float)abs(hoverStampEnd.y - m_hoverStampPos.y) + 1.0f, 0.0f);
			ion::Vector3 boxPos(floor((left - (mapWidth / 2.0f) + (boxScale.x / 2.0f)) * tileWidth),
				floor((bottom - (mapHeight / 2.0f) + (boxScale.y / 2.0f)) * tileHeight), z);

			boxMtx.SetTranslation(boxPos);
			boxMtx.SetScale(boxScale);

			renderer.SetAlphaBlending(ion::render::Renderer::Translucent);
			m_selectionMaterial->Bind(boxMtx, cameraInverseMtx, projectionMtx);
			renderer.DrawVertexBuffer(m_previewPrimitive->GetVertexBuffer(), m_previewPrimitive->GetIndexBuffer());
			m_selectionMaterial->Unbind();
			renderer.SetAlphaBlending(ion::render::Renderer::NoBlend);
		}
	}
}

void MapPanel::PaintMap(const Map& map)
{
	int mapWidth = map.GetWidth();
	int mapHeight = map.GetHeight();

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
}

void MapPanel::CreateStampPreview(Stamp* stamp)
{
	if(m_stampPreviewPrimitive)
		delete m_stampPreviewPrimitive;

	int width = stamp->GetWidth();
	int height = stamp->GetHeight();
	m_stampPreviewPrimitive = new ion::render::Chessboard(ion::render::Chessboard::xy, ion::Vector2(width * 4.0f, height * 4.0f), width, height, true);

	//Set primitive UV coords
	ion::render::TexCoord coords[4];

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			TileId tileId = stamp->GetTile(x, y);
			u32 tileFlags = stamp->GetTileFlags(x, y);
			m_mainWindow->GetTileTexCoords(tileId, coords, tileFlags);
			int y_inv = height - 1 - y;
			m_stampPreviewPrimitive->SetTexCoords((y_inv * width) + x, coords);
		}
	}
}