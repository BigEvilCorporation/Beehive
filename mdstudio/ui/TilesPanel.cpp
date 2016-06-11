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

#include "TilesPanel.h"
#include "MainWindow.h"

#include <wx/Menu.h>
#include <algorithm>

TilesPanel::TilesPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, project, renderer, glContext, renderResources, parent, winid, pos, size, style, name)
{
	m_selectedTile = InvalidTileId;
	m_hoverTile = InvalidTileId;

	//Custom zoom/pan handling
	EnableZoom(false);
	EnablePan(false);

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	//Create selection quad
	m_selectionPrimitive = new ion::render::Quad(ion::render::Quad::xy, ion::Vector2(tileWidth / 2.0f, tileHeight / 2.0f));
}

TilesPanel::~TilesPanel()
{

}

void TilesPanel::OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta)
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
			float halfCanvas = (m_canvasSize.y * ((float)tileHeight / 2.0f));
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

void TilesPanel::OnKeyboard(wxKeyEvent& event)
{
	ViewPanel::OnKeyboard(event);
}

void TilesPanel::OnResize(wxSizeEvent& event)
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

void TilesPanel::OnMouseTileEvent(int buttonBits, int x, int y)
{
	ViewPanel::OnMouseTileEvent(buttonBits, x, y);

	TileId selectedTile = InvalidTileId;

	//If in range, get tile under mouse cursor
	if(x >= 0 && y >= 0 && x < m_canvasSize.x && y < m_canvasSize.y)
	{
		selectedTile = (y * m_canvasSize.x) + x;
	}

	//Set mouse hover tile
	m_hoverTile = selectedTile;
	m_hoverTilePos.x = x;
	m_hoverTilePos.y = y;

	if((buttonBits & eMouseLeft) && !(m_prevMouseBits & eMouseLeft))
	{
		//Left click, set current tile
		m_selectedTile = selectedTile;
		m_selectedTilePos = m_hoverTilePos;

		//Set as current painting tile
		m_project.SetPaintTile(selectedTile);

		//Set tile paint tool
		m_mainWindow->SetMapTool(eToolPaintTile);

		//Refresh tile editor panel
		m_mainWindow->RefreshPanel(MainWindow::ePanelTileEditor);

		//Refresh collision tile editor panel
		m_mainWindow->RefreshPanel(MainWindow::ePanelTerrainTileEditor);
	}

	if(buttonBits & eMouseRight)
	{
		if(m_hoverTile != InvalidTileId)
		{
			//Right-click menu
			wxMenu contextMenu;

			contextMenu.Append(eMenuDeleteTile, wxString("Delete tile"));
			contextMenu.Append(eMenuUseAsBgTile, wxString("Use as background tile"));
			contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&TilesPanel::OnContextMenuClick, NULL, this);
			PopupMenu(&contextMenu);
		}
	}

	//Redraw
	Refresh();
}

void TilesPanel::OnContextMenuClick(wxCommandEvent& event)
{
	if(event.GetId() == eMenuDeleteTile)
	{
		//Delete tile
		m_project.DeleteTile(m_hoverTile);

		//Invalidate hover/selected tile
		m_hoverTile = InvalidTileId;
		m_selectedTile = InvalidTileId;

		//Recreate tileset texture
		m_mainWindow->RefreshTileset();

		//Refresh
		m_mainWindow->RefreshAll();
	}
	else if(event.GetId() == eMenuUseAsBgTile)
	{
		//Set background tile
		m_project.SetBackgroundTile(m_hoverTile);

		//Refresh map
		m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
	}
}

void TilesPanel::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	//Render canvas
	RenderCanvas(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	//Render selected tile
	if(m_selectedTile)
	{
		if(Tile* tile = m_project.GetTileset().GetTile(m_selectedTile))
		{
			ion::debug::Assert(tile, "Invalid tile");
			ion::Vector2 size(1, 1);
			const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourSelected);
			RenderBox(m_selectedTilePos, size, colour, renderer, cameraInverseMtx, projectionMtx, z);
		}
	}

	z += zOffset;

	//Render mouse hover tile
	if(m_hoverTile && m_hoverTile != m_selectedTile)
	{
		if(Tile* tile = m_project.GetTileset().GetTile(m_hoverTile))
		{
			ion::debug::Assert(tile, "Invalid tile");
			ion::Vector2 size(1, 1);
			const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourHighlight);
			RenderBox(m_hoverTilePos, size, colour, renderer, cameraInverseMtx, projectionMtx, z);
		}
	}

	z += zOffset;

	//Render grid
	if(m_project.GetShowGrid())
	{
		RenderGrid(renderer, cameraInverseMtx, projectionMtx, z);
	}
}

void TilesPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	//If tiles invalidated
	if(m_project.TilesAreInvalidated())
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

ion::Vector2i TilesPanel::CalcCanvasSize()
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;
	wxSize panelSize = GetClientSize();
	int numTiles = m_project.GetTileset().GetCount();
	int numCols = ion::maths::Ceil((float)panelSize.x / 8.0f / tileWidth);
	int numRows = max(numCols, (int)ion::maths::Ceil((float)numTiles / (float)numCols / tileHeight));
	return ion::Vector2i(numCols, numRows);
}

void TilesPanel::InitPanel(int numCols, int numRows)
{
	//Recreate canvas
	CreateCanvas(numCols, numRows);

	//Fill with invalid tile
	FillTiles(InvalidTileId, ion::Vector2i(0, 0), ion::Vector2i(numCols - 1, numRows - 1));

	//Redraw tiles on canvas
	PaintTiles();

	//Recreate grid
	CreateGrid(numCols, numRows, numCols, numRows);

	//Centre camera and reset zoom
	ResetZoomPan();
}

void TilesPanel::PaintTiles()
{
	Tileset& tileset = m_project.GetTileset();

	for(int i = 0; i < tileset.GetCount(); i++)
	{
		int x = max(0, i % m_canvasSize.x);
		int y = max(0, m_canvasSize.y - 1 - (i / m_canvasSize.x));

		PaintTile(i, x, y, 0);
	}
}

void TilesPanel::RenderBox(const ion::Vector2i& pos, const ion::Vector2& size, const ion::Colour& colour, ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
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

void TilesPanel::ResetZoomPan()
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

void TilesPanel::ScrollToTop()
{
	//Reset zoom to identity
	//float zoom = m_cameraZoom;
	//SetCameraZoom(1.0f);

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	//Scroll to top
	float halfCanvas = (m_canvasSize.y * (tileHeight / 2.0f));
	float maxY = halfCanvas - ((float)m_panelSize.y / m_cameraZoom);
	ion::Vector3 cameraPos(-(m_panelSize.x / 2.0f / m_cameraZoom), maxY, 0.0f);

	//Re-apply zoom
	//SetCameraZoom(zoom);
}