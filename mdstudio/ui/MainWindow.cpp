///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/aui/framemanager.h>
#include <wx/msgdlg.h>

#include <string>

#include "MainWindow.h"

#include "PalettesPanel.h"
#include "TilesPanel.h"
#include "MapPanel.h"

MainWindow::MainWindow()
	: MainWindowBase(NULL)
{
	m_auiManager.SetManagedWindow(m_dockArea);
	m_project = NULL;
	SetStatusText("Big Evil Editor v0.1");
}

MainWindow::~MainWindow()
{
}

void MainWindow::SetProject(Project* project)
{
	if(project && project != m_project)
	{
		m_project = project;

		//New project, open default panels
		ShowPanelPalettes();
		ShowPanelTiles();
		ShowPanelMap();
	}
	else
	{
		//Project closed, close panels
		if(m_tilesPanel)
			delete m_tilesPanel;

		if(m_palettesPanel)
			delete m_palettesPanel;

		if(m_mapPanel)
			delete m_mapPanel;

		m_project = NULL;
	}

	//Refresh whole window
	RefreshAll();
}

void MainWindow::ShowPanelPalettes()
{
	if(m_project)
	{
		if(!m_palettesPanel)
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(200, 100);
			paneInfo.Left();
			paneInfo.Caption("Palettes");
			paneInfo.CaptionVisible(true);

			m_palettesPanel = new PalettesPanel(m_dockArea, NewControlId());
			m_palettesPanel->SetProject(m_project);
			m_auiManager.AddPane(m_palettesPanel, paneInfo);
		}

		if(!m_palettesPanel->IsShown())
		{
			m_palettesPanel->Show();
		}
	}
}

void MainWindow::ShowPanelTiles()
{
	if(m_project)
	{
		if(!m_tilesPanel)
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(200, 100);
			paneInfo.Left();
			paneInfo.Caption("Tiles");
			paneInfo.CaptionVisible(true);
			
			m_tilesPanel = new TilesPanel(m_dockArea, NewControlId(), wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxALWAYS_SHOW_SB);
			m_tilesPanel->SetProject(m_project);
			m_auiManager.AddPane(m_tilesPanel, paneInfo);
		}

		if(!m_tilesPanel->IsShown())
		{
			m_tilesPanel->Show();
		}
	}
}

void MainWindow::ShowPanelMap()
{
	if(m_project)
	{
		if(!m_mapPanel)
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(300, 300);
			paneInfo.Centre();
			paneInfo.Caption("Map");
			paneInfo.CaptionVisible(true);

			m_mapPanel = new MapPanel(m_dockArea, NewControlId());
			m_mapPanel->SetProject(m_project);
			m_auiManager.AddPane(m_mapPanel, paneInfo);
		}

		if(!m_mapPanel->IsShown())
		{
			m_mapPanel->Show();
		}
	}
}

void MainWindow::RefreshAll()
{
	m_auiManager.Update();
	Refresh();

	if(m_project)
	{
		m_project->InvalidateTiles(true);
		m_project->InvalidateTiles(true);
	}

	if(m_palettesPanel)
		m_palettesPanel->Refresh();

	if(m_tilesPanel)
		m_tilesPanel->Refresh();

	if(m_mapPanel)
		m_mapPanel->Refresh();
}

void MainWindow::OnBtnProjOpen(wxRibbonButtonBarEvent& event)
{
	wxFileDialog dialogue(this, _("Open BEE file"), "", "", "BEE files (*.bee)|*.bee", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(dialogue.ShowModal() == wxID_OK)
	{
		SetStatusText("Opening...");

		Project* project = new Project();

		if(project->Load(dialogue.GetPath().c_str().AsChar()))
		{
			//Destroy old project
			delete m_project;

			//Set new project
			m_project = project;
			
			if(m_palettesPanel)
				m_palettesPanel->SetProject(project);

			if(m_tilesPanel)
				m_tilesPanel->SetProject(project);

			if(m_mapPanel)
				m_mapPanel->SetProject(project);

			//Refresh whole application
			RefreshAll();

			SetStatusText("Load complete");
		}
		else
		{
			delete project;
			SetStatusText("Load error");
			wxMessageBox("Error loading project", "Error", wxOK | wxICON_ERROR);
		}
	}
}

void MainWindow::OnBtnProjSave(wxRibbonButtonBarEvent& event)
{
	if(m_project)
	{
		const std::string& lastFilename = m_project->GetFilename();

		if(!lastFilename.size())
		{
			//No previous filename, open "Save As..." dialog
			OnBtnProjSaveAs(event);
		}
		else
		{
			//Save with last filename
			SetStatusText("Saving...");

			if(m_project->Save(lastFilename))
			{
				SetStatusText("Save complete");
			}
			else
			{
				wxMessageBox("Error writing project", "Error", wxOK | wxICON_ERROR);
				SetStatusText("Save error");
			}
		}
	}
}

void MainWindow::OnBtnProjSaveAs(wxRibbonButtonBarEvent& event)
{
	if(m_project)
	{
		wxFileDialog dialogue(this, _("Open BEE file"), "", "", "BEE files (*.bee)|*.bee", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if(dialogue.ShowModal() == wxID_OK)
		{
			SetStatusText("Saving...");

			if(m_project->Save(dialogue.GetPath().c_str().AsChar()))
			{
				SetStatusText("Save complete");
			}
			else
			{
				wxMessageBox("Error writing project", "Error", wxOK | wxICON_ERROR);
				SetStatusText("Save error");
			}
		}
	}
}

void MainWindow::OnBtnProjExport(wxRibbonButtonBarEvent& event)
{
	if(m_project)
	{
		wxDirDialog dialogue(this, "Select export directory");
		if(dialogue.ShowModal() == wxID_OK)
		{
			SetStatusText("Exporting...");

			std::string directory = dialogue.GetPath();
			std::string filenamePalettes = directory + "\\" + m_project->GetName() + "_pal.asm";
			std::string filenameTiles = directory + "\\" + m_project->GetName() + "_til.asm";
			std::string filenameMap = directory + "\\" + m_project->GetName() + "_map.asm";
			std::string filenameCollision = directory + "\\" + m_project->GetName() + "_col.asm";

			m_project->ExportPalettes(filenamePalettes);
			m_project->ExportTiles(filenameTiles);
			m_project->ExportMap(filenameMap);
			m_project->ExportCollision(filenameCollision);

			SetStatusText("Export complete");
			wxMessageBox("Export complete", "Error", wxOK | wxICON_INFORMATION);
		}
	}
}

void MainWindow::OnBtnTilesImport( wxRibbonButtonBarEvent& event )
{
	if(m_project)
	{
		wxFileDialog dialogue(this, _("Open BMP file"), "", "", "BMP files (*.bmp)|*.bmp", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
		if(dialogue.ShowModal() == wxID_OK)
		{
			SetStatusText("Importing...");

			m_project->ImportBitmap(dialogue.GetPath().c_str().AsChar(), Project::eBMPImportReplaceAll | Project::eBMPImportDrawToMap);

			//Refresh whole application
			RefreshAll();

			SetStatusText("Import complete");
		}
	}
}

void MainWindow::OnBtnToolsMapEdit( wxRibbonButtonBarEvent& event )
{

}

void MainWindow::OnBtnToolsTiles( wxRibbonButtonBarEvent& event )
{
	ShowPanelTiles();
}

void MainWindow::OnBtnToolsPalettes( wxRibbonButtonBarEvent& event )
{
	ShowPanelPalettes();
}