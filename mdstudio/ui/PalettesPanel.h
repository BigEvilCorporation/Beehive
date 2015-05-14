///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <wx/frame.h>
#include <wx/event.h>
#include <wx/dcbuffer.h>
#include <wx/colordlg.h>

#include <vector>

#include "UIBase.h"
#include "../Project.h"

class MainWindow;

class PalettesPanel : public wxPanel
{
public:
	enum Orientation
	{
		eHorizontal,
		eVertical
	};

	PalettesPanel(	MainWindow* mainWindow,
					wxWindow *parent,
					wxWindowID id,
					const wxPoint& pos = wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxDEFAULT_FRAME_STYLE,
					const wxString& name = wxFrameNameStr);

	void SetProject(Project* project) { m_project = project; }

	void OnMouse(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnResize(wxSizeEvent& event);

private:
	Project* m_project;
	MainWindow* m_mainWindow;
	Orientation m_orientation;
};