///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "TerrainTilesPanel.h"
#include "MainWindow.h"

#include <wx/Menu.h>

#include <algorithm>

const float TerrainTilesPanel::s_TerrainTileSize = 4.0f;

TerrainTilesPanel::TerrainTilesPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, project, renderer, glContext, renderResources, parent, winid, pos, size, style, name)
{
	m_selectedTerrainTile = InvalidTerrainTileId;
	m_hoverTerrainTile = InvalidTerrainTileId;

	//Custom zoom/pan handling
	EnableZoom(false);
	EnablePan(false);

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	//Create selection quad
	m_selectionPrimitive = new ion::render::Quad(ion::render::Quad::xy, ion::Vector2(tileWidth / 2.0f, tileHeight / 2.0f));
}

TerrainTilesPanel::~TerrainTilesPanel()
{

}

void TerrainTilesPanel::OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta)
{
	ViewPanel::OnMouse(event, mouseDelta);

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	//Camera pan Y (if canvas is taller than panel)
	if((m_canvasSize.y * tileHeight) > (m_panelSize.y / m_cameraZoom))
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
			float halfCanvas = (m_canvasSize.y * (tileHeight / 2.0f));
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

void TerrainTilesPanel::OnKeyboard(wxKeyEvent& event)
{
	ViewPanel::OnKeyboard(event);
}

void TerrainTilesPanel::OnResize(wxSizeEvent& event)
{
	ViewPanel::OnResize(event);

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	if(m_panelSize.x > tileWidth && m_panelSize.y > tileHeight)
	{
		ion::Vector2i canvasSize = CalcCanvasSize();

		if(canvasSize.x != m_canvasSize.x || canvasSize.y != m_canvasSize.y)
		{
			InitPanel(canvasSize.x, canvasSize.y);
		}
	}

	ResetZoomPan();
}

void TerrainTilesPanel::OnMouseTileEvent(int buttonBits, int x, int y)
{
	ViewPanel::OnMouseTileEvent(buttonBits, x, y);

	TerrainTileId selectedTerrainTile = InvalidTerrainTileId;

	//If in range, get terrain tile under mouse cursor
	if(x >= 0 && y >= 0 && x < m_canvasSize.x && y < m_canvasSize.y)
	{
		selectedTerrainTile = (y * m_canvasSize.x) + x;
	}

	//Set mouse hover terrain tile
	m_hoverTerrainTile = selectedTerrainTile;
	m_hoverTerrainTilePos.x = x;
	m_hoverTerrainTilePos.y = y;

	if((buttonBits & eMouseLeft) && !(m_prevMouseBits & eMouseLeft))
	{
		//Left click, set current terrain tile
		m_selectedTerrainTile = selectedTerrainTile;
		m_selectedTerrainTilePos = m_hoverTerrainTilePos;

		//Set as current painting terrain tile
		m_project.SetPaintTerrainTile(selectedTerrainTile);

		//Set terrain tile paint tool
		m_mainWindow->SetMapTool(eToolPaintTerrainTile);

		//Refresh terrain tile editor panel
		m_mainWindow->RefreshPanel(MainWindow::ePanelTerrainTileEditor);
	}

	if(buttonBits & eMouseRight)
	{
		if(m_hoverTerrainTile != InvalidTerrainTileId)
		{
			//Right-click menu
			wxMenu contextMenu;

			contextMenu.Append(eMenuDeleteTile, wxString("Delete terrain tile"));
			contextMenu.Append(eMenuUseAsDefaultTile, wxString("Use as default terrain tile"));
			contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&TerrainTilesPanel::OnContextMenuClick, NULL, this);
			PopupMenu(&contextMenu);
		}
	}

	//Redraw
	Refresh();
}

void TerrainTilesPanel::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	//Render canvas
	RenderCanvas(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	//Render selected TerrainTile
	if(m_selectedTerrainTile != InvalidTerrainTileId)
	{
		if(TerrainTile* terrainTile = m_project.GetTerrainTileset().GetTerrainTile(m_selectedTerrainTile))
		{
			ion::Vector2 size(1, 1);
			const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourSelected);
			RenderBox(m_selectedTerrainTilePos, size, colour, renderer, cameraInverseMtx, projectionMtx, z);
		}
	}

	z += zOffset;

	//Render mouse hover TerrainTile
	if(m_hoverTerrainTile != InvalidTerrainTileId && m_hoverTerrainTile != m_selectedTerrainTile)
	{
		if(TerrainTile* terrainTile = m_project.GetTerrainTileset().GetTerrainTile(m_hoverTerrainTile))
		{
			ion::Vector2 size(1, 1);
			const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourHighlight);
			RenderBox(m_hoverTerrainTilePos, size, colour, renderer, cameraInverseMtx, projectionMtx, z);
		}
	}

	z += zOffset;

	//Render grid
	if(m_project.GetShowGrid())
	{
		RenderGrid(renderer, cameraInverseMtx, projectionMtx, z);
	}
}

void TerrainTilesPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	//If TerrainTiles invalidated
	if(m_project.TerrainTilesAreInvalidated())
	{
		const int tileWidth = m_project.GetPlatformConfig().tileWidth;
		const int tileHeight = m_project.GetPlatformConfig().tileHeight;

		if(m_panelSize.x > tileWidth && m_panelSize.y > tileHeight)
		{
			ion::Vector2i canvasSize = CalcCanvasSize();
			InitPanel(canvasSize.x, canvasSize.y);
		}
	}

	ViewPanel::Refresh(eraseBackground, rect);
}

ion::Vector2i TerrainTilesPanel::CalcCanvasSize()
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;
	wxSize panelSize = GetClientSize();
	int numTerrainTiles = m_project.GetTerrainTileset().GetCount();
	int numCols = ion::maths::Ceil((float)panelSize.x / tileWidth / s_TerrainTileSize);
	int numRows = max(numCols, (int)ion::maths::Ceil((float)numTerrainTiles / (float)numCols / s_TerrainTileSize));
	return ion::Vector2i(numCols, numRows);
}

void TerrainTilesPanel::InitPanel(int numCols, int numRows)
{
	//Recreate canvas
	CreateCanvas(numCols, numRows);

	//Fill with invalid TerrainTile
	FillTerrainTiles(InvalidTerrainTileId, ion::Vector2i(0, 0), ion::Vector2i(numCols - 1, numRows - 1));

	//Redraw TerrainTiles on canvas
	PaintCollisionTerrainTiles();

	//Recreate grid
	CreateGrid(numCols, numRows, numCols, numRows);

	//Centre camera and reset zoom
	ResetZoomPan();
}

void TerrainTilesPanel::PaintCollisionTerrainTiles()
{
	TerrainTileset& TerrainTileset = m_project.GetTerrainTileset();

	for(int i = 0; i < TerrainTileset.GetCount(); i++)
	{
		int x = max(0, i % m_canvasSize.x);
		int y = max(0, m_canvasSize.y - 1 - (i / m_canvasSize.x));

		PaintCollisionTerrainTile(i, x, y);
	}
}

void TerrainTilesPanel::PaintCollisionTerrainTile(TerrainTileId tileId, int x, int y)
{
	//Set texture coords for cell
	ion::render::TexCoord coords[4];
	m_renderResources.GetTerrainTileTexCoords(tileId, coords);
	m_canvasPrimitive->SetTexCoords((y * m_canvasSize.x) + x, coords);
}

void TerrainTilesPanel::FillTerrainTiles(TerrainTileId tileId, const ion::Vector2i& boxCorner1, const ion::Vector2i& boxCorner2)
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
			PaintCollisionTerrainTile(tileId, x, y_inv);
		}
	}
}

void TerrainTilesPanel::RenderCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//No depth test (stops grid cells Z fighting)
	renderer.SetDepthTest(ion::render::Renderer::eAlways);

	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTerrainTileset);
	const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourHighlight);

	//Draw map
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	material->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_canvasPrimitive->GetVertexBuffer(), m_canvasPrimitive->GetIndexBuffer());
	material->Unbind();

	renderer.SetDepthTest(ion::render::Renderer::eLessEqual);
}

void TerrainTilesPanel::RenderBox(const ion::Vector2i& pos, const ion::Vector2& size, const ion::Colour& colour, ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;
	const int quadHalfExtentsX = 4;
	const int quadHalfExtentsY = 4;

	float bottom = (m_canvasSize.y - (pos.y + size.y));

	ion::Matrix4 boxMtx;
	ion::Vector3 boxScale(size.x, size.y, 0.0f);
	ion::Vector3 boxPos(floor((pos.x - (m_canvasSize.x / 2.0f) + (boxScale.x / 2.0f)) * tileWidth),
		floor((bottom - (m_canvasSize.y / 2.0f) + (boxScale.y / 2.0f)) * tileHeight), z);

	boxMtx.SetTranslation(boxPos);
	boxMtx.SetScale(boxScale);

	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);

	renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
	material->SetDiffuseColour(colour);
	material->Bind(boxMtx, cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_selectionPrimitive->GetVertexBuffer(), m_selectionPrimitive->GetIndexBuffer());
	material->Unbind();
	renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);
}

void TerrainTilesPanel::ResetZoomPan()
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	m_cameraZoom = (float)m_panelSize.x / (m_canvasSize.x * tileWidth);

	//Scroll to top
	float halfCanvas = (m_canvasSize.y * ((float)tileHeight / 2.0f));
	float maxY = halfCanvas - ((float)m_panelSize.y / m_cameraZoom);
	ion::Vector3 cameraPos(-(m_panelSize.x / 2.0f / m_cameraZoom), maxY, 0.0f);

	m_camera.SetZoom(ion::Vector3(m_cameraZoom, m_cameraZoom, 1.0f));
	m_camera.SetPosition(cameraPos);
}

void TerrainTilesPanel::ScrollToTop()
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

void TerrainTilesPanel::OnContextMenuClick(wxCommandEvent& event)
{
	if(event.GetId() == eMenuDeleteTile)
	{
		//Delete tile
		m_project.DeleteTerrainTile(m_hoverTerrainTile);

		//Invalidate hover/selected tile
		m_hoverTerrainTile = InvalidTerrainTileId;
		m_selectedTerrainTile = InvalidTerrainTileId;

		//Recreate tileset texture
		m_mainWindow->RefreshTileset();

		//Refresh
		m_mainWindow->RefreshAll();
	}
	else if(event.GetId() == eMenuUseAsDefaultTile)
	{
		//Set default tile
		m_project.SetDefaultTerrainTile(m_hoverTerrainTile);

		//Refresh map
		m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
	}
}