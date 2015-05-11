///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
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
	SetStatusText("BEEhive v0.1");

	//Create blank OpenGL panel to create global DC
	wxGLCanvas* m_blankCanvas = new wxGLCanvas(this, wxID_ANY, NULL);

	//Get GL context
	m_context = m_blankCanvas->GetContext();

	//Create renderer (from global DC
	m_renderer = ion::render::Renderer::Create(m_blankCanvas->GetHDC());
}

MainWindow::~MainWindow()
{
	//Close project and panels
	SetProject(NULL);

	//Delete renderer and OpenGL context
	delete m_renderer;
	delete m_context;
}

void MainWindow::SetProject(Project* project)
{
	if(project && project != m_project)
	{
		m_project = project;

		//New project, open default panels
		ShowPanelToolbox();
		ShowPanelPalettes();
		ShowPanelTiles();
		ShowPanelStamps();
		ShowPanelMap();

		//Sync settings widgets states
		SyncSettingsWidgets();
	}
	else
	{
		//Project closed, close panels
		if(m_tilesPanel)
		{
			m_auiManager.DetachPane(m_tilesPanel);
			delete m_tilesPanel;
		}

		if(m_palettesPanel)
		{
			m_auiManager.DetachPane(m_palettesPanel);
			delete m_palettesPanel;
		}

		if(m_mapPanel)
		{
			m_auiManager.DetachPane(m_mapPanel);
			delete m_mapPanel;
		}

		if(m_toolboxPanel)
		{
			m_auiManager.DetachPane(m_toolboxPanel);
			delete m_toolboxPanel;
		}

		if(m_stampsPanel)
		{
			m_auiManager.DetachPane(m_stampsPanel);
			delete m_stampsPanel;
		}

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
			paneInfo.BestSize(300, 100);
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
			paneInfo.BestSize(300, 500);
			paneInfo.Right();
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

void MainWindow::ShowPanelStamps()
{
	if(m_project)
	{
		if(!m_stampsPanel)
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(300, 300);
			paneInfo.Right();
			paneInfo.Caption("Stamps");
			paneInfo.CaptionVisible(true);

			m_stampsPanel = new StampsPanel(*m_renderer, m_context, m_dockArea, NewControlId());
			m_stampsPanel->SetProject(m_project);
			m_auiManager.AddPane(m_stampsPanel, paneInfo);
		}

		if(!m_stampsPanel->IsShown())
		{
			m_stampsPanel->Show();
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

			m_mapPanel = new MapPanel(*m_renderer, m_context, m_dockArea, NewControlId());
			m_mapPanel->SetProject(m_project);
			m_auiManager.AddPane(m_mapPanel, paneInfo);
		}

		if(!m_mapPanel->IsShown())
		{
			m_mapPanel->Show();
		}
	}
}

void MainWindow::ShowPanelToolbox()
{
	if(!m_toolboxPanel)
	{
		wxAuiPaneInfo paneInfo;
		paneInfo.Dockable(true);
		paneInfo.DockFixed(false);
		paneInfo.BestSize(100, 200);
		paneInfo.Left();
		paneInfo.Caption("Toolbox");
		paneInfo.CaptionVisible(true);

		m_toolboxPanel = new MapToolbox(m_dockArea, NewControlId());
		m_auiManager.AddPane(m_toolboxPanel, paneInfo);

		//Subscribe to toolbox buttons
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_SELECT);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_PAINT);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_STAMP);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_PICKER);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_FLIPX);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_FLIPY);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_FILL);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_CLONE);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_CREATESTAMP);
	}

	if(!m_toolboxPanel->IsShown())
	{
		m_toolboxPanel->Show();
	}
}

void MainWindow::SyncSettingsWidgets()
{
	if(m_project)
	{
		m_ribbonButtonBarGrid->ToggleButton(wxID_BTN_GRID_SHOW, m_project->GetShowGrid());
		m_ribbonButtonBarGrid->ToggleButton(wxID_BTN_GRID_SNAP, m_project->GetGridSnap());
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
			SetProject(NULL);

			//Set new project
			SetProject(project);
			
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
	ShowPanelToolbox();
}

void MainWindow::OnBtnToolsTiles( wxRibbonButtonBarEvent& event )
{
	ShowPanelTiles();
}

void MainWindow::OnBtnToolsPalettes( wxRibbonButtonBarEvent& event )
{
	ShowPanelPalettes();
}

void MainWindow::OnBtnGridShow(wxCommandEvent& event)
{
	if(m_project)
	{
		m_project->SetShowGrid(!m_project->GetShowGrid());
	}
}

void MainWindow::OnBtnGridSnap(wxCommandEvent& event)
{
	if(m_project)
	{
		m_project->SetGridSnap(!m_project->GetGridSnap());
	}
}

void MainWindow::OnBtnTool(wxCommandEvent& event)
{
	if(m_mapPanel)
	{
		switch(event.GetId())
		{
		case wxID_TOOL_SELECT:
			m_mapPanel->SetTool(MapPanel::eToolSelect);
			break;
		case wxID_TOOL_PAINT:
			m_mapPanel->SetTool(MapPanel::eToolPaintTile);
			break;
		case wxID_TOOL_STAMP:
			m_mapPanel->SetTool(MapPanel::eToolPaintStamp);
			break;
		case wxID_TOOL_PICKER:
			m_mapPanel->SetTool(MapPanel::eToolPicker);
			break;
		case wxID_TOOL_FLIPX:
			m_mapPanel->SetTool(MapPanel::eToolFlipX);
			break;
		case wxID_TOOL_FLIPY:
			m_mapPanel->SetTool(MapPanel::eToolFlipY);
			break;
		case wxID_TOOL_FILL:
			m_mapPanel->SetTool(MapPanel::eToolFill);
			break;
		case wxID_TOOL_CLONE:
			m_mapPanel->SetTool(MapPanel::eToolClone);
			break;
		case wxID_TOOL_CREATESTAMP:
			m_mapPanel->SetTool(MapPanel::eToolCreateStamp);
			break;
		}
	}
}