///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
/////////////////////////////////////////////////////

#include "Dialogs.h"
#include "SpriteCanvas.h"

#include "../BMPReader.h"

#include <wx/msgdlg.h>

DialogNewProject::DialogNewProject(wxWindow* parent)
	: DialogNewProjectBase(parent) 
{
	for(int i = 0; i < ePlatformNum; i++)
	{
		m_choicePreset->AppendString(PlatformPresets::s_configs[i].name);
	}

	PopulatePreset(PlatformPresets::ePresetMegaDrive);
	m_choicePreset->SetSelection(PlatformPresets::ePresetMegaDrive);
}

void DialogNewProject::OnChoicePreset(wxCommandEvent& event)
{
	PopulatePreset(event.GetInt());
}

void DialogNewProject::PopulatePreset(int index)
{
	PlatformConfig& config = PlatformPresets::s_configs[index];
	m_spinCtrlTileWidth->SetValue(config.tileWidth);
	m_spinCtrlTileHeight->SetValue(config.tileHeight);
	m_spinCtrlMapWidth->SetValue(config.scrollPlaneWidthTiles);
	m_spinCtrlMapHeight->SetValue(config.scrollPlaneHeightTiles);
}

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

ImportDialogSpriteSheet::ImportDialogSpriteSheet(wxWindow* parent, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources)
	: ImportDialogSpriteSheetBase(parent)
	, m_renderResources(renderResources)
	, m_project(project)
{
	m_canvas->SetProject(&project);
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
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	BMPReader reader;
	if(reader.Read(event.GetPath().GetData().AsChar()))
	{
		if(reader.GetWidth() % tileWidth != 0 || reader.GetHeight() % tileHeight != 0)
		{
			if(wxMessageBox("Bitmap width/height is not multiple of target platform tile width/height, image will be truncated", "Warning", wxOK | wxCANCEL | wxICON_WARNING) == wxCANCEL)
			{
				return;
			}
		}

		//Create texture from bitmap
		m_renderResources.CreateSpriteSheetPreviewTexture(reader);

		m_canvas->SetSpriteSheetDimentionsPixels(ion::Vector2i(reader.GetWidth(), reader.GetHeight()));

		m_textName->SetValue(m_filePicker->GetFileName().GetName());
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