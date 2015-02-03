#pragma once

#include <maths/Vector.h>

#include <wx/event.h>
#include <wx/panel.h>
#include <wx/dc.h>
#include <wx/dcclient.h>

#include "MainWindowTemplate.h"
#include "../Map.h"
#include "../Tile.h"

class MapPanel : public wxPanel
{
public:
	MapPanel(wxWindow *parent,  wxWindowID winid = wxID_ANY,  const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);

	void OnMouse(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& evt);
    void Render(wxDC& dc);

	void SetPaintTile(const Tile* tile) { m_paintTile = m_paintTile; }

private:
	void PaintTile(ion::Vector2 mousePos);
	void EraseTile(ion::Vector2 mousePos);

	void PaintMapToCanvas(wxDC& dc);
	void PaintTileToCanvas(int x, int y, const Tile& tile, wxDC& dc);

	ion::Vector2 m_cameraPos;
	float m_cameraZoom;

	Map m_map;
	/* const */ Tile* m_paintTile;
};