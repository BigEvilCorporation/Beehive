///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <string>

#include "MainWindow.h"
#include "MapPanel.h"

MainWindow::MainWindow()
	: MainWindowBase(NULL)
{
	m_project = NULL;
	m_toolboxMapEdit = NULL;
}

MainWindow::~MainWindow()
{
}

void MainWindow::SetProject(Project* project)
{
	if(project && project != m_project)
	{
		m_project = project;
		m_mapPanel->SetProject(project);

		//New project, open default toolboxes
		ShowToolboxTiles();
		ShowToolboxMapEdit();
		ShowToolboxPalettes();
	}
	else
	{
		//Project closed, close toolboxes
		if(m_toolboxMapEdit)
			delete m_toolboxMapEdit;

		if(m_toolboxTiles)
			delete m_toolboxTiles;

		if(m_toolboxPalettes)
			delete m_toolboxPalettes;

		m_mapPanel->SetProject(NULL);
		m_project = NULL;
	}

	//Refresh whole window
	Refresh();
}

void MainWindow::ShowToolboxTiles()
{
	if(m_project)
	{
		if(!m_toolboxTiles)
		{
			m_toolboxTiles = new ToolboxTiles(*m_project, this);
			m_toolboxTiles->SetPosition(wxPoint(600, 200));
		}

		if(!m_toolboxTiles->IsShown())
		{
			m_toolboxTiles->Show();
		}
	}
}

void MainWindow::ShowToolboxMapEdit()
{
	if(m_project)
	{
		if(!m_toolboxMapEdit)
		{
			m_toolboxMapEdit = new ToolboxMapEdit(*m_project, this);
			m_toolboxMapEdit->SetPosition(wxPoint(50, 200));
		}

		if(!m_toolboxMapEdit->IsShown())
		{
			m_toolboxMapEdit->Show();
		}
	}
}

void MainWindow::ShowToolboxPalettes()
{
	if(m_project)
	{
		if(!m_toolboxPalettes)
		{
			m_toolboxPalettes = new ToolboxPalettes(*m_project, this);
			m_toolboxPalettes->SetPosition(wxPoint(400, 400));
		}

		if(!m_toolboxPalettes->IsShown())
		{
			m_toolboxPalettes->Show();
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
	ShowToolboxMapEdit();
}

void MainWindow::OnBtnToolsTiles( wxRibbonButtonBarEvent& event )
{
	ShowToolboxTiles();
}

void MainWindow::OnBtnToolsPalettes( wxRibbonButtonBarEvent& event )
{
	ShowToolboxPalettes();
}