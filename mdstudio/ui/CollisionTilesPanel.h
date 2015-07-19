///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "ViewPanel.h"

class CollisionTilesPanel : public ViewPanel
{
	static const float s_CollisionTileSize;

public:
	CollisionTilesPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~CollisionTilesPanel();

	//Events
	virtual void OnMouse(wxMouseEvent& event, const ion::Vector2& mouseDelta);
	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnResize(wxSizeEvent& event);

	//Set current project
	virtual void SetProject(Project* project);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

protected:

	//Mouse click or changed CollisionTile callback
	virtual void OnMouseTileEvent(ion::Vector2 mouseDelta, int buttonBits, int x, int y);

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

private:
	//Calc canvas size
	ion::Vector2i CalcCanvasSize();

	//Create canvas and grid
	void InitPanel(int numCols, int numRows);

	//Paint all CollisionTiles to canvas
	void PaintCollisionTiles();

	//Paint single collision tile to canvas
	void PaintCollisionTile(TileId tileId, int x, int y);

	//Fill selection with single collision tile
	void FillCollisionTiles(CollisionTileId tileId, const ion::Vector2i& boxCorner1, const ion::Vector2i& boxCorner2);

	//Render canvas
	void RenderCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	//Render selection box
	void RenderBox(const ion::Vector2i& pos, const ion::Vector2& size, const ion::Colour& colour, ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	//Centre camera, reset zoom
	void ResetZoomPan();

	//Reset scroll
	void ScrollToTop();

	//Current/hover CollisionTile
	CollisionTileId m_selectedCollisionTile;
	CollisionTileId m_hoverCollisionTile;

	//Current/hover stamp pos
	ion::Vector2i m_selectedCollisionTilePos;
	ion::Vector2i m_hoverCollisionTilePos;

	//Rendering primitives
	ion::render::Quad* m_selectionPrimitive;
};