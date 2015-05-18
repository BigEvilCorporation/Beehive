///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "ViewPanel.h"

class TileEditorPanel : public ViewPanel
{
public:
	TileEditorPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, ion::render::Texture* tilesetTexture, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~TileEditorPanel();

	//Events
	virtual void OnMouse(wxMouseEvent& event, const ion::Vector2& mouseDelta);
	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnResize(wxSizeEvent& event);

	//Set current project
	virtual void SetProject(Project* project);

	//Set editing tile
	void SetTile(TileId tileId);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

protected:

	//Mouse click or changed tile callback
	virtual void OnMouseTileEvent(ion::Vector2 mouseDelta, int buttonBits, int x, int y);

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

private:
	void CreateTexture();
	void RenderTile(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	//Rendering primitive
	ion::render::Quad* m_tilePrimitive;

	//Rendering material
	ion::render::Material* m_tileMaterial;

	//Texture
	ion::render::Texture* m_texture;

	//Tile ID
	TileId m_tileId;

	static const int s_tileWidth = 8;
	static const int s_tileHeight = 8;
};