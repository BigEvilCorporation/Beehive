///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#include "PalettesPanel.h"
#include "MainWindow.h"
#include <wx/App.h>
#include <wx/Menu.h>
#include <maths/Vector.h>

#include <stdio.h>

PalettesPanel::PalettesPanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxPanel(parent, id, pos, size, style, name)
	, m_project(project)
{
	m_mainWindow = mainWindow;
	m_orientation = eVertical;

	m_dragPalette = -1;
	m_dragColour = -1;

	Bind(wxEVT_LEFT_DOWN,		&PalettesPanel::OnMouse, this, GetId());
	Bind(wxEVT_LEFT_UP,			&PalettesPanel::OnMouse, this, GetId());
	Bind(wxEVT_LEFT_DCLICK,		&PalettesPanel::OnMouse, this, GetId());
	Bind(wxEVT_MOTION,			&PalettesPanel::OnMouse, this, GetId());
	Bind(wxEVT_RIGHT_UP,		&PalettesPanel::OnMouse, this, GetId());
	Bind(wxEVT_PAINT,			&PalettesPanel::OnPaint, this, GetId());
	Bind(wxEVT_ERASE_BACKGROUND,&PalettesPanel::OnErase, this, GetId());
	Bind(wxEVT_SIZE,			&PalettesPanel::OnResize, this, GetId());

	SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void PalettesPanel::OnMouse(wxMouseEvent& event)
{
	//Get mouse position in map space
	wxClientDC clientDc(this);
	wxPoint mouseCanvasPosWx = event.GetLogicalPosition(clientDc);
	ion::Vector2 mousePosMapSpace(mouseCanvasPosWx.x, mouseCanvasPosWx.y);

	//Get panel size
	wxSize panelSize = GetClientSize();

	float x = (m_orientation == eVertical) ? mousePosMapSpace.y : mousePosMapSpace.x;
	float y = (m_orientation == eVertical) ? mousePosMapSpace.x : mousePosMapSpace.y;
	float colourRectSize = (m_orientation == eVertical) ? (panelSize.y / Palette::coloursPerPalette) : (panelSize.x / Palette::coloursPerPalette);

	//Get current selection
	unsigned int colourId = (unsigned int)floor(x / colourRectSize);
	unsigned int paletteId = (unsigned int)floor(y / colourRectSize);

	if(paletteId < 4)
	{
		if(event.Dragging() && (m_dragPalette == -1))
		{
			//Begin drag/drop
			m_dragPalette = paletteId;
			m_dragColour = colourId;
		}

		if(event.LeftUp() && (m_dragPalette != -1))
		{
			//End drag/drop

			//TODO: Fix for non-active palette
			if(m_dragPalette != paletteId && m_dragColour != colourId)
			{
				if(Palette* palette = m_project.GetPalette(paletteId))
				{
					if(palette->IsColourUsed(m_dragColour) && palette->IsColourUsed(colourId))
					{
						//Swap colours in palette
						Colour originalColour = palette->GetColour(m_dragColour);
						Colour newColour = palette->GetColour(colourId);
						palette->SetColour(colourId, originalColour);
						palette->SetColour(m_dragColour, newColour);

						//Swap colours in all tiles
						m_project.SwapPaletteEntries(m_dragColour, colourId);

						//Refresh all
						m_mainWindow->RefreshAll();
					}
				}
			}

			m_dragPalette = -1;
			m_dragColour = -1;
		}

		if(event.RightUp())
		{
			//Right-click slot menu
			m_selectedPaletteId = paletteId;
			m_seletedColourId = colourId;

			wxMenu slotMenu;
			wxMenu* saveMenu = new wxMenu();
			wxMenu* loadMenu = new wxMenu();

			char text[1024] = { 0 };
			int numSlots = m_project.GetNumPaletteSlots();

			for(int i = 0; i < numSlots; i++)
			{
				sprintf(text, "Slot %u", i);
				saveMenu->Append(i | eMenuSave, wxString(text));
				loadMenu->Append(i | eMenuLoad, wxString(text));
			}

			saveMenu->AppendSeparator();
			saveMenu->Append(eMenuNew, wxString("Backup to new slot"));

			slotMenu.AppendSubMenu(saveMenu, "Save");
			slotMenu.AppendSubMenu(loadMenu, "Load");
			slotMenu.Append(eMenuImport, wxString("Import..."));
			slotMenu.Append(eMenuExport, wxString("Export..."));
			saveMenu->AppendSeparator();
			slotMenu.Append(eMenuSetAsBg, wxString("Set as Background/Transparency Colour"));
				
			slotMenu.SetClientData((void*)paletteId);
			slotMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&PalettesPanel::OnSlotsMenuClick, NULL, this);
			PopupMenu(&slotMenu, event.GetPosition());
		}

		if(paletteId < m_project.GetNumPalettes() && colourId < Palette::coloursPerPalette)
		{
			if(Palette* palette = m_project.GetPalette((PaletteId)paletteId))
			{
				if(event.LeftDClick())
				{
					wxColourDialog dialogue(this);
					if(dialogue.ShowModal() == wxID_OK)
					{
						wxColour wxcolour = dialogue.GetColourData().GetColour();
						Colour colour(wxcolour.Red(), wxcolour.Green(), wxcolour.Blue());
						palette->SetColour(colourId, colour);

						//Refresh tiles, stamps and map panels
						m_mainWindow->RefreshAll();
					}
				}

				if(palette->IsColourUsed(colourId))
				{
					if(event.ButtonIsDown(wxMOUSE_BTN_LEFT))
					{
						//Set current paint colour
						m_project.SetPaintColour(colourId);
					}
				}
			}
		}
	}

	event.Skip();
}

void PalettesPanel::OnSlotsMenuClick(wxCommandEvent& event)
{
	int menuItemId = event.GetId();
	int slotId = menuItemId & 0xFF;
	int numSlots = m_project.GetNumPaletteSlots();
	PaletteId paletteId = (PaletteId)event.GetClientData();

	if(menuItemId & eMenuNew)
	{
		//Backup palette to new slot
		Palette* palette = m_project.GetPalette(paletteId);
		m_project.AddPaletteSlot(*palette);
	}
	else if(menuItemId & eMenuSave)
	{
		//Backup palette to existing slot
		Palette* palette = m_project.GetPalette(paletteId);
		Palette* slot = m_project.GetPaletteSlot(slotId);
		*slot = *palette;
	}
	else if(menuItemId & eMenuLoad)
	{
		//Set active palette slot
		m_project.SetActivePaletteSlot(paletteId, slotId);

		//Refresh tiles, stamps and map panels
		m_mainWindow->RefreshAll();
	}
	else if(menuItemId & eMenuImport)
	{
		wxFileDialog dialogue(this, _("Open BEE Palettes file"), "", "", "BEE_Palette files (*.bee_palette)|*.bee_palette", wxFD_OPEN);
		if(dialogue.ShowModal() == wxID_OK)
		{
			std::string filename = dialogue.GetPath().c_str().AsChar();
			m_project.ImportPaletteSlots(filename);
			m_mainWindow->RefreshAll();
		}
	}
	else if(menuItemId & eMenuExport)
	{
		wxFileDialog dialogue(this, _("Save BEE Palettes file"), "", "", "BEE_Palette files (*.bee_palette)|*.bee_palette", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if(dialogue.ShowModal() == wxID_OK)
		{
			std::string filename = dialogue.GetPath().c_str().AsChar();
			m_project.ExportPaletteSlots(filename);
		}
	}
	else if(menuItemId & eMenuSetAsBg)
	{
		if(Palette* palette = m_project.GetPalette(m_selectedPaletteId))
		{
			//Swap colours in palette
			Colour originalColour = palette->GetColour(0);
			Colour newColour = palette->GetColour(m_seletedColourId);
			palette->SetColour(m_seletedColourId, originalColour);
			palette->SetColour(0, newColour);

			//Swap colours in all tiles
			m_project.SetBackgroundColour(m_seletedColourId);

			//Refresh all
			m_mainWindow->RefreshAll();
		}
	}
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

	float colourRectSize = (m_orientation == eVertical) ? (clientSize.y / Palette::coloursPerPalette) : (clientSize.x / Palette::coloursPerPalette);

	for(int i = 0; i < m_project.GetNumPalettes(); i++)
	{
		const Palette* palette = m_project.GetPalette(i);

		for(int j = 0; j < Palette::coloursPerPalette; j++)
		{
			int x = (m_orientation == eVertical) ? i : j;
			int y = (m_orientation == eVertical) ? j : i;

			wxBrush brush;

			if(palette->IsColourUsed(j))
			{
				const Colour& colour = palette->GetColour(j);
				brush.SetColour(wxColour(colour.GetRed(), colour.GetGreen(), colour.GetBlue()));
			}
			else
			{
				brush.SetStyle(wxBRUSHSTYLE_CROSSDIAG_HATCH);
				brush.SetColour(wxColour(100, 100, 100, 50));
			}

			destDC.SetBrush(brush);
			destDC.DrawRectangle(x * colourRectSize, y * colourRectSize, colourRectSize, colourRectSize);
		}
	}
}

void PalettesPanel::OnErase(wxEraseEvent& event)
{
	//Ignore event
}

void PalettesPanel::OnResize(wxSizeEvent& event)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		wxSize newSize = event.GetSize();

		if(newSize.x > newSize.y)
		{
			//Set new orientation
			m_orientation = eHorizontal;

			//Limit height
			int colourRectSize = (newSize.x / Palette::coloursPerPalette);
			SetMinSize(wxSize(1, colourRectSize * m_project.GetNumPalettes()));
		}
		else
		{
			//Set new orientation
			m_orientation = eVertical;

			//Limit width
			int colourRectSize = (newSize.y / Palette::coloursPerPalette);
			SetMinSize(wxSize(colourRectSize * m_project.GetNumPalettes(), 1));
		}

		Refresh();
	}
}