#include "ViewPanel.h"

ViewPanel::ViewPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxGLCanvas(parent, glContext, winid, pos, size, style, name, NULL, wxNullPalette)
	, m_renderer(renderer)
	, m_viewport(128, 128, ion::render::Viewport::Ortho2DAbsolute)
{
	m_project = NULL;
	m_mainWindow = mainWindow;
	m_canvasPrimitive = NULL;
	m_gridPrimitive = NULL;
	m_cameraZoom = 1.0f;
	m_cameraPanSpeed = 1.0f;
	m_tilesetSizeSq = 1;
	m_cellSizeTexSpaceSq = 1.0f;
	m_panelSize = size;
	m_prevMouseBits = 0;
	m_enableZoom = true;
	m_enablePan = true;

	SetBackgroundStyle(wxBG_STYLE_PAINT);

	//Set viewport clear colour
	m_viewport.SetClearColour(ion::Colour(0.3f, 0.3f, 0.3f));

	//Load shaders
	m_vertexShader = ion::render::Shader::Create();
	if(!m_vertexShader->Load("shaders/flattextured_v.ion.shader"))
	{
		ion::debug::Error("Error loading vertex shader");
	}

	m_pixelShader = ion::render::Shader::Create();
	if(!m_pixelShader->Load("shaders/flattextured_p.ion.shader"))
	{
		ion::debug::Error("Error loading pixel shader");
	}

	//Create tileset texture
	m_tilesetTexture = ion::render::Texture::Create();

	//Create map material
	m_canvasMaterial = new ion::render::Material();
	m_canvasMaterial->AddDiffuseMap(m_tilesetTexture);
	m_canvasMaterial->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f));
	m_canvasMaterial->SetVertexShader(m_vertexShader);
	m_canvasMaterial->SetPixelShader(m_pixelShader);

	//Create grid material
	m_gridMaterial = new ion::render::Material();
	m_gridMaterial->SetDiffuseColour(ion::Colour(0.0f, 0.0f, 0.0f));
	m_gridMaterial->SetVertexShader(m_vertexShader);
	m_gridMaterial->SetPixelShader(m_pixelShader);

	//Bind events
	Bind(wxEVT_LEFT_DOWN, &ViewPanel::EventHandlerMouse, this, GetId());
	Bind(wxEVT_LEFT_UP, &ViewPanel::EventHandlerMouse, this, GetId());
	Bind(wxEVT_MIDDLE_DOWN, &ViewPanel::EventHandlerMouse, this, GetId());
	Bind(wxEVT_MIDDLE_UP, &ViewPanel::EventHandlerMouse, this, GetId());
	Bind(wxEVT_RIGHT_DOWN, &ViewPanel::EventHandlerMouse, this, GetId());
	Bind(wxEVT_RIGHT_UP, &ViewPanel::EventHandlerMouse, this, GetId());
	Bind(wxEVT_MOTION, &ViewPanel::EventHandlerMouse, this, GetId());
	Bind(wxEVT_MOUSEWHEEL, &ViewPanel::EventHandlerMouse, this, GetId());
	Bind(wxEVT_KEY_DOWN, &ViewPanel::EventHandlerKeyboard, this, GetId());
	Bind(wxEVT_KEY_UP, &ViewPanel::EventHandlerKeyboard, this, GetId());
	Bind(wxEVT_PAINT, &ViewPanel::EventHandlerPaint, this, GetId());
	Bind(wxEVT_ERASE_BACKGROUND, &ViewPanel::EventHandlerErase, this, GetId());
	Bind(wxEVT_SIZE, &ViewPanel::EventHandlerResize, this, GetId());
}

ViewPanel::~ViewPanel()
{
	delete m_canvasMaterial;
	delete m_gridMaterial;
	delete m_tilesetTexture;
	delete m_vertexShader;
	delete m_pixelShader;

	if(m_canvasPrimitive)
		delete m_canvasPrimitive;

	if(m_gridPrimitive)
		delete m_gridPrimitive;
}

void ViewPanel::EventHandlerMouse(wxMouseEvent& event)
{
	OnMouse(event);
	event.Skip();
}

void ViewPanel::EventHandlerKeyboard(wxKeyEvent& event)
{
	OnKeyboard(event);
	event.Skip();
}

void ViewPanel::EventHandlerPaint(wxPaintEvent& event)
{
	//Begin rendering to current viewport
	m_renderer.BeginFrame(m_viewport, GetHDC());
	m_renderer.ClearColour();
	m_renderer.ClearDepth();

	if(m_project)
	{
		ion::Matrix4 cameraInverseMtx = m_camera.GetTransform().GetInverse();
		ion::Matrix4 projectionMtx = m_renderer.GetProjectionMatrix();

		//Z order
		const float zOffset = 0.0001f;
		float z = 0.0f;

		//Render callback
		OnRender(m_renderer, cameraInverseMtx, projectionMtx, z, zOffset);
	}

	//End rendering
	m_renderer.SwapBuffers();
	m_renderer.EndFrame();

	event.Skip();
}

void ViewPanel::EventHandlerErase(wxEraseEvent& event)
{
	//Ignore event
}

void ViewPanel::EventHandlerResize(wxSizeEvent& event)
{
	OnResize(event);
	event.Skip();
}

void ViewPanel::SetProject(Project* project)
{
	m_project = project;

	//Reset zoom
	m_cameraZoom = 1.0f;
	m_camera.SetZoom(ion::Vector3(m_cameraZoom, m_cameraZoom, 1.0f));

	//Centre camera on canvas
	CentreCamera();

	//Recreate tileset texture
	CreateTilesetTexture(project->GetTileset());

	//Recreate index cache
	CacheTileIndices();

	//Refresh panel
	Refresh();
}

void ViewPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	wxGLCanvas::Refresh(eraseBackground, rect);
}

void ViewPanel::CreateCanvas(int width, int height)
{
	//Create rendering primitive
	if(m_canvasPrimitive)
		delete m_canvasPrimitive;

	m_canvasPrimitive = new ion::render::Chessboard(ion::render::Chessboard::xy, ion::Vector2((float)width * 4.0f, (float)height * 4.0f), width, height, true);
	m_canvasSize.x = width;
	m_canvasSize.y = height;
}

void ViewPanel::CreateGrid(int width, int height, int cellsX, int cellsY)
{
	if(m_gridPrimitive)
		delete m_gridPrimitive;

	int mapWidth = m_project->GetMap().GetWidth();
	int mapHeight = m_project->GetMap().GetHeight();
	m_gridPrimitive = new ion::render::Grid(ion::render::Grid::xy, ion::Vector2((float)width * 4.0f, (float)height * 4.0f), cellsX, cellsY);
}

void ViewPanel::CreateTilesetTexture(const Tileset& tileset)
{
	const int tileWidth = 8;
	const int tileHeight = 8;

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

	m_tilesetTexture->Load(textureWidth, textureHeight, ion::render::Texture::eRGB, ion::render::Texture::eBPP24, data);
	m_tilesetTexture->SetMinifyFilter(ion::render::Texture::eFilterNearest);
	m_tilesetTexture->SetMagnifyFilter(ion::render::Texture::eFilterNearest);
	m_tilesetTexture->SetWrapping(ion::render::Texture::eWrapClamp);

	delete data;
}

int ViewPanel::GetTileIndex(TileId tileId) const
{
	std::map<TileId, u32>::const_iterator it = m_tileIndexMap.find(tileId);
	ion::debug::Assert(it != m_tileIndexMap.end(), "TileId not found in tileset");
	return it->second;
}

void ViewPanel::GetTileTexCoords(TileId tileId, ion::render::TexCoord texCoords[4], u32 flipFlags) const
{
	if(tileId == InvalidTileId)
	{
		//Invalid tile, use top-left pixel
		float onePixelTexSpace = m_cellSizeTexSpaceSq / 8.0f;

		//Top left
		texCoords[0].x = 0.0f;
		texCoords[0].y = 0.0f;
		//Bottom left
		texCoords[1].x = 0.0f;
		texCoords[1].y = onePixelTexSpace;
		//Bottom right
		texCoords[2].x = onePixelTexSpace;
		texCoords[2].y = onePixelTexSpace;
		//Top right
		texCoords[3].x = onePixelTexSpace;
		texCoords[3].y = 0.0f;
	}
	else
	{
		//Map tile ID to index
		u32 tileIndex = GetTileIndex(tileId);

		//Map tile to X/Y on tileset texture
		int tilesetX = (tileIndex % m_tilesetSizeSq);
		int tilesetY = (tileIndex / m_tilesetSizeSq);
		ion::Vector2 textureBottomLeft(m_cellSizeTexSpaceSq * tilesetX, m_cellSizeTexSpaceSq * tilesetY);

		bool flipX = (flipFlags & Map::eFlipX) != 0;
		bool flipY = (flipFlags & Map::eFlipY) != 0;

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
}

void ViewPanel::PaintTile(TileId tileId, int x, int y, u32 flipFlags)
{
	if(m_project)
	{
		//Set texture coords for cell
		ion::render::TexCoord coords[4];
		GetTileTexCoords(tileId, coords, flipFlags);
		m_canvasPrimitive->SetTexCoords((y * m_canvasSize.x) + x, coords);
	}
}

void ViewPanel::PaintStamp(const Stamp& stamp, int x, int y, u32 flipFlags)
{
	if(m_project)
	{
		int width = stamp.GetWidth();
		int height = stamp.GetHeight();

		for(int stampX = 0; stampX < width; stampX++)
		{
			for(int stampY = 0; stampY < height; stampY++)
			{
				int sourceX = (flipFlags & Map::eFlipX) ? (width - 1 - stampX) : stampX;
				int sourceY = (flipFlags & Map::eFlipY) ? (height - 1 - stampY) : stampY;

				TileId tileId = stamp.GetTile(sourceX, sourceY);
				u32 tileFlags = stamp.GetTileFlags(stampX, stampY);
				tileFlags ^= flipFlags;
				int canvasX = stampX + x;
				int canvasY = stampY + y;
				int y_inv = m_canvasSize.y - 1 - canvasY;

				//Paint on canvas
				PaintTile(tileId, canvasX, y_inv, tileFlags);
			}
		}
	}
}

void ViewPanel::FillTiles(TileId tileId, const ion::Vector2i& boxCorner1, const ion::Vector2i& boxCorner2)
{
	//Sanitise ordering before looping
	int top = min(boxCorner1.y, boxCorner2.y);
	int left = min(boxCorner1.x, boxCorner2.x);
	int bottom = max(boxCorner1.y, boxCorner2.y);
	int right = max(boxCorner1.x, boxCorner2.x);

	for(int x = left; x <= right; x++)
	{
		for(int y = top; y <= bottom; y++)
		{
			//Invert for OpenGL
			int y_inv = m_canvasSize.y - 1 - y;

			//Paint tile to canvas
			PaintTile(tileId, x, y_inv, 0);
		}
	}
}

void ViewPanel::FillTiles(TileId tileId, const std::vector<ion::Vector2i>& selection)
{
	for(int i = 0; i < selection.size(); i++)
	{
		int x = selection[i].x;
		int y = selection[i].y;

		//Invert for OpenGL
		int y_inv = m_canvasSize.y - 1 - y;

		//Paint tile to canvas
		PaintTile(tileId, x, y_inv, 0);
	}
}

void ViewPanel::FindBounds(const std::vector<ion::Vector2i>& tiles, int& left, int& top, int& right, int& bottom) const
{
	left = INT_MAX;
	top = INT_MAX;
	right = 0;
	bottom = 0;

	for(int i = 0; i < tiles.size(); i++)
	{
		int x = tiles[i].x;
		int y = tiles[i].y;

		if(x < left)
			left = x;
		if(x > right)
			right = x;
		if(y < top)
			top = y;
		if(y > bottom)
			bottom = y;
	}
}

void ViewPanel::CacheTileIndices()
{
	m_tileIndexMap.clear();

	if(m_project)
	{
		const Tileset& tileset = m_project->GetTileset();
		u32 tileIndex = 0;

		for(TTileMap::const_iterator it = tileset.Begin(), end = tileset.End(); it != end; ++it, ++tileIndex)
		{
			//Map ID to index
			m_tileIndexMap.insert(std::make_pair(it->first, tileIndex));
		}
	}
}

void ViewPanel::OnMouse(wxMouseEvent& event)
{
	if(m_project)
	{
		//If a tool is active, ensure this window has keyboard focus (to catch SHIFT/CTRL keys)
		SetFocus();

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
		ion::Vector2 cameraPos(m_camera.GetPosition().x * m_cameraZoom, m_camera.GetPosition().y * m_cameraZoom);
		ion::Vector2 canvasSizePixels(m_canvasSize.x * tileWidth * m_cameraZoom, m_canvasSize.y * tileHeight * m_cameraZoom);
		ion::Vector2 mousePosCanvasSpace;
		mousePosCanvasSpace.x = (canvasSizePixels.x - (canvasSizePixels.x / 2.0f - cameraPos.x - mousePos.x)) / m_cameraZoom;
		mousePosCanvasSpace.y = (canvasSizePixels.y - (canvasSizePixels.y / 2.0f - cameraPos.y - mousePos.y)) / m_cameraZoom;

		//Get tile x/y
		int x = (int)floor(mousePosCanvasSpace.x / (float)tileWidth);
		int y_inv = (int)floor(mousePosCanvasSpace.y / (float)tileHeight);

		//Invert for OpenGL
		int y = (m_canvasSize.y - 1 - y_inv);

		//Get button bits
		int buttonBits = 0;
		if(event.LeftIsDown())
			buttonBits |= eMouseLeft;
		if(event.MiddleDown())
			buttonBits |= eMouseMiddle;
		if(event.RightDown())
			buttonBits |= eMouseRight;

		if((buttonBits != m_prevMouseBits) || (x != m_prevMouseOverTilePos.x) || (y != m_prevMouseOverTilePos.y))
		{
			//Mouse button clicked or changed grid pos
			OnMouseTileEvent(mouseDelta, buttonBits, x, y);

			m_prevMouseOverTilePos.x = x;
			m_prevMouseOverTilePos.y = y;
		}

		m_prevMouseBits = buttonBits;

		//Camera pan/zoom
		if(event.Dragging() && event.ButtonIsDown(wxMOUSE_BTN_MIDDLE))
		{
			if(m_enablePan)
			{
				//Pan camera (invert Y for OpenGL)
				ion::Vector3 cameraPos = m_camera.GetPosition();
				cameraPos.x -= mouseDelta.x * m_cameraPanSpeed / m_cameraZoom;
				cameraPos.y += mouseDelta.y * m_cameraPanSpeed / m_cameraZoom;
				m_camera.SetPosition(cameraPos);

				//Invalidate rect
				Refresh();
			}
		}
		else if(event.GetWheelRotation() != 0)
		{
			if(m_enableZoom)
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
				m_camera.SetZoom(ion::Vector3(m_cameraZoom, m_cameraZoom, 1.0f));

				//Compensate camera pos
				wxSize panelSize = GetClientSize();
				ion::Vector2 originalViewportSize((float)panelSize.x / prevZoom, (float)panelSize.y / prevZoom);
				ion::Vector2 newViewportSize((float)panelSize.x / m_cameraZoom, (float)panelSize.y / m_cameraZoom);
				ion::Vector3 cameraPos = m_camera.GetPosition();
				cameraPos.x -= (newViewportSize.x - originalViewportSize.x) / 2.0f;
				cameraPos.y -= (newViewportSize.y - originalViewportSize.y) / 2.0f;
				m_camera.SetPosition(cameraPos);

				//Invalidate rect
				Refresh();
			}
		}
	}
}

void ViewPanel::OnKeyboard(wxKeyEvent& event)
{

}

void ViewPanel::RenderCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//No depth test (stops grid cells Z fighting)
	renderer.SetDepthTest(ion::render::Renderer::Always);

	//Draw map
	m_canvasMaterial->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_canvasPrimitive->GetVertexBuffer(), m_canvasPrimitive->GetIndexBuffer());
	m_canvasMaterial->Unbind();

	renderer.SetDepthTest(ion::render::Renderer::LessEqual);
}

void ViewPanel::RenderGrid(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//Draw grid
	ion::Matrix4 gridMtx;
	gridMtx.SetTranslation(ion::Vector3(0.0f, 0.0f, z));
	gridMtx.SetScale(ion::Vector3((float)m_project->GetGridSize(), (float)m_project->GetGridSize(), 1.0f));
	m_gridMaterial->Bind(gridMtx, cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_gridPrimitive->GetVertexBuffer());
	m_gridMaterial->Unbind();
}

void ViewPanel::OnResize(wxSizeEvent& event)
{
	wxSize panelSize = GetClientSize();

	if(panelSize.x > 1 && panelSize.y > 1)
	{
		//Filter out superflous resize events (wx sends them if UI thread doesn't respond during saving/loading)
		if(m_panelSize.x != panelSize.x || m_panelSize.y != panelSize.y)
		{
			m_panelSize = panelSize;
			m_viewport.Resize(panelSize.x, panelSize.y);
			CentreCamera();
		}
	}

	Refresh();
}

void ViewPanel::CentreCamera()
{
	wxRect clientRect = GetClientRect();
	ion::Vector3 cameraPos(-(clientRect.width / 2.0f), -(clientRect.height / 2.0f), 0.0f);
	m_camera.SetPosition(cameraPos);
}