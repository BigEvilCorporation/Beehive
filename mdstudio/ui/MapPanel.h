#pragma once

#include <maths/Vector.h>

#include <wx/event.h>
#include <wx/panel.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>

#include "MainWindowTemplate.h"
#include "../Map.h"
#include "../Tile.h"

class MapPanel : public wxPanel
{
public:
	MapPanel(wxWindow *parent,  wxWindowID winid = wxID_ANY,  const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);

	void OnMouse(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);

	//Set current tile used for painting
	void SetPaintTile(const Tile* tile) { m_paintTile = tile; }

	//Set current tile used for erasing
	void SetEraseTile(const Tile* tile) { m_eraseTile = tile; }

private:
	//Paint tile to canvas
	void PaintTile(ion::Vector2 mousePos, const Tile& tile);

	//Paint whole map to dc
	void PaintMapToDc(wxDC& dc);

	//Genesis map
	Map m_map;

	//Local drawing canvas
	wxBitmap m_canvas;

	//Tile used for painting
	const Tile* m_paintTile;

	//Tile used for erasing
	const Tile* m_eraseTile;

	//Camera
	ion::Vector2 m_cameraPos;
	float m_cameraZoom;
	float m_cameraPanSpeed;

	//For mouse delta
	ion::Vector2 m_mousePrevPos;
};