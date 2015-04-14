///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "TilesPanel.h"
#include "TileRendering.h"

#include <maths/Vector.h>

TilesPanel::TilesPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxPanel(parent, id, pos, size, style, name)
	, m_canvas(8, 8)
	, m_scrollHelper(this)
{
	m_project = NULL;
	m_zoom = 4.0f;
	m_currentSelectionLeft = -1;
	m_currentSelectionRight = -1;
	m_tileCount = 0;

	Bind(wxEVT_LEFT_DOWN,		 &TilesPanel::OnMouse, this, GetId());
	Bind(wxEVT_RIGHT_DOWN,		 &TilesPanel::OnMouse, this, GetId());
	Bind(wxEVT_MOUSEWHEEL,		 &TilesPanel::OnMouse, this, GetId());
	Bind(wxEVT_PAINT,			 &TilesPanel::OnPaint, this, GetId());
	Bind(wxEVT_ERASE_BACKGROUND, &TilesPanel::OnErase, this, GetId());
	Bind(wxEVT_SIZE,			 &TilesPanel::OnSize, this, GetId());

	SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void TilesPanel::SetProject(Project* project)
{
	m_project = project;
	Refresh();
}

void TilesPanel::OnMouse(wxMouseEvent& event)
{
	if(event.ButtonIsDown(wxMOUSE_BTN_LEFT) || event.ButtonIsDown(wxMOUSE_BTN_RIGHT))
	{
		//Get mouse position
		wxClientDC clientDc(this);
		wxPoint mouseCanvasPosWx = event.GetLogicalPosition(clientDc);

		//Get scroll offset
		wxPoint scrollOffset = GetScrollHelper()->CalcUnscrolledPosition(wxPoint());

		//To map space
		ion::Vector2 mousePosMapSpace(mouseCanvasPosWx.x / m_zoom, (mouseCanvasPosWx.y + scrollOffset.y) / m_zoom);

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
				TileId tileId = 0;

				//TODO: Very slow, use indexed multimap
				auto it = m_project->GetMap().GetTileset().Begin();
				auto end = m_project->GetMap().GetTileset().End();
				for(int i = 0; i <= selection && it != end; ++i, ++it)
				{
					tileId = it->first;
				}

				//Set tile to draw on left click
				m_project->SetPaintTile(tileId);

				//Invalidate old and new rects
				InvalidateTileRect(m_currentSelectionLeft);
				InvalidateTileRect(selection);

				//Update selection
				m_currentSelectionLeft = selection;
			}
			else if(event.ButtonIsDown(wxMOUSE_BTN_RIGHT))
			{
				TileId tileId = 0;

				//TODO: Very slow, use indexed multimap
				auto it = m_project->GetMap().GetTileset().Begin();
				auto end = m_project->GetMap().GetTileset().End();
				for(int i = 0; i <= selection && it != end; ++i, ++it)
				{
					tileId = it->first;
				}

				//Set tile to draw on right click
				m_project->SetEraseTile(tileId);

				//Invalidate old and new rects
				InvalidateTileRect(m_currentSelectionLeft);
				InvalidateTileRect(selection);

				//Update selection
				m_currentSelectionRight = selection;
			}
		}
	}
	else if(event.GetEventType() == wxEVT_MOUSEWHEEL || event.GetWheelRotation() != 0)
	{
		//Zoom camera
		int wheelDelta = event.GetWheelRotation();
		const float zoomSpeed = 1.0f;

		//One notch at a time
		if(wheelDelta > 0)
			m_zoom += zoomSpeed;
		else if(wheelDelta < 0)
			m_zoom -= zoomSpeed;

		//Clamp
		if(m_zoom < 1.0f)
			m_zoom = 1.0f;
		else if(m_zoom > 10.0f)
			m_zoom = 10.0f;

		//Scroll size changed, re-init panel
		InitPanel();

		//Invalidate whole frame
		Refresh();
	}

	event.Skip();
}

void TilesPanel::OnPaint(wxPaintEvent& event)
{
	int tileCount = m_project ? m_project->GetMap().GetTileset().GetCount() : 0;
	if(m_tileCount != tileCount)
	{
		//Tile count changed, redraw canvas
		InitPanel();
		m_tileCount = tileCount;
	}

	//Double buffered dest dc
	wxAutoBufferedPaintDC destDC(this);

	//Bitmap source dc
	wxMemoryDC sourceDC(m_canvas);

	//Get scroll offset
	wxPoint scrollOffset = GetScrollHelper()->CalcUnscrolledPosition(wxPoint());

	//Update all invalidated regions
	for(wxRegionIterator it = GetUpdateRegion(); it; it++)
	{
		//Get invalidated rect
		wxRect destRect(it.GetRect());
		wxRect sourceRect(it.GetRect());

		//Offset scroll
		sourceRect.y += scrollOffset.y;

		//Copy rect from canvas
		destDC.Blit(destRect.x, destRect.y, destRect.width, destRect.height, &sourceDC, sourceRect.x, sourceRect.y);
	}

	//Draw rectangles around current selections
	int numCols = (GetClientSize().x / 8) / m_zoom;
	if(m_tileCount && numCols > 0)
	{
		destDC.SetBrush(*wxTRANSPARENT_BRUSH);

		if(m_currentSelectionLeft > -1)
		{
			wxPen pen(*wxRED_PEN);
			pen.SetWidth(2);
			destDC.SetPen(pen);
			int currSelectionY = ((m_currentSelectionLeft / numCols) * 8 * m_zoom) - scrollOffset.y;
			int currSelectionX = (m_currentSelectionLeft % numCols) * 8 * m_zoom;
			destDC.DrawRectangle(currSelectionX, currSelectionY, 8 * m_zoom, 8 * m_zoom);
		}

		if(m_currentSelectionRight > -1)
		{
			wxPen pen(*wxYELLOW_PEN);
			pen.SetWidth(2);
			destDC.SetPen(pen);
			int currSelectionY = ((m_currentSelectionRight / numCols) * 8 * m_zoom) - scrollOffset.y;
			int currSelectionX = (m_currentSelectionRight % numCols) * 8 * m_zoom;
			destDC.DrawRectangle(currSelectionX, currSelectionY, 8 * m_zoom, 8 * m_zoom);
		}
	}
}

void TilesPanel::OnErase(wxEraseEvent& event)
{
	//Ignore event
}

void TilesPanel::OnSize(wxSizeEvent& event)
{
	//Recalculate size and redraw tiles to canvas
	InitPanel();
}

void TilesPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(m_project && m_project->TilesAreInvalidated())
	{
		//Full refresh, redraw tiles to canvas
		InitPanel();

		m_project->InvalidateTiles(false);
	}

	wxPanel::Refresh(eraseBackground, rect);
}

void TilesPanel::InitPanel()
{
	if(m_project)
	{
		//Update frame title
		m_tileCount = m_project->GetMap().GetTileset().GetCount();
		std::stringstream name;
		name << "Tiles (" << m_tileCount << ")";
		SetLabel(name.str());

		//Update scroll page size and scroll rate
		int numCols = (GetClientSize().x / 8) / m_zoom;
		int scrollPageHeight = max(numCols > 0 ? ((m_tileCount / numCols) * 8 * m_zoom) : 0, 8 * m_zoom);
		SetVirtualSize(wxSize(GetClientSize().x, scrollPageHeight));
		GetScrollHelper()->SetScrollRate(0, 8 * m_zoom);

		//Recreate local canvas
		m_canvas = wxBitmap(GetClientSize().x, scrollPageHeight);

		//Paint all tiles to canvas
		wxMemoryDC dc(m_canvas);
		PaintAllToDc(dc);
	}
}

void TilesPanel::InvalidateTileRect(int tileId)
{
	if(tileId >= 0)
	{
		wxPoint scrollOffset = GetScrollHelper()->CalcUnscrolledPosition(wxPoint());
		int numCols = (GetClientSize().x / 8) / m_zoom;
		int tileY = (tileId / numCols) * 8 * m_zoom;
		int tileX = (tileId % numCols) * 8 * m_zoom;
		RefreshRect(wxRect(tileX - 1, tileY - 1 - scrollOffset.y, (8 * m_zoom) + 1, (8 * m_zoom) + 1));
	}
}

void TilesPanel::PaintAllToDc(wxDC& dc)
{
	if(m_project)
	{
		//Get renderable rect
		wxSize canvasSize = m_canvas.GetSize();
		wxRect canvasRect(0, 0, canvasSize.x, canvasSize.y);

		//Clear canvas
		dc.SetBrush(*wxBLACK_BRUSH);
		dc.DrawRectangle(canvasRect);

		m_tileCount = m_project->GetMap().GetTileset().GetCount();
		int numCols = (GetClientSize().x / 8) / m_zoom;

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
					tilerendering::PaintTileToDc(x, y, it->second, *palette, dc);
				}
			}

			//Scale canvas
			int originalWidth = numCols * 8;
			int originalHeight = (m_tileCount / numCols) * 8;
			wxRect sourceRect(0, 0, originalWidth, originalHeight);
			wxRect destRect(0, 0, originalWidth * m_zoom, originalHeight * m_zoom);

			//Copy rect from canvas
			dc.StretchBlit(destRect.x, destRect.y, destRect.width, destRect.height, &dc, sourceRect.x, sourceRect.y, sourceRect.width, sourceRect.height);
		}
	}
}