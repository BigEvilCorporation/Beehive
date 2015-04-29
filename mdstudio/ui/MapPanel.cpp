///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include <ion/core/memory/Memory.h>

#include "MapPanel.h"
#include "TileRendering.h"

MapPanel::MapPanel(ion::io::ResourceManager& resourceManager, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxGLCanvas(parent, winid, NULL, pos, size, style, name, wxNullPalette)
	, m_resourceManager(resourceManager)
{
	m_project = NULL;
	m_mapPrimitive = NULL;
	m_gridPrimitive = NULL;
	m_cameraZoom = 1.0f;
	m_cameraPanSpeed = 1.0f;
	m_tilesetSizeSq = 1;
	m_cellSizeTexSpaceSq = 1.0f;
	m_lastMouseHoverTileX = -1;
	m_lastMouseHoverTileY = -1;
	m_keyHeldFlipX = false;
	m_keyHeldFlipY = false;
	m_panelSize = size;

	Bind(wxEVT_LEFT_DOWN,		&MapPanel::OnMouse, this, GetId());
	Bind(wxEVT_LEFT_UP,			&MapPanel::OnMouse, this, GetId());
	Bind(wxEVT_MIDDLE_DOWN,		&MapPanel::OnMouse, this, GetId());
	Bind(wxEVT_MIDDLE_UP,		&MapPanel::OnMouse, this, GetId());
	Bind(wxEVT_RIGHT_DOWN,		&MapPanel::OnMouse, this, GetId());
	Bind(wxEVT_RIGHT_UP,		&MapPanel::OnMouse, this, GetId());
	Bind(wxEVT_MOTION,			&MapPanel::OnMouse, this, GetId());
	Bind(wxEVT_MOUSEWHEEL,		&MapPanel::OnMouse, this, GetId());
	Bind(wxEVT_KEY_DOWN,		&MapPanel::OnKeyboard, this, GetId());
	Bind(wxEVT_KEY_UP,			&MapPanel::OnKeyboard, this, GetId());
	Bind(wxEVT_PAINT,			&MapPanel::OnPaint, this, GetId());
	Bind(wxEVT_ERASE_BACKGROUND,&MapPanel::OnErase, this, GetId());
	Bind(wxEVT_SIZE,			&MapPanel::OnResize, this, GetId());

	SetBackgroundStyle(wxBG_STYLE_PAINT);

	//Create wx compatible OpenGL context
	m_context = new wxGLContext(this);

	//Set context
	SetCurrent(*m_context);

	//Get panel window handle
	HWND windowHandle = (HWND)GetHWND();

	//Create renderer (no panel size yet, init with a default)
	m_renderer = ion::render::Renderer::Create(windowHandle, m_context->GetGLRC(), 320, 240);

	//Set 2D orthographic mode
	m_renderer->SetPerspectiveMode(ion::render::Renderer::Ortho2DAbsolute);

	//Create camera
	m_camera = new ion::render::Camera();
	m_camera->SetPosition(ion::Vector3(0.0f, 0.0f, 0.0f));

	//Set scene clear colour
	m_renderer->SetClearColour(ion::Colour(0.3f, 0.3f, 0.3f));

	//Load shaders
	m_vertexShader = m_resourceManager.GetResource<ion::render::Shader>("flattextured_v.ion.shader");
	m_pixelShader = m_resourceManager.GetResource<ion::render::Shader>("flattextured_p.ion.shader");

	//Hack: wait for resources
	//TODO: SetVertexShader() fetches param handles, only succeeds if finished loading
	while(m_resourceManager.GetNumResourcesWaiting() > 0)
	{
		wxSleep(1);
	}

	//Create tileset texture
	m_tilesetTextureHndl = m_resourceManager.AddResource("tilesetTexture", *ion::render::Texture::Create());

	//Create map material
	m_mapMaterial = new ion::render::Material();
	m_mapMaterial->AddDiffuseMap(m_tilesetTextureHndl);
	m_mapMaterial->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f));
	m_mapMaterial->SetVertexShader(m_vertexShader);
	m_mapMaterial->SetPixelShader(m_pixelShader);

	//Create grid material
	m_gridMaterial = new ion::render::Material();
	m_gridMaterial->SetDiffuseColour(ion::Colour(0.0f, 0.0f, 0.0f));
	m_gridMaterial->SetVertexShader(m_vertexShader);
	m_gridMaterial->SetPixelShader(m_pixelShader);

	//Create preview quad
	m_previewPrimitive = new ion::render::Quad(ion::render::Quad::xy, ion::Vector2(4.0f, 4.0f), ion::Vector3());
}

MapPanel::~MapPanel()
{
	delete m_mapMaterial;
	delete m_gridMaterial;
	delete m_previewPrimitive;
	m_tilesetTextureHndl.Clear();
	m_vertexShader.Clear();
	m_pixelShader.Clear();
	delete m_camera;

	if(m_mapPrimitive)
		delete m_mapPrimitive;

	if(m_gridPrimitive)
		delete m_gridPrimitive;

	//Hack: wait for resources
	while(m_resourceManager.GetNumResourcesWaiting() > 0)
	{
		wxSleep(1);
	}

	m_resourceManager.RemoveResource("tilesetTexture");

	delete m_context;
	delete m_renderer;
}

void MapPanel::SetProject(Project* project)
{
	m_project = project;

	//Reset zoom
	m_cameraZoom = 1.0f;
	m_camera->SetZoom(ion::Vector3(m_cameraZoom, m_cameraZoom, 1.0f));

	//Create canvas
	CreateCanvas();

	//Create grid
	CreateGrid();

	//Centre camera on canvas
	CentreCamera();

	//Recreate tileset texture
	CreateTilesetTexture();

	//Recreate index cache
	CacheTileIndices();

	//Redraw map
	PaintWholeMap();

	//Refresh panel
	Refresh();
}

void MapPanel::CreateCanvas()
{
	if(m_project)
	{
		//Create rendering primitive
		if(m_mapPrimitive)
			delete m_mapPrimitive;

		int mapWidth = m_project->GetMap().GetWidth();
		int mapHeight = m_project->GetMap().GetHeight();
		m_mapPrimitive = new ion::render::Chessboard(ion::render::Chessboard::xy, ion::Vector2((float)mapWidth * 4.0f, (float)mapHeight * 4.0f), mapWidth, mapHeight, true);
	}
}

void MapPanel::CreateGrid()
{
	if(m_project)
	{
		if(m_gridPrimitive)
			delete m_gridPrimitive;

		int mapWidth = m_project->GetMap().GetWidth();
		int mapHeight = m_project->GetMap().GetHeight();
		m_gridPrimitive = new ion::render::Grid(ion::render::Grid::xy, ion::Vector2((float)mapWidth * 4.0f, (float)mapHeight * 4.0f), mapWidth / m_project->GetGridSize(), mapHeight / m_project->GetGridSize());
	}
}

void MapPanel::CreateTilesetTexture()
{
	if(m_project)
	{
		const int tileWidth = 8;
		const int tileHeight = 8;

		const Tileset& tileset = m_project->GetMap().GetTileset();
		u32 numTiles = tileset.GetCount();
		m_tilesetSizeSq = (u32)ion::maths::Ceil(ion::maths::Sqrt((float)numTiles));
		u32 textureWidth = m_tilesetSizeSq * tileWidth;
		u32 textureHeight = m_tilesetSizeSq * tileHeight;
		u32 bytesPerPixel = 3;
		u32 textureSize = textureWidth * textureHeight * bytesPerPixel;
		m_cellSizeTexSpaceSq = 1.0f / (float)m_tilesetSizeSq;

		u8* data = new u8[textureSize];
		ion::memory::MemSet(data, 0, textureSize);

		u32 tileIndex = 0;

		for(TTileMap::const_iterator it = tileset.Begin(), end = tileset.End(); it != end; ++it, ++tileIndex)
		{
			const Tile& tile = it->second;
			PaletteId paletteId = tile.GetPaletteId();
			Palette* palette = m_project->GetPalette(paletteId);

			u32 x = tileIndex % m_tilesetSizeSq;
			u32 y = tileIndex / m_tilesetSizeSq;

			for(int pixelY = 0; pixelY < 8; pixelY++)
			{
				for(int pixelX = 0; pixelX < 8; pixelX++)
				{
					//Invert Y for OpenGL
					int pixelY_OGL = tileHeight - 1 - pixelY;
					const Colour& colour = palette->GetColour(tile.GetPixelColour(pixelX, pixelY_OGL));

					int destPixelX = (x * tileWidth) + pixelX;
					int destPixelY = (y * tileHeight) + pixelY;
					u32 pixelIdx = (destPixelY * textureWidth) + destPixelX;
					u32 dataOffset = pixelIdx * bytesPerPixel;
					ion::debug::Assert(dataOffset + 2 < textureSize, "Out of bounds");
					data[dataOffset] = colour.r;
					data[dataOffset + 1] = colour.g;
					data[dataOffset + 2] = colour.b;
				}
			}
		}

		m_tilesetTextureHndl->Load(textureWidth, textureHeight, ion::render::Texture::eRGB, ion::render::Texture::eBPP24, data);
		m_tilesetTextureHndl->SetMinifyFilter(ion::render::Texture::eFilterNearest);
		m_tilesetTextureHndl->SetMagnifyFilter(ion::render::Texture::eFilterNearest);
		m_tilesetTextureHndl->SetWrapping(ion::render::Texture::eWrapClamp);

		delete data;
	}
}

void MapPanel::PaintWholeMap()
{
	if(m_project)
	{
		int mapWidth = m_project->GetMap().GetWidth();
		int mapHeight = m_project->GetMap().GetHeight();

		for(int y = 0; y < mapHeight; y++)
		{
			for(int x = 0; x < mapWidth; x++)
			{
				//Get tile
				TileId tileId = m_project->GetMap().GetTile(x, y);

				//Invert Y for OpenGL
				int yInv = mapHeight - 1 - y;

				//Get V/H flip
				u32 tileFlags = m_project->GetMap().GetTileFlags(x, y);
				bool flipX = tileFlags & Map::eFlipX;
				bool flipY = tileFlags & Map::eFlipY;

				//Paint tile
				PaintTile(tileId, x, yInv, flipX, flipY);
			}
		}
	}
}

int MapPanel::GetTileIndex(TileId tileId) const
{
	std::map<TileId, u32>::const_iterator it = m_tileIndexMap.find(tileId);
	ion::debug::Assert(it != m_tileIndexMap.end(), "TileId not found in tileset");
	return it->second;
}

void MapPanel::GetTileTexCoords(TileId tileId, ion::render::TexCoord texCoords[4], bool flipX, bool flipY) const
{
	//Map tile ID to index
	u32 tileIndex = GetTileIndex(tileId);

	//Map tile to X/Y on tileset texture
	int tilesetX = (tileIndex % m_tilesetSizeSq);
	int tilesetY = (tileIndex / m_tilesetSizeSq);
	ion::Vector2 textureBottomLeft(m_cellSizeTexSpaceSq * tilesetX, m_cellSizeTexSpaceSq * tilesetY);

	float top = flipY ? (textureBottomLeft.y) : (textureBottomLeft.y + m_cellSizeTexSpaceSq);
	float left = flipX ? (textureBottomLeft.x + m_cellSizeTexSpaceSq) : (textureBottomLeft.x);
	float bottom = flipY ? (textureBottomLeft.y + m_cellSizeTexSpaceSq) : (textureBottomLeft.y);
	float right = flipX ? (textureBottomLeft.x) : (textureBottomLeft.x + m_cellSizeTexSpaceSq);

	//Top left
	texCoords[0].x = left;
	texCoords[0].y = top;
	//Bottom left
	texCoords[1].x = left;
	texCoords[1].y = bottom;
	//Bottom right
	texCoords[2].x = right;
	texCoords[2].y = bottom;
	//Top right
	texCoords[3].x = right;
	texCoords[3].y = top;
}

void MapPanel::PaintTile(TileId tileId, int x, int y, bool flipX, bool flipY)
{
	if(m_project)
	{
		int mapWidth = m_project->GetMap().GetWidth();
		int mapHeight = m_project->GetMap().GetHeight();

		//Set texture coords for cell
		ion::render::TexCoord coords[4];
		GetTileTexCoords(tileId, coords, flipX, flipY);
		m_mapPrimitive->SetTexCoords((y * mapWidth) + x, coords);
	}
}

void MapPanel::CacheTileIndices()
{
	m_tileIndexMap.clear();

	if(m_project)
	{
		const Tileset& tileset = m_project->GetMap().GetTileset();
		u32 tileIndex = 0;

		for(TTileMap::const_iterator it = tileset.Begin(), end = tileset.End(); it != end; ++it, ++tileIndex)
		{
			//Map ID to index
			m_tileIndexMap.insert(std::make_pair(it->first, tileIndex));
		}
	}
}

void MapPanel::OnMouse(wxMouseEvent& event)
{
	if(m_project)
	{
		const int mapWidth = m_project->GetMap().GetWidth();
		const int mapHeight = m_project->GetMap().GetHeight();
		const int tileWidth = 8;
		const int tileHeight = 8;

		//Get mouse delta
		ion::Vector2 mousePosScreenSpace(event.GetX(), event.GetY());
		ion::Vector2 mouseDelta = m_mousePrevPos - mousePosScreenSpace;
		m_mousePrevPos = mousePosScreenSpace;

		//Get mouse position in panel space
		wxClientDC clientDc(this);
		wxPoint mousePanelPosWx = event.GetLogicalPosition(clientDc);
		wxSize panelSizeWx = GetClientSize();

		//Centre of map quad is 0,0
		ion::Vector2 mousePos(mousePanelPosWx.x, panelSizeWx.y - mousePanelPosWx.y);
		ion::Vector2 viewportSize(panelSizeWx.x, panelSizeWx.y);
		ion::Vector2 cameraPos(m_camera->GetPosition().x * m_cameraZoom, m_camera->GetPosition().y * m_cameraZoom);
		ion::Vector2 mapSize(mapWidth * tileWidth * m_cameraZoom, mapHeight * tileHeight * m_cameraZoom);
		ion::Vector2 mousePosMapSpace;
		mousePosMapSpace.x = (mapSize.x - (mapSize.x / 2.0f - cameraPos.x - mousePos.x)) / m_cameraZoom;
		mousePosMapSpace.y = (mapSize.y - (mapSize.y / 2.0f - cameraPos.y - mousePos.y)) / m_cameraZoom;

		//Panting/erasing
		if(mousePosMapSpace.x >= 0.0f
			&&	mousePosMapSpace.y >= 0.0f
			&&	mousePosMapSpace.x < (mapWidth * tileWidth)
			&& mousePosMapSpace.y < (mapHeight * tileHeight))
		{
			int x = (int)floor(mousePosMapSpace.x / (float)tileWidth);
			int y = (int)floor(mousePosMapSpace.y / (float)tileHeight);

			//Get tile ID to paint
			TileId tileId = InvalidTileId;
			if(event.ButtonIsDown(wxMOUSE_BTN_LEFT))
				tileId = m_project->GetPaintTile();
			else if(event.ButtonIsDown(wxMOUSE_BTN_RIGHT))
				tileId = m_project->GetEraseTile();

			if(tileId != InvalidTileId)
			{
				//Set on map
				m_project->GetMap().SetTile(x, mapHeight - 1 - y, m_project->GetPaintTile());

				//Set V/H flip flags
				u32 tileFlags = 0;
				if(m_keyHeldFlipX)
					tileFlags |= Map::eFlipX;
				if(m_keyHeldFlipY)
					tileFlags |= Map::eFlipY;

				//Invert Y from OpenGL back to map space
				m_project->GetMap().SetTileFlags(x, mapHeight - 1 - y, tileFlags);

				//Paint to canvas
				PaintTile(m_project->GetPaintTile(), x, y, m_keyHeldFlipX, m_keyHeldFlipY);

				//Invalidate rect
				Refresh();
			}

			if(m_project->GetPaintTile())
			{
				if(m_lastMouseHoverTileX != x || m_lastMouseHoverTileY != y)
				{
					//Preview tile pos changed, rRedraw
					Refresh();
				}

				//Update preview tile pos
				m_lastMouseHoverTileX = x;
				m_lastMouseHoverTileY = y;
			}
		}
		else
		{
			//Out of map range, invalidate preview tile pos
			m_lastMouseHoverTileX = -1;
			m_lastMouseHoverTileY = -1;
		}

		//Camera pan/zoom
		if(event.Dragging() && event.ButtonIsDown(wxMOUSE_BTN_MIDDLE))
		{
			//Pan camera (invert Y for OpenGL)
			ion::Vector3 cameraPos = m_camera->GetPosition();
			cameraPos.x -= mouseDelta.x * m_cameraPanSpeed / m_cameraZoom;
			cameraPos.y += mouseDelta.y * m_cameraPanSpeed / m_cameraZoom;
			m_camera->SetPosition(cameraPos);

			//Invalidate rect
			Refresh();
		}
		else if(event.GetWheelRotation() != 0)
		{
			float prevZoom = m_cameraZoom;

			//Zoom camera
			int wheelDelta = event.GetWheelRotation();
			float zoomSpeed = 1.0f;

			//Reduce speed for <1.0f
			if((wheelDelta < 0 && m_cameraZoom <= 1.0f) || (wheelDelta > 0 && m_cameraZoom < 1.0f))
			{
				zoomSpeed = 0.2f;
			}

			//One notch at a time
			if(wheelDelta > 0)
				m_cameraZoom += zoomSpeed;
			else if(wheelDelta < 0)
				m_cameraZoom -= zoomSpeed;

			//Clamp
			if(m_cameraZoom < 0.2f)
				m_cameraZoom = 0.2f;
			else if(m_cameraZoom > 10.0f)
				m_cameraZoom = 10.0f;

			//Set camera zoom
			m_camera->SetZoom(ion::Vector3(m_cameraZoom, m_cameraZoom, 1.0f));

			//Compensate camera pos
			wxSize panelSize = GetClientSize();
			ion::Vector2 originalViewportSize((float)panelSize.x / prevZoom, (float)panelSize.y / prevZoom);
			ion::Vector2 newViewportSize((float)panelSize.x / m_cameraZoom, (float)panelSize.y / m_cameraZoom);
			ion::Vector3 cameraPos = m_camera->GetPosition();
			cameraPos.x -= (newViewportSize.x - originalViewportSize.x) / 2.0f;
			cameraPos.y -= (newViewportSize.y - originalViewportSize.y) / 2.0f;
			m_camera->SetPosition(cameraPos);

			//Invalidate rect
			Refresh();
		}
	}

	event.Skip();
}

void MapPanel::OnKeyboard(wxKeyEvent& event)
{
	if(m_keyHeldFlipX != event.ShiftDown())
	{
		//SHIFT held, set H flip and refresh preview tile
		m_keyHeldFlipX = event.ShiftDown();
		Refresh();
	}

	if(m_keyHeldFlipY != event.ControlDown())
	{
		//CTRL held, set V flip and refresh preview tile
		m_keyHeldFlipY = event.ControlDown();
		Refresh();
	}
}

void MapPanel::OnPaint(wxPaintEvent& event)
{
	//Begin rendering
	m_renderer->BeginFrame();
	m_renderer->ClearColour();
	m_renderer->ClearDepth();

	if(m_project)
	{
		ion::Matrix4 cameraInverseMtx = m_camera->GetTransform().GetInverse();
		ion::Matrix4 projectionMtx = m_renderer->GetProjectionMatrix();

		//Z order
		const float zOffset = 0.0001f;
		float z = 0.0f;

		//No depth test (stops grid cells Z fighting)
		m_renderer->SetDepthTest(ion::render::Renderer::Always);

		//Draw map
		m_mapMaterial->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
		m_renderer->DrawVertexBuffer(m_mapPrimitive->GetVertexBuffer(), m_mapPrimitive->GetIndexBuffer());
		m_mapMaterial->Unbind();

		m_renderer->SetDepthTest(ion::render::Renderer::LessEqual);

		z += zOffset;

		//Draw preview tile
		if(m_project->GetPaintTile() && m_lastMouseHoverTileX >= 0 && m_lastMouseHoverTileY >= 0)
		{
			const int mapWidth = m_project->GetMap().GetWidth();
			const int mapHeight = m_project->GetMap().GetHeight();
			const int tileWidth = 8;
			const int tileHeight = 8;
			const int quadHalfExtentsX = 4;
			const int quadHalfExtentsY = 4;

			//Set preview quad texture coords
			ion::render::TexCoord coords[4];
			GetTileTexCoords(m_project->GetPaintTile(), coords, m_keyHeldFlipX, m_keyHeldFlipY);
			m_previewPrimitive->SetTexCoords(coords);

			ion::Matrix4 previewQuadMtx;
			ion::Vector3 previewQuadPos(((m_lastMouseHoverTileX - (mapWidth / 2)) * tileWidth) + quadHalfExtentsX,
				((m_lastMouseHoverTileY - (mapHeight / 2)) * tileHeight) + quadHalfExtentsY, z);
			previewQuadMtx.SetTranslation(previewQuadPos);

			m_mapMaterial->Bind(previewQuadMtx, cameraInverseMtx, projectionMtx);
			m_renderer->DrawVertexBuffer(m_previewPrimitive->GetVertexBuffer(), m_previewPrimitive->GetIndexBuffer());
			m_mapMaterial->Unbind();
		}

		z += zOffset;

		if(m_project->GetShowGrid())
		{
			//Draw grid
			ion::Matrix4 gridMtx;
			gridMtx.SetTranslation(ion::Vector3(0.0f, 0.0f, z));
			gridMtx.SetScale(ion::Vector3((float)m_project->GetGridSize(), (float)m_project->GetGridSize(), 1.0f));
			m_gridMaterial->Bind(gridMtx, cameraInverseMtx, projectionMtx);
			m_renderer->DrawVertexBuffer(m_gridPrimitive->GetVertexBuffer());
			m_gridMaterial->Unbind();
		}
	}

	//End rendering
	m_renderer->SwapBuffers();
	m_renderer->EndFrame();
}

void MapPanel::OnErase(wxEraseEvent& event)
{
	//Ignore event
}

void MapPanel::OnResize(wxSizeEvent& event)
{
	wxSize panelSize = GetClientSize();

	//Filter out superflous resize events (wx sends them if UI thread doesn't respond during saving/loading)
	if(m_panelSize.x != panelSize.x || m_panelSize.y != panelSize.y)
	{
		m_panelSize = panelSize;
		m_renderer->OnResize(panelSize.x, panelSize.y);
		CentreCamera();
	}

	Refresh();
}

void MapPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(m_project)
	{
		//If map invalidated
		if(m_project->MapIsInvalidated())
		{
			//Recreate canvas
			CreateCanvas();

			//Recreate grid
			CreateGrid();

			//Recreate tileset texture
			CreateTilesetTexture();

			//Recreate index cache
			CacheTileIndices();

			//Redraw map
			PaintWholeMap();

			m_project->InvalidateMap(false);
		}
	}

	wxGLCanvas::Refresh(eraseBackground, rect);
}

void MapPanel::CentreCamera()
{
	wxRect clientRect = GetClientRect();
	ion::Vector3 cameraPos(-(clientRect.width / 2.0f), -(clientRect.height / 2.0f), 0.0f);
	m_camera->SetPosition(cameraPos);
}
