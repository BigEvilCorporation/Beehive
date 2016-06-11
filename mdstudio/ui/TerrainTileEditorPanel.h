///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "ViewPanel.h"

class TerrainTileEditorPanel : public ViewPanel
{
	static const float s_defaultZoom;

public:
	TerrainTileEditorPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~TerrainTileEditorPanel();

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

private:
	void RenderTile(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderCollision(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	//Paint background tile
	void PaintTile();

	//Paint collision type bitmaps to canvas
	void PaintCollision();

	//Rendering primitives
	ion::render::Quad* m_tilePrimitive;
	ion::render::Quad* m_collisionPrimitive;
};