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
	m_cameraZoom = 1.0f;
	m_cameraPanSpeed = 1.0f;

	Bind(wxEVT_LEFT_DOWN,		&MapPanel::OnMouse, this, GetId());
	Bind(wxEVT_LEFT_UP,			&MapPanel::OnMouse, this, GetId());
	Bind(wxEVT_MIDDLE_DOWN,		&MapPanel::OnMouse, this, GetId());
	Bind(wxEVT_MIDDLE_UP,		&MapPanel::OnMouse, this, GetId());
	Bind(wxEVT_RIGHT_DOWN,		&MapPanel::OnMouse, this, GetId());
	Bind(wxEVT_RIGHT_UP,		&MapPanel::OnMouse, this, GetId());
	Bind(wxEVT_MOTION,			&MapPanel::OnMouse, this, GetId());
	Bind(wxEVT_MOUSEWHEEL,		&MapPanel::OnMouse, this, GetId());
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
	m_vertexShader = m_resourceManager.GetResource<ion::render::Shader>("default_v.ion.shader");
	m_pixelShader = m_resourceManager.GetResource<ion::render::Shader>("default_p.ion.shader");

	//Hack: wait for resources
	//TODO: SetVertexShader() fetches param handles, only succeeds if finished loading
	while(m_resourceManager.GetNumResourcesWaiting() > 0)
	{
		wxSleep(1);
	}

	//Create tileset texture
	m_tilesetTexture = ion::render::Texture::Create();
	m_tilesetTextureHndl = m_resourceManager.AddResource("tilesetTexture", *m_tilesetTexture);

	//Create default material
	m_material = new ion::render::Material();
	m_material->AddDiffuseMap(m_tilesetTextureHndl);
	m_material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f));
	m_material->SetVertexShader(m_vertexShader);
	m_material->SetPixelShader(m_pixelShader);

	//Create rendering primitive
	m_primitive = new ion::render::Grid(ion::render::Grid::xy, ion::Vector2(100.0f, 100.0f), 64, 64, true);
}

MapPanel::~MapPanel()
{
	delete m_renderer;
	delete m_context;
}

void MapPanel::SetProject(Project* project)
{
	m_project = project;

	//Reset zoom
	m_cameraZoom = 1.0f;
	m_camera->SetZoom(ion::Vector3(m_cameraZoom, m_cameraZoom, 1.0f));

	//Create canvas
	CreateCanvas();

	//Centre camera on canvas
	CentreCamera();

	//Recreate tileset texture
	CreateTilesetTexture();

	//Recreate index cache
	CacheTileIndices();

	//Redraw map
	PaintWholeMap();
}

void MapPanel::CreateCanvas()
{
	if(m_project)
	{
		//Create rendering primitive
		if(m_primitive)
			delete m_primitive;

		int mapWidth = m_project->GetMap().GetWidth();
		int mapHeight = m_project->GetMap().GetHeight();
		m_primitive = new ion::render::Grid(ion::render::Grid::xy, ion::Vector2(((float)mapWidth * 8.0f) / 2.0f, ((float)mapHeight * 8.0f) / 2.0f), mapWidth, mapHeight, true);
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
		u32 bytesPerPixel = 3;
		u32 tilesetSizeSqrt = (u32)ion::maths::Ceil(ion::maths::Sqrt((float)numTiles));
		u32 textureWidth = tilesetSizeSqrt * tileWidth;
		u32 textureHeight = tilesetSizeSqrt * tileHeight;
		u32 textureSize = textureWidth * textureHeight * bytesPerPixel;

		u8* data = new u8[textureSize];
		ion::memory::MemSet(data, 0, textureSize);

		u32 tileIndex = 0;

		for(TTileMap::const_iterator it = tileset.Begin(), end = tileset.End(); it != end; ++it, ++tileIndex)
		{
			const Tile& tile = it->second;
			PaletteId paletteId = tile.GetPaletteId();
			Palette* palette = m_project->GetPalette(paletteId);

			u32 x = tileIndex % tilesetSizeSqrt;
			u32 y = tileIndex / tilesetSizeSqrt;

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

		m_tilesetTexture->Load(textureWidth, textureHeight, ion::render::Texture::eRGB, ion::render::Texture::eBPP24, data);
		m_tilesetTexture->SetMinifyFilter(ion::render::Texture::eFilterNearest);
		m_tilesetTexture->SetMagnifyFilter(ion::render::Texture::eFilterNearest);
		m_tilesetTexture->SetWrapping(ion::render::Texture::eWrapClamp);

		delete data;
	}
}

void MapPanel::PaintWholeMap()
{
	if(m_project)
	{
		//Map texture coords to tileset
		const int tileWidth = 8;
		const int tileHeight = 8;

		const Tileset& tileset = m_project->GetMap().GetTileset();
		u32 numTiles = tileset.GetCount();
		u32 tilesetSizeSqrt = (u32)ion::maths::Ceil(ion::maths::Sqrt((float)numTiles));

		ion::render::TexCoord coords[4];
		float cellSizeTexSpaceSq(1.0f / (float)tilesetSizeSqrt);

		int mapWidth = m_project->GetMap().GetWidth();
		int mapHeight = m_project->GetMap().GetHeight();

		for(int mapY = 0; mapY < mapHeight; mapY++)
		{
			for(int mapX = 0; mapX < mapWidth; mapX++)
			{
				//Invert Y for OpenGL
				int mapY_OGL = mapHeight - 1 - mapY;

				//Map tile ID to index
				TileId tileId = m_project->GetMap().GetTile(mapX, mapY_OGL);
				std::map<TileId, u32>::iterator it = m_tileIndexMap.find(tileId);
				ion::debug::Assert(it != m_tileIndexMap.end(), "TileId not found in tileset");
				u32 tileIndex = it->second;
				int tilesetX = (tileIndex % tilesetSizeSqrt);
				int tilesetY = (tileIndex / tilesetSizeSqrt);
				ion::Vector2 textureBottomLeft(cellSizeTexSpaceSq * tilesetX, cellSizeTexSpaceSq * tilesetY);

				//Top left
				coords[0].x = textureBottomLeft.x;
				coords[0].y = textureBottomLeft.y + cellSizeTexSpaceSq;
				//Bottom left
				coords[1].x = textureBottomLeft.x;
				coords[1].y = textureBottomLeft.y;
				//Bottom right
				coords[2].x = textureBottomLeft.x + cellSizeTexSpaceSq;
				coords[2].y = textureBottomLeft.y;
				//Top right
				coords[3].x = textureBottomLeft.x + cellSizeTexSpaceSq;
				coords[3].y = textureBottomLeft.y + cellSizeTexSpaceSq;

				m_primitive->SetCellTexCoords((mapY * mapWidth) + mapX, coords);
			}
		}
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
		//Get mouse delta
		ion::Vector2 mousePos(event.GetX(), event.GetY());
		ion::Vector2 mouseDelta = m_mousePrevPos - mousePos;
		m_mousePrevPos = mousePos;

		//Get mouse position in canvas and map space
		wxClientDC clientDc(this);
		wxPoint mouseCanvasPosWx = event.GetLogicalPosition(clientDc);

		ion::Vector2 mousePosCanvasSpace(mouseCanvasPosWx.x, mouseCanvasPosWx.y);
		ion::Vector2 mousePosMapSpace((mouseCanvasPosWx.x - m_cameraPos.x) / m_cameraZoom, (mouseCanvasPosWx.y - m_cameraPos.y) / m_cameraZoom);

		//Panting/erasing
		if(		mousePosMapSpace.x >= 0.0f
			&&	mousePosMapSpace.y >= 0.0f
			&&	mousePosMapSpace.x < (m_project->GetMap().GetWidth() * 8)
			&&	mousePosMapSpace.y < (m_project->GetMap().GetHeight() * 8))
		{
			if(event.ButtonIsDown(wxMOUSE_BTN_LEFT))
			{
				//Paint
				//if(m_project->GetPaintTile())
				//	PaintTile(mousePosMapSpace, m_project->GetPaintTile());
			}
			else if(event.ButtonIsDown(wxMOUSE_BTN_RIGHT))
			{
				//Erase
				//if(m_project->GetEraseTile())
				//	PaintTile(mousePosMapSpace, m_project->GetEraseTile());
			}
		}

		//Camera pan/zoom
		if(event.Dragging() && event.ButtonIsDown(wxMOUSE_BTN_MIDDLE))
		{
			//Pan camera
			m_cameraPos.x -= mouseDelta.x * m_cameraPanSpeed / m_cameraZoom;
			m_cameraPos.y += mouseDelta.y * m_cameraPanSpeed / m_cameraZoom;

			m_camera->SetPosition(ion::Vector3(m_cameraPos.x, m_cameraPos.y, 0.0f));

			//Invalidate whole frame
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
			m_cameraPos.x -= (newViewportSize.x - originalViewportSize.x) / 2.0f;
			m_cameraPos.y -= (newViewportSize.y - originalViewportSize.y) / 2.0f;
			m_camera->SetPosition(ion::Vector3(m_cameraPos.x, m_cameraPos.y, 0.0f));

			//Invalidate whole frame
			Refresh();
		}
	}

	event.Skip();
}

void MapPanel::OnPaint(wxPaintEvent& event)
{
	//Begin rendering
	m_renderer->BeginFrame();
	m_renderer->ClearColour();
	m_renderer->ClearDepth();

	//Draw primitive
	m_material->Bind(ion::Matrix4(), m_camera->GetTransform().GetInverse(), m_renderer->GetProjectionMatrix());
	m_renderer->DrawVertexBuffer(m_primitive->GetVertexBuffer(), m_primitive->GetIndexBuffer());
	m_material->Unbind();

	//End rendering
	m_renderer->SwapBuffers();
	m_renderer->EndFrame();

	/*
	//Double buffered dest context
	wxAutoBufferedPaintDC destContext(this);

	//Draw grid
	if(m_project)
	{
		destContext.SetBrush(*wxBLACK_BRUSH);

		int mapWidth = m_project->GetMap().GetWidth();
		int mapHeight = m_project->GetMap().GetHeight();

		for(int x = 1; x < mapWidth; x++)
		{
			float lineHeight = (float)mapHeight * (8.0f * m_cameraZoom);
			float lineX = ((x * 8) * m_cameraZoom) + m_cameraPos.x;

			wxPoint pointY1(lineX, m_cameraPos.y);
			wxPoint pointY2(lineX, m_cameraPos.y + lineHeight);
			destContext.DrawLine(pointY1, pointY2);

			for(int y = 1; y < mapHeight; y++)
			{
				float lineWidth = (float)mapWidth * (8.0f * m_cameraZoom);
				float lineY = ((y * 8) * m_cameraZoom) + m_cameraPos.y;

				wxPoint pointX1(m_cameraPos.x, lineY);
				wxPoint pointX2(m_cameraPos.x + lineWidth, lineY);
				destContext.DrawLine(pointX1, pointX2);
			}
		}
	}
	*/
}

void MapPanel::OnErase(wxEraseEvent& event)
{
	//Ignore event
}

void MapPanel::OnResize(wxSizeEvent& event)
{
	CentreCamera();

	wxSize panelSize = GetClientSize();
	m_renderer->OnResize(panelSize.x, panelSize.y);

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
	if(m_project)
	{
		wxRect clientRect = GetClientRect();
		//m_cameraPos.x = (clientRect.width / 2.0f) - ((m_project->GetMap().GetWidth() * 8.0f) / 2.0f);
		//m_cameraPos.y = (clientRect.height / 2.0f) - ((m_project->GetMap().GetHeight() * 8.0f) / 2.0f);
	}
}
