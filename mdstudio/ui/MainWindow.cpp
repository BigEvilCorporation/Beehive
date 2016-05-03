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
#include "Dialogs.h"
#include "PalettesPanel.h"
#include "TilesPanel.h"
#include "MapPanel.h"

wxDEFINE_SCOPED_PTR(Project, ProjectPtr)

MainWindow::MainWindow()
	: MainWindowBase(NULL)
{
	SetStatusText("BEEhive v0.11");

	//Setup panel docking manager
	m_auiManager.SetManagedWindow(m_dockArea);
	m_auiManager.SetFlags(	wxAUI_MGR_ALLOW_FLOATING			//Allow floating panels
							| wxAUI_MGR_TRANSPARENT_HINT		//Draw possible dock locations
							| wxAUI_MGR_TRANSPARENT_DRAG		//Transparent panel whilst dragging
							| wxAUI_MGR_LIVE_RESIZE);			//Refresh on each discreet resize

	//Bind events
	Bind(wxEVT_KEY_DOWN, &MainWindow::EventHandlerKeyboard, this, GetId());
	Bind(wxEVT_KEY_UP, &MainWindow::EventHandlerKeyboard, this, GetId());

	//Create blank OpenGL panel to create global DC
	wxGLCanvas* m_blankCanvas = new wxGLCanvas(this, wxID_ANY, NULL);

	//Create GL context
	m_context = new wxGLContext(m_blankCanvas);

	//Create renderer (from global DC
	m_renderer = ion::render::Renderer::Create(m_blankCanvas->GetHDC());

	//Create and load rendering resources
	m_renderResources = new RenderResources;

	//Create default project
	Project* defaultProject = new Project();

	//Open welcome project
	static bool openWelcomeProject = true;
	if(openWelcomeProject)
	{
		wxString directory = wxGetCwd();
		wxString path = directory + "\\projects\\welcome.bee";
		defaultProject->Load(path.c_str().AsChar());
	}

	//Set default project
	SetProject(defaultProject);

	SetFocus();
}

MainWindow::~MainWindow()
{
	//Close project and panels
	SetProject(NULL);

	//Delete rendering resources
	delete m_renderResources;

	//Delete renderer and OpenGL context
	delete m_renderer;
	delete m_context;
}

void MainWindow::EventHandlerKeyboard(wxKeyEvent& event)
{
	//Pass to map window
	if(m_mapPanel)
	{
		m_mapPanel->GetEventHandler()->ProcessEvent(event);
	}

	event.Skip();
}

void MainWindow::SetProject(Project* project)
{
	if(project != m_project.get())
	{
		//Changed project, close all panels
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

		if(m_tileEditorPanel)
		{
			m_auiManager.DetachPane(m_tileEditorPanel);
			delete m_tileEditorPanel;
		}

		if(m_terrainTilesPanel)
		{
			m_auiManager.DetachPane(m_terrainTilesPanel);
			delete m_terrainTilesPanel;
		}

		if(m_TerrainTileEditorPanel)
		{
			m_auiManager.DetachPane(m_TerrainTileEditorPanel);
			delete m_TerrainTileEditorPanel;
		}

		if(m_gameObjectTypePanel)
		{
			m_auiManager.DetachPane(m_gameObjectTypePanel);
			delete m_gameObjectTypePanel;
		}

		//Delete previous, set new
		m_project.reset(project);

		if(project)
		{
			//Set render resources project
			m_renderResources->SetProject(project);

			//Recreate tileset/collision set/sprite textures, and tile index cache
			RefreshTileset();
			RefreshTerrainTileset();
			RefreshSprites();

			//Open bottom panels
			ShowPanelPalettes();
			ShowPanelTiles();
			ShowPanelTerrainEditor();
			ShowPanelTerrainTiles();
			ShowPanelTileEditor();

			//Open left panels
			ShowPanelToolbox();

			//Open right panels
			ShowPanelStamps();

			//Open centre panels
			ShowPanelMap();

			//Sync settings widgets states
			SyncSettingsWidgets();
		}

		//Refresh whole window
		RefreshAll();
	}
}

void MainWindow::SetPanelCaptions()
{
	if(m_tilesPanel.get())
	{
		wxAuiPaneInfo& paneInfo = m_auiManager.GetPane(m_tilesPanel.get());
		wxString caption;
		caption << "Tileset " << "(" << m_project->GetTileset().GetCount() << ")";
		paneInfo.Caption(caption);
		m_auiManager.Update();
	}

	if(m_terrainTilesPanel.get())
	{
		wxAuiPaneInfo& paneInfo = m_auiManager.GetPane(m_terrainTilesPanel.get());
		wxString caption;
		caption << "Terrain Tileset " << "(" << m_project->GetTerrainTileset().GetCount() << ")";
		paneInfo.Caption(caption);
		m_auiManager.Update();
	}
}

void MainWindow::ShowPanelPalettes()
{
	if(m_project.get())
	{
		if(!m_palettesPanel)
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(50, 300);
			paneInfo.Bottom();
			paneInfo.Caption("Palettes");
			paneInfo.CaptionVisible(true);

			m_palettesPanel = new PalettesPanel(this, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_palettesPanel, paneInfo);
			m_palettesPanel->Show();

			m_palettesPanel->SetProject(m_project.get());
		}

		if(!m_palettesPanel->IsShown())
		{
			m_palettesPanel->Show();
		}
	}
}

void MainWindow::ShowPanelTiles()
{
	if(m_project.get())
	{
		if(!m_tilesPanel)
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(450, 300);
			paneInfo.Bottom();
			paneInfo.Caption("Tileset");
			paneInfo.CaptionVisible(true);
			
			m_tilesPanel = new TilesPanel(this, *m_renderer, m_context, *m_renderResources, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_tilesPanel, paneInfo);
			m_tilesPanel->Show();

			m_tilesPanel->SetProject(m_project.get());
		}

		if(!m_tilesPanel->IsShown())
		{
			m_tilesPanel->Show();
		}

		SetPanelCaptions();
	}
}

void MainWindow::ShowPanelTerrainTiles()
{
	if(m_project.get())
	{
		if(!m_terrainTilesPanel)
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(300, 300);
			paneInfo.Bottom();
			paneInfo.Caption("Tileset");
			paneInfo.CaptionVisible(true);

			m_terrainTilesPanel = new TerrainTilesPanel(this, *m_renderer, m_context, *m_renderResources, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_terrainTilesPanel, paneInfo);
			m_terrainTilesPanel->Show();

			m_terrainTilesPanel->SetProject(m_project.get());
		}

		if(!m_terrainTilesPanel->IsShown())
		{
			m_terrainTilesPanel->Show();
		}

		SetPanelCaptions();
	}
}

void MainWindow::ShowPanelStamps()
{
	if(m_project.get())
	{
		if(!m_stampsPanel)
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(300, 800);
			paneInfo.Right();
			paneInfo.Row(0);
			paneInfo.Caption("Stamps");
			paneInfo.CaptionVisible(true);

			m_stampsPanel = new StampsPanel(this, *m_renderer, m_context, *m_renderResources, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_stampsPanel, paneInfo);
			m_stampsPanel->Show();

			m_stampsPanel->SetProject(m_project.get());
		}

		if(!m_stampsPanel->IsShown())
		{
			m_stampsPanel->Show();
		}
	}
}

void MainWindow::ShowPanelMap()
{
	if(m_project.get())
	{
		if(!m_mapPanel)
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(800, 600);
			paneInfo.Centre();
			paneInfo.Caption("Map");
			paneInfo.CaptionVisible(true);

			m_mapPanel = new MapPanel(this, *m_renderer, m_context, *m_renderResources, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_mapPanel, paneInfo);
			m_mapPanel->Show();

			m_mapPanel->SetProject(m_project.get());
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
		paneInfo.BestSize(100, 300);
		paneInfo.Left();
		paneInfo.Row(0);
		paneInfo.Caption("Toolbox");
		paneInfo.CaptionVisible(true);

		m_toolboxPanel = new MapToolbox(m_dockArea, NewControlId());
		m_auiManager.AddPane(m_toolboxPanel, paneInfo);

		//Subscribe to toolbox buttons
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_SELECTTILE);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_SELECTSTAMP);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_PAINT);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_STAMP);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_TILEPICKER);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_STAMPPICKER);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_FLIPX);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_FLIPY);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_FILL);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_CLONE);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_CREATESTAMP);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_REMOVESTAMP);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_COL_PAINTTERRAIN);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_COL_PAINTSOLID);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_COL_DELETETERRTILE);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_COL_ADDTERRAINBEZIER);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_COL_EDITTERRAINBEZIER);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_SELECTGAMEOBJ);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_PLACEGAMEOBJ);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_REMOVEGAMEOBJ);
	}

	if(!m_toolboxPanel->IsShown())
	{
		m_toolboxPanel->Show();
	}
}

void MainWindow::ShowPanelTileEditor()
{
	if(m_project.get())
	{
		if(!m_tileEditorPanel)
		{
			wxSize clientSize = GetClientSize();

			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(300, 300);
			paneInfo.Bottom();
			paneInfo.Caption("Tile");
			paneInfo.CaptionVisible(true);

			m_tileEditorPanel = new TileEditorPanel(this, *m_renderer, m_context, *m_renderResources, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_tileEditorPanel, paneInfo);
			m_tileEditorPanel->Show();
			m_auiManager.Update();

			m_tileEditorPanel->SetProject(m_project.get());
		}

		if(!m_tileEditorPanel->IsShown())
		{
			m_tileEditorPanel->Show();
		}
	}
}

void MainWindow::ShowPanelTerrainEditor()
{
	if(m_project.get())
	{
		if(!m_TerrainTileEditorPanel)
		{
			wxSize clientSize = GetClientSize();

			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(300, 300);
			paneInfo.Bottom();
			paneInfo.Caption("Collision Tile");
			paneInfo.CaptionVisible(true);

			m_TerrainTileEditorPanel = new TerrainTileEditorPanel(this, *m_renderer, m_context, *m_renderResources, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_TerrainTileEditorPanel, paneInfo);
			m_TerrainTileEditorPanel->Show();
			m_auiManager.Update();

			m_TerrainTileEditorPanel->SetProject(m_project.get());
		}

		if(!m_TerrainTileEditorPanel->IsShown())
		{
			m_TerrainTileEditorPanel->Show();
		}
	}
}

void MainWindow::ShowPanelGameObjectTypes()
{
	if(m_project.get())
	{
		if(!m_gameObjectTypePanel)
		{
			wxSize clientSize = GetClientSize();

			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.Float();
			paneInfo.DockFixed(false);
			paneInfo.BestSize(300, 300);
			paneInfo.Bottom();
			paneInfo.Caption("Game Object Types");
			paneInfo.CaptionVisible(true);

			m_gameObjectTypePanel = new GameObjectTypesPanel(this, *m_project.get(), m_dockArea, NewControlId());
			m_auiManager.AddPane(m_gameObjectTypePanel, paneInfo);
			m_gameObjectTypePanel->Show();
			m_auiManager.Update();
		}

		if(!m_gameObjectTypePanel->IsShown())
		{
			m_gameObjectTypePanel->Show();
		}
	}
}

void MainWindow::ShowPanelGameObjectParams()
{
	if(m_project.get())
	{
		if(!m_gameObjectParamsPanel)
		{
			wxSize clientSize = GetClientSize();

			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.Float();
			paneInfo.DockFixed(false);
			paneInfo.BestSize(300, 300);
			paneInfo.Bottom();
			paneInfo.Caption("Game Object Params");
			paneInfo.CaptionVisible(true);

			m_gameObjectParamsPanel = new GameObjectParamsPanel(*this, *m_project.get(), m_dockArea, NewControlId());
			m_auiManager.AddPane(m_gameObjectParamsPanel, paneInfo);
			m_gameObjectParamsPanel->Show();
			m_auiManager.Update();
		}

		if(!m_gameObjectParamsPanel->IsShown())
		{
			m_gameObjectParamsPanel->Show();
		}
	}
}

void MainWindow::SetMapTool(ToolType tool)
{
	if(m_mapPanel)
	{
		m_mapPanel->SetTool(tool);
	}
}

void MainWindow::SetSelectedGameObject(GameObject* gameObject)
{
	if(m_gameObjectParamsPanel)
	{
		m_gameObjectParamsPanel->SetGameObject(gameObject);
	}
}

void MainWindow::SyncSettingsWidgets()
{
	if(m_project.get())
	{
		m_ribbonButtonBarGrid->ToggleButton(wxID_BTN_GRID_SHOW, m_project->GetShowGrid());
		m_ribbonButtonBarGrid->ToggleButton(wxID_BTN_GRID_SNAP, m_project->GetGridSnap());
		m_ribbonButtonBarGrid->ToggleButton(wxID_BTN_SHOW_OUTLINES, m_project->GetShowStampOutlines());
	}
}

void MainWindow::RefreshAll()
{
	m_auiManager.Update();
	Refresh();

	if(m_project.get())
	{
		m_project->InvalidateMap(true);
		m_project->InvalidateTiles(true);
		m_project->InvalidateTerrainTiles(true);
		m_project->InvalidateTerrainBeziers(true);
		m_project->InvalidateStamps(true);
	}

	RedrawAll();

	if(m_project.get())
	{
		m_project->InvalidateMap(false);
		m_project->InvalidateTiles(false);
		m_project->InvalidateTerrainTiles(false);
		m_project->InvalidateTerrainBeziers(false);
		m_project->InvalidateStamps(false);
	}
}

void MainWindow::RedrawAll()
{
	RefreshTileset();
	RefreshTerrainTileset();
	SetPanelCaptions();

	if(m_palettesPanel)
		m_palettesPanel->Refresh();

	if(m_tilesPanel)
		m_tilesPanel->Refresh();

	if(m_mapPanel)
		m_mapPanel->Refresh();

	if(m_stampsPanel)
		m_stampsPanel->Refresh();

	if(m_tileEditorPanel)
		m_tileEditorPanel->Refresh();

	if(m_terrainTilesPanel)
		m_terrainTilesPanel->Refresh();

	if(m_TerrainTileEditorPanel)
		m_TerrainTileEditorPanel->Refresh();

	if(m_gameObjectTypePanel)
		m_gameObjectTypePanel->Refresh();
}

void MainWindow::RefreshTileset()
{
	if(m_project.get())
	{
		//Recreate tileset texture
		m_renderResources->CreateTilesetTexture();
	}
}

void MainWindow::RefreshTerrainTileset()
{
	if(m_project.get())
	{
		//Recreate collision set texture
		m_renderResources->CreateCollisionTypesTexture();
		m_renderResources->CreateTerrainTilesTexture();
	}
}

void MainWindow::RefreshSprites()
{
	if(m_project.get())
	{
		m_renderResources->CreateSpriteResources(*m_project.get());
	}
}

void MainWindow::RefreshPanel(Panel panel)
{
	if(m_project.get())
	{
		m_project->InvalidateMap(true);
		m_project->InvalidateTiles(true);
		m_project->InvalidateTerrainTiles(true);
		m_project->InvalidateTerrainBeziers(true);
		m_project->InvalidateStamps(true);

	}
	
	RedrawPanel(panel);
	SetPanelCaptions();

	if(m_project.get())
	{
		m_project->InvalidateMap(false);
		m_project->InvalidateTiles(false);
		m_project->InvalidateTerrainTiles(false);
		m_project->InvalidateTerrainBeziers(false);
		m_project->InvalidateStamps(false);
	}
}

void MainWindow::RedrawPanel(Panel panel)
{
	switch(panel)
	{
	case ePanelMap:
		if(m_mapPanel)
			m_mapPanel->Refresh();
		break;
	case ePanelStamps:
		if(m_stampsPanel)
			m_stampsPanel->Refresh();
		break;
	case ePanelTiles:
		if(m_tilesPanel)
			m_tilesPanel->Refresh();
		break;
	case ePanelPalettes:
		if(m_palettesPanel)
			m_palettesPanel->Refresh();
		break;
	case ePanelTerrainTiles:
		if(m_terrainTilesPanel)
			m_terrainTilesPanel->Refresh();
		break;
	case ePanelTileEditor:
		if(m_tileEditorPanel)
			m_tileEditorPanel->Refresh();
		break;
	case ePanelTerrainTileEditor:
		if(m_TerrainTileEditorPanel)
			m_TerrainTileEditorPanel->Refresh();
		break;
	case ePanelGameObjectTypes:
		if(m_gameObjectTypePanel)
			m_gameObjectTypePanel->Refresh();
	}
}

void MainWindow::OnBtnProjNew(wxRibbonButtonBarEvent& event)
{
	if(wxMessageBox("Unsaved changes will be lost, are you sure?", "New Project", wxOK | wxCANCEL) == wxOK)
	{
		SetProject(new Project());
	}
}

void MainWindow::OnBtnProjOpen(wxRibbonButtonBarEvent& event)
{
	wxFileDialog dialog(this, _("Open BEE file"), "", "", "BEE files (*.bee)|*.bee", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(dialog.ShowModal() == wxID_OK)
	{
		SetStatusText("Opening...");

		Project* project = new Project();

		if(project->Load(dialog.GetPath().c_str().AsChar()))
		{
			//Set new project, deletes old
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
	if(m_project.get())
	{
		std::string filename = m_project->GetFilename();

		if(!filename.size())
		{
			//No previous filename
			wxFileDialog dialog(this, _("Open BEE file"), "", "", "BEE files (*.bee)|*.bee", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
			if(dialog.ShowModal() == wxID_OK)
			{
				filename = dialog.GetPath().c_str().AsChar();
			}
		}
		
		if(filename.size())
		{
			//Save
			SetStatusText("Saving...");

			if(m_project->Save(filename))
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
	if(m_project.get())
	{
		ExportDialog dialog(this);

		dialog.m_txtProjectName->SetValue(m_project->GetName());
		dialog.m_filePickerPalettes->SetPath(m_project->m_exportFilenames.palettes);
		dialog.m_filePickerTileset->SetPath(m_project->m_exportFilenames.tileset);
		dialog.m_filePickerMap->SetPath(m_project->m_exportFilenames.map);
		dialog.m_filePickerTerrainTiles->SetPath(m_project->m_exportFilenames.TerrainTiles);
		dialog.m_filePickerCollisionMap->SetPath(m_project->m_exportFilenames.collisionMap);
		dialog.m_filePickerGameObj->SetPath(m_project->m_exportFilenames.gameObjects);

		if(dialog.ShowModal() == wxID_OK)
		{
			SetStatusText("Exporting...");

			m_project->SetName(std::string(dialog.m_txtProjectName->GetValue()));
			m_project->m_exportFilenames.palettes = dialog.m_filePickerPalettes->GetPath();
			m_project->m_exportFilenames.tileset = dialog.m_filePickerTileset->GetPath();
			m_project->m_exportFilenames.map = dialog.m_filePickerMap->GetPath();
			m_project->m_exportFilenames.TerrainTiles = dialog.m_filePickerTerrainTiles->GetPath();
			m_project->m_exportFilenames.collisionMap = dialog.m_filePickerCollisionMap->GetPath();
			m_project->m_exportFilenames.gameObjects = dialog.m_filePickerGameObj->GetPath();

if(dialog.m_chkPalettes->GetValue())
m_project->ExportPalettes(m_project->m_exportFilenames.palettes);

if(dialog.m_chkTileset->GetValue())
m_project->ExportTiles(m_project->m_exportFilenames.tileset, dialog.m_btnBinary->GetValue());

if(dialog.m_chkMap->GetValue())
m_project->ExportMap(m_project->m_exportFilenames.map, dialog.m_btnBinary->GetValue());

if(dialog.m_chkTerrainTiles->GetValue())
m_project->ExportTerrainTiles(m_project->m_exportFilenames.TerrainTiles, dialog.m_btnBinary->GetValue());

if(dialog.m_chkCollisionMap->GetValue())
m_project->ExportCollisionMap(m_project->m_exportFilenames.collisionMap, dialog.m_btnBinary->GetValue());

if(dialog.m_chkGameObj->GetValue())
m_project->ExportGameObjects(m_project->m_exportFilenames.gameObjects);

SetStatusText("Export complete");
wxMessageBox("Export complete", "Error", wxOK | wxICON_INFORMATION);
		}
	}
}

void MainWindow::OnBtnTilesImport(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		ImportDialog dialog(this);
		if(dialog.ShowModal() == wxID_OK)
		{
			SetStatusText("Importing...");

			u32 flags = 0;

			if(dialog.m_chkImportToStamp->GetValue())
				flags |= Project::eBMPImportToStamp;
			if(dialog.m_chkImportToSprite->GetValue())
				flags |= Project::eBMPImportToSprite;
			if(dialog.m_chkInsertBGTile->GetValue())
				flags |= Project::eBMPImportInsertBGTile;

			//Unsupported flags if multiple files selected
			if(dialog.m_paths.size() > 0)
			{
				if(dialog.m_chkImportPalette->GetValue())
					flags |= Project::eBMPImportWholePalette;
				if(dialog.m_chkClearMap->GetValue())
					flags |= Project::eBMPImportClearMap;
				if(dialog.m_chkPaintToMap->GetValue())
					flags |= Project::eBMPImportDrawToMap;
				if(dialog.m_chkClearPalettes->GetValue())
					flags |= Project::eBMPImportClearPalettes;
				if(dialog.m_chkClearTiles->GetValue())
					flags |= Project::eBMPImportClearTiles;
			}

			u32 palettes = 0;

			if(dialog.m_chkPalette1->GetValue())
				palettes |= (1 << 0);
			if(dialog.m_chkPalette2->GetValue())
				palettes |= (1 << 1);
			if(dialog.m_chkPalette3->GetValue())
				palettes |= (1 << 2);
			if(dialog.m_chkPalette4->GetValue())
				palettes |= (1 << 3);

			for(int i = 0; i < dialog.m_paths.size(); i++)
			{
				m_project->ImportBitmap(dialog.m_paths[i].c_str().AsChar(), flags, palettes);
			}

			//Refresh tileset
			RefreshTileset();

			//Refresh collison tileset
			RefreshTerrainTileset();

			//Refresh whole application
			RefreshAll();

			SetStatusText("Import complete");
		}
	}
}

void MainWindow::OnBtnSpritesImport(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		ImportDialogSprite dialog(this, *m_renderer, *m_context, *m_renderResources);
		if(dialog.ShowModal() == wxID_OK)
		{
			//Create new sprite
			SpriteId spriteId = m_project->CreateSprite();
			Sprite* sprite = m_project->GetSprite(spriteId);

			//Import bitmap
			if(sprite->ImportBitmap(dialog.m_filePicker->GetPath().GetData().AsChar(), dialog.m_textName->GetValue().GetData().AsChar(), dialog.m_spinWidthCells->GetValue(), dialog.m_spinHeightCells->GetValue(), dialog.m_spinCellCount->GetValue()))
			{
				//Create render resources
				m_renderResources->CreateSpriteResources(spriteId, *sprite);

				wxMessageBox("Sprite imported successfully", "Success", wxOK);
			}
			else
			{
				//Failed, remove sprite
				wxMessageBox("Error importing sprite", "Error", wxOK);
				m_project->DeleteSprite(spriteId);
			}
		}
	}
}

void MainWindow::OnBtnTilesCreate(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		//Add new tile
		TileId tileId = m_project->GetTileset().AddTile();

		//Recreate tileset texture
		RefreshTileset();

		//Set as current paint tile
		m_project->SetPaintTile(tileId);

		//Set paint tool
		SetMapTool(eToolPaintTile);

		//Refresh tiles and tile editor panels
		RefreshPanel(ePanelTiles);
		RefreshPanel(ePanelTileEditor);
	}
}

void MainWindow::OnBtnTilesDelete(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		TileId tileId = m_project->GetPaintTile();
		if(tileId != InvalidTileId)
		{
			//Delete tile
			m_project->DeleteTile(tileId);

			//Recreate tileset texture
			RefreshTileset();

			//Revert to select tool
			SetMapTool(eToolSelectTiles);

			//Refresh
			RefreshAll();
		}
	}
}

void MainWindow::OnBtnTilesCleanup(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		if(m_project->CleanupTiles())
		{
			RefreshAll();
		}
	}
}

void MainWindow::OnBtnColMapClear(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		m_project->GetCollisionMap().Clear();
		RefreshAll();
	}
}

void MainWindow::OnBtnColGenTerrainBezier(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		DialogTerrainGen dialog(this);

		if(dialog.ShowModal() == wxID_OK)
		{
			int granularity = dialog.m_spinCtrlGranularity->GetValue();

			if(wxMessageBox("This will clear all terrain tiles, are you sure?", "Generate Terrain", wxOK | wxCANCEL) == wxOK)
			{
				m_project->GenerateTerrainFromBeziers(granularity);
				RefreshAll();
			}
		}
	}
}

void MainWindow::OnBtnColTilesCleanup(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		if(m_project->CleanupTerrainTiles(true))
		{
			RefreshAll();
		}
	}
}

void MainWindow::OnBtnColTilesCreate(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		//Add new collision tile
		TerrainTileId tileId = m_project->GetTerrainTileset().AddTerrainTile();

		//Recreate tileset texture
		RefreshTerrainTileset();

		//Set as current paint collision tile
		m_project->SetPaintTerrainTile(tileId);

		//Set paint tool
		SetMapTool(eToolPaintTerrainTile);

		//Refresh collision tiles and collision tile editor panels
		RefreshPanel(ePanelTerrainTiles);
		RefreshPanel(ePanelTerrainTileEditor);
	}
}

void MainWindow::OnBtnColTilesDelete(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		TerrainTileId tileId = m_project->GetPaintTerrainTile();
		if(tileId != InvalidTerrainTileId && tileId != m_project->GetDefaultTerrainTile())
		{
			CollisionMap& map = m_project->GetCollisionMap();
			TerrainTileset& tileset = m_project->GetTerrainTileset();

			//Delete tile
			m_project->DeleteTerrainTile(tileId);

			//Recreate tileset texture
			RefreshTerrainTileset();

			//Revert to select tool
			SetMapTool(eToolSelectTiles);

			//Refresh
			RefreshAll();
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

void MainWindow::OnBtnToolsGameObjs(wxRibbonButtonBarEvent& event)
{
	ShowPanelGameObjectTypes();
}

void MainWindow::OnBtnToolsGameObjParams(wxRibbonButtonBarEvent& event)
{
	ShowPanelGameObjectParams();
}

void MainWindow::OnBtnMapClear(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		m_project->GetMap().Clear();
		m_project->GetCollisionMap().Clear();
		RefreshPanel(ePanelMap);
	}
}

void MainWindow::OnBtnMapResize(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		Map& map = m_project->GetMap();
		CollisionMap& collisionMap = m_project->GetCollisionMap();

		int originalWidth = map.GetWidth();
		int originalHeight = map.GetHeight();

		DialogMapSize dialog(this);
		dialog.m_spinCtrlWidth->SetValue(originalWidth);
		dialog.m_spinCtrlHeight->SetValue(originalHeight);

		if(dialog.ShowModal() == wxID_OK)
		{
			int width = dialog.m_spinCtrlWidth->GetValue();
			int height = dialog.m_spinCtrlHeight->GetValue();

			if(width > 0 && width <= 10000 && height > 0 && height <= 10000)
			{
				//Resize map
				map.Resize(width, height);
				collisionMap.Resize(width, height);

				//Refresh map panel
				RefreshPanel(ePanelMap);
			}
		}
	}
}

void MainWindow::OnBtnGridShow(wxCommandEvent& event)
{
	if(m_project.get())
	{
		m_project->SetShowGrid(!m_project->GetShowGrid());
		RefreshPanel(ePanelMap);
		RefreshPanel(ePanelStamps);
		RefreshPanel(ePanelTileEditor);
	}
}

void MainWindow::OnBtnGridSnap(wxCommandEvent& event)
{
	if(m_project.get())
	{
		m_project->SetGridSnap(!m_project->GetGridSnap());
	}
}

void MainWindow::OnBtnShowOutlines(wxCommandEvent& event)
{
	if(m_project.get())
	{
		m_project->SetShowStampOutlines(!m_project->GetShowStampOutlines());
		RefreshPanel(ePanelMap);
		RefreshPanel(ePanelStamps);
	}
}

void MainWindow::OnBtnTerrainTileEdit(wxRibbonButtonBarEvent& event)
{
	ShowPanelTerrainEditor();
}

void MainWindow::OnBtnGameObjTypes(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		GameObjectTypeDialog dialog(*this, *m_project);
		dialog.ShowModal();
	}
}

void MainWindow::OnBtnTool(wxCommandEvent& event)
{
	if(m_mapPanel)
	{
		switch(event.GetId())
		{
		case wxID_TOOL_SELECTTILE:
			m_mapPanel->SetTool(eToolSelectTiles);
			break;
		case wxID_TOOL_SELECTSTAMP:
			m_mapPanel->SetTool(eToolSelectStamp);
			break;
		case wxID_TOOL_PAINT:
			m_mapPanel->SetTool(eToolPaintTile);
			break;
		case wxID_TOOL_STAMP:
			m_mapPanel->SetTool(eToolPaintStamp);
			break;
		case wxID_TOOL_COL_PAINTTERRAIN:
			m_mapPanel->SetTool(eToolPaintCollisionTerrain);
			break;
		case wxID_TOOL_COL_PAINTSOLID:
			m_mapPanel->SetTool(eToolPaintCollisionSolid);
			break;
		case wxID_TOOL_COL_DELETETERRTILE:
			m_mapPanel->SetTool(eToolDeleteTerrainTile);
			break;
		case wxID_TOOL_COL_ADDTERRAINBEZIER:
			m_mapPanel->SetTool(eToolDrawTerrainBezier);
			break;
		case wxID_TOOL_COL_EDITTERRAINBEZIER:
			m_mapPanel->SetTool(eToolSelectTerrainBezier);
			break;
		case wxID_TOOL_TILEPICKER:
			m_mapPanel->SetTool(eToolTilePicker);
			break;
		case wxID_TOOL_STAMPPICKER:
			m_mapPanel->SetTool(eToolStampPicker);
			break;
		case wxID_TOOL_FLIPX:
			m_mapPanel->SetTool(eToolFlipX);
			break;
		case wxID_TOOL_FLIPY:
			m_mapPanel->SetTool(eToolFlipY);
			break;
		case wxID_TOOL_FILL:
			m_mapPanel->SetTool(eToolFill);
			break;
		case wxID_TOOL_CLONE:
			m_mapPanel->SetTool(eToolClone);
			break;
		case wxID_TOOL_CREATESTAMP:
			m_mapPanel->SetTool(eToolCreateStamp);
			break;
		case wxID_TOOL_REMOVESTAMP:
			m_mapPanel->SetTool(eToolRemoveStamp);
			break;
		case wxID_TOOL_SELECTGAMEOBJ:
			m_mapPanel->SetTool(eToolSelectGameObject);
			break;
		case wxID_TOOL_PLACEGAMEOBJ:
			m_mapPanel->SetTool(eToolPlaceGameObject);
			break;
		case wxID_TOOL_REMOVEGAMEOBJ:
			m_mapPanel->SetTool(eToolRemoveGameObject);
			break;
		}
	}
}