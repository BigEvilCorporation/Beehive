///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "ViewPanel.h"

class TerrainTilesPanel : public ViewPanel
{
	static const float s_TerrainTileSize;

public:
	TerrainTilesPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~TerrainTilesPanel();

	//Events
	virtual void OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta);
	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnResize(wxSizeEvent& event);

	//Set current project
	virtual void SetProject(Project* project);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

protected:

	//Mouse click or changed TerrainTile callback
	virtual void OnMouseTileEvent(int buttonBits, int x, int y);

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

	//Right-click menu callback
	void OnContextMenuClick(wxCommandEvent& event);

private:
	enum MenuItems
	{
		eMenuDeleteTile,
		eMenuUseAsDefaultTile
	};

	//Calc canvas size
	ion::Vector2i CalcCanvasSize();

	//Create canvas and grid
	void InitPanel(int numCols, int numRows);

	//Paint all TerrainTiles to canvas
	void PaintCollisionTerrainTiles();

	//Paint single collision tile to canvas
	void PaintCollisionTerrainTile(TileId tileId, int x, int y);

	//Fill selection with single collision tile
	void FillTerrainTiles(TerrainTileId tileId, const ion::Vector2i& boxCorner1, const ion::Vector2i& boxCorner2);

	//Render canvas
	void RenderCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	//Render selection box
	void RenderBox(const ion::Vector2i& pos, const ion::Vector2& size, const ion::Colour& colour, ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	//Centre camera, reset zoom
	void ResetZoomPan();

	//Reset scroll
	void ScrollToTop();

	//Current/hover TerrainTile
	TerrainTileId m_selectedTerrainTile;
	TerrainTileId m_hoverTerrainTile;

	//Current/hover stamp pos
	ion::Vector2i m_selectedTerrainTilePos;
	ion::Vector2i m_hoverTerrainTilePos;

	//Rendering primitives
	ion::render::Quad* m_selectionPrimitive;
};