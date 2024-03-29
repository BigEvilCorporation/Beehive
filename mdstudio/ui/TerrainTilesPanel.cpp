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

#include "TerrainTilesPanel.h"
#include "MainWindow.h"

#include <ion/core/utils/STL.h>

#include <wx/Menu.h>

#include <algorithm>

const float TerrainTilesPanel::s_TerrainTileSize = 4.0f;

TerrainTilesPanel::TerrainTilesPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, wxGLAttributes& glAttributes, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, project, renderer, glContext, glAttributes, renderResources, parent, winid, pos, size, style, name)
{
	m_selectedTerrainTile = InvalidTerrainTileId;
	m_hoverTerrainTile = InvalidTerrainTileId;

	//Custom zoom/pan handling
	EnableZoom(false);
	EnablePan(false);

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	//Create selection quad
	m_selectionPrimitive = new ion::render::Quad(ion::render::Quad::Axis::xy, ion::Vector2(tileWidth / 2.0f, tileHeight / 2.0f));
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
	if(!m_mainWindow->IsRefreshLocked())
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
}

void TerrainTilesPanel::OnMouseTileEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int x, int y)
{
	ViewPanel::OnMouseTileEvent(mousePos, mouseDelta, tileDelta, buttonBits, x, y);

	TerrainTileId selectedTerrainTile = InvalidTerrainTileId;
	int tileWidth = m_project.GetPlatformConfig().tileWidth;
	int tileHeight = m_project.GetPlatformConfig().tileHeight;

	//If in range, get terrain tile under mouse cursor
	if(x >= 0 && y >= 0 && x < m_canvasSize.x && y < m_canvasSize.y)
	{
		selectedTerrainTile = (y * m_canvasSize.x) + x;
	}

	//Set mouse hover terrain tile
	m_hoverTerrainTile = selectedTerrainTile;
	m_hoverTerrainTilePos.x = x;
	m_hoverTerrainTilePos.y = y;

#if !BEEHIVE_FIXED_STAMP_MODE //No collision editing in fixed mode
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
#endif

	if (const TerrainTile* tile = m_project.GetTerrainTileset().GetTerrainTile(m_hoverTerrainTile))
	{
		std::stringstream tipStr;
		tipStr << "Tile 0x" << SSTREAM_HEX4(m_hoverTerrainTile) << " (" << m_hoverTerrainTile << ")" << std::endl;
		//tipStr << "Angle: " << tile->GetAngle() << " (" << ion::maths::RadiansToDegrees(tile->GetAngle()) << " deg)" << std::endl;
		tipStr << "Addr: 0x" << SSTREAM_HEX8(m_hoverTerrainTile * tileWidth) << std::endl;
		tipStr << "Data:" << std::endl;
		tipStr << " H: ";

		for (int i = 0; i < tileWidth; i++)
		{
			tipStr << SSTREAM_HEX2(tile->GetHeight(i)) << " ";
		}

		tipStr << std::endl;

		tipStr << " W: ";

		for (int i = 0; i < tileHeight; i++)
		{
			tipStr << SSTREAM_HEX2(tile->GetWidth(i)) << " ";
		}

		tipStr << std::endl;

		SetToolTip(tipStr.str().c_str());
	}
	else
	{
		UnsetToolTip();
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
	if(!m_mainWindow->IsRefreshLocked())
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
}

ion::Vector2i TerrainTilesPanel::CalcCanvasSize()
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;
	wxSize panelSize = GetClientSize();
	int numTerrainTiles = m_project.GetTerrainTileset().GetCount();
	int numCols = ion::maths::Ceil((float)panelSize.x / tileWidth / s_TerrainTileSize);
	int numRows = ion::maths::Max(numCols, (int)ion::maths::Ceil((float)numTerrainTiles / (float)numCols / s_TerrainTileSize));
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
		int x = ion::maths::Max(0, i % m_canvasSize.x);
		int y = ion::maths::Max(0, m_canvasSize.y - 1 - (i / m_canvasSize.x));

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
	int top = ion::maths::Min(boxCorner1.y, boxCorner2.y);
	int left = ion::maths::Min(boxCorner1.x, boxCorner2.x);
	int bottom = ion::maths::Max(boxCorner1.y, boxCorner2.y);
	int right = ion::maths::Max(boxCorner1.x, boxCorner2.x);

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
	renderer.SetDepthTest(ion::render::Renderer::DepthTest::Always);

	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);

	//Draw terrain heightmaps
	{
		ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTerrainTilesetHeight);
		material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
		renderer.BindMaterial(*material, ion::Matrix4(), cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(m_canvasPrimitive->GetVertexBuffer(), m_canvasPrimitive->GetIndexBuffer());
		renderer.UnbindMaterial(*material);
	}

	//Draw terrain widthmaps
	{
		ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTerrainTilesetWidth);
		material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
		renderer.BindMaterial(*material, ion::Matrix4(), cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(m_canvasPrimitive->GetVertexBuffer(), m_canvasPrimitive->GetIndexBuffer());
		renderer.UnbindMaterial(*material);
	}

	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);

	renderer.SetDepthTest(ion::render::Renderer::DepthTest::LessOrEqual);
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

	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
	material->SetDiffuseColour(colour);
	renderer.BindMaterial(*material, boxMtx, cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_selectionPrimitive->GetVertexBuffer(), m_selectionPrimitive->GetIndexBuffer());
	renderer.UnbindMaterial(*material);
	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);
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