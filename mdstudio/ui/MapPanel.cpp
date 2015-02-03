#include "MapPanel.h"

MapPanel::MapPanel(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxPanel(parent, winid, pos, size, style, name)
{
	m_paintTile = NULL;
	m_cameraZoom = 1.0f;

	Bind(wxEVT_LEFT_DOWN,	&MapPanel::OnMouse, this, wxID_MAPPANEL);
	Bind(wxEVT_LEFT_UP,		&MapPanel::OnMouse, this, wxID_MAPPANEL);
	Bind(wxEVT_MIDDLE_DOWN, &MapPanel::OnMouse, this, wxID_MAPPANEL);
	Bind(wxEVT_MIDDLE_UP,	&MapPanel::OnMouse, this, wxID_MAPPANEL);
	Bind(wxEVT_RIGHT_DOWN,	&MapPanel::OnMouse, this, wxID_MAPPANEL);
	Bind(wxEVT_RIGHT_UP,	&MapPanel::OnMouse, this, wxID_MAPPANEL);
	Bind(wxEVT_MOTION,		&MapPanel::OnMouse, this, wxID_MAPPANEL);
	Bind(wxEVT_MOUSEWHEEL,	&MapPanel::OnMouse, this, wxID_MAPPANEL);
	Bind(wxEVT_PAINT,		&MapPanel::OnPaint, this, wxID_MAPPANEL);

	//TEST
	m_paintTile = new Tile;

	int x = 0;
	int y = 0;
	for(int i = 0; i < 8; i++)
	{
		m_paintTile->SetPixelColour(x, y, 1);
		x++;
		y++;
	}
}

void MapPanel::OnMouse(wxMouseEvent& event)
{
	//Get mouse position in dc space
	wxPaintDC paintDc(this);
	wxAffineMatrix2D matrix;
	matrix.Translate(m_cameraPos.x, m_cameraPos.y);
	matrix.Scale(m_cameraZoom, m_cameraZoom);
	paintDc.SetTransformMatrix(matrix);
	wxPoint mousePosWx = event.GetLogicalPosition(paintDc);
	ion::Vector2 mousePos(mousePosWx.x, mousePosWx.y);

	if(event.Dragging())
	{
		if(event.ButtonIsDown(wxMOUSE_BTN_LEFT))
		{
			//Paint
			PaintTile(mousePos);
		}
		else if(event.ButtonIsDown(wxMOUSE_BTN_RIGHT))
		{
			//Erase
			EraseTile(mousePos);
		}
		else if(event.ButtonIsDown(wxMOUSE_BTN_MIDDLE))
		{
			//Pan
			m_cameraPos.x += event.GetX();
			m_cameraPos.y += event.GetX();
		}
	}
	else if(event.LeftDown())
	{
		//Paint
		PaintTile(mousePos);
	}
	else if(event.RightDown())
	{
		//Erase
		EraseTile(mousePos);
	}
	else if(event.GetWheelDelta() != 0)
	{
		//Zoom
		int wheelDelta = event.GetWheelDelta();
		const float zoomSpeed = 0.1f;
		m_cameraZoom += (float)wheelDelta * zoomSpeed;
	}
}

void MapPanel::OnPaint(wxPaintEvent& evt)
{
	wxPaintDC paintDc(this);
	Render(paintDc);
}

void MapPanel::Render(wxDC& dc)
{
	wxAffineMatrix2D matrix;
	matrix.Translate(m_cameraPos.x, m_cameraPos.y);
	matrix.Scale(m_cameraZoom, m_cameraZoom);
	dc.SetTransformMatrix(matrix);
 
	PaintMapToCanvas(dc);
}

void MapPanel::PaintTile(ion::Vector2 mousePos)
{
	if(m_paintTile)
	{
		int x = (int)floor(mousePos.x / 8.0f);
		int y = (int)floor(mousePos.y / 8.0f);
		m_map.SetTile(x, y, *m_paintTile);
	}
}

void MapPanel::EraseTile(ion::Vector2 mousePos)
{
}

void MapPanel::PaintMapToCanvas(wxDC& dc)
{
	for(int x = 0; x < m_map.width; x++)
	{
		for(int y = 0; y < m_map.height; y++)
		{
			PaintTileToCanvas(x, y, m_map.GetTile(x, y), dc);
		}
	}
}

void MapPanel::PaintTileToCanvas(int x, int y, const Tile& tile, wxDC& dc)
{
	int startPixelX = x * 8;
	int startPixelY = y * 8;

	for(int tileX = 0; tileX < 8; tileX++)
	{
		for(int tileY = 0; tileY < 8; tileY++)
		{
			u8 colourIdx = tile.GetPixelColour(tileX, tileY);

			//TODO: Convert to MD colour
			if(colourIdx == 0)
				dc.SetPen(*wxGREEN_PEN);
			else
				dc.SetPen(*wxBLUE_PEN);

			dc.DrawPoint(startPixelX + tileX, startPixelY + tileY);
		}
	}
}