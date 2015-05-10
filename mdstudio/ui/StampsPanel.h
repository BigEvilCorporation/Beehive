///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "ViewPanel.h"

class StampsPanel : public ViewPanel
{
public:
	StampsPanel(ion::render::Renderer& renderer, wxGLContext* glContext, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~StampsPanel();

	//Events
	virtual void OnMouse(wxMouseEvent& event);
	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnResize(wxSizeEvent& event);

	//Set current project
	virtual void SetProject(Project* project);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

protected:

	//Mouse click or changed tile callback
	virtual void OnMouseTileEvent(ion::Vector2 mouseDelta, int buttonBits, int x, int y);

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

private:

	//Paint all stamps using position map to canvas
	void PaintStamps();

	//Recalc all stamp positions and canvas size
	void ArrangeStamps(int canvasWidth);

	//Fixed canvas width (tiles)
	int m_canvasWidth;

	//Dynamic canvas height (tiles)
	int m_canvasHeight;

	//Stamp position map
	std::vector< std::pair<StampId, ion::Vector2i> > m_stampPosMap;
};