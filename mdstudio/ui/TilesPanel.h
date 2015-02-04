#pragma once

#include <wx/scrolwin.h>
#include <wx/event.h>
#include <wx/dcbuffer.h>

#include <vector>

#include "MainWindowTemplate.h"
#include "../Tile.h"

class TilesPanel : public wxScrolledWindow
{
public:
	TilesPanel(wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxHSCROLL | wxVSCROLL, const wxString& name = "scrolledWindow");

	void AddTile(const Tile& tile);

	void OnMouse(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);

private:
	std::vector<const Tile*> m_tiles;
	float m_zoom;
	int m_currentSelection;
};