///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/aui/framemanager.h>

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

	//Refresh AUI manager
	m_auiManager.Update();

	//Refresh whole window
	Refresh();
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
			
			m_tilesPanel = new TilesPanel(m_dockArea, NewControlId());
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

void MainWindow::OnBtnProjExport(wxRibbonButtonBarEvent& event)
{
	if(m_project)
	{
		wxDirDialog dialogue(this, "Select export directory");
		if(dialogue.ShowModal() == wxID_OK)
		{
			std::string directory = dialogue.GetPath();
			std::string filenamePalettes = directory + "\\" + m_project->GetName() + "_pal.asm";
			std::string filenameTiles = directory + "\\" + m_project->GetName() + "_til.asm";
			std::string filenameMap = directory + "\\" + m_project->GetName() + "_map.asm";
			std::string filenameCollision = directory + "\\" + m_project->GetName() + "_col.asm";

			m_project->ExportPalettes(filenamePalettes);
			m_project->ExportTiles(filenameTiles);
			m_project->ExportMap(filenameMap);
			m_project->ExportCollision(filenameCollision);
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
			m_project->ImportBitmap(dialogue.GetPath().c_str().AsChar(), Project::eBMPImportReplaceAll | Project::eBMPImportDrawToMap);

			//Refresh whole application
			m_mapPanel->Refresh();
			Refresh();
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