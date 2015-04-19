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
	, m_canvas(Map::defaultWidth * 8, Map::defaultHeight * 8)
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
	m_camera->SetPosition(ion::Vector3(0.0f, 0.0f, 1.0f));

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
	//m_primitive = new ion::render::Quad(ion::render::Quad::xz, ion::Vector2(50.0f, 50.0f));
	m_primitive = new ion::render::Box(ion::Vector3(0.5f, 0.5f, 0.5f));
}

MapPanel::~MapPanel()
{
	delete m_renderer;
	delete m_context;
}

void MapPanel::SetProject(Project* project)
{
	m_project = project;

	//Centre camera on canvas
	CentreCamera();

	//Reset zoom
	m_cameraZoom = 1.0f;

	//Recreate tileset texture
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

	u32 i = 0;

	for(TTileMap::const_iterator it = tileset.Begin(), end = tileset.End(); it != end; ++it, ++i)
	{
		const Tile& tile = it->second;
		PaletteId paletteId = tile.GetPaletteId();
		Palette* palette = m_project->GetPalette(paletteId);

		u32 x = i % tilesetSizeSqrt;
		u32 y = i / tilesetSizeSqrt;

		for(int pixelX = 0; pixelX < 8; pixelX++)
		{
			for(int pixelY = 0; pixelY < 8; pixelY++)
			{
				const Colour& colour = palette->GetColour(tile.GetPixelColour(pixelX, pixelY));

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
				if(m_project->GetPaintTile())
					PaintTile(mousePosMapSpace, m_project->GetPaintTile());
			}
			else if(event.ButtonIsDown(wxMOUSE_BTN_RIGHT))
			{
				//Erase
				if(m_project->GetEraseTile())
					PaintTile(mousePosMapSpace, m_project->GetEraseTile());
			}
		}

		//Camera pan/zoom
		if(event.Dragging() && event.ButtonIsDown(wxMOUSE_BTN_MIDDLE))
		{
			//Pan camera
			m_cameraPos.x += mouseDelta.x * m_cameraPanSpeed;
			m_cameraPos.y += mouseDelta.y * m_cameraPanSpeed;

			//Invalidate whole frame
			Refresh();
		}
		else if(event.GetWheelRotation() != 0)
		{
			//Zoom camera
			int wheelDelta = event.GetWheelRotation();
			const float zoomSpeed = 1.0f;

			//One notch at a time
			if(wheelDelta > 0)
				m_cameraZoom += zoomSpeed;
			else if(wheelDelta < 0)
				m_cameraZoom -= zoomSpeed;

			//Clamp
			if(m_cameraZoom < 1.0f)
				m_cameraZoom = 1.0f;
			else if(m_cameraZoom > 10.0f)
				m_cameraZoom = 10.0f;

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
	return;

	//Source context
	wxMemoryDC sourceContext(m_canvas);

	//Double buffered dest context
	wxAutoBufferedPaintDC destContext(this);

	//Update all invalidated regions
	for(wxRegionIterator it = GetUpdateRegion(); it; it++)
	{
		//Get invalidated rect
		wxRect destRect(it.GetRect());

		//Clear rect
		destContext.SetBrush(*wxLIGHT_GREY_BRUSH);
		destContext.DrawRectangle(destRect);

		//Transform panned/zoomed dest rect back to orignal source rect
		wxRect sourceRect(destRect);
		sourceRect.x -= m_cameraPos.x;
		sourceRect.y -= m_cameraPos.y;
		sourceRect.x /= m_cameraZoom;
		sourceRect.y /= m_cameraZoom;
		sourceRect.width /= m_cameraZoom;
		sourceRect.height /= m_cameraZoom;

		//Adjust dest rect for rounding error
		destRect.x -= (int)((float)destRect.x - (((float)sourceRect.x * m_cameraZoom) + m_cameraPos.x));
		destRect.y -= (int)((float)destRect.y - (((float)sourceRect.y * m_cameraZoom) + m_cameraPos.y));
		destRect.width -= (int)((float)destRect.width - ((float)sourceRect.width * m_cameraZoom));
		destRect.height -= (int)((float)destRect.height - ((float)sourceRect.height * m_cameraZoom));
		
		//Copy rect from canvas
		destContext.StretchBlit(destRect.x, destRect.y, destRect.width, destRect.height, &sourceContext, sourceRect.x, sourceRect.y, sourceRect.width, sourceRect.height);
	}

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
	if(m_project && m_project->MapIsInvalidated())
	{
		//Full refresh, resize canvas and redraw map
		m_canvas = wxBitmap(m_project->GetMap().GetWidth() * 8, m_project->GetMap().GetHeight() * 8);
		wxMemoryDC dc(m_canvas);
		PaintMapToDc(dc);
		m_project->InvalidateMap(false);
	}

	wxGLCanvas::Refresh(eraseBackground, rect);
}

void MapPanel::CentreCamera()
{
	if(m_project)
	{
		wxRect clientRect = GetClientRect();
		m_cameraPos.x = (clientRect.width / 2.0f) - ((m_project->GetMap().GetWidth() * 8.0f) / 2.0f);
		m_cameraPos.y = (clientRect.height / 2.0f) - ((m_project->GetMap().GetHeight() * 8.0f) / 2.0f);
	}
}

void MapPanel::PaintTile(ion::Vector2 mousePos, TileId tileId)
{
	if(m_project)
	{
		int x = (int)floor(mousePos.x / 8.0f);
		int y = (int)floor(mousePos.y / 8.0f);

		//Set new tile in map
		m_project->GetMap().SetTile(x, y, tileId);

		//Paint tile to canvas dc
		if(const Tile* tile = m_project->GetMap().GetTileset().GetTile(tileId))
		{
			u32 tileFlags = m_project->GetMap().GetTileFlags(x, y);

			if(const Palette* palette = m_project->GetPalette(tile->GetPaletteId()))
			{
				wxMemoryDC dc(m_canvas);
				TileRenderer::PaintTileToDc(x, y, *tile, *palette, tileFlags, dc);
			}
		}
		
		//Invalidate screen rect
		wxRect refreshRect(	((x * 8) * m_cameraZoom) + m_cameraPos.x,
							((y * 8) * m_cameraZoom) + m_cameraPos.y,
							8 * m_cameraZoom,
							8 * m_cameraZoom);

		RefreshRect(refreshRect);
	}
}

void MapPanel::PaintMapToDc(wxMemoryDC& dc)
{
	if(m_project)
	{
		for(int x = 0; x < m_project->GetMap().GetWidth(); x++)
		{
			for(int y = 0; y < m_project->GetMap().GetHeight(); y++)
			{
				TileId tileId = m_project->GetMap().GetTile(x, y);
				if(const Tile* tile = m_project->GetMap().GetTileset().GetTile(tileId))
				{
					u32 tileFlags = m_project->GetMap().GetTileFlags(x, y);

					if(const Palette* palette = m_project->GetPalette(tile->GetPaletteId()))
					{
						TileRenderer::PaintTileToDc(x, y, *tile, *palette, tileFlags, dc);
					}
				}
			}
		}
	}
}
