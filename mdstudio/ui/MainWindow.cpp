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

#pragma once

#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/dir.h>
#include <wx/aui/framemanager.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>

#include <string>

#include "../buildconf.h"
#include "MainWindow.h"
#include "Dialogs.h"
#include "ExportDialog.h"
#include "SpriteAnimEditorDialog.h"
#include "ProjectSettingsDialog.h"
#include "PalettesPanel.h"
#include "TilesPanel.h"
#include "MapPanel.h"

#include "maths\Maths.h"

#if defined BEEHIVE_PLUGIN_LUMINARY
#include <luminary/Types.h>
#include <luminary/Tags.h>
#include <luminary/EntityParser.h>
#include <luminary/SceneExporter.h>
#include <luminary/SpriteExporter.h>
#include <luminary/TilesetExporter.h>
#include <luminary/MapExporter.h>
#include <luminary/PaletteExporter.h>
#include <luminary/TerrainExporter.h>
#endif

wxDEFINE_SCOPED_PTR(Project, ProjectPtr)

#define PANEL_SIZE_X(x) (m_width / 100) * x
#define PANEL_SIZE_Y(y) (m_height / 100) * y

MainWindow::MainWindow()
	: MainWindowBase(NULL)
{
	SetStatusText("Beehive Studio v0.5");

	m_refreshLockStack = 0;

	//Lock refresh during init
	LockRefresh();

	//Create resource manager and set resource directories
	m_resourceManager = new ion::io::ResourceManager();
	m_resourceManager->SetResourceDirectory<ion::render::Shader>("shaders", ".ion.shader");
	m_resourceManager->SetResourceDirectory<ion::render::Texture>("textures", ".ion.texture");

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
	wxGLAttributes dispAttrs;
	dispAttrs.PlatformDefaults().Defaults().EndList();
	m_blankCanvas = new wxGLCanvas(this, dispAttrs, wxID_ANY);

	//Create GL context
	m_context = new wxGLContext(m_blankCanvas);

	if(!m_context->IsOK())
	{
		ion::debug::Error("MainWindow::MainWindow() - Invalid OpenGL context created");
	}

	//Create renderer (from global DC
	m_renderer = ion::render::Renderer::Create(m_blankCanvas->GetHDC(), m_context->GetGLRC());

	//Create default Mega Drive project
	Project* defaultProject = new Project(PlatformPresets::s_configs[PlatformPresets::ePresetMegaDrive]);

	//Create and load rendering resources
	m_renderResources = new RenderResources(*defaultProject, *m_resourceManager);

	//Open welcome project
	static bool openWelcomeProject = true;
	if(openWelcomeProject)
	{
		wxString directory = wxGetCwd();
		wxString path = directory + "\\projects\\welcome.bee";
		defaultProject->Load(path.c_str().AsChar());
	}

	//Maximize
	Maximize();

	//Get window size
	GetSize(&m_width, &m_height);

	//Set default project
	SetProject(defaultProject);

	//Set window focus
	SetFocus();

	//Unlock refresh (and refreshes all)
	UnlockRefresh();
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

	//Delete resource manager
	delete m_resourceManager;
}

void MainWindow::OnPostInit()
{
#if BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
	delete m_ribbonPanelTiles;
	m_ribbonBar->Realise();
	delete m_ribbonPanelCollision;
	m_ribbonBar->Realise();
	m_ribbonButtonBarStamps->DeleteButton(wxID_BTN_STAMPS_CREATE);
	m_ribbonBar->Realise();
	m_ribbonButtonBarGrid->DeleteButton(wxID_BTN_GRID_SNAP);
	m_ribbonBar->Realise();
#endif

#if BEEHIVE_LEAN_UI
	m_ribbonButtonBarStamps->DeleteButton(wxID_BTN_STAMPS_DELETE);
#endif

#if BEEHIVE_FIXED_STAMP_MODE || BEEHIVE_LEAN_UI
	m_ribbonBar->Realise();
#endif
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
	//Lock refresh
	LockRefresh();

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

		if(m_mapListPanel)
		{
			m_auiManager.DetachPane(m_mapListPanel);
			delete m_mapListPanel;
		}

		if(m_toolboxPanelTiles)
		{
			m_auiManager.DetachPane(m_toolboxPanelTiles);
			delete m_toolboxPanelTiles;
		}

		if(m_toolboxPanelStamps)
		{
			m_auiManager.DetachPane(m_toolboxPanelStamps);
			delete m_toolboxPanelStamps;
		}

		if(m_toolboxPanelTerrain)
		{
			m_auiManager.DetachPane(m_toolboxPanelTerrain);
			delete m_toolboxPanelTerrain;
		}

		if(m_toolboxPanelGameObjs)
		{
			m_auiManager.DetachPane(m_toolboxPanelGameObjs);
			delete m_toolboxPanelGameObjs;
		}

		if(m_stampsPanel)
		{
			m_auiManager.DetachPane(m_stampsPanel);
			delete m_stampsPanel;
		}

		if(m_blocksPanel)
		{
			m_auiManager.DetachPane(m_blocksPanel);
			delete m_blocksPanel;
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

		if(m_gameObjectParamsPanel)
		{
			m_auiManager.DetachPane(m_gameObjectParamsPanel);
			delete m_gameObjectParamsPanel;
		}

		if(m_timelinePanel)
		{
			m_auiManager.DetachPane(m_timelinePanel);
			delete m_timelinePanel;
		}

		if(m_renderResources)
		{
			delete m_renderResources;
			m_renderResources = NULL;
		}

		//Delete previous, set new
		m_project.reset(project);

		if(project)
		{
			//Create render resources
			m_renderResources = new RenderResources(*m_project, *m_resourceManager);

			//Recreate tileset/collision set/spriteSheet textures, and tile index cache
			RefreshTileset();
			RefreshTerrainTileset();
			RefreshSpriteSheets();

#if !BEEHIVE_LEAN_UI
			//Open bottom panels
			ShowPanelPalettes();
			ShowPanelMapList();
#endif

#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
			ShowPanelTiles();
			ShowPanelTerrainEditor();
			ShowPanelTerrainTiles();
			ShowPanelTileEditor();
#endif

			//Open left panels
			ShowToolboxStamps();
			ShowToolboxObjects();

#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
			ShowToolboxTiles();
			ShowToolboxCollision();
#endif

			//Open right panels
			ShowPanelStamps();

			//Open centre panels
			ShowPanelMap();

			//Sync settings widgets states
			SyncSettingsWidgets();

			//Restore window layout
			RestoreWindowLayout();
		}
	}

	//Unlock refresh
	UnlockRefresh();
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

	if(m_blocksPanel.get())
	{
		wxAuiPaneInfo& paneInfo = m_auiManager.GetPane(m_blocksPanel.get());
		wxString caption;
		caption << "Blocks " << "(" << m_blocksPanel->GetUniqueBlockCount() << ")";
		paneInfo.Caption(caption);
		m_auiManager.Update();
	}
}

void MainWindow::SaveWindowLayout()
{
	wxFileName filename(GetWindowLayoutConfig());
	filename.Mkdir(511, wxPATH_MKDIR_FULL);
	
	ion::io::File file;
	if(file.Open(filename.GetFullPath().c_str().AsChar(), ion::io::File::eOpenWrite))
	{
		wxString data = m_auiManager.SavePerspective();
		file.Write(data.c_str().AsChar(), data.size());
		file.Close();
	}
}

void MainWindow::RestoreWindowLayout()
{
	wxFileName filename(GetWindowLayoutConfig());

	ion::io::File file;
	if(file.Open(filename.GetFullPath().c_str().AsChar(), ion::io::File::eOpenRead))
	{
		char* data = new char[file.GetSize() + 1];
		file.Read(data, file.GetSize());
		data[file.GetSize()] = 0;
		m_auiManager.LoadPerspective(data);
		file.Close();
		delete data;
	}
}

wxString MainWindow::GetWindowLayoutConfig() const
{
	return wxStandardPaths::Get().GetUserConfigDir() + "/BigEvilCorporation/Beehive/layout.cfg";
}

void MainWindow::ShowPanelPalettes()
{
	if(m_project.get())
	{
		if (m_palettesPanel)
		{
			m_auiManager.GetPane("Palettes").Show();
		}
		else
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Name("Palettes");
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(PANEL_SIZE_X(10), PANEL_SIZE_Y(30));
			paneInfo.Bottom();
			paneInfo.Caption("Palettes");
			paneInfo.CaptionVisible(true);

			m_palettesPanel = new PalettesPanel(this, *m_project, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_palettesPanel, paneInfo);
			paneInfo.Show();
		}

		m_auiManager.Update();
	}
}

void MainWindow::ShowPanelTiles()
{
	if(m_project.get())
	{
		if (m_tilesPanel)
		{
			m_auiManager.GetPane("Tiles").Show();
		}
		else
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Name("Tiles");
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(PANEL_SIZE_X(45), PANEL_SIZE_Y(30));
			paneInfo.Bottom();
			paneInfo.Caption("Tileset");
			paneInfo.CaptionVisible(true);
			
			m_tilesPanel = new TilesPanel(this, *m_project, *m_renderer, m_context, *m_renderResources, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_tilesPanel, paneInfo);
			paneInfo.Show();
		}

		m_auiManager.Update();

		SetPanelCaptions();
	}
}

void MainWindow::ShowPanelTerrainTiles()
{
	if(m_project.get())
	{
		if (m_terrainTilesPanel)
		{
			m_auiManager.GetPane("TerrainTiles").Show();
		}
		else
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Name("TerrainTiles");
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(PANEL_SIZE_X(30), PANEL_SIZE_Y(30));
			paneInfo.Bottom();
			paneInfo.Caption("Tileset");
			paneInfo.CaptionVisible(true);

			m_terrainTilesPanel = new TerrainTilesPanel(this, *m_project, *m_renderer, m_context, *m_renderResources, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_terrainTilesPanel, paneInfo);
			paneInfo.Show();
		}

		m_auiManager.Update();

		SetPanelCaptions();
	}
}

void MainWindow::ShowPanelStamps()
{
	if(m_project.get())
	{
		if (m_stampsPanel)
		{
			m_auiManager.GetPane("Stamps").Show();
		}
		else
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Name("Stamps");
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(PANEL_SIZE_X(20), PANEL_SIZE_Y(70));
			paneInfo.Right();
			paneInfo.Row(1);
			paneInfo.Caption("Stamps");
			paneInfo.CaptionVisible(true);

			m_stampsPanel = new StampsPanel(this, *m_project, *m_renderer, m_context, *m_renderResources, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_stampsPanel, paneInfo);
			paneInfo.Show();
		}

		m_auiManager.Update();
	}
}

void MainWindow::ShowPanelBlocks()
{
	if(m_project.get())
	{
		if (m_blocksPanel)
		{
			m_auiManager.GetPane("Blocks").Show();
		}
		else
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Name("Blocks");
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(PANEL_SIZE_X(20), PANEL_SIZE_Y(70));
			paneInfo.Right();
			paneInfo.Row(1);
			paneInfo.Caption("Blocks");
			paneInfo.CaptionVisible(true);

			m_blocksPanel = new BlocksPanel(this, *m_project, *m_renderer, m_context, *m_renderResources, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_blocksPanel, paneInfo);
			paneInfo.Show();
		}

		m_auiManager.Update();
	}
}

void MainWindow::ShowPanelMap()
{
	if(m_project.get())
	{
		if (m_mapPanel)
		{
			m_auiManager.GetPane("Map").Show();
		}
		else
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Name("Map");
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(PANEL_SIZE_X(60), PANEL_SIZE_Y(40));
			paneInfo.Centre();
			paneInfo.Caption("Map");
			paneInfo.CaptionVisible(true);

			m_mapPanel = new MapPanel(this, *m_project, *m_renderer, m_context, *m_renderResources, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_mapPanel, paneInfo);
			paneInfo.Show();
		}

		m_auiManager.Update();
	}
}

void MainWindow::ShowPanelMapList()
{
	if(m_project.get())
	{
		if (m_gameObjectTypePanel)
		{
			m_auiManager.GetPane("MapList").Show();
		}
		else
		{
			wxSize clientSize = GetClientSize();

			wxAuiPaneInfo paneInfo;
			paneInfo.Name("MapList");
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(PANEL_SIZE_X(20), PANEL_SIZE_Y(10));
			paneInfo.Right();
			paneInfo.Row(1);
			paneInfo.Caption("Maps");
			paneInfo.CaptionVisible(true);

			m_mapListPanel = new MapListPanel(this, *m_project.get(), m_dockArea, NewControlId());
			m_auiManager.AddPane(m_mapListPanel, paneInfo);
			paneInfo.Show();
		}

		m_auiManager.Update();
	}
}

void MainWindow::ShowToolboxTiles()
{
	if (m_toolboxPanelTiles)
	{
		m_auiManager.GetPane("ToolboxTiles").Show();
	}
	else
	{
		wxAuiPaneInfo paneInfo;
		paneInfo.Name("ToolboxTiles");
		paneInfo.Dockable(true);
		paneInfo.DockFixed(false);
		paneInfo.BestSize(PANEL_SIZE_X(7), PANEL_SIZE_Y(20));
		paneInfo.Left();
		paneInfo.Row(0);
		paneInfo.Caption("Tile Tools");
		paneInfo.CaptionVisible(true);

		m_toolboxPanelTiles = new MapToolboxTiles(m_dockArea, NewControlId());
		m_auiManager.AddPane(m_toolboxPanelTiles, paneInfo);
		paneInfo.Show();

		//Subscribe to toolbox buttons
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_SELECTTILE);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_TILEPICKER);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_CREATE_SCENE_ANIM);
		

#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_PAINT);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_FLIPX);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_FLIPY);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_FILL);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_CLONE);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_COPY_TO_NEW_MAP);
#endif

#if BEEHIVE_FIXED_STAMP_MODE //No tile editing in fixed mode
		delete m_toolboxPanelTiles->m_toolPaint;
		delete m_toolboxPanelTiles->m_toolFlipX;
		delete m_toolboxPanelTiles->m_toolFlipY;
		delete m_toolboxPanelTiles->m_toolFill;
		delete m_toolboxPanelTiles->m_toolClone;
		delete m_toolboxPanelTiles->m_toolCopyToNewMap;
#else
		//Hide unused
#if !INCLUDE_UI_TOOLBOX_TILES_CLONE
		delete m_toolboxPanelTiles->m_toolClone;
#endif

#if !INCLUDE_UI_TOOLBOX_TILES_CREATESCENEANIM
		delete m_toolboxPanelTiles->m_toolCreateSceneAnim;
#endif

#if !INCLUDE_UI_TOOLBOX_TILES_COPYTONEWMAP
		delete m_toolboxPanelTiles->m_toolCopyToNewMap;
#endif
#endif
	}

	m_auiManager.Update();
}

void MainWindow::ShowToolboxCollision()
{
	if (m_toolboxPanelTerrain)
	{
		m_auiManager.GetPane("ToolboxTerrain").Show();
	}
	else
	{
		wxAuiPaneInfo paneInfo;
		paneInfo.Name("ToolboxTerrain");
		paneInfo.Dockable(true);
		paneInfo.DockFixed(false);
		paneInfo.BestSize(PANEL_SIZE_X(7), PANEL_SIZE_X(20));
		paneInfo.Left();
		paneInfo.Row(1);
		paneInfo.Caption("Collision Tools");
		paneInfo.CaptionVisible(true);

		m_toolboxPanelTerrain = new MapToolboxTerrain(m_dockArea, NewControlId());
		m_auiManager.AddPane(m_toolboxPanelTerrain, paneInfo);
		paneInfo.Show();

#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
		//Subscribe to toolbox buttons
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_COL_PAINTTERRAIN);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_COL_PAINTSOLID);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_COL_PAINTHOLE);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_COL_DELETETERRTILE);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_COL_ADDTERRAINBEZIER);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_COL_EDITTERRAINBEZIER);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_COL_DELETETERRAINBEZIER);
#endif

		//Hide unused
#if !INCLUDE_UI_TOOLBOX_TERRAIN_PAINT
		delete m_toolboxPanelTerrain->m_toolPaintCollisionPixel;
#endif
	}

	m_auiManager.Update();
}

void MainWindow::ShowToolboxStamps()
{
	if (m_toolboxPanelStamps)
	{
		m_auiManager.GetPane("ToolboxStamps").Show();
	}
	else
	{
		wxAuiPaneInfo paneInfo;
		paneInfo.Name("ToolboxStamps");
		paneInfo.Dockable(true);
		paneInfo.DockFixed(false);
		paneInfo.BestSize(PANEL_SIZE_X(7), PANEL_SIZE_Y(20));
		paneInfo.Left();
		paneInfo.Row(0);
		paneInfo.Caption("Stamp Tools");
		paneInfo.CaptionVisible(true);

		m_toolboxPanelStamps = new MapToolboxStamps(m_dockArea, NewControlId());
		m_auiManager.AddPane(m_toolboxPanelStamps, paneInfo);
		paneInfo.Show();

		//Subscribe to toolbox buttons
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_SELECTSTAMP);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_STAMP);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_STAMPPICKER);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_REMOVESTAMP);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_MOVESTAMP);

#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_CREATESTAMP);
#endif

#if BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
		delete m_toolboxPanelStamps->m_toolCreateStamp;
#else
		//Hide unused
#if !INCLUDE_UI_TOOLBOX_STAMPS_PAINT
		delete m_toolboxPanelStamps->m_toolPaintStamp;
#endif
#endif
	}

	m_auiManager.Update();
}

void MainWindow::ShowToolboxObjects()
{
	if (m_toolboxPanelGameObjs)
	{
		m_auiManager.GetPane("ToolboxObjects").Show();
	}
	else
	{
		wxAuiPaneInfo paneInfo;
		paneInfo.Name("ToolboxObjects");
		paneInfo.Dockable(true);
		paneInfo.DockFixed(false);
		paneInfo.BestSize(PANEL_SIZE_X(7), PANEL_SIZE_Y(20));
		paneInfo.Left();
		paneInfo.Caption("Object Tools");
		paneInfo.CaptionVisible(true);

#if BEEHIVE_FIXED_STAMP_MODE //No tile editing in fixed mode, fewer toolboxes
		paneInfo.Row(0);
#else
		paneInfo.Row(1);
#endif

		m_toolboxPanelGameObjs = new MapToolboxGameObjs(m_dockArea, NewControlId());
		m_auiManager.AddPane(m_toolboxPanelGameObjs, paneInfo);
		paneInfo.Show();

		//Subscribe to toolbox buttons
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_SELECTGAMEOBJ);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_PLACEGAMEOBJ);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_DUPLICATEGAMEOBJ);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_DRAWGAMEOBJ);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_MOVEGAMEOBJ);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_ANIMATEGAMEOBJ);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_REMOVEGAMEOBJ);

		//Hide unused
#if !INCLUDE_UI_TOOLBOX_GAMEOBJ_ANIMATE
		delete m_toolboxPanelGameObjs->m_toolAnimateGameObject;
#endif
	}

	m_auiManager.Update();
}

void MainWindow::ShowPanelTileEditor()
{
	if(m_project.get())
	{
		if (m_tileEditorPanel)
		{
			m_auiManager.GetPane("Tile").Show();
		}
		else
		{
			wxSize clientSize = GetClientSize();

			wxAuiPaneInfo paneInfo;
			paneInfo.Name("Tile");
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(PANEL_SIZE_X(30), PANEL_SIZE_Y(30));
			paneInfo.Bottom();
			paneInfo.Caption("Tile");
			paneInfo.CaptionVisible(true);

			m_tileEditorPanel = new TileEditorPanel(this, *m_project, *m_renderer, m_context, *m_renderResources, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_tileEditorPanel, paneInfo);
			paneInfo.Show();
		}

		m_auiManager.Update();
	}
}

void MainWindow::ShowPanelTerrainEditor()
{
	if(m_project.get())
	{
		if (m_TerrainTileEditorPanel)
		{
			m_auiManager.GetPane("TerrainTile").Show();
		}
		else
		{
			wxSize clientSize = GetClientSize();

			wxAuiPaneInfo paneInfo;
			paneInfo.Name("TerrainTile");
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(PANEL_SIZE_X(30), PANEL_SIZE_Y(30));
			paneInfo.Bottom();
			paneInfo.Caption("Terrain Tile");
			paneInfo.CaptionVisible(true);

			m_TerrainTileEditorPanel = new TerrainTileEditorPanel(this, *m_project, *m_renderer, m_context, *m_renderResources, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_TerrainTileEditorPanel, paneInfo);
			paneInfo.Show();
			
		}

		m_auiManager.Update();
	}
}

void MainWindow::ShowPanelGameObjectTypes()
{
	if(m_project.get())
	{
		if (m_gameObjectTypePanel)
		{
			m_auiManager.GetPane("Objects").Show();
		}
		else
		{
			wxSize clientSize = GetClientSize();

			wxAuiPaneInfo paneInfo;
			paneInfo.Name("Objects");
			paneInfo.Dockable(true);
			paneInfo.Float();
			paneInfo.DockFixed(false);
			paneInfo.BestSize(PANEL_SIZE_X(30), PANEL_SIZE_Y(30));
			paneInfo.Bottom();
			paneInfo.Caption("Game Object Types");
			paneInfo.CaptionVisible(true);

			m_gameObjectTypePanel = new GameObjectTypesPanel(this, *m_project.get(), m_dockArea, NewControlId());
			m_auiManager.AddPane(m_gameObjectTypePanel, paneInfo);
			paneInfo.Show();
			
		}

		m_auiManager.Update();
	}
}

void MainWindow::ShowPanelGameObjectParams()
{
	if(m_project.get())
	{
		if (m_gameObjectParamsPanel)
		{
			m_auiManager.GetPane("ObjectParams").Show();
		}
		else
		{
			wxSize clientSize = GetClientSize();

			wxAuiPaneInfo paneInfo;
			paneInfo.Name("ObjectParams");
			paneInfo.Dockable(true);
			paneInfo.Float();
			paneInfo.DockFixed(false);
			paneInfo.BestSize(PANEL_SIZE_X(30), PANEL_SIZE_Y(30));
			paneInfo.Bottom();
			paneInfo.Caption("Game Object Params");
			paneInfo.CaptionVisible(true);

			m_gameObjectParamsPanel = new GameObjectParamsPanel(*this, *m_project.get(), m_dockArea, NewControlId());
			m_auiManager.AddPane(m_gameObjectParamsPanel, paneInfo);
			paneInfo.Show();
		}

		m_auiManager.Update();
	}
}

void MainWindow::ShowPanelTimeline()
{
	if(m_project.get())
	{
		if (m_timelinePanel)
		{
			m_auiManager.GetPane("AnimTimeline").Show();
		}
		else
		{
			wxSize clientSize = GetClientSize();

			wxAuiPaneInfo paneInfo;
			paneInfo.Name("AnimTimeline");
			paneInfo.Dockable(true);
			paneInfo.Float();
			paneInfo.DockFixed(false);
			paneInfo.BestSize(PANEL_SIZE_X(60), PANEL_SIZE_Y(30));
			paneInfo.Bottom();
			paneInfo.Caption("Animation Timeline");
			paneInfo.CaptionVisible(true);

			m_timelinePanel = new TimelinePanel(*this, *m_project.get(), *m_renderer, *m_context, *m_renderResources, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_timelinePanel, paneInfo);
			paneInfo.Show();
		}

		m_auiManager.Update();
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

AnimationId MainWindow::GetSelectedAnimation()
{
	if(m_timelinePanel)
	{
		return m_timelinePanel->GetCurrentAnimation();
	}
	else
	{
		return InvalidAnimationId;
	}
}

void MainWindow::SetSelectedAnimObject(GameObjectId gameObjectId)
{
	if(m_timelinePanel)
	{
		m_timelinePanel->SetCurrentActor(gameObjectId);
	}
}

void MainWindow::RefreshAnimActors()
{
	if (m_timelinePanel)
	{
		m_timelinePanel->PopulateActors();
	}
}

void MainWindow::SyncSettingsWidgets()
{
	if(m_project.get())
	{
		m_ribbonButtonBarGrid->ToggleButton(wxID_BTN_GRID_SHOW, m_project->GetShowGrid());
		m_ribbonButtonBarGrid->ToggleButton(wxID_BTN_GRID_SNAP, m_project->GetGridSnap());
		m_ribbonButtonBarGrid->ToggleButton(wxID_BTN_SHOW_OUTLINES, m_project->GetShowStampOutlines());
		m_ribbonButtonBarGrid->ToggleButton(wxID_BTN_SHOW_COLLISION, m_project->GetShowCollision());
		m_ribbonButtonBarGrid->ToggleButton(wxID_BTN_SHOW_DISPLAYFRAME, m_project->GetShowDisplayFrame());
	}
}

void MainWindow::RefreshAll()
{
	if(!IsRefreshLocked())
	{
		if(m_project.get())
		{
			m_project->InvalidateMap(true);
			m_project->InvalidateTiles(true);
			m_project->InvalidateTerrainTiles(true);
			m_project->InvalidateTerrainBeziers(true);
			m_project->InvalidateStamps(true);
			m_project->InvalidateBlocks(true);
		}

		RedrawAll();

		m_auiManager.Update();
		Refresh();

		if(m_project.get())
		{
			m_project->InvalidateMap(false);
			m_project->InvalidateTiles(false);
			m_project->InvalidateTerrainTiles(false);
			m_project->InvalidateTerrainBeziers(false);
			m_project->InvalidateStamps(false);
			m_project->InvalidateBlocks(false);
			m_project->InvalidateCamera(false);
		}
	}
}

void MainWindow::LockRefresh()
{
	m_refreshLockStack++;
}

void MainWindow::UnlockRefresh()
{
	m_refreshLockStack--;

	if(!m_refreshLockStack)
	{
		RefreshAll();
	}
}

bool MainWindow::IsRefreshLocked() const
{
	return m_refreshLockStack > 0;
}

void MainWindow::RedrawAll()
{
	RefreshTileset();
	RefreshTerrainTileset();
	RefreshSpriteSheets();
	SetPanelCaptions();

	if(m_palettesPanel)
		m_palettesPanel->Refresh();

	if(m_tilesPanel)
		m_tilesPanel->Refresh();

	if(m_mapPanel)
		m_mapPanel->Refresh();

	if(m_mapListPanel)
		m_mapListPanel->Refresh();

	if(m_stampsPanel)
		m_stampsPanel->Refresh();

	if(m_blocksPanel)
		m_blocksPanel->Refresh();

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
	if(!IsRefreshLocked())
	{
		if(m_project.get())
		{
			//Recreate tileset texture
			m_renderResources->CreateTilesetTexture();
		}
	}
}

void MainWindow::RefreshTerrainTileset()
{
	if(!IsRefreshLocked())
	{
		if(m_project.get())
		{
			//Recreate collision set texture
			m_renderResources->CreateCollisionTypesTexture();
			m_renderResources->CreateTerrainTilesTextures();
		}
	}
}

void MainWindow::RefreshSpriteSheets()
{
	if(m_project.get())
	{
		m_renderResources->CreateSpriteSheetResources(*m_project.get());
	}
}

void MainWindow::RefreshPanel(Panel panel)
{
	if(!IsRefreshLocked())
	{
		if(m_project.get())
		{
			m_project->InvalidateMap(true);
			m_project->InvalidateTiles(true);
			m_project->InvalidateTerrainTiles(true);
			m_project->InvalidateTerrainBeziers(true);
			m_project->InvalidateStamps(true);
			m_project->InvalidateBlocks(true);

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
			m_project->InvalidateBlocks(false);
		}
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
	case ePanelMapList:
		if(m_mapListPanel)
			m_mapListPanel->Refresh();
		break;
	case ePanelStamps:
		if(m_stampsPanel)
			m_stampsPanel->Refresh();
		break;
	case ePanelBlocks:
		if(m_blocksPanel)
			m_blocksPanel->Refresh();
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
		DialogNewProject dialog(this);
		if(dialog.ShowModal() == wxID_OK)
		{
			PlatformConfig config = PlatformPresets::s_configs[PlatformPresets::ePresetMegaDrive];
			config.tileWidth = dialog.m_spinCtrlTileWidth->GetValue();
			config.tileHeight = dialog.m_spinCtrlTileHeight->GetValue();
			config.stampWidth = dialog.m_spinCtrlStampWidth->GetValue();
			config.stampHeight = dialog.m_spinCtrlStampHeight->GetValue();

#if BEEHIVE_FIXED_STAMP_MODE
			config.scrollPlaneWidthTiles = dialog.m_spinCtrlMapWidth->GetValue() * config.stampWidth;
			config.scrollPlaneHeightTiles = dialog.m_spinCtrlMapHeight->GetValue() * config.stampHeight;
#else
			config.scrollPlaneWidthTiles = dialog.m_spinCtrlMapWidth->GetValue();
			config.scrollPlaneHeightTiles = dialog.m_spinCtrlMapHeight->GetValue();
#endif

			if (config.stampWidth > 0)
			{
				config.scrollPlaneWidthTiles = ion::maths::RoundUpToNearest(config.scrollPlaneWidthTiles, config.stampWidth);
			}

			if (config.stampHeight > 0)
			{
				config.scrollPlaneHeightTiles = ion::maths::RoundUpToNearest(config.scrollPlaneHeightTiles, config.stampHeight);
			}

			SetProject(new Project(config));
		}
	}
}

void MainWindow::OnBtnProjOpen(wxRibbonButtonBarEvent& event)
{
	wxFileDialog dialog(this, _("Open BEE file"), "", "", "BEE files (*.bee)|*.bee", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(dialog.ShowModal() == wxID_OK)
	{
		SetStatusText("Opening...");

		Project* project = new Project(PlatformPresets::s_configs[ePlatformMegaDrive]);

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

void MainWindow::OnBtnProjSettings(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		ProjectSettingsDialog dialog(*this, *m_project, *m_renderResources);
		dialog.ShowModal();
	}
}

#if defined BEEHIVE_PLUGIN_LUMINARY
void ExportEntity(const Project& project, const GameObjectType& gameObjectType, const GameObject& gameObject, luminary::Entity& entity)
{
	//Type name
	entity.name = gameObjectType.GetName();

	//Entity name
	if (gameObject.GetName().size() > 0)
		entity.spawnData.name = gameObject.GetName();
	else
		entity.spawnData.name = std::string("ent") + std::to_string(gameObject.GetId());

	//Spawn position
	entity.spawnData.positionX = gameObject.GetPosition().x + GameObject::spriteSheetBorderX;
	entity.spawnData.positionY = gameObject.GetPosition().y + GameObject::spriteSheetBorderY;
	entity.spawnData.width = (gameObject.GetDimensions().x > 0) ? gameObject.GetDimensions().x : gameObjectType.GetDimensions().x;
	entity.spawnData.height = (gameObject.GetDimensions().y > 0) ? gameObject.GetDimensions().y : gameObjectType.GetDimensions().y;

	//Sprite
	std::string spriteActorName = "";
	std::string spriteSheetName = "";
	std::string spriteAnimName = "";
	int spriteSizeTiles = 0;
	u8 spriteLayout = 0;

	if (const Actor* actor = project.GetActor(gameObject.GetSpriteActorId()))
	{
		spriteActorName = actor->GetName();

		if (const SpriteSheet* spriteSheet = actor->GetSpriteSheet(gameObject.GetSpriteSheetId()))
		{
			spriteSheetName = spriteSheet->GetName();
			spriteSizeTiles = spriteSheet->GetWidthTiles() * spriteSheet->GetHeightTiles();
			spriteLayout = (u8)luminary::SpriteExporter::GetSpriteLayout(spriteSheet->GetWidthTiles(), spriteSheet->GetHeightTiles());

			if (const SpriteAnimation* spriteAnim = spriteSheet->GetAnimation(gameObject.GetSpriteAnim()))
			{
				spriteAnimName = spriteAnim->GetName();
			}
		}
	}

	//Create entity and component spawn params
	int paramIdx = 0;
	int componentIdx = -1;

	const std::vector<GameObjectVariable>& variables = gameObjectType.GetVariables();

	for (int j = 0; j < variables.size(); j++, paramIdx++)
	{
		const GameObjectVariable& variable = variables[j];
		luminary::Param* param = nullptr;

		if (variable.m_componentIdx == -1)
		{
			//Entity param
			entity.spawnData.params.resize(paramIdx + 1);
			param = &entity.spawnData.params[paramIdx];
		}
		else
		{
			//Component param
			if (componentIdx != variable.m_componentIdx)
			{
				componentIdx = variable.m_componentIdx;
				entity.components.resize(componentIdx + 1);
				entity.components[componentIdx].name = variable.m_componentName;
				paramIdx = 0;
			}

			entity.components[componentIdx].spawnData.params.resize(paramIdx + 1);
			param = &entity.components[componentIdx].spawnData.params[paramIdx];
		}

		param->name = variable.m_name;
		param->value = "0x0";

		//Search for supported tags
		if (variable.HasTag(luminary::tags::GetTagName(luminary::tags::TagType::PositionX)))
		{
			param->value = std::to_string(gameObject.GetPosition().x + GameObject::spriteSheetBorderX);
		}
		else if (variable.HasTag(luminary::tags::GetTagName(luminary::tags::TagType::PositionY)))
		{
			param->value = std::to_string(gameObject.GetPosition().y + GameObject::spriteSheetBorderY);
		}
		else if (variable.HasTag(luminary::tags::GetTagName(luminary::tags::TagType::SpriteSheet)))
		{
			if (spriteActorName.size() > 0 && spriteSheetName.size() > 0)
			{
				std::stringstream stream;
				stream << "actor_" << spriteActorName << "_spritesheet_" << spriteSheetName;
				param->value = stream.str();
			}
		}
		else if (variable.HasTag(luminary::tags::GetTagName(luminary::tags::TagType::SpriteTileData)))
		{
			if (spriteActorName.size() > 0 && spriteSheetName.size() > 0)
			{
				std::stringstream stream;
				stream << "actor_" << spriteActorName << "_sheet_" << spriteSheetName << "_frame_0";
				param->value = stream.str();
			}
		}
		else if (variable.HasTag(luminary::tags::GetTagName(luminary::tags::TagType::SpriteTileCount)))
		{
			if (spriteActorName.size() > 0 && spriteSheetName.size() > 0)
			{
				std::stringstream stream;
				stream << "actor_" << spriteActorName << "_sheet_" << spriteSheetName << "_frame_0_size_t";
				param->value = stream.str();
			}
		}
		else if (variable.HasTag(luminary::tags::GetTagName(luminary::tags::TagType::SpriteLayout)))
		{
			param->value = std::to_string(spriteLayout);
		}
		else if (variable.HasTag(luminary::tags::GetTagName(luminary::tags::TagType::SpriteAnimation)))
		{
			if (spriteActorName.size() > 0 && spriteSheetName.size() > 0 && spriteAnimName.size() > 0)
			{
				std::stringstream stream;
				stream << "actor_" << spriteActorName << "_sheet_" << spriteSheetName << "_anim_" << spriteAnimName;
				param->value = stream.str();
			}
		}
		else
		{
			param->value = variable.m_value;

			//If game object has overridden the variable, take that value instead
			if (const GameObjectVariable* overriddenVar = gameObject.FindVariable(variable.m_name))
			{
				param->value = overriddenVar->m_value;
			}
		}

		switch (variable.m_size)
		{
		case eSizeByte:
			param->size = luminary::ParamSize::Byte;
			break;
		case eSizeWord:
			param->size = luminary::ParamSize::Word;
			break;
		case eSizeLong:
			param->size = luminary::ParamSize::Long;
			break;
		}
	}
}
#endif

void MainWindow::OnBtnProjExport(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
#if defined BEEHIVE_PLUGIN_LUMINARY
		luminary::TilesetExporter tilesetExporter;
		luminary::SceneExporter sceneExporter;
		luminary::MapExporter mapExporter;
		luminary::PaletteExporter paletteExporter;
		luminary::TerrainExporter terrainExporter;

		std::vector<std::pair<std::string,std::string>> includeFilenames;

		//Export sprite data
		// TODO: All-in-one sprites export dir + include file
		// TODO: Luminary (binary) data formats
		m_project->ExportSpriteSheets(m_project->m_settings.spritesExportDir, Project::ExportFormat::BinaryCompressed);
		m_project->ExportSpriteAnims(m_project->m_settings.spriteAnimsExportDir, Project::ExportFormat::BinaryCompressed);
		m_project->ExportSpritePalettes(m_project->m_settings.spritePalettesExportDir);

		//Export luminary palettes
		int numPalettes = 0;
		std::string palettesLabel = std::string("palettes_") + m_project->GetName();
		std::string palettesFilename = m_project->m_settings.sceneExportDir + "\\" + "PALETTES.ASM";
		if (m_project->GetNumPalettes() > 0)
		{
			std::vector<Palette> palettes;

			for (int i = 0; i < m_project->GetNumPalettes(); i++)
			{
				if (m_project->GetPalette(i)->GetUsedColourMask() > 0)
				{
					palettes.push_back(*m_project->GetPalette(i));
				}
			}

			if (paletteExporter.ExportPalettes(palettesFilename, palettes))
			{
				includeFilenames.push_back(std::make_pair(palettesLabel, palettesFilename));
			}

			numPalettes = palettes.size();
		}

		//Export Luminary tileset
		std::string tilesetLabel = std::string("tileset_") + m_project->GetName();
		std::string tilesetFilename = m_project->m_settings.sceneExportDir + "\\" + "GTILES.BIN";
		if (tilesetExporter.ExportTileset(tilesetFilename, m_project->GetTileset()))
		{
			includeFilenames.push_back(std::make_pair(tilesetLabel, tilesetFilename));
		}

		//Export Luminary stamp set
		m_project->CompactStampIds();
		std::vector<Stamp> stamps;
		for (int i = 0; i < m_project->GetStampCount(); i++)
		{
			if (const Stamp* stamp = m_project->GetStamp(i))
			{
				stamps.push_back(*stamp);
			}
		}

		std::string stampsetLabel = std::string("stampset_") + m_project->GetName();
		std::string stampsetFilename = m_project->m_settings.sceneExportDir + "\\" + "GSTAMPS.BIN";
		if (tilesetExporter.ExportStamps(stampsetFilename, stamps, m_project->GetTileset(), m_project->GetBackgroundTile()))
		{
			includeFilenames.push_back(std::make_pair(stampsetLabel, stampsetFilename));
		}

		//Export Luminary maps
		for (TMapMap::iterator it = m_project->MapsBegin(), end = m_project->MapsEnd(); it != end; ++it)
		{
			const Map& map = m_project->GetMap(it->first);
			std::string mapLabel = std::string("map_") + m_project->GetName() + "_" + map.GetName();
			std::string mapFilename = m_project->m_settings.sceneExportDir + "\\G" + ion::string::ToUpper(map.GetName()) + ".BIN";
			if (mapExporter.ExportMap(mapFilename, map, m_project->GetPlatformConfig().stampWidth, m_project->GetPlatformConfig().stampHeight))
			{
				includeFilenames.push_back(std::make_pair(mapLabel, mapFilename));
			}
		}

		//Export Luminary terrain tileset
		std::string terrainTilesetLabel = std::string("collision_tileset_") + m_project->GetName();
		std::string terrainTilesetFilename = m_project->m_settings.sceneExportDir + "\\" + "CTILES.BIN";
		if (terrainExporter.ExportTerrainTileset(terrainTilesetFilename, m_project->GetTerrainTileset(), m_project->GetPlatformConfig().tileWidth))
		{
			includeFilenames.push_back(std::make_pair(terrainTilesetLabel, terrainTilesetFilename));
		}

		//Export Luminary terrain stamps
		std::string terrainStampsetLabel = std::string("collision_stampset_") + m_project->GetName();
		std::string terrainStampsetFilename = m_project->m_settings.sceneExportDir + "\\" + "CSTAMPS.BIN";
		if (terrainExporter.ExportTerrainStamps(terrainStampsetFilename, stamps, m_project->GetTerrainTileset(), m_project->GetDefaultTerrainTile()))
		{
			includeFilenames.push_back(std::make_pair(terrainStampsetLabel, terrainStampsetFilename));
		}

		//Export Luminary terrain maps
		for (TMapMap::iterator it = m_project->MapsBegin(), end = m_project->MapsEnd(); it != end; ++it)
		{
			const Map& map = m_project->GetMap(it->first);
			std::string terrainMapLabel = std::string("collision_map_") + m_project->GetName() + "_" + map.GetName();
			std::string terrainMapFilename = m_project->m_settings.sceneExportDir + "\\C" + ion::string::ToUpper(map.GetName()) + ".BIN";
			if (terrainExporter.ExportTerrainMap(terrainMapFilename, map, m_project->GetPlatformConfig().stampWidth, m_project->GetPlatformConfig().stampHeight))
			{
				includeFilenames.push_back(std::make_pair(terrainMapLabel, terrainMapFilename));
			}
		}

		//Export Luminary scenes
		for (TMapMap::iterator it = m_project->MapsBegin(), end = m_project->MapsEnd(); it != end; ++it)
		{
			const Map& map = m_project->GetMap(it->first);
			const TGameObjectPosMap& gameObjMap = map.GetGameObjects();
			int entityIdx = 0;

			luminary::SceneExporter::SceneData sceneData;

			for (TGameObjectPosMap::const_iterator it = gameObjMap.begin(), end = gameObjMap.end(); it != end; ++it)
			{
				const GameObjectType* gameObjectType = m_project->GetGameObjectType(it->first);
				if (gameObjectType)
				{
					if (gameObjectType->IsStaticType())
					{
						for (int i = 0; i < it->second.size(); i++, entityIdx++)
						{
							const GameObject& gameObject = it->second[i].m_gameObject;
							sceneData.staticEntities.push_back(luminary::Entity());
							luminary::Entity& entity = sceneData.staticEntities.back();
							ExportEntity(*m_project, *gameObjectType, gameObject, entity);
						}
					}
					else
					{
						for (int i = 0; i < it->second.size(); i++, entityIdx++)
						{
							const GameObject& gameObject = it->second[i].m_gameObject;
							sceneData.dynamicEntities.push_back(luminary::Entity());
							luminary::Entity& entity = sceneData.dynamicEntities.back();
							ExportEntity(*m_project, *gameObjectType, gameObject, entity);
						}
					}
				}
			}

			sceneData.palettesLabel = palettesLabel;
			sceneData.numPalettes = numPalettes;

			sceneData.mapLabel = std::string("map_") + m_project->GetName() + "_" + map.GetName();
			sceneData.stampsetLabel = stampsetLabel;
			sceneData.tilesetLabel = tilesetLabel;
			sceneData.collisionMapLabel = std::string("collision_map_") + m_project->GetName() + "_" + map.GetName();
			sceneData.collisionStampsetLabel = terrainStampsetLabel;
			sceneData.collisionTilesetLabel = terrainTilesetLabel;

			sceneData.numTiles = m_project->GetTileset().GetCount();
			sceneData.numStamps = m_project->GetStampCount();
			sceneData.mapWidthStamps = map.GetWidth() / m_project->GetPlatformConfig().stampWidth;
			sceneData.mapHeightStamps = map.GetHeight() / m_project->GetPlatformConfig().stampHeight;
			sceneData.numCollisionTiles = m_project->GetTerrainTileset().GetCount();
			sceneData.numCollisionStamps = terrainExporter.GetNumUniqueTerrainStamps();
			sceneData.collisionMapWidthStamps = map.GetWidth() / m_project->GetPlatformConfig().stampWidth;
			sceneData.collisionMapHeightStamps = map.GetHeight() / m_project->GetPlatformConfig().stampHeight;

			std::string sceneName = m_project->GetName() + "_" + map.GetName();
			std::string sceneFilename = m_project->m_settings.sceneExportDir + "\\" + ion::string::ToUpper(map.GetName()) + ".ASM";
			if (sceneExporter.ExportScene(sceneFilename, sceneName, sceneData))
			{
				includeFilenames.push_back(std::make_pair(std::string("scene_") + sceneName, sceneFilename));
			}
		}

		//Generate uber include file
		if (includeFilenames.size() > 0)
		{
			m_project->WriteIncludeFile(m_project->m_settings.projectExportDir, m_project->m_settings.sceneExportDir, "INCLUDE.ASM", includeFilenames, true);
		}

		SetStatusText("Export complete");
		wxMessageBox("Export complete", "Error", wxOK | wxICON_INFORMATION);

		//Update counts
		SetPanelCaptions();
#else
		ExportDialog dialog(this, *m_project);

		dialog.m_txtProjectName->SetValue(m_project->GetName());
		dialog.m_filePickerPalettes->SetPath(m_project->m_exportFilenames.palettes);
		dialog.m_filePickerTileset->SetPath(m_project->m_exportFilenames.tileset);
		dialog.m_filePickerBlocks->SetPath(m_project->m_exportFilenames.blocks);
		dialog.m_filePickerStamps->SetPath(m_project->m_exportFilenames.stamps);
		dialog.m_filePickerStampAnims->SetPath(m_project->m_exportFilenames.stampAnims);
		dialog.m_filePickerTerrainTiles->SetPath(m_project->m_exportFilenames.terrainTiles);
		dialog.m_filePickerTerrainBlocks->SetPath(m_project->m_exportFilenames.terrainBlocks);
		dialog.m_filePickerTerrainAngles->SetPath(m_project->m_exportFilenames.terrainAngles);
		dialog.m_filePickerGameObjTypes->SetPath(m_project->m_exportFilenames.gameObjTypes);
		dialog.m_dirPickerSpriteSheets->SetPath(m_project->m_exportFilenames.spriteSheets);
		dialog.m_dirPickerSpriteAnims->SetPath(m_project->m_exportFilenames.spriteAnims);
		dialog.m_dirPickerSpritePalettes->SetPath(m_project->m_exportFilenames.spritePalettes);

		dialog.m_chkPalettes->SetValue(m_project->m_exportFilenames.palettesExportEnabled);
		dialog.m_chkTileset->SetValue(m_project->m_exportFilenames.tilesetExportEnabled);
		dialog.m_chkBlocks->SetValue(m_project->m_exportFilenames.blockExportEnabled);
		dialog.m_chkStamps->SetValue(m_project->m_exportFilenames.stampsExportEnabled);
		dialog.m_chkStampAnims->SetValue(m_project->m_exportFilenames.stampAnimsExportEnabled);
		dialog.m_chkTerrainTiles->SetValue(m_project->m_exportFilenames.terrainTilesExportEnabled);
		dialog.m_chkTerrainBlocks->SetValue(m_project->m_exportFilenames.terrainBlockExportEnabled);
		dialog.m_chkTerrainAngles->SetValue(m_project->m_exportFilenames.terrainAngleExportEnabled);
		dialog.m_chkGameObjTypes->SetValue(m_project->m_exportFilenames.gameObjTypesExportEnabled);
		dialog.m_chkSpriteSheets->SetValue(m_project->m_exportFilenames.spriteSheetsExportEnabled);
		dialog.m_chkSpriteAnims->SetValue(m_project->m_exportFilenames.spriteAnimsExportEnabled);
		dialog.m_chkSpritePalettes->SetValue(m_project->m_exportFilenames.spritePalettesExportEnabled);

		dialog.m_btnBinary->SetValue(m_project->m_exportFilenames.exportFormat == Project::ExportFormat::Binary);
		dialog.m_btnCompressed->SetValue(m_project->m_exportFilenames.exportFormat == Project::ExportFormat::BinaryCompressed);
		dialog.m_btnText->SetValue(m_project->m_exportFilenames.exportFormat == Project::ExportFormat::Text);

		int mapIndex = 0;
		for(TMapMap::const_iterator it = m_project->MapsBegin(), end = m_project->MapsEnd(); it != end; ++it, ++mapIndex)
		{
			dialog.SetMapFormValues(mapIndex, it->second.m_exportFilenames);
		}

		if(dialog.ShowModal() == wxID_OK)
		{
			SetStatusText("Exporting...");
			
			m_project->SetName(std::string(dialog.m_txtProjectName->GetValue()));
			m_project->m_exportFilenames.palettes = dialog.m_filePickerPalettes->GetPath();
			m_project->m_exportFilenames.tileset = dialog.m_filePickerTileset->GetPath();
			m_project->m_exportFilenames.blocks = dialog.m_filePickerBlocks->GetPath();
			m_project->m_exportFilenames.stamps = dialog.m_filePickerStamps->GetPath();
			m_project->m_exportFilenames.stampAnims = dialog.m_filePickerStampAnims->GetPath();
			m_project->m_exportFilenames.terrainTiles = dialog.m_filePickerTerrainTiles->GetPath();
			m_project->m_exportFilenames.terrainBlocks = dialog.m_filePickerTerrainBlocks->GetPath();
			m_project->m_exportFilenames.terrainAngles = dialog.m_filePickerTerrainAngles->GetPath();
			m_project->m_exportFilenames.gameObjTypes = dialog.m_filePickerGameObjTypes->GetPath();
			m_project->m_exportFilenames.spriteSheets = dialog.m_dirPickerSpriteSheets->GetPath();
			m_project->m_exportFilenames.spriteAnims = dialog.m_dirPickerSpriteAnims->GetPath();
			m_project->m_exportFilenames.spritePalettes = dialog.m_dirPickerSpritePalettes->GetPath();

			m_project->m_exportFilenames.palettesExportEnabled = dialog.m_chkPalettes->GetValue();
			m_project->m_exportFilenames.tilesetExportEnabled = dialog.m_chkTileset->GetValue();
			m_project->m_exportFilenames.blockExportEnabled = dialog.m_chkBlocks->GetValue();
			m_project->m_exportFilenames.stampsExportEnabled = dialog.m_chkStamps->GetValue();
			m_project->m_exportFilenames.stampAnimsExportEnabled = dialog.m_chkStampAnims->GetValue();
			m_project->m_exportFilenames.terrainTilesExportEnabled = dialog.m_chkTerrainTiles->GetValue();
			m_project->m_exportFilenames.terrainBlockExportEnabled = dialog.m_chkTerrainBlocks->GetValue();
			m_project->m_exportFilenames.terrainAngleExportEnabled = dialog.m_chkTerrainAngles->GetValue();
			m_project->m_exportFilenames.gameObjTypesExportEnabled = dialog.m_chkGameObjTypes->GetValue();
			m_project->m_exportFilenames.spriteSheetsExportEnabled = dialog.m_chkSpriteSheets->GetValue();
			m_project->m_exportFilenames.spriteAnimsExportEnabled = dialog.m_chkSpriteAnims->GetValue();
			m_project->m_exportFilenames.spritePalettesExportEnabled = dialog.m_chkSpritePalettes->GetValue();

			bool binary = dialog.m_btnBinary->GetValue();
			bool compressed = dialog.m_btnCompressed->GetValue();
			bool bee = dialog.m_btnBeeFormat->GetValue();

			Project::ExportFormat format = Project::ExportFormat::Text;

			if (bee)
				format = Project::ExportFormat::Beehive;
			else if(compressed)
				format = Project::ExportFormat::BinaryCompressed;
			else if (binary)
				format = Project::ExportFormat::Binary;

			m_project->m_exportFilenames.exportFormat = format;
			
			if(dialog.m_chkPalettes->GetValue())
				m_project->ExportPalettes(m_project->m_exportFilenames.palettes, format);

			if (dialog.m_chkTileset->GetValue())
				m_project->ExportTiles(m_project->m_exportFilenames.tileset, format);

			if(dialog.m_chkStamps->GetValue())
				m_project->ExportStamps(m_project->m_exportFilenames.stamps, format);

			if(dialog.m_chkStampAnims->GetValue())
				m_project->ExportStampAnims(m_project->m_exportFilenames.stampAnims, format);

			if(dialog.m_chkTerrainTiles->GetValue())
				m_project->ExportTerrainTiles(m_project->m_exportFilenames.terrainTiles, format);

			if (dialog.m_chkGameObjTypes->GetValue())
				m_project->ExportGameObjectTypes(m_project->m_exportFilenames.gameObjTypes, format, true);

			if(dialog.m_chkSpriteSheets->GetValue())
				m_project->ExportSpriteSheets(m_project->m_exportFilenames.spriteSheets, format);
			
			if(dialog.m_chkSpriteAnims->GetValue())
				m_project->ExportSpriteAnims(m_project->m_exportFilenames.spriteAnims, format);
			
			if(dialog.m_chkSpritePalettes->GetValue())
				m_project->ExportSpritePalettes(m_project->m_exportFilenames.spritePalettes);

			int blockWidth = m_project->GetPlatformConfig().blockWidth;
			int blockHeight = m_project->GetPlatformConfig().blockHeight;
			int terrainBlockWidth = m_project->GetPlatformConfig().terrainBlockWidth;
			int terrainBlockHeight = m_project->GetPlatformConfig().terrainBlockHeight;

			if(dialog.m_chkBlocks->GetValue())
			{
				//Export blocks
				m_project->ExportBlocks(m_project->m_exportFilenames.blocks, format, blockWidth, blockHeight);
			}

			if(dialog.m_chkTerrainBlocks->GetValue())
			{
				//Export terrain blocks
				m_project->ExportTerrainBlocks(m_project->m_exportFilenames.terrainBlocks, format, terrainBlockWidth, terrainBlockHeight);
			}

			if (dialog.m_chkTerrainAngles->GetValue())
			{
				//Export terrain angle data
				m_project->ExportTerrainAngles(m_project->m_exportFilenames.terrainAngles, format);
			}

			int mapIndex = 0;
			for(TMapMap::iterator it = m_project->MapsBegin(), end = m_project->MapsEnd(); it != end; ++it, ++mapIndex)
			{
				dialog.GetMapFormValues(mapIndex, it->second.m_exportFilenames);

				if(it->second.m_exportFilenames.mapExportEnabled)
					m_project->ExportMap(it->first, it->second.m_exportFilenames.map, format);

				if(it->second.m_exportFilenames.blockMapExportEnabled)
					m_project->ExportBlockMap(it->first, it->second.m_exportFilenames.blockMap, format, blockWidth, blockHeight);

				if(it->second.m_exportFilenames.stampMapExportEnabled)
					m_project->ExportStampMap(it->first, it->second.m_exportFilenames.stampMap, format);
				
				if(it->second.m_exportFilenames.collisionMapExportEnabled)
					m_project->ExportCollisionMap(it->first, it->second.m_exportFilenames.collisionMap, format);

				if(it->second.m_exportFilenames.terrainBlockMapExportEnabled)
					m_project->ExportTerrainBlockMap(it->first, it->second.m_exportFilenames.terrainBlockMap, format, terrainBlockWidth, terrainBlockHeight);

				if(it->second.m_exportFilenames.sceneAnimExportEnabled)
					m_project->ExportSceneAnimations(it->first, it->second.m_exportFilenames.sceneAnims, format);

				if (it->second.m_exportFilenames.gameObjectsExportEnabled)
					m_project->ExportGameObjects(it->first, it->second.m_exportFilenames.gameObjects, format);
			}

			SetStatusText("Export complete");
			wxMessageBox("Export complete", "Error", wxOK | wxICON_INFORMATION);

			//Update counts
			SetPanelCaptions();
		}
#endif // BEEHIVE_PLUGIN_LUMINARY
	}
}

void MainWindow::OnBtnTilesImport(wxRibbonButtonBarEvent& event)
{
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
	if(m_project.get())
	{
		ImportDialog dialog(this);
		if(dialog.ShowModal() == wxID_OK)
		{
			SetStatusText("Importing...");

			u32 flags = 0;

			if(dialog.m_chkImportToStamp->GetValue())
				flags |= Project::eBMPImportToStamp;
			if(dialog.m_chkReplaceStamps->GetValue())
				flags |= Project::eBMPImportReplaceStamp;
			if(dialog.m_chkInsertBGTile->GetValue())
				flags |= Project::eBMPImportInsertBGTile;
			if(dialog.m_chkOnlyExisting->GetValue())
				flags |= Project::eBMPImportOnlyExistingStamps;
			if(dialog.m_chkNoDuplicateTileCheck->GetValue())
				flags |= Project::eBMPImportNoDuplicateTileCheck;

			//Unsupported flags if multiple files/stamp directory selected
			if((dialog.m_dirStamps->GetPath().size() == 0) || (dialog.m_paths.size() > 1))
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
				if(dialog.m_chkInsertBGTile->GetValue())
					flags |= Project::eBMPImportInsertBGTile;
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

			wxArrayString filenames;

			if(dialog.m_dirStamps->GetPath().size() > 0)
			{
				//Enumerate all files in directory
				wxString directoryPath = dialog.m_dirStamps->GetDirName().GetPath();
				wxDir dir(directoryPath);

				wxString filename;
				bool next = dir.GetFirst(&filename, "*.bmp", wxDIR_FILES | wxDIR_NO_FOLLOW);
				while(next)
				{
					//Get stamp name
					std::string stampName = filename;

					const size_t lastSlash = stampName.find_last_of('\\');
					if(std::string::npos != lastSlash)
					{
						stampName.erase(0, lastSlash + 1);
					}

					// Remove extension if present.
					const size_t period = stampName.rfind('.');
					if(std::string::npos != period)
					{
						stampName.erase(period);
					}

					//If only to import existing, stamp by this name must already exist
					if(!(flags & Project::eBMPImportOnlyExistingStamps) || m_project->FindStamp(stampName))
					{
						filenames.Add(directoryPath + "\\" + filename);
					}

					next = dir.GetNext(&filename);
				}
			}
			else
			{
				//Use files from file selection dlg
				filenames = dialog.m_paths;
			}

			for(int i = 0; i < filenames.size(); i++)
			{
				Stamp* stampToReplace = NULL;

				if(flags & Project::eBMPImportReplaceStamp)
				{
					std::string stampName = filenames[i].c_str().AsChar();

					const size_t lastSlash = stampName.find_last_of('\\');
					if(std::string::npos != lastSlash)
					{
						stampName.erase(0, lastSlash + 1);
					}

					// Remove extension if present.
					const size_t period = stampName.rfind('.');
					if(std::string::npos != period)
					{
						stampName.erase(period);
					}

					stampToReplace = m_project->FindStamp(stampName);
				}

				m_project->ImportBitmap(filenames[i].c_str().AsChar(), flags, palettes, stampToReplace);
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
#endif
}

void MainWindow::OnBtnStampsImport(wxRibbonButtonBarEvent& event)
{
	if (m_project.get())
	{
		ImportStampsDialog dialog(this);
		if (dialog.ShowModal() == wxID_OK)
		{
			SetStatusText("Importing...");

			//Importing from stamp(s)
			u32 flags = Project::eBMPImportToStamp;

			//Enumerate flags
			if (dialog.m_chkReplaceStamps->GetValue())
				flags |= Project::eBMPImportReplaceStamp;

			//Unsupported flags if multiple files/stamp directory selected
			if (!dialog.m_radioStampDir->GetValue())
			{
				if (dialog.m_chkImportPalette->GetValue())
					flags |= Project::eBMPImportWholePalette;
				if (dialog.m_chkClearPalettes->GetValue())
					flags |= Project::eBMPImportClearPalettes;
			}

			u32 palettes = (1 << dialog.m_radioBoxPal->GetSelection());

			//Clear palette if checked
			if (dialog.m_chkClearPalettes->GetValue())
			{
				m_project->GetPalette(dialog.m_radioBoxPal->GetSelection())->Clear();
			}

			wxArrayString filenames;

			if (dialog.m_radioStampDir->GetValue() && dialog.m_dirStamps->GetPath().size() > 0)
			{
				//Enumerate all files in directory
				wxString directoryPath = dialog.m_dirStamps->GetDirName().GetPath();
				wxDir dir(directoryPath);

				wxString filename;
				bool next = dir.GetFirst(&filename, "*.bmp", wxDIR_FILES | wxDIR_NO_FOLLOW);
				while (next)
				{
					//Get stamp name
					std::string stampName = filename;

					const size_t lastSlash = stampName.find_last_of('\\');
					if (std::string::npos != lastSlash)
					{
						stampName.erase(0, lastSlash + 1);
					}

					// Remove extension if present.
					const size_t period = stampName.rfind('.');
					if (std::string::npos != period)
					{
						stampName.erase(period);
					}

					//If only to import existing, stamp by this name must already exist
					if (!(flags & Project::eBMPImportOnlyExistingStamps) || m_project->FindStamp(stampName))
					{
						filenames.Add(directoryPath + "\\" + filename);
					}

					next = dir.GetNext(&filename);
				}
			}
			else if(dialog.m_radioSingleStamp->GetValue())
			{
				//Use files from file selection dlg
				filenames = dialog.m_paths;
			}

			for (int i = 0; i < filenames.size(); i++)
			{
				Stamp* stampToReplace = NULL;

				if (flags & Project::eBMPImportReplaceStamp)
				{
					std::string stampName = filenames[i].c_str().AsChar();

					const size_t lastSlash = stampName.find_last_of('\\');
					if (std::string::npos != lastSlash)
					{
						stampName.erase(0, lastSlash + 1);
					}

					// Remove extension if present.
					const size_t period = stampName.rfind('.');
					if (std::string::npos != period)
					{
						stampName.erase(period);
					}

					stampToReplace = m_project->FindStamp(stampName);
				}

				m_project->ImportBitmap(filenames[i].c_str().AsChar(), flags, palettes, stampToReplace);
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

void MainWindow::OnBtnSpriteEditor(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		SpriteAnimEditorDialog dialog(this, SpriteAnimEditorDialog::eAnimEditModeSpriteAnim, *m_project, *m_renderer, *m_context, *m_renderResources);
		dialog.ShowModal();
	}
}

void MainWindow::OnBtnStampsExportBMPs(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		wxDirDialog dialog(this);
		if(dialog.ShowModal() == wxID_OK)
		{
			m_project->ExportStampBitmaps(dialog.GetPath().c_str().AsChar());
		}
	}
}

void MainWindow::OnBtnStampsCleanup(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		if(m_project->CleanupStamps())
		{
			RefreshAll();
		}
	}
}

void MainWindow::OnBtnTilesCreate(wxRibbonButtonBarEvent& event)
{
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
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
#endif
}

void MainWindow::OnBtnTilesDelete(wxRibbonButtonBarEvent& event)
{
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
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
#endif
}

void MainWindow::OnBtnTilesCleanup(wxRibbonButtonBarEvent& event)
{
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
	if(m_project.get())
	{
		if(m_project->CleanupTiles())
		{
			RefreshAll();
		}
	}
#endif
}

void MainWindow::OnBtnColMapClear(wxRibbonButtonBarEvent& event)
{
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
	if(m_project.get())
	{
		m_project->GetEditingCollisionMap().Clear();
		RefreshAll();
	}
#endif
}

void MainWindow::OnBtnColGenTerrainBezier(wxRibbonButtonBarEvent& event)
{
#if !BEEHIVE_PLUGIN_LUMINARY
	if(m_project.get())
	{
		DialogTerrainGen dialog(this);

		if (dialog.ShowModal() == wxID_OK)
		{
			int granularity = dialog.m_spinCtrlGranularity->GetValue();

			if (wxMessageBox("This will clear all terrain tiles, are you sure?", "Generate Terrain", wxOK | wxCANCEL) == wxOK)
			{
				//Heights only legacy mode
				if (!m_project->GenerateTerrainFromBeziers_HeightsOnly(granularity))
				{
					wxMessageBox("Error generating terrain - out of tile space", "Error", wxOK, this);
				}

				RefreshAll();
			}
		}
	}
#endif
}

void MainWindow::OnBtnColTilesCleanup(wxRibbonButtonBarEvent& event)
{
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
	if(m_project.get())
	{
		if(m_project->CleanupTerrainTiles(true))
		{
			RefreshAll();
		}
	}
#endif
}

void MainWindow::OnBtnColTilesCreate(wxRibbonButtonBarEvent& event)
{
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
	if(m_project.get())
	{
		//Add new collision tile
		TerrainTileId tileId = m_project->GetTerrainTileset().AddTerrainTile();

		if(tileId == InvalidTerrainTileId)
		{
			wxMessageBox("Max terrain tiles reached", "Error", wxOK, this);
		}
		else
		{
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
#endif
}

void MainWindow::OnBtnColTilesDelete(wxRibbonButtonBarEvent& event)
{
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
	if(m_project.get())
	{
		TerrainTileId tileId = m_project->GetPaintTerrainTile();
		if(tileId != InvalidTerrainTileId && tileId != m_project->GetDefaultTerrainTile())
		{
			CollisionMap& map = m_project->GetEditingCollisionMap();
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
#endif
}

void MainWindow::OnBtnToolsMapEdit( wxRibbonButtonBarEvent& event )
{
	ShowToolboxStamps();
	ShowToolboxObjects();

#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
	ShowToolboxTiles();
	ShowToolboxCollision();
#endif
}

void MainWindow::OnBtnToolsTiles( wxRibbonButtonBarEvent& event )
{
	ShowPanelTiles();
}

void MainWindow::OnBtnToolsCollisionTiles(wxRibbonButtonBarEvent& event)
{
	ShowPanelTerrainTiles();
}

void MainWindow::OnBtnToolsStamps(wxRibbonButtonBarEvent& event)
{
	ShowPanelStamps();
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

void MainWindow::OnBtnToolsTimeline(wxRibbonButtonBarEvent& event)
{
	ShowPanelTimeline();
}

void MainWindow::OnBtnMapClear(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		m_project->GetEditingMap().Clear();
		m_project->GetEditingCollisionMap().Clear();
		RefreshPanel(ePanelMap);
	}
}

void MainWindow::OnBtnMapResize(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		Map& map = m_project->GetEditingMap();
		CollisionMap& collisionMap = m_project->GetEditingCollisionMap();
		const PlatformConfig& config = m_project->GetPlatformConfig();

#if BEEHIVE_FIXED_STAMP_MODE
		const int originalWidth = map.GetWidth() / config.stampWidth;
		const int originalHeight = map.GetHeight() / config.stampHeight;
#else
		const int originalWidth = map.GetWidth();
		const int originalHeight = map.GetHeight();
#endif

		const int tileWidth = m_project->GetPlatformConfig().tileWidth;
		const int tileHeight = m_project->GetPlatformConfig().tileHeight;

		DialogMapSize dialog(this);
		dialog.m_spinCtrlWidth->SetValue(originalWidth);
		dialog.m_spinCtrlHeight->SetValue(originalHeight);

		if(dialog.ShowModal() == wxID_OK)
		{
#if BEEHIVE_FIXED_STAMP_MODE
			int width = dialog.m_spinCtrlWidth->GetValue() * config.stampWidth;
			int height = dialog.m_spinCtrlHeight->GetValue() * config.stampHeight;
#else
			int width = dialog.m_spinCtrlWidth->GetValue();
			int height = dialog.m_spinCtrlHeight->GetValue();
#endif

			bool shiftRight = dialog.m_radioBoxShiftX->GetSelection() != 0;
			bool shiftDown = dialog.m_radioBoxShiftY->GetSelection() != 0;

			if(width > 0 && width <= 10000 && height > 0 && height <= 10000)
			{
				const PlatformConfig& config = m_project->GetPlatformConfig();

				if (config.stampWidth > 0)
				{
					width = ion::maths::RoundUpToNearest(width, config.stampWidth);
				}

				if (config.stampHeight > 0)
				{
					height = ion::maths::RoundUpToNearest(height, config.stampHeight);
				}

				//Resize map
				map.Resize(width, height, shiftRight, shiftDown);
				collisionMap.Resize(width, height, shiftRight, shiftDown);

				//Repaint all
				m_project->InvalidateTerrainBeziers(true);

				//Refresh map panel
				RefreshPanel(ePanelMap);

				m_project->InvalidateTerrainBeziers(false);
			}
		}
	}
}

void MainWindow::OnBtnMapExportBMP(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		wxDirDialog dialog(this);
		if(dialog.ShowModal() == wxID_OK)
		{
			m_project->ExportMapBitmaps(dialog.GetPath().c_str().AsChar());
		}
	}
}

void MainWindow::OnBtnSaveLayout(wxRibbonButtonBarEvent& event)
{
	SaveWindowLayout();
}

void MainWindow::OnBtnGridShow(wxCommandEvent& event)
{
	if(m_project.get())
	{
		m_project->SetShowGrid(!m_project->GetShowGrid());
		RedrawPanel(ePanelMap);
		RedrawPanel(ePanelStamps);
		RedrawPanel(ePanelTileEditor);
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
		RedrawPanel(ePanelMap);
		RedrawPanel(ePanelStamps);
	}
}

void MainWindow::OnBtnShowCollision(wxCommandEvent& event)
{
	if(m_project.get())
	{
		m_project->SetShowCollision(!m_project->GetShowCollision());
		RedrawPanel(ePanelMap);
	}
}

void MainWindow::OnBtnShowDisplayFrame(wxCommandEvent& event)
{
	if(m_project.get())
	{
		m_project->SetShowDisplayFrame(!m_project->GetShowDisplayFrame());
		RedrawPanel(ePanelMap);
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
		GameObjectTypeDialog dialog(*this, *m_project, *m_renderResources);
		dialog.ShowModal();
	}
}

void MainWindow::OnBtnTool(wxCommandEvent& event)
{
	if(m_mapPanel)
	{
		switch(event.GetId())
		{
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
		case wxID_TOOL_PAINT:
			m_mapPanel->SetTool(eToolPaintTile);
			break;
		case wxID_TOOL_COL_PAINTTERRAIN:
			m_mapPanel->SetTool(eToolPaintCollisionTerrain);
			break;
		case wxID_TOOL_COL_PAINTSOLID:
			m_mapPanel->SetTool(eToolPaintCollisionSolid);
			break;
		case wxID_TOOL_COL_PAINTHOLE:
			m_mapPanel->SetTool(eToolPaintCollisionHole);
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
		case wxID_TOOL_COL_DELETETERRAINBEZIER:
			m_mapPanel->SetTool(eToolDeleteTerrainBezier);
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
		case wxID_TOOL_COPY_TO_NEW_MAP:
			m_mapPanel->SetTool(eToolCopyToNewMap);
			break;
		case wxID_TOOL_CREATESTAMP:
			m_mapPanel->SetTool(eToolCreateStamp);
			break;
#endif
		case wxID_TOOL_SELECTTILE:
			m_mapPanel->SetTool(eToolSelectTiles);
			break;
		case wxID_TOOL_SELECTSTAMP:
			m_mapPanel->SetTool(eToolSelectStamp);
			break;
		case wxID_TOOL_STAMP:
			m_mapPanel->SetTool(eToolPaintStamp);
			break;
		case wxID_TOOL_TILEPICKER:
			m_mapPanel->SetTool(eToolTilePicker);
			break;
		case wxID_TOOL_STAMPPICKER:
			m_mapPanel->SetTool(eToolStampPicker);
			break;
		case wxID_TOOL_CREATE_SCENE_ANIM:
			m_mapPanel->SetTool(eToolCreateStampAnim);
			break;
		case wxID_TOOL_MOVESTAMP:
			m_mapPanel->SetTool(eToolMoveStamp);
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
		case wxID_TOOL_DUPLICATEGAMEOBJ:
			m_mapPanel->SetTool(eToolDuplicateGameObject);
			break;
		case wxID_TOOL_DRAWGAMEOBJ:
			m_mapPanel->SetTool(eToolDrawGameObject);
			break;
		case wxID_TOOL_MOVEGAMEOBJ:
			m_mapPanel->SetTool(eToolMoveGameObject);
			break;
		case wxID_TOOL_ANIMATEGAMEOBJ:
			m_mapPanel->SetTool(eToolAnimateGameObject);
			break;
		case wxID_TOOL_REMOVEGAMEOBJ:
			m_mapPanel->SetTool(eToolRemoveGameObject);
			break;
		}
	}
}