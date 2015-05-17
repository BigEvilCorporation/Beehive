///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "TilesPanel.h"
#include "TileRendering.h"
#include "MainWindow.h"

#include <algorithm>

#include <maths/Vector.h>

TilesPanel::TilesPanel(MainWindow* mainWindow, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxPanel(parent, id, pos, size, style, name)
	, m_canvas(8, 8)
	, m_scrollHelper(this)
{
	m_mainWindow = mainWindow;
	m_project = NULL;
	m_zoom = 4.0f;
	m_currentSelectionLeft = -1;
	m_currentSelectionRight = -1;
	m_tileCount = 0;
	m_numCols = 0;
	m_numRows = 0;

	Bind(wxEVT_LEFT_DOWN,		 &TilesPanel::OnMouse, this, GetId());
	Bind(wxEVT_RIGHT_DOWN,		 &TilesPanel::OnMouse, this, GetId());
	Bind(wxEVT_LEFT_DCLICK,		 &TilesPanel::OnMouse, this, GetId());
	Bind(wxEVT_MOUSEWHEEL,		 &TilesPanel::OnMouse, this, GetId());
	Bind(wxEVT_PAINT,			 &TilesPanel::OnPaint, this, GetId());
	Bind(wxEVT_ERASE_BACKGROUND, &TilesPanel::OnErase, this, GetId());
	Bind(wxEVT_SIZE,			 &TilesPanel::OnSize, this, GetId());

	SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void TilesPanel::SetProject(Project* project)
{
	m_project = project;
}

void TilesPanel::OnMouse(wxMouseEvent& event)
{
	TileId tileId = InvalidTileId;

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
		int selection = (y * m_numCols) + x;

		if(selection < m_project->GetTileset().GetCount())
		{
			if(event.ButtonIsDown(wxMOUSE_BTN_LEFT))
			{
				//TODO: Very slow, use indexed multimap
				auto it = m_project->GetTileset().Begin();
				auto end = m_project->GetTileset().End();
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
				auto it = m_project->GetTileset().Begin();
				auto end = m_project->GetTileset().End();
				for(int i = 0; i <= selection && it != end; ++i, ++it)
				{
					tileId = it->first;
				}

				//Set tile to draw on right click
				m_project->SetEraseTile(tileId);

				//Invalidate old and new rects
				InvalidateTileRect(m_currentSelectionRight);
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

	if(event.LeftDClick() && tileId)
	{
		//Left double-click, edit tile
		m_mainWindow->EditTile(tileId);
	}

	event.Skip();
}

void TilesPanel::OnPaint(wxPaintEvent& event)
{
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
	if(m_tileCount > 0)
	{
		destDC.SetBrush(*wxTRANSPARENT_BRUSH);

		if(m_currentSelectionLeft > -1)
		{
			wxPen pen(*wxRED_PEN);
			pen.SetWidth(2);
			destDC.SetPen(pen);
			int currSelectionY = ((m_currentSelectionLeft / m_numCols) * 8 * m_zoom) - scrollOffset.y;
			int currSelectionX = (m_currentSelectionLeft % m_numCols) * 8 * m_zoom;
			destDC.DrawRectangle(currSelectionX, currSelectionY, 8 * m_zoom, 8 * m_zoom);
		}

		if(m_currentSelectionRight > -1)
		{
			wxPen pen(*wxYELLOW_PEN);
			pen.SetWidth(2);
			destDC.SetPen(pen);
			int currSelectionY = ((m_currentSelectionRight / m_numCols) * 8 * m_zoom) - scrollOffset.y;
			int currSelectionX = (m_currentSelectionRight % m_numCols) * 8 * m_zoom;
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
	}

	wxPanel::Refresh(eraseBackground, rect);
}

void TilesPanel::InitPanel()
{
	if(m_project)
	{
		//Update tile count
		m_tileCount = m_project->GetTileset().GetCount();
		m_numCols = ion::maths::Ceil(((float)GetClientSize().x / 8.0f) / m_zoom);
		m_numRows = m_numCols > 0 ? ion::maths::Ceil((float)m_tileCount / (float)m_numCols) : 0;

		//Update frame title
		std::stringstream name;
		name << "Tiles (" << m_tileCount << ")";
		SetLabel(name.str());

		//Set pixels per scroll unit, scroll rate and num units
		int pixelsPerScrollUnitY = 8 * m_zoom;
		GetScrollHelper()->SetScrollbars(1, pixelsPerScrollUnitY, 1, m_numRows, 0, 0, true);
		GetScrollHelper()->SetScrollRate(0, 1);

		//Calc canvas size
		int scrollPageHeight = std::max(pixelsPerScrollUnitY * m_numRows, (int)(8.0f * m_zoom));
		wxSize canvasSize(GetClientSize().x, scrollPageHeight);

		//Set scroll page size
		SetVirtualSize(canvasSize);

		//Recreate local canvas
		m_canvas = wxBitmap(canvasSize);

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
		int tileY = (tileId / m_numCols) * 8 * m_zoom;
		int tileX = (tileId % m_numCols) * 8 * m_zoom;
		RefreshRect(wxRect(tileX - 1, tileY - 1 - scrollOffset.y, (8 * m_zoom) + 1, (8 * m_zoom) + 1));
	}
}

void TilesPanel::PaintAllToDc(wxMemoryDC& dc)
{
	if(m_project)
	{
		//Get renderable rect
		wxSize canvasSize = m_canvas.GetSize();
		wxRect canvasRect(0, 0, canvasSize.x, canvasSize.y);

		//Clear canvas
		dc.SetBrush(*wxBLACK_BRUSH);
		dc.DrawRectangle(canvasRect);

		if(m_numCols > 0)
		{
			int i = 0;
			for(auto it = m_project->GetTileset().Begin(), end = m_project->GetTileset().End(); it != end; ++it, ++i)
			{
				int y = i / m_numCols;
				int x = i % m_numCols;

				const Palette* palette = m_project->GetPalette(it->second.GetPaletteId());
				if(palette)
				{
					TileRenderer::PaintTileToDc(x, y, it->second, *palette, 0, dc);
				}
			}

			//Scale canvas
			int originalWidth = m_numCols * 8;
			int originalHeight = m_numRows * 8;
			wxRect sourceRect(0, 0, originalWidth, originalHeight);
			wxRect destRect(0, 0, originalWidth * m_zoom, originalHeight * m_zoom);

			//Copy rect from canvas
			dc.StretchBlit(destRect.x, destRect.y, destRect.width, destRect.height, &dc, sourceRect.x, sourceRect.y, sourceRect.width, sourceRect.height);
		}
	}
}