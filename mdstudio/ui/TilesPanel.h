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

class TilesPanel : public wxScrolledWindow
{
public:
	TilesPanel(wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxHSCROLL | wxVSCROLL, const wxString& name = "scrolledWindow");

	void SetProject(Project* project) { m_project = project; }

	void OnMouse(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);

private:
	Project* m_project;
	float m_zoom;
	int m_currentSelectionLeft;
	int m_currentSelectionRight;
};