#include "ViewPanel.h"
#include "MainWindow.h"

ViewPanel::ViewPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxGLCanvas(parent, glContext, winid, pos, size, style, name, NULL, wxNullPalette)
	, m_renderer(renderer)
	, m_renderResources(renderResources)
	, m_viewport(128, 128, ion::render::Viewport::eOrtho2DAbsolute)
	, m_project(project)
{
	m_mainWindow = mainWindow;
	m_canvasPrimitive = NULL;
	m_gridPrimitive = NULL;
	m_cameraZoom = 1.0f;
	m_cameraPanSpeed = 1.0f;
	m_prevMouseBits = 0;
	m_enableZoom = true;
	m_enablePan = true;

	SetBackgroundStyle(wxBG_STYLE_PAINT);

	//Set viewport clear colour
	m_viewport.SetClearColour(ion::Colour(0.3f, 0.3f, 0.3f));

	//Bind events
	Bind(wxEVT_LEFT_DOWN, &ViewPanel::EventHandlerMouse, this, GetId());
	Bind(wxEVT_LEFT_UP, &ViewPanel::EventHandlerMouse, this, GetId());
	Bind(wxEVT_LEFT_DCLICK, &ViewPanel::EventHandlerMouse, this, GetId());
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

	//Centre camera on canvas
	CentreCamera();

	//Reset zoom
	SetCameraZoom(1.0f);

	//Refresh panel
	Refresh();
}

ViewPanel::~ViewPanel()
{

}

void ViewPanel::EventHandlerMouse(wxMouseEvent& event)
{
	//Get mouse delta
	ion::Vector2i mousePosScreenSpace(event.GetX(), event.GetY());
	ion::Vector2i mouseDelta = mousePosScreenSpace - m_mousePrevPos;
	m_mousePrevPos = mousePosScreenSpace;

	OnMouse(event, mouseDelta);
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

	ion::Matrix4 cameraInverseMtx = m_camera.GetTransform().GetInverse();
	ion::Matrix4 projectionMtx = m_renderer.GetProjectionMatrix();

	//Z order
	const float zOffset = 0.0001f;
	float z = 0.0f;

	//Render callback
	OnRender(m_renderer, cameraInverseMtx, projectionMtx, z, zOffset);

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

	m_gridPrimitive = new ion::render::Grid(ion::render::Grid::xy, ion::Vector2((float)width * 4.0f, (float)height * 4.0f), cellsX, cellsY);
}

void ViewPanel::PaintTile(TileId tileId, int x, int y, u32 flipFlags)
{
	//Set texture coords for cell
	ion::render::TexCoord coords[4];
	m_renderResources.GetTileTexCoords(tileId, coords, flipFlags);
	m_canvasPrimitive->SetTexCoords((y * m_canvasSize.x) + x, coords);
}

void ViewPanel::PaintStamp(const Stamp& stamp, int x, int y, u32 flipFlags)
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
			u32 tileFlags = stamp.GetTileFlags(sourceX, sourceY);
			tileFlags ^= flipFlags;
			int canvasX = stampX + x;
			int canvasY = stampY + y;
			int y_inv = m_canvasSize.y - 1 - canvasY;

			//Paint on canvas
			PaintTile(tileId, canvasX, y_inv, tileFlags);
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

void ViewPanel::OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta)
{
	const int tileWidth = 8;
	const int tileHeight = 8;

	//Get mouse position in panel space
	wxClientDC clientDc(this);
	wxPoint mousePanelPosWx = event.GetLogicalPosition(clientDc);

	//Centre of map quad is 0,0
	ion::Vector2 mousePos(mousePanelPosWx.x, m_panelSize.y - mousePanelPosWx.y);
	ion::Vector2 viewportSize(m_panelSize.x, m_panelSize.y);
	ion::Vector2 cameraPos(m_camera.GetPosition().x * m_cameraZoom, m_camera.GetPosition().y * m_cameraZoom);
	ion::Vector2 canvasSizePixels(m_canvasSize.x * tileWidth * m_cameraZoom, m_canvasSize.y * tileHeight * m_cameraZoom);
	ion::Vector2 mousePosCanvasSpace;
	mousePosCanvasSpace.x = (canvasSizePixels.x - (canvasSizePixels.x / 2.0f - cameraPos.x - mousePos.x)) / m_cameraZoom;
	mousePosCanvasSpace.y = (canvasSizePixels.y - (canvasSizePixels.y / 2.0f - cameraPos.y - mousePos.y)) / m_cameraZoom;

	//Get pixel x/y
	int mousePixelY_inv = (m_canvasSize.y * tileHeight) - (int)floor(mousePosCanvasSpace.y) - 1;
	ion::Vector2i mousePixelPosCanvas((int)floor(mousePosCanvasSpace.x), mousePixelY_inv);

	//Get tile x/y
	int x = (int)floor(mousePosCanvasSpace.x / (float)tileWidth);
	int y_inv = (int)floor(mousePosCanvasSpace.y / (float)tileHeight);

	//Invert for OpenGL
	int y = (m_canvasSize.y - 1 - y_inv);

	//Get button bits
	int buttonBits = 0;
	if(event.LeftIsDown())
		buttonBits |= eMouseLeft;
	if(event.MiddleIsDown())
		buttonBits |= eMouseMiddle;
	if(event.RightIsDown())
		buttonBits |= eMouseRight;

	if((buttonBits != m_prevMouseBits) || (x != m_prevMouseOverTilePos.x) || (y != m_prevMouseOverTilePos.y))
	{
		//Mouse button clicked or changed grid pos
		OnMouseTileEvent(buttonBits, x, y);

		m_prevMouseOverTilePos.x = x;
		m_prevMouseOverTilePos.y = y;
	}

	if((buttonBits != m_prevMouseBits) || (mousePixelPosCanvas.x != m_prevMouseOverPixelPos.x) || (mousePixelPosCanvas.y != m_prevMouseOverPixelPos.y))
	{
		//Mouse button clicked or changed pixel pos
		OnMousePixelEvent(mousePixelPosCanvas, mouseDelta, buttonBits, x, y);
		m_prevMouseOverPixelPos = mousePixelPosCanvas;
	}

	m_prevMouseBits = buttonBits;

	//Camera pan/zoom
	float zoomDelta = 0.0f;

	if(event.Dragging() && event.ButtonIsDown(wxMOUSE_BTN_MIDDLE))
	{
		if(event.ShiftDown())
		{
			//SHIFT + middle mouse + drag = zoom
			float zoomSpeed = 0.05f;
			zoomDelta -= mouseDelta.y * zoomSpeed;
		}
		else
		{
			if(m_enablePan)
			{
				//Middle mouse + drag = pan
				ion::Vector3 cameraPos = m_camera.GetPosition();
				cameraPos.x -= mouseDelta.x * m_cameraPanSpeed / m_cameraZoom;
				cameraPos.y += mouseDelta.y * m_cameraPanSpeed / m_cameraZoom;
				m_camera.SetPosition(cameraPos);

				//Invalidate rect
				Refresh();
			}
		}
	}
	else if(event.GetWheelRotation() != 0)
	{
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
			zoomDelta = zoomSpeed;
		else if(wheelDelta < 0)
			zoomDelta = -zoomSpeed;
	}

	if(m_enableZoom && zoomDelta != 0.0f)
	{
		float zoom = m_cameraZoom + zoomDelta;

		//Clamp
		if(zoom < 0.2f)
			zoom = 0.2f;
		else if(zoom > 10.0f)
			zoom = 10.0f;

		//Set camera zoom
		SetCameraZoom(zoom);

		//Invalidate rect
		Refresh();
	}
}

void ViewPanel::OnKeyboard(wxKeyEvent& event)
{

}

void ViewPanel::RenderCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//No depth test (stops grid cells Z fighting)
	renderer.SetDepthTest(ion::render::Renderer::eAlways);

	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTileset);

	//Draw map
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	material->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_canvasPrimitive->GetVertexBuffer(), m_canvasPrimitive->GetIndexBuffer());
	material->Unbind();

	renderer.SetDepthTest(ion::render::Renderer::eLessEqual);
}

void ViewPanel::RenderGrid(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//Draw grid
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
	const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourGrid);

	ion::Matrix4 gridMtx;
	gridMtx.SetTranslation(ion::Vector3(0.0f, 0.0f, z));
	gridMtx.SetScale(ion::Vector3((float)m_project.GetGridSize(), (float)m_project.GetGridSize(), 1.0f));
	material->SetDiffuseColour(colour);
	material->Bind(gridMtx, cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_gridPrimitive->GetVertexBuffer());
	material->Unbind();
}

void ViewPanel::OnResize(wxSizeEvent& event)
{
	wxSize clientSize = event.GetSize();
	m_panelSize.x = clientSize.x;
	m_panelSize.y = clientSize.y;

	if(m_panelSize.x > 1 && m_panelSize.y > 1)
	{
		//Filter out superflous resize events (wx sends them if UI thread doesn't respond during saving/loading)
		if(m_prevPanelSize.x != m_panelSize.x || m_prevPanelSize.y != m_panelSize.y)
		{
			m_prevPanelSize = m_panelSize;
			m_viewport.Resize(m_panelSize.x, m_panelSize.y);
			CentreCamera();
		}
	}

	Refresh();
}

void ViewPanel::CentreCamera()
{
	//Reset zoom to identity
	float zoom = m_cameraZoom;
	SetCameraZoom(1.0f);

	//Centre camera
	ion::Vector3 cameraPos(-(m_panelSize.x / 2.0f), -(m_panelSize.y / 2.0f), 0.0f);
	m_camera.SetPosition(cameraPos);

	//Re-apply zoom
	SetCameraZoom(zoom);
}

void ViewPanel::SetCameraZoom(float zoom)
{
	float prevZoom = m_cameraZoom;

	//Set camera zoom
	m_camera.SetZoom(ion::Vector3(zoom, zoom, 1.0f));

	//Compensate camera pos
	ion::Vector2 originalViewportSize((float)m_panelSize.x / prevZoom, (float)m_panelSize.y / prevZoom);
	ion::Vector2 newViewportSize((float)m_panelSize.x / zoom, (float)m_panelSize.y / zoom);
	ion::Vector3 cameraPos = m_camera.GetPosition();
	cameraPos.x -= (newViewportSize.x - originalViewportSize.x) / 2.0f;
	cameraPos.y -= (newViewportSize.y - originalViewportSize.y) / 2.0f;
	m_camera.SetPosition(cameraPos);

	m_cameraZoom = zoom;
}