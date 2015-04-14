///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <wx/scrolwin.h>
#include <wx/event.h>
#include <wx/dcbuffer.h>

#include <vector>

#include "UIBase.h"
#include "../Project.h"

class TilesPanel : public wxPanel
{
public:
	TilesPanel(wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxVSCROLL, const wxString& name = "scrolledWindow");

	void SetProject(Project* project);

	void OnMouse(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

private:
	//Recreate canvas, draw all tiles, update scroll size and title
	void InitPanel();

	//Paint all tiles to dc
	void PaintAllToDc(wxDC& dc);

	//Invalidate tile rect
	void InvalidateTileRect(int tileId);

	Project* m_project;
	float m_zoom;
	int m_currentSelectionLeft;
	int m_currentSelectionRight;
	int m_tileCount;

	//Local drawing canvas
	wxBitmap m_canvas;

	//Scroll helper
	wxScrollHelper m_scrollHelper;
};