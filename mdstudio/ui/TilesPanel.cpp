#include "TilesPanel.h"
#include "TileRendering.h"

#include <maths/Vector.h>

TilesPanel::TilesPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxScrolledWindow(parent, id, pos, size, style, name)
{
	m_zoom = 4.0f;
	m_currentSelection = 0;

	Bind(wxEVT_LEFT_DOWN,		&TilesPanel::OnMouse, this, wxID_TILESPANEL);
	Bind(wxEVT_PAINT,			&TilesPanel::OnPaint, this, wxID_TILESPANEL);
	Bind(wxEVT_ERASE_BACKGROUND,&TilesPanel::OnErase, this, wxID_TILESPANEL);

	//TEST
	Tile* testPaintTile = new Tile;
	Tile* testEraseTile = new Tile;

	int testTile[8*8] = 
	{
		0,0,0,0,0,0,0,0,
		0,0,1,1,1,1,0,0,
		0,1,0,1,1,0,1,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,1,0,1,1,0,1,0,
		0,0,1,1,1,1,0,0,
		0,0,0,0,0,0,0,0
	};

	for(int i = 0; i < 8*8; i++)
	{
		int y = i / 8;
		int x = i % 8;
		testPaintTile->SetPixelColour(x, y, testTile[i]);
	}

	AddTile(*testPaintTile);
}

void TilesPanel::AddTile(const Tile& tile)
{
	m_tiles.push_back(&tile);
	Refresh();
}

void TilesPanel::OnMouse(wxMouseEvent& event)
{
	if(event.ButtonIsDown(wxMOUSE_BTN_LEFT))
	{
		//Get mouse position in map space
		wxPaintDC paintDc(this);
		wxPoint mouseCanvasPosWx = event.GetLogicalPosition(paintDc);
		ion::Vector2 mousePosMapSpace(mouseCanvasPosWx.x / m_zoom, mouseCanvasPosWx.y / m_zoom);

		//Set current selection
		int x = (int)floor(mousePosMapSpace.x / 8.0f);
		int y = (int)floor(mousePosMapSpace.y / 8.0f);
		wxSize clientSize = GetClientSize();
		int numCols = (clientSize.x / 8) / m_zoom;
		m_currentSelection = (y * numCols) + x;

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

	int numTiles = m_tiles.size();
	int numCols = (clientSize.x / 8) / m_zoom;

	//Bitmap source and dc
	wxBitmap canvas(clientRect.width, clientRect.height);
	wxMemoryDC sourceDC(canvas);

	//Paint tiles to source dc
	if(numCols > 0)
	{
		for(int i = 0; i < numTiles; i++)
		{
			int y = i / numCols;
			int x = i % numCols;

			tilerendering::PaintTileToDc(x, y, *m_tiles[i], sourceDC);
		}
	}

	//Clear dest rect
	destDC.SetBrush(*wxBLACK_BRUSH);
	destDC.DrawRectangle(clientRect);

	//Paint scaled tiles to dest dc
	wxRect sourceRect(0, 0, clientRect.width, clientRect.height);
	wxRect destRect(0, 0, clientRect.width * m_zoom, clientRect.height * m_zoom);
		
	//Copy rect from canvas
	destDC.StretchBlit(destRect.x, destRect.y, destRect.width, destRect.height, &sourceDC, sourceRect.x, sourceRect.y, sourceRect.width, sourceRect.height);

	//Draw rectangle around current selection
	destDC.SetPen(*wxRED_PEN);
	destDC.SetBrush(*wxTRANSPARENT_BRUSH);
	int currSelectionY = (m_currentSelection / numCols) * 8 * m_zoom;
	int currSelectionX = (m_currentSelection % numCols) * 8 * m_zoom;
	destDC.DrawRectangle(currSelectionX, currSelectionY, 8 * m_zoom, 8 * m_zoom);
}

void TilesPanel::OnErase(wxEraseEvent& event)
{
	//Ignore event
}