///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "SpriteCanvas.h"

SpriteCanvas::SpriteCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxGLCanvas(parent, id, pos, size, style, name)
	, m_viewport(128, 128, ion::render::Viewport::Ortho2DAbsolute)
{
	m_gridPrimitive = NULL;
	m_cameraZoom = 1.0f;
	m_gridColour = ion::Colour(0.0f, 0.0f, 0.0f, 1.0f);

	//Set viewport clear colour
	m_viewport.SetClearColour(ion::Colour(0.3f, 0.3f, 0.3f));

	//Subscribe to events
	Bind(wxEVT_PAINT, &SpriteCanvas::EventHandlerPaint, this, GetId());
	Bind(wxEVT_SIZE, &SpriteCanvas::EventHandlerResize, this, GetId());
	Bind(wxEVT_MOUSEWHEEL, &SpriteCanvas::EventHandlerMouse, this, GetId());
}

SpriteCanvas::~SpriteCanvas()
{
	if(m_gridPrimitive)
		delete m_gridPrimitive;
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

void SpriteCanvas::OnMouse(wxMouseEvent& event, const ion::Vector2& mouseDelta)
{
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

		m_cameraZoom = ion::maths::Clamp(m_cameraZoom, 0.1f, 10.0f);

		Refresh();
	}
}

void SpriteCanvas::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	//Render preview
	RenderPreview(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;

	//Render grid
	RenderGrid(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;
}

void SpriteCanvas::RenderSpriteSheet(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{

}

void SpriteCanvas::RenderPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialSprite);
	ion::render::Primitive* primitive = m_renderResources->GetPrimitive(RenderResources::ePrimitiveUnitQuad);

	ion::Matrix4 boxMtx;
	ion::Vector3 boxScale((m_spriteSheetDimentions.x / 8.0f) * m_cameraZoom, (m_spriteSheetDimentions.y / 8.0f) * m_cameraZoom, 0.0f);
	ion::Vector3 boxPos(0.0f, 0.0f, z);

	boxMtx.SetTranslation(boxPos);
	boxMtx.SetScale(boxScale);

	//gridMtx.SetScale(ion::Vector3((float)m_project->GetGridSize(), (float)m_project->GetGridSize(), 1.0f));
	material->Bind(boxMtx, cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
	material->Unbind();
}

void SpriteCanvas::RenderGrid(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_gridPrimitive)
	{
		//Draw grid
		ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialFlatColour);

		ion::Matrix4 gridMtx;
		gridMtx.SetTranslation(ion::Vector3(0.0f, 0.0f, z));
		gridMtx.SetScale(ion::Vector3(m_cameraZoom, m_cameraZoom, 1.0f));
		material->SetDiffuseColour(m_gridColour);
		material->Bind(gridMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(m_gridPrimitive->GetVertexBuffer());
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
	ion::Vector2 mousePosScreenSpace(event.GetX(), event.GetY());
	ion::Vector2 mouseDelta = mousePosScreenSpace - m_mousePrevPos;
	m_mousePrevPos = mousePosScreenSpace;

	OnMouse(event, mouseDelta);
	event.Skip();
}