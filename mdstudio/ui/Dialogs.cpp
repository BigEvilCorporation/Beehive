///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
/////////////////////////////////////////////////////

#include "Dialogs.h"
#include "SpriteCanvas.h"

#include "../BMPReader.h"

#include <wx/msgdlg.h>

ImportDialog::ImportDialog(wxWindow* parent) : ImportDialogBase(parent)
{

}

void ImportDialog::OnBtnBrowse(wxCommandEvent& event)
{
	wxFileDialog dialog(this, _("Open BMP files"), "", "", "BMP files (*.bmp)|*.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
	if(dialog.ShowModal() == wxID_OK)
	{
		dialog.GetPaths(m_paths);

		if(m_paths.size() == 0)
		{
			m_filenames->Clear();
		}
		else if(m_paths.size() == 1)
		{
			m_filenames->SetValue(m_paths[0]);
		}
		else
		{
			char text[128] = { 0 };
			sprintf(text, "(%u) BMP files", m_paths.size());
			m_filenames->SetValue(wxString(text));
		}
	}
}

ImportDialogSpriteSheet::ImportDialogSpriteSheet(wxWindow* parent, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources)
	: ImportDialogSpriteSheetBase(parent)
	, m_renderResources(renderResources)
{
	m_canvas->SetupRendering(&renderer, &glContext, &renderResources);
	m_canvas->SetSpriteSheetDimentionsCells(ion::Vector2i(m_spinWidthCells->GetValue(), m_spinHeightCells->GetValue()));
	m_canvas->SetDrawGrid(true);
	m_canvas->SetDrawPreview(true, 0);

	m_spinCellCount->SetValue(m_spinWidthCells->GetValue() * m_spinHeightCells->GetValue());
	m_spinCellCount->SetMax(m_spinWidthCells->GetValue() * m_spinHeightCells->GetValue());
	m_canvas->SetDrawPreview(true, m_spinCellCount->GetValue());
}

void ImportDialogSpriteSheet::OnFileOpened(wxFileDirPickerEvent& event)
{
	BMPReader reader;
	if(reader.Read(event.GetPath().GetData().AsChar()))
	{
		if(reader.GetWidth() % 8 != 0 || reader.GetHeight() % 8 != 0)
		{
			if(wxMessageBox("Bitmap width/height is not multiple of 8, image will be truncated", "Warning", wxOK | wxCANCEL | wxICON_WARNING) == wxCANCEL)
			{
				return;
			}
		}

		//Create texture from bitmap
		m_renderResources.CreateSpriteSheetPreviewTexture(reader);

		m_canvas->SetSpriteSheetDimentionsPixels(ion::Vector2i(reader.GetWidth(), reader.GetHeight()));
	}
}

void ImportDialogSpriteSheet::OnGridColourChanged(wxColourPickerEvent& event)
{
	ion::Colour colour((float)event.GetColour().Red() / 255.0f, (float)event.GetColour().Green() / 255.0f, (float)event.GetColour().Blue() / 255.0f);
	m_canvas->SetGridColour(colour);
}

void ImportDialogSpriteSheet::OnSpinWidthCells(wxSpinEvent& event)
{
	m_canvas->SetSpriteSheetDimentionsCells(ion::Vector2i(m_spinWidthCells->GetValue(), m_spinHeightCells->GetValue()));
	m_spinCellCount->SetValue(m_spinWidthCells->GetValue() * m_spinHeightCells->GetValue());
	m_spinCellCount->SetMax(m_spinWidthCells->GetValue() * m_spinHeightCells->GetValue());
	m_canvas->SetDrawPreview(true, m_spinCellCount->GetValue());
}

void ImportDialogSpriteSheet::OnSpinHeightCells(wxSpinEvent& event)
{
	m_canvas->SetSpriteSheetDimentionsCells(ion::Vector2i(m_spinWidthCells->GetValue(), m_spinHeightCells->GetValue()));
	m_spinCellCount->SetValue(m_spinWidthCells->GetValue() * m_spinHeightCells->GetValue());
	m_spinCellCount->SetMax(m_spinWidthCells->GetValue() * m_spinHeightCells->GetValue());
	m_canvas->SetDrawPreview(true, m_spinCellCount->GetValue());
}

void ImportDialogSpriteSheet::OnSpinCellCount(wxSpinEvent& event)
{
	m_canvas->SetDrawPreview(true, m_spinCellCount->GetValue());
}