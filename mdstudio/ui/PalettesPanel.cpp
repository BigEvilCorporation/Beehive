///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "PalettesPanel.h"
#include <wx/App.h>
#include <maths/Vector.h>

PalettesPanel::PalettesPanel(	wxWindow *parent,
								wxWindowID id,
								const wxPoint& pos,
								const wxSize& size,
								long style,
								const wxString& name)
	: wxPanel(parent, id, pos, size, style, name)
{
	m_project = NULL;

	Bind(wxEVT_LEFT_DOWN,		&PalettesPanel::OnMouse, this, GetId());
	Bind(wxEVT_PAINT,			&PalettesPanel::OnPaint, this, GetId());
	Bind(wxEVT_ERASE_BACKGROUND,&PalettesPanel::OnErase, this, GetId());

	SetBackgroundStyle(wxBG_STYLE_PAINT);
}

namespace
{
	wxWindow* FindTop(wxWindow* window)
	{
		wxWindow* top = window;
		if(wxWindow* parent = window->GetParent())
		{
			top = FindTop(parent);
		}
		return top;
	}

	void RefreshAll(wxWindow* window)
	{
		window->Refresh();
		window->Update();
		wxWindowList& children = window->GetChildren();
		for(wxWindowList::iterator it = children.begin(), end = children.end(); it != end; ++it)
		{
			RefreshAll(*it);
		}
	}
}

void PalettesPanel::OnMouse(wxMouseEvent& event)
{
	if(event.ButtonIsDown(wxMOUSE_BTN_LEFT))
	{
		//Get mouse position in map space
		wxClientDC clientDc(this);
		wxPoint mouseCanvasPosWx = event.GetLogicalPosition(clientDc);
		ion::Vector2 mousePosMapSpace(mouseCanvasPosWx.x, mouseCanvasPosWx.y);

		//Get current selection
		unsigned int colourId = (unsigned int)floor(mousePosMapSpace.x / (float)colourRectSize);
		unsigned int paletteId = (unsigned int)floor(mousePosMapSpace.y / (float)colourRectSize);

		if(paletteId < 4 && colourId < Palette::coloursPerPalette)
		{
			if(Palette* palette = m_project->GetPalette((PaletteId)paletteId))
			{
				wxColourDialog dialogue(this);
				if(dialogue.ShowModal() == wxID_OK)
				{
					wxColour wxcolour = dialogue.GetColourData().GetColour();
					Colour colour(wxcolour.Red(), wxcolour.Green(), wxcolour.Blue());
					palette->SetColour(colourId, colour);

					//Invalidate map
					m_project->InvalidateMap(true);

					//Redraw all
					RefreshAll(FindTop(this));
				}
			}
		}
	}

	event.Skip();
}

void PalettesPanel::OnPaint(wxPaintEvent& event)
{
	//Double buffered dest dc
	wxAutoBufferedPaintDC destDC(this);

	//Get renderable client rect
	wxSize clientSize = GetClientSize();
	wxRect clientRect(0, 0, clientSize.x, clientSize.y);

	//Clear dest rect
	destDC.SetBrush(*wxBLACK_BRUSH);
	destDC.DrawRectangle(clientRect);

	//No outline
	destDC.SetPen(wxNullPen);

	for(int i = 0; i < 4; i++)
	{
		const Palette* palette = m_project->GetPalette(i);

		for(int j = 0; j < palette->GetNumColours(); j++)
		{
			const Colour& colour = palette->GetColour(j);
			wxBrush brush;
			brush.SetColour(wxColour(colour.r, colour.g, colour.b));
			destDC.SetBrush(brush);
			destDC.DrawRectangle(j * colourRectSize, i * colourRectSize, colourRectSize, colourRectSize);
		}
	}
}

void PalettesPanel::OnErase(wxEraseEvent& event)
{
	//Ignore event
}