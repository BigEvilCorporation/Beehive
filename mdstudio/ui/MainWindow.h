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

#include <wx/aui/aui.h>

#include <ion/renderer/Renderer.h>
#include <ion/renderer/Texture.h>
#include <ion/io/Filesystem.h>
#include <ion/io/ResourceManager.h>
#include <ion/beehive/Project.h>

#include "UIBase.h"
#include "RenderResources.h"
#include "Tool.h"

#include "SceneExplorerPanel.h"
#include "PropertyPanel.h"
#include "ScriptCompilePanel.h"
#include "PalettesPanel.h"
#include "TilesPanel.h"
#include "StampsPanel.h"
#include "BlocksPanel.h"
#include "MapPanel.h"
#include "MapListPanel.h"
#include "TileEditorPanel.h"
#include "TerrainTilesPanel.h"
#include "TerrainTileEditorPanel.h"
#include "GameObjectTypeDialog.h"
#include "GameObjectTypePanel.h"
#include "GameObjectParamsPanel.h"
#include "TimelinePanel.h"

wxDECLARE_SCOPED_PTR(Project, ProjectPtr)

class MainWindow : public MainWindowBase
{
public:
	enum Panel
	{
		ePanelMap,
		ePanelMapList,
		ePanelStamps,
		ePanelBlocks,
		ePanelTiles,
		ePanelPalettes,
		ePanelTileEditor,
		ePanelTerrainTiles,
		ePanelTerrainTileEditor,
		ePanelGameObjectTypes,
		ePanelGameObjectParams,
		ePanelSceneExplorer,
		ePanelProperties,
		ePanelScriptCompile
	};

	MainWindow();
	virtual ~MainWindow();

	//Open panels
	void ShowPanelSceneExplorer();
	void ShowPanelProperties();
	void ShowPanelScriptCompile();
	void ShowPanelPalettes();
	void ShowPanelTiles();
	void ShowPanelStamps();
	void ShowPanelBlocks();
	void ShowPanelMap();
	void ShowPanelMapList();
	void ShowPanelTileEditor();
	void ShowPanelTerrainTiles();
	void ShowPanelTerrainEditor();
	void ShowPanelGameObjectTypes();
	void ShowPanelGameObjectParams();
	void ShowPanelTimeline();

	//Open toolbox
	void ShowToolboxTiles();
	void ShowToolboxCollision();
	void ShowToolboxStamps();
	void ShowToolboxObjects();

	//Get panels
	PropertyPanel* GetPropertiesPanel();
	MapPanel* GetMapPanel();
	ScriptCompilePanel* GetScriptCompilePanel();

	//Set current map editing tool
	void SetMapTool(ToolType tool);

	//Set selected game object
	void SetSelectedGameObject(GameObject* gameObject);

	//Get selected animation
	AnimationId GetSelectedAnimation();

	//Set selected animation object
	void SetSelectedAnimObject(GameObjectId gameObjectId);

	//Refresh anim actors list
	void RefreshAnimActors();

	//Sync project settings and their respective UI widget states
	void SyncSettingsWidgets();

	//Refresh/redraw panels
	void RefreshPanel(Panel panel);
	void RedrawPanel(Panel panel);
	void RefreshAll();
	void RedrawAll();

	//Refresh tileset
	void RefreshTileset();

	//Refresh terrain tileset
	void RefreshTerrainTileset();

	//Refresh sprite sheets
	void RefreshSpriteSheets();

	//Panel refresh lock (to speed up initialisation)
	void LockRefresh();
	void UnlockRefresh();
	bool IsRefreshLocked() const;

	void OnPostInit();

protected:
	virtual void OnBtnProjNew(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjOpen(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjSave(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjSettings(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjExport(wxRibbonButtonBarEvent& event);
	virtual void OnBtnTilesImport(wxRibbonButtonBarEvent& event);
	virtual void OnBtnTilesCreate(wxRibbonButtonBarEvent& event);
	virtual void OnBtnTilesDelete(wxRibbonButtonBarEvent& event);
	virtual void OnBtnTilesCleanup(wxRibbonButtonBarEvent& event);
	virtual void OnBtnSpriteEditor(wxRibbonButtonBarEvent& event);
	virtual void OnBtnStampsImport(wxRibbonButtonBarEvent& event);
	virtual void OnBtnStampsExportBMPs(wxRibbonButtonBarEvent& event);
	virtual void OnBtnStampsCleanup(wxRibbonButtonBarEvent& event);
	virtual void OnBtnColMapClear(wxRibbonButtonBarEvent& event);
	virtual void OnBtnColGenTerrainBezier(wxRibbonButtonBarEvent& event);
	virtual void OnBtnColTilesCreate(wxRibbonButtonBarEvent& event);
	virtual void OnBtnColTilesDelete(wxRibbonButtonBarEvent& event);
	virtual void OnBtnColTilesCleanup(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsMapEdit(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsMapList(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsTiles(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsCollisionTiles(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsStamps(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsPalettes(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsGameObjs(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsGameObjParams(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsTimeline(wxRibbonButtonBarEvent& event);
	virtual void OnBtnMapClear(wxRibbonButtonBarEvent& event);
	virtual void OnBtnMapResize(wxRibbonButtonBarEvent& event);
	virtual void OnBtnMapExportBMP(wxRibbonButtonBarEvent& event);
	virtual void OnBtnSaveLayout(wxRibbonButtonBarEvent& event);
	virtual void OnBtnGridShow(wxCommandEvent& event);
	virtual void OnBtnGridSnap(wxCommandEvent& event);
	virtual void OnBtnShowOutlines(wxCommandEvent& event);
	virtual void OnBtnShowCollision(wxCommandEvent& event);
	virtual void OnBtnShowDisplayFrame(wxCommandEvent& event);
	virtual void OnBtnTerrainTileEdit(wxRibbonButtonBarEvent& event);
	virtual void OnBtnGameObjTypes(wxRibbonButtonBarEvent& event);

	void OnBtnTool(wxCommandEvent& event);

	void EventHandlerKeyboard(wxKeyEvent& event);

private:
	//Set current project (opens default panels)
	void SetProject(Project* project);

	//Set all panel captions
	void SetPanelCaptions();

	//Save/restore layout
	void SaveWindowLayout();
	void RestoreWindowLayout();
	wxString GetWindowLayoutConfig() const;

	wxAuiManager m_auiManager;

	wxWeakRef<MapToolboxTiles> m_toolboxPanelTiles;
	wxWeakRef<MapToolboxStamps> m_toolboxPanelStamps;
	wxWeakRef<MapToolboxTerrain> m_toolboxPanelTerrain;
	wxWeakRef<MapToolboxGameObjs> m_toolboxPanelGameObjs;
	wxWeakRef<SceneExplorerPanel> m_sceneExplorerPanel;
	wxWeakRef<PropertyPanel> m_propertyPanel;
	wxWeakRef<ScriptCompilePanel> m_scriptCompilePanel;
	wxWeakRef<PalettesPanel> m_palettesPanel;
	wxWeakRef<MapPanel> m_mapPanel;
	wxWeakRef<MapListPanel> m_mapListPanel;
	wxWeakRef<TilesPanel> m_tilesPanel;
	wxWeakRef<StampsPanel> m_stampsPanel;
	wxWeakRef<BlocksPanel> m_blocksPanel;
	wxWeakRef<TileEditorPanel> m_tileEditorPanel;
	wxWeakRef<TerrainTilesPanel> m_terrainTilesPanel;
	wxWeakRef<TerrainTileEditorPanel> m_TerrainTileEditorPanel;
	wxWeakRef<GameObjectTypesPanel> m_gameObjectTypePanel;
	wxWeakRef<GameObjectParamsPanel> m_gameObjectParamsPanel;
	wxWeakRef<TimelinePanel> m_timelinePanel;

	ProjectPtr m_project;

	//Filesystem
	ion::io::FileSystem m_fileSystem;

	//Resource manager
	ion::io::ResourceManager* m_resourceManager;

	//Renderer
	ion::render::Renderer* m_renderer;

	//OpenGL context
	wxGLContext* m_context;

	//Blank canvas for creating gobal DC
	wxWeakRef<wxGLCanvas> m_blankCanvas;

	//Rendering resources
	RenderResources* m_renderResources;

	//Refresh lock
	int m_refreshLockStack;

	//Window total width/height
	int m_width;
	int m_height;
};
