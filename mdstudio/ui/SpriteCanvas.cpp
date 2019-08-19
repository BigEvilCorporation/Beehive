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

#include "SpriteCanvas.h"
#include "Mouse.h"

#include <wx/dc.h>
#include <wx/dcclient.h>

SpriteCanvas::SpriteCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxGLCanvas(parent, id, NULL, pos, size, style, name)
	, m_viewport(128, 128, ion::render::Viewport::eOrtho2DAbsolute)
{
	m_gridPrimitive = NULL;
	m_boundsPrimitive = NULL;
	m_cameraZoom = 1.0f;
	m_gridColour = ion::Colour(0.0f, 0.0f, 0.0f, 1.0f);
	m_drawPreview = false;
	m_drawPreviewMaxFrames = 0;
	m_drawGrid = false;
	m_drawSpriteSheet = InvalidSpriteSheetId;
	m_drawSpriteSheetFrame = 0;
	m_tileFramePrimitive = NULL;

	//Set viewport clear colour
	m_viewport.SetClearColour(ion::Colour(0.3f, 0.3f, 0.3f));

	//Subscribe to events
	Bind(wxEVT_PAINT, &SpriteCanvas::EventHandlerPaint, this, GetId());
	Bind(wxEVT_SIZE, &SpriteCanvas::EventHandlerResize, this, GetId());
	Bind(wxEVT_LEFT_DOWN, &SpriteCanvas::EventHandlerMouse, this, GetId());
	Bind(wxEVT_LEFT_UP, &SpriteCanvas::EventHandlerMouse, this, GetId());
	Bind(wxEVT_LEFT_DCLICK, &SpriteCanvas::EventHandlerMouse, this, GetId());
	Bind(wxEVT_MIDDLE_DOWN, &SpriteCanvas::EventHandlerMouse, this, GetId());
	Bind(wxEVT_MIDDLE_UP, &SpriteCanvas::EventHandlerMouse, this, GetId());
	Bind(wxEVT_RIGHT_DOWN, &SpriteCanvas::EventHandlerMouse, this, GetId());
	Bind(wxEVT_RIGHT_UP, &SpriteCanvas::EventHandlerMouse, this, GetId());
	Bind(wxEVT_MOTION, &SpriteCanvas::EventHandlerMouse, this, GetId());
	Bind(wxEVT_MOUSEWHEEL, &SpriteCanvas::EventHandlerMouse, this, GetId());
}

SpriteCanvas::~SpriteCanvas()
{
	if(m_gridPrimitive)
		delete m_gridPrimitive;
	if(m_boundsPrimitive)
		delete m_boundsPrimitive;
}

void SpriteCanvas::SetupRendering(ion::render::Renderer* renderer, wxGLContext* glContext, RenderResources* renderResources)
{
	m_renderer = renderer;
	m_renderResources = renderResources;
	SetCurrent(*glContext);
}

void SpriteCanvas::SetSpriteSheetDimentionsPixels(const ion::Vector2i& spriteSheetDimentions)
{
	m_spriteSheetDimentions = spriteSheetDimentions;

	CreateGrid(m_spriteSheetDimentions.x, m_spriteSheetDimentions.y, m_spriteSheetDimentionsCells.x, m_spriteSheetDimentionsCells.y);

	Refresh();
}

void SpriteCanvas::SetSpriteSheetDimentionsCells(const ion::Vector2i& spriteSheetDimentionsCells)
{
	m_spriteSheetDimentionsCells = spriteSheetDimentionsCells;

	CreateGrid(m_spriteSheetDimentions.x, m_spriteSheetDimentions.y, m_spriteSheetDimentionsCells.x, m_spriteSheetDimentionsCells.y);

	Refresh();
}

void SpriteCanvas::SetGridColour(const ion::Colour& colour)
{
	m_gridColour = colour;
	Refresh();
}

void SpriteCanvas::SetDrawPreview(bool drawPreview, u32 maxFrames)
{
	m_drawPreview = drawPreview;
	m_drawPreviewMaxFrames = maxFrames;
	Refresh();
}

void SpriteCanvas::SetDrawGrid(bool drawGrid)
{
	m_drawGrid = drawGrid;
	Refresh();
}

void SpriteCanvas::SetDrawSpriteSheet(SpriteSheetId spriteSheet, u32 frame, const ion::Vector2i& size, const ion::Vector2i& offset, const ion::Vector2i& topLeft, const ion::Vector2i& bottomRight)
{
	m_drawSpriteSheet = spriteSheet;
	m_drawSpriteSheetFrame = frame;
	m_drawOffset = offset;
	m_topLeft = topLeft;
	m_bottomRight = bottomRight;

	if(m_boundsPrimitive)
		delete m_boundsPrimitive;

	ion::Vector2 boundsHalfExtents((float)(bottomRight.x - topLeft.x) / 2.0f, (float)(bottomRight.y - topLeft.y) / 2.0f);
	ion::Vector2 boundsOffset((float)((-size.x / 2.0f) + boundsHalfExtents.x + topLeft.x), (float)((size.y / 2.0f) - boundsHalfExtents.y - topLeft.y));
	m_boundsPrimitive = new ion::render::LineQuad(ion::render::LineQuad::xy, boundsHalfExtents, boundsOffset);

	Refresh();
}

void SpriteCanvas::Refresh(bool eraseBackground, const wxRect *rect)
{
	wxGLCanvas::Refresh(eraseBackground, rect);
}

void SpriteCanvas::CreateGrid(int width, int height, int cellsX, int cellsY)
{
	if(m_gridPrimitive)
		delete m_gridPrimitive;

	m_gridPrimitive = new ion::render::Grid(ion::render::Grid::xy, ion::Vector2((float)width / 2, (float)height / 2), cellsX, cellsY);
}

void SpriteCanvas::OnResize(wxSizeEvent& event)
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
			
			//Centre camera
			ion::Vector3 cameraPos(-(m_panelSize.x / 2.0f), -(m_panelSize.y / 2.0f), 0.0f);
			m_camera.SetPosition(cameraPos);
		}
	}

	Refresh();
}

void SpriteCanvas::OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta)
{
	const int tileWidth = m_project->GetPlatformConfig().tileWidth;
	const int tileHeight = m_project->GetPlatformConfig().tileHeight;

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
	if (event.LeftIsDown())
		buttonBits |= eMouseLeft;
	if (event.MiddleIsDown())
		buttonBits |= eMouseMiddle;
	if (event.RightIsDown())
		buttonBits |= eMouseRight;

	ion::Vector2i tileDelta(x - m_prevMouseOverTilePos.x, y - m_prevMouseOverTilePos.y);

	if ((buttonBits != m_prevMouseBits) || (x != m_prevMouseOverTilePos.x) || (y != m_prevMouseOverTilePos.y))
	{
		//Mouse button clicked or changed grid pos
		OnMouseTileEvent(mousePixelPosCanvas, mouseDelta, tileDelta, buttonBits, x, y);

		m_prevMouseOverTilePos.x = x;
		m_prevMouseOverTilePos.y = y;
	}

	if ((buttonBits != m_prevMouseBits) || (mousePixelPosCanvas.x != m_prevMouseOverPixelPos.x) || (mousePixelPosCanvas.y != m_prevMouseOverPixelPos.y))
	{
		//Mouse button clicked or changed pixel pos
		OnMousePixelEvent(mousePixelPosCanvas, mouseDelta, tileDelta, buttonBits, x, y);
		m_prevMouseOverPixelPos = mousePixelPosCanvas;
	}

	m_prevMouseBits = buttonBits;

	if(event.GetWheelRotation() != 0)
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
			m_cameraZoom += zoomSpeed;
		else if(wheelDelta < 0)
			m_cameraZoom -= zoomSpeed;

		//Set camera zoom
		SetCameraZoom(ion::maths::Clamp(m_cameraZoom, 0.1f, 10.0f));

		Refresh();
	}
}

void SpriteCanvas::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	//Render preview
	RenderPreview(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;

	//Render SpriteSheet
	RenderSpriteSheet(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;

	//Render tile frame
	RenderTileFrame(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;

	//Render grid
	RenderGrid(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;

	//Render bounding box
	RenderBounds(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;
}

void SpriteCanvas::RenderSpriteSheet(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_drawSpriteSheet != InvalidSpriteSheetId)
	{
		const int tileWidth = m_project->GetPlatformConfig().tileWidth;
		const int tileHeight = m_project->GetPlatformConfig().tileHeight;

		//Render spriteSheet
		RenderResources::SpriteSheetRenderResources* spriteSheetResources = m_renderResources->GetSpriteSheetResources(m_drawSpriteSheet);
		ion::debug::Assert(spriteSheetResources, "SpriteCanvas::RenderSpriteSheet() - Missing spriteSheet render resources");
		ion::debug::Assert(spriteSheetResources->m_frames.size() > m_drawSpriteSheetFrame, "SpriteCanvas::RenderSpriteSheet() - Invalid frame");

		ion::render::Primitive* primitive = spriteSheetResources->m_primitive;
		ion::render::Material* material = spriteSheetResources->m_frames[m_drawSpriteSheetFrame].material;
		material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));

		ion::Matrix4 boxMtx;
		ion::Vector3 boxPos(m_drawOffset.x, m_drawOffset.y, z);

		boxMtx.SetTranslation(boxPos);

		material->Bind(boxMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		material->Unbind();
	}
}

void SpriteCanvas::RenderTileFrame(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_tileFramePrimitive)
	{
		const int tileWidth = m_project->GetPlatformConfig().tileWidth;
		const int tileHeight = m_project->GetPlatformConfig().tileHeight;

		//Render spriteSheet
		ion::render::Primitive* primitive = m_tileFramePrimitive;
		ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialTileset);
		material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));

		ion::Matrix4 boxMtx;
		ion::Vector3 boxPos(m_drawOffset.x, m_drawOffset.y, z);

		boxMtx.SetTranslation(boxPos);

		material->Bind(boxMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		material->Unbind();
	}
}

void SpriteCanvas::RenderPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_drawPreview)
	{
		ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialSpriteSheet);
		ion::render::Primitive* primitive = m_renderResources->GetPrimitive(RenderResources::ePrimitiveUnitQuad);

		ion::Matrix4 boxMtx;
		ion::Vector3 boxScale(m_spriteSheetDimentions.x, m_spriteSheetDimentions.y, 0.0f);
		ion::Vector3 boxPos(0.0f, 0.0f, z);

		boxMtx.SetTranslation(boxPos);
		boxMtx.SetScale(boxScale);

		material->Bind(boxMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		material->Unbind();

		if(m_drawPreviewMaxFrames > 0)
		{
			//Grey out invalid frames
			material = m_renderResources->GetMaterial(RenderResources::eMaterialFlatColour);
			material->SetDiffuseColour(ion::Colour(0.3f, 0.3f, 0.3f));

			int lastFrame = m_spriteSheetDimentionsCells.x * m_spriteSheetDimentionsCells.y;
			int firstFrame = m_drawPreviewMaxFrames;

			float frameWidth = (m_spriteSheetDimentions.x / m_spriteSheetDimentionsCells.x);
			float frameHeight = (m_spriteSheetDimentions.y / m_spriteSheetDimentionsCells.y);
			float sheetWidth = m_spriteSheetDimentions.x;
			float sheetHeight = m_spriteSheetDimentions.y;

			for(int i = firstFrame; i < lastFrame; i++)
			{
				int frameX = m_spriteSheetDimentionsCells.x - 1 - (i % m_spriteSheetDimentionsCells.x);
				int frameY = i / m_spriteSheetDimentionsCells.x;

				boxScale = ion::Vector3(frameWidth, frameHeight, 0.0f);
				boxPos = ion::Vector3((sheetWidth / 2.0f) - (frameWidth / 2.0f) - (frameX * frameWidth), (sheetHeight / 2.0f) - (frameHeight / 2.0f) - (frameY * frameHeight), z);

				boxMtx.SetTranslation(boxPos);
				boxMtx.SetScale(boxScale);

				material->Bind(boxMtx, cameraInverseMtx, projectionMtx);
				renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
				material->Unbind();
			}
		}
	}
}

void SpriteCanvas::RenderGrid(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_gridPrimitive && m_drawGrid)
	{
		//Draw grid
		ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialFlatColour);

		ion::Matrix4 gridMtx;
		gridMtx.SetTranslation(ion::Vector3(0.0f, 0.0f, z));
		material->SetDiffuseColour(m_gridColour);
		material->Bind(gridMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(m_gridPrimitive->GetVertexBuffer());
		material->Unbind();
	}
}

void SpriteCanvas::RenderBounds(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_boundsPrimitive)
	{
		ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialFlatColour);
		const ion::Colour& colour = m_renderResources->GetColour(RenderResources::eColourOutline);

		ion::Matrix4 outlineMtx;
		ion::Vector2i size = (m_bottomRight - m_topLeft);
		outlineMtx.SetTranslation(ion::Vector3(m_drawOffset.x, m_drawOffset.y, z));

		material->SetDiffuseColour(colour);
		material->Bind(outlineMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(m_boundsPrimitive->GetVertexBuffer());
		material->Unbind();
	}
}

void SpriteCanvas::EventHandlerPaint(wxPaintEvent& event)
{
	//Begin rendering to current viewport
	m_renderer->BeginFrame(m_viewport, GetHDC());
	m_renderer->ClearColour();
	m_renderer->ClearDepth();

	ion::Matrix4 cameraInverseMtx = m_camera.GetTransform().GetInverse();
	ion::Matrix4 projectionMtx = m_renderer->GetProjectionMatrix();

	//Z order
	const float zOffset = 0.0001f;
	float z = 0.0f;

	//Render callback
	OnRender(*m_renderer, cameraInverseMtx, projectionMtx, z, zOffset);

	//End rendering
	m_renderer->SwapBuffers();
	m_renderer->EndFrame();

	event.Skip();
}

void SpriteCanvas::EventHandlerResize(wxSizeEvent& event)
{
	OnResize(event);
	event.Skip();
}

void SpriteCanvas::EventHandlerMouse(wxMouseEvent& event)
{
	//Get mouse delta
	ion::Vector2i mousePosScreenSpace(event.GetX(), event.GetY());
	ion::Vector2i mouseDelta = mousePosScreenSpace - m_mousePrevPos;
	m_mousePrevPos = mousePosScreenSpace;

	OnMouse(event, mouseDelta);
	event.Skip();
}

void SpriteCanvas::SetCameraZoom(float zoom)
{
	float prevZoom = m_cameraZoom;

	//Set camera zoom
	m_camera.SetZoom(ion::Vector3(zoom, zoom, 1.0f));

	ion::Vector3 cameraPos(-((m_panelSize.x / zoom) / 2.0f), -((m_panelSize.y / zoom) / 2.0f), 0.0f);
	m_camera.SetPosition(cameraPos);
	m_cameraZoom = zoom;
}