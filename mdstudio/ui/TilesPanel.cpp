///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "TilesPanel.h"
#include "TileRendering.h"

#include <maths/Vector.h>

TilesPanel::TilesPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxScrolledWindow(parent, id, pos, size, style, name)
{
	m_project = NULL;
	m_zoom = 4.0f;
	m_currentSelectionLeft = -1;
	m_currentSelectionRight = -1;

	Bind(wxEVT_LEFT_DOWN,		&TilesPanel::OnMouse, this, wxID_TILESPANEL);
	Bind(wxEVT_RIGHT_DOWN,		&TilesPanel::OnMouse, this, wxID_TILESPANEL);
	Bind(wxEVT_PAINT,			&TilesPanel::OnPaint, this, wxID_TILESPANEL);
	Bind(wxEVT_ERASE_BACKGROUND,&TilesPanel::OnErase, this, wxID_TILESPANEL);

	SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void TilesPanel::OnMouse(wxMouseEvent& event)
{
	if(event.ButtonIsDown(wxMOUSE_BTN_LEFT) || event.ButtonIsDown(wxMOUSE_BTN_RIGHT))
	{
		//Get mouse position in map space
		wxClientDC clientDc(this);
		wxPoint mouseCanvasPosWx = event.GetLogicalPosition(clientDc);
		ion::Vector2 mousePosMapSpace(mouseCanvasPosWx.x / m_zoom, mouseCanvasPosWx.y / m_zoom);

		//Set current selection
		int x = (int)floor(mousePosMapSpace.x / 8.0f);
		int y = (int)floor(mousePosMapSpace.y / 8.0f);
		wxSize clientSize = GetClientSize();
		int numCols = (clientSize.x / 8) / m_zoom;
		int selection = (y * numCols) + x;

		if(selection < m_project->GetMap().GetTileset().GetCount())
		{
			if(event.ButtonIsDown(wxMOUSE_BTN_LEFT))
			{
				m_currentSelectionLeft = selection;

				TileId tileId = 0;

				//TODO: Very slow, use indexed multimap
				auto it = m_project->GetMap().GetTileset().Begin();
				auto end = m_project->GetMap().GetTileset().End();
				for(int i = 0; i <= m_currentSelectionLeft && it != end; ++i, ++it)
				{
					tileId = it->first;
				}

				m_project->SetPaintTile(tileId);
			}
			else if(event.ButtonIsDown(wxMOUSE_BTN_RIGHT))
			{
				m_currentSelectionRight = selection;

				TileId tileId = 0;

				//TODO: Very slow, use indexed multimap
				auto it = m_project->GetMap().GetTileset().Begin();
				auto end = m_project->GetMap().GetTileset().End();
				for(int i = 0; i <= m_currentSelectionRight && it != end; ++i, ++it)
				{
					tileId = it->first;
				}

				m_project->SetEraseTile(tileId);
			}
		}

		//Redraw window
		Refresh();
	}

	event.Skip();
}

void TilesPanel::OnPaint(wxPaintEvent& event)
{
	//Double buffered dest dc
	wxAutoBufferedPaintDC destDC(this);

	//Get renderable client rect
	wxSize clientSize = GetClientSize();
	wxRect clientRect(0, 0, clientSize.x, clientSize.y);

	//Clear dest rect
	destDC.SetBrush(*wxBLACK_BRUSH);
	destDC.DrawRectangle(clientRect);

	int numTiles = m_project->GetMap().GetTileset().GetCount();
	int numCols = (clientSize.x / 8) / m_zoom;

	//Bitmap source and dc
	wxBitmap canvas(clientRect.width, clientRect.height);
	wxMemoryDC sourceDC(canvas);

	//Paint tiles to source dc
	if(numCols > 0)
	{
		int i = 0;
		for(auto it = m_project->GetMap().GetTileset().Begin(), end = m_project->GetMap().GetTileset().End(); it != end; ++it, ++i)
		{
			int y = i / numCols;
			int x = i % numCols;

			const Palette* palette = m_project->GetPalette(it->second.GetPaletteId());
			if(palette)
			{
				tilerendering::PaintTileToDc(x, y, it->second, *palette, sourceDC);
			}
		}
	}

	//Paint scaled tiles to dest dc
	wxRect sourceRect(0, 0, clientRect.width, clientRect.height);
	wxRect destRect(0, 0, clientRect.width * m_zoom, clientRect.height * m_zoom);
		
	//Copy rect from canvas
	destDC.StretchBlit(destRect.x, destRect.y, destRect.width, destRect.height, &sourceDC, sourceRect.x, sourceRect.y, sourceRect.width, sourceRect.height);

	//Draw rectangles around current selections
	destDC.SetBrush(*wxTRANSPARENT_BRUSH);

	if(m_currentSelectionLeft > -1)
	{
		wxPen pen(*wxRED_PEN);
		pen.SetWidth(2);
		destDC.SetPen(pen);
		int currSelectionY = (m_currentSelectionLeft / numCols) * 8 * m_zoom;
		int currSelectionX = (m_currentSelectionLeft % numCols) * 8 * m_zoom;
		destDC.DrawRectangle(currSelectionX, currSelectionY, 8 * m_zoom, 8 * m_zoom);
	}

	if(m_currentSelectionRight > -1)
	{
		wxPen pen(*wxYELLOW_PEN);
		pen.SetWidth(2);
		destDC.SetPen(pen);
		int currSelectionY = (m_currentSelectionRight / numCols) * 8 * m_zoom;
		int currSelectionX = (m_currentSelectionRight % numCols) * 8 * m_zoom;
		destDC.DrawRectangle(currSelectionX, currSelectionY, 8 * m_zoom, 8 * m_zoom);
	}
}

void TilesPanel::OnErase(wxEraseEvent& event)
{
	//Ignore event
}