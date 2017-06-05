///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#pragma once

#include <ion/beehive/Project.h>

#include <wx/frame.h>
#include <wx/event.h>
#include <wx/dcbuffer.h>
#include <wx/colordlg.h>

#include <vector>

#include "UIBase.h"

class MainWindow;

class PalettesPanel : public wxPanel
{
public:
	enum Orientation
	{
		eHorizontal,
		eVertical
	};

	PalettesPanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxFrameNameStr);

	void OnMouse(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnResize(wxSizeEvent& event);
	void OnSlotsMenuClick(wxCommandEvent& event);

private:
	enum MenuItems
	{
		eMenuSave =		(1 << 0) << 8,
		eMenuLoad =		(1 << 1) << 8,
		eMenuNew =		(1 << 2) << 8,
		eMenuImport =	(1 << 3) << 8,
		eMenuExport =	(1 << 4) << 8,
		eMenuExportBMP =(1 << 5) << 8,
		eMenuSetAsBg =	(1 << 6) << 8,
	};

	Project& m_project;
	MainWindow* m_mainWindow;
	Orientation m_orientation;

	u32 m_selectedPaletteId;
	u32 m_seletedColourId;

	s32 m_dragPalette;
	s32 m_dragColour;
};