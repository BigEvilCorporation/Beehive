///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <maths/Vector.h>

#include <wx/event.h>
#include <wx/panel.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>

#include "UIBase.h"
#include "../Project.h"

class MapPanel : public wxPanel
{
public:
	MapPanel(wxWindow *parent,  wxWindowID winid = wxID_ANY,  const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);

	//Events
	void OnMouse(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);

	//Set current project
	void SetProject(Project* project);

	virtual void Refresh(bool eraseBackground=true, const wxRect *rect=NULL);

private:
	//Paint tile to canvas
	void PaintTile(ion::Vector2 mousePos, TileId tile);

	//Paint whole map to dc
	void PaintMapToDc(wxDC& dc);

	//Main project
	Project* m_project;

	//Local drawing canvas
	wxBitmap m_canvas;

	//Camera
	ion::Vector2 m_cameraPos;
	float m_cameraZoom;
	float m_cameraPanSpeed;

	//For mouse delta
	ion::Vector2 m_mousePrevPos;
};