///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "CollisionTilesPanel.h"
#include "MainWindow.h"

#include <algorithm>

const float CollisionTilesPanel::s_CollisionTileSize = 4.0f;

CollisionTilesPanel::CollisionTilesPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, renderer, glContext, renderResources, parent, winid, pos, size, style, name)
{
	m_selectedCollisionTile = InvalidCollisionTileId;
	m_hoverCollisionTile = InvalidCollisionTileId;

	//Custom zoom/pan handling
	EnableZoom(false);
	EnablePan(false);

	//Create selection quad
	m_selectionPrimitive = new ion::render::Quad(ion::render::Quad::xy, ion::Vector2(4.0f, 4.0f));
}

CollisionTilesPanel::~CollisionTilesPanel()
{

}

void CollisionTilesPanel::OnMouse(wxMouseEvent& event, const ion::Vector2& mouseDelta)
{
	ViewPanel::OnMouse(event, mouseDelta);

	//Camera pan Y (if canvas is taller than panel)
	if((m_canvasSize.y * 8.0f) > (m_panelSize.y / m_cameraZoom))
	{
		float panDeltaY = 0.0f;

		if(event.Dragging() && event.ButtonIsDown(wxMOUSE_BTN_MIDDLE))
		{
			panDeltaY = mouseDelta.y;
		}
		else if(event.GetWheelRotation() != 0)
		{
			panDeltaY = (float)event.GetWheelRotation();
		}

		if(panDeltaY != 0.0f)
		{
			ion::Vector3 cameraPos = m_camera.GetPosition();
			cameraPos.y += panDeltaY * m_cameraPanSpeed / m_cameraZoom;

			//Clamp to size
			float halfCanvas = (m_canvasSize.y * 4.0f);
			float minY = -halfCanvas;
			float maxY = halfCanvas - ((float)m_panelSize.y / m_cameraZoom);

			if(cameraPos.y > maxY)
				cameraPos.y = maxY;
			else if(cameraPos.y < minY)
				cameraPos.y = minY;

			m_camera.SetPosition(cameraPos);

			Refresh();
		}
	}
}

void CollisionTilesPanel::OnKeyboard(wxKeyEvent& event)
{
	ViewPanel::OnKeyboard(event);
}

void CollisionTilesPanel::OnResize(wxSizeEvent& event)
{
	ViewPanel::OnResize(event);

	if(m_panelSize.x > 8 && m_panelSize.y > 8)
	{
		ion::Vector2i canvasSize = CalcCanvasSize();

		if(canvasSize.x != m_canvasSize.x || canvasSize.y != m_canvasSize.y)
		{
			InitPanel(canvasSize.x, canvasSize.y);
		}
	}

	ResetZoomPan();
}

void CollisionTilesPanel::OnMouseTileEvent(int buttonBits, int x, int y)
{
	ViewPanel::OnMouseTileEvent(buttonBits, x, y);

	CollisionTileId selectedCollisionTile = InvalidCollisionTileId;

	//If in range, get CollisionTile under mouse cursor
	if(x >= 0 && y >= 0 && x < m_canvasSize.x && y < m_canvasSize.y)
	{
		selectedCollisionTile = (y * m_canvasSize.x) + x;
	}

	//Set mouse hover CollisionTile
	m_hoverCollisionTile = selectedCollisionTile;
	m_hoverCollisionTilePos.x = x;
	m_hoverCollisionTilePos.y = y;

	if((buttonBits & eMouseLeft) && !(m_prevMouseBits & eMouseLeft))
	{
		//Left click, set current CollisionTile
		m_selectedCollisionTile = selectedCollisionTile;
		m_selectedCollisionTilePos = m_hoverCollisionTilePos;

		//Set as current painting CollisionTile
		m_project->SetPaintCollisionTile(selectedCollisionTile);

		//Set CollisionTile paint tool
		m_mainWindow->SetMapTool(eToolPaintCollisionTile);

		//Refresh CollisionTile editor panel
		m_mainWindow->RefreshPanel(MainWindow::ePanelCollisionTileEditor);
	}

	//Redraw
	Refresh();
}

void CollisionTilesPanel::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	//Render canvas
	RenderCanvas(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	//Render selected CollisionTile
	if(m_selectedCollisionTile)
	{
		if(CollisionTile* collisionTile = m_project->GetCollisionTileset().GetCollisionTile(m_selectedCollisionTile))
		{
			ion::Vector2 size(1, 1);
			const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourSelected);
			RenderBox(m_selectedCollisionTilePos, size, colour, renderer, cameraInverseMtx, projectionMtx, z);
		}
	}

	z += zOffset;

	//Render mouse hover CollisionTile
	if(m_hoverCollisionTile && m_hoverCollisionTile != m_selectedCollisionTile)
	{
		if(CollisionTile* collisionTile = m_project->GetCollisionTileset().GetCollisionTile(m_hoverCollisionTile))
		{
			ion::Vector2 size(1, 1);
			const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourHighlight);
			RenderBox(m_hoverCollisionTilePos, size, colour, renderer, cameraInverseMtx, projectionMtx, z);
		}
	}

	z += zOffset;

	//Render grid
	if(m_project->GetShowGrid())
	{
		RenderGrid(renderer, cameraInverseMtx, projectionMtx, z);
	}
}

void CollisionTilesPanel::SetProject(Project* project)
{
	ViewPanel::SetProject(project);

	//Refresh
	Refresh();
}

void CollisionTilesPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(m_project)
	{
		//If CollisionTiles invalidated
		if(m_project->CollisionTilesAreInvalidated())
		{
			if(m_panelSize.x > 8 && m_panelSize.y > 8)
			{
				ion::Vector2i canvasSize = CalcCanvasSize();
				InitPanel(canvasSize.x, canvasSize.y);
			}
		}
	}

	ViewPanel::Refresh(eraseBackground, rect);
}

ion::Vector2i CollisionTilesPanel::CalcCanvasSize()
{
	wxSize panelSize = GetClientSize();
	int numCollisionTiles = m_project->GetCollisionTileset().GetCount();
	int numCols = ion::maths::Ceil((float)panelSize.x / 8.0f / s_CollisionTileSize);
	int numRows = max(numCols, (int)ion::maths::Ceil((float)numCollisionTiles / (float)numCols / s_CollisionTileSize));
	return ion::Vector2i(numCols, numRows);
}

void CollisionTilesPanel::InitPanel(int numCols, int numRows)
{
	//Recreate canvas
	CreateCanvas(numCols, numRows);

	//Fill with invalid CollisionTile
	FillCollisionTiles(InvalidCollisionTileId, ion::Vector2i(0, 0), ion::Vector2i(numCols - 1, numRows - 1));

	//Redraw CollisionTiles on canvas
	PaintCollisionTiles();

	//Recreate grid
	CreateGrid(numCols, numRows, numCols, numRows);

	//Centre camera and reset zoom
	ResetZoomPan();
}

void CollisionTilesPanel::PaintCollisionTiles()
{
	CollisionTileset& collisionTileset = m_project->GetCollisionTileset();

	for(int i = 0; i < collisionTileset.GetCount(); i++)
	{
		int x = max(0, i % m_canvasSize.x);
		int y = max(0, m_canvasSize.y - 1 - (i / m_canvasSize.x));

		PaintCollisionTile(i, x, y);
	}
}

void CollisionTilesPanel::PaintCollisionTile(CollisionTileId tileId, int x, int y)
{
	if(m_project)
	{
		//Set texture coords for cell
		ion::render::TexCoord coords[4];
		m_renderResources.GetCollisionTileTexCoords(tileId, coords);
		m_canvasPrimitive->SetTexCoords((y * m_canvasSize.x) + x, coords);
	}
}

void CollisionTilesPanel::FillCollisionTiles(CollisionTileId tileId, const ion::Vector2i& boxCorner1, const ion::Vector2i& boxCorner2)
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
			PaintCollisionTile(tileId, x, y_inv);
		}
	}
}

void CollisionTilesPanel::RenderCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//No depth test (stops grid cells Z fighting)
	renderer.SetDepthTest(ion::render::Renderer::Always);

	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialCollisionTileset);
	const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourHighlight);

	//Draw map
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	material->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_canvasPrimitive->GetVertexBuffer(), m_canvasPrimitive->GetIndexBuffer());
	material->Unbind();

	renderer.SetDepthTest(ion::render::Renderer::LessEqual);
}

void CollisionTilesPanel::RenderBox(const ion::Vector2i& pos, const ion::Vector2& size, const ion::Colour& colour, ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	const int CollisionTileWidth = 8;
	const int CollisionTileHeight = 8;
	const int quadHalfExtentsX = 4;
	const int quadHalfExtentsY = 4;

	float bottom = (m_canvasSize.y - (pos.y + size.y));

	ion::Matrix4 boxMtx;
	ion::Vector3 boxScale(size.x, size.y, 0.0f);
	ion::Vector3 boxPos(floor((pos.x - (m_canvasSize.x / 2.0f) + (boxScale.x / 2.0f)) * CollisionTileWidth),
		floor((bottom - (m_canvasSize.y / 2.0f) + (boxScale.y / 2.0f)) * CollisionTileHeight), z);

	boxMtx.SetTranslation(boxPos);
	boxMtx.SetScale(boxScale);

	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);

	renderer.SetAlphaBlending(ion::render::Renderer::Translucent);
	material->SetDiffuseColour(colour);
	material->Bind(boxMtx, cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_selectionPrimitive->GetVertexBuffer(), m_selectionPrimitive->GetIndexBuffer());
	material->Unbind();
	renderer.SetAlphaBlending(ion::render::Renderer::NoBlend);
}

void CollisionTilesPanel::ResetZoomPan()
{
	m_cameraZoom = (float)m_panelSize.x / (m_canvasSize.x * 8.0f);

	//Scroll to top
	float halfCanvas = (m_canvasSize.y * 4.0f);
	float maxY = halfCanvas - ((float)m_panelSize.y / m_cameraZoom);
	ion::Vector3 cameraPos(-(m_panelSize.x / 2.0f / m_cameraZoom), maxY, 0.0f);

	m_camera.SetZoom(ion::Vector3(m_cameraZoom, m_cameraZoom, 1.0f));
	m_camera.SetPosition(cameraPos);
}

void CollisionTilesPanel::ScrollToTop()
{
	//Reset zoom to identity
	//float zoom = m_cameraZoom;
	//SetCameraZoom(1.0f);

	//Scroll to top
	float halfCanvas = (m_canvasSize.y * 4.0f);
	float maxY = halfCanvas - ((float)m_panelSize.y / m_cameraZoom);
	ion::Vector3 cameraPos(-(m_panelSize.x / 2.0f / m_cameraZoom), maxY, 0.0f);

	//Re-apply zoom
	//SetCameraZoom(zoom);
}