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
#include <wx/tipwin.h>
#include <wx/bitmap.h>

#include <vector>

#include "UIBase.h"
#include "../Project.h"
#include "../Collision.h"

class MainWindow;

class CollisionTypePanel : public wxPanel
{
public:
	enum Orientation
	{
		eHorizontal,
		eVertical
	};

	CollisionTypePanel(	MainWindow* mainWindow,
					wxWindow *parent,
					wxWindowID id,
					const wxPoint& pos = wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxDEFAULT_FRAME_STYLE,
					const wxString& name = wxFrameNameStr);

	void SetProject(Project* project);

	void OnMouse(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnResize(wxSizeEvent& event);

private:
	static const int sIconWidth = 16;
	static const int sIconHeight = 16;

	Project* m_project;
	MainWindow* m_mainWindow;
	Orientation m_orientation;
	wxWeakRef<wxTipWindow> m_tooltip;

	std::vector<wxBitmap> m_icons;
};