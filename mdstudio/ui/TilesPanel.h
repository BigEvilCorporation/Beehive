///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "ViewPanel.h"

class TilesPanel : public ViewPanel
{
	static const float s_tileSize;

public:
	TilesPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~TilesPanel();

	//Events
	virtual void OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta);
	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnResize(wxSizeEvent& event);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

protected:

	//Mouse click or changed tile callback
	virtual void OnMouseTileEvent(int buttonBits, int x, int y);

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

	//Right-click menu callback
	void OnContextMenuClick(wxCommandEvent& event);

private:
	enum MenuItems
	{
		eMenuDeleteTile,
		eMenuUseAsBgTile
	};

	//Calc canvas size
	ion::Vector2i CalcCanvasSize();

	//Create canvas and grid
	void InitPanel(int numCols, int numRows);

	//Paint all tiles to canvas
	void PaintTiles();

	//Render selection box
	void RenderBox(const ion::Vector2i& pos, const ion::Vector2& size, const ion::Colour& colour, ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	//Centre camera, reset zoom
	void ResetZoomPan();

	//Reset scroll
	void ScrollToTop();

	//Current/hover tile
	TileId m_selectedTile;
	TileId m_hoverTile;

	//Current/hover stamp pos
	ion::Vector2i m_selectedTilePos;
	ion::Vector2i m_hoverTilePos;

	//Rendering primitives
	ion::render::Quad* m_selectionPrimitive;
};