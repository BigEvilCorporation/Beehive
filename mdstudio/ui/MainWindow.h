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
#include <ion/core/io/Filesystem.h>
#include <ion/resource/ResourceManager.h>
#include <ion/beehive/Project.h>

#include "UIBase.h"
#include "RenderResources.h"
#include "MapTool.h"

#include "SceneExplorerPanel.h"
#include "PropertyPanel.h"
#include "ScriptCompilePanel.h"
#include "AssemblerPanel.h"
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
		ePanelScriptCompile,
		ePanelAnimation
	};

	//OpenGL setup
	static wxGLAttributes GetGLAttributes();

	MainWindow();
	virtual ~MainWindow();

	//Open panels
	void ShowPanelSceneExplorer();
	void ShowPanelProperties();
	void ShowPanelScriptCompile();
	void ShowPanelAssembler();
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
	AssemblerPanel* GetAssemblerPanel();

	//Set current map editing tool
	void SetMapTool(ToolType tool);

	//Set selected game object
	void SetSelectedGameObject(GameObject* gameObject);
	void SetSelectedGameObjectType(GameObjectType* gameObjectType);
	void SetSelectedPrefabChild(GameObjectType* rootObjectType, GameObject* rootObject, GameObjectType* childObjectType, GameObjectId childInstanceId);

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

	//Scan project directories
	void ScanProject(const std::string& engineDir, const std::string& projectDir);
	void ScanStamps(const std::string& stampsDir);

	void OnPostInit();

protected:
	virtual void OnMenuToolsTweaksGameObjCentreOrigin(wxCommandEvent& event);
	virtual void OnMenuAnimationImport(wxCommandEvent& event);
	virtual void OnMenuAnimationExport(wxCommandEvent& event);
	virtual void OnMenuAnimationConvertToRelativeCoords(wxCommandEvent& event);
	virtual void OnMenuAnimationBindPrefabs(wxCommandEvent& event);

	virtual void OnBtnProjNew(wxCommandEvent& event);
	virtual void OnBtnProjOpen(wxCommandEvent& event);
	virtual void OnBtnProjSave(wxCommandEvent& event);
	virtual void OnBtnProjSettings(wxCommandEvent& event);
	virtual void OnBtnBuildExport(wxCommandEvent& event);
	virtual void OnBtnBuildAssemble(wxCommandEvent& event);
	virtual void OnBtnBuildRun(wxCommandEvent& event);
	virtual void OnBtnTilesImport(wxCommandEvent& event);
	virtual void OnBtnTilesCreate(wxCommandEvent& event);
	virtual void OnBtnTilesDelete(wxCommandEvent& event);
	virtual void OnBtnTilesCleanup(wxCommandEvent& event);
	virtual void OnBtnSpriteEditor(wxCommandEvent& event);
	virtual void OnBtnStampsImport(wxCommandEvent& event);
	virtual void OnBtnStampsUpdate(wxCommandEvent& event);
	virtual void OnBtnStampsExportBMPs(wxCommandEvent& event);
	virtual void OnBtnStampsCleanup(wxCommandEvent& event);
	virtual void OnBtnColMapClear(wxCommandEvent& event);
	virtual void OnBtnColGenTerrainBezier(wxCommandEvent& event);
	virtual void OnBtnColTilesCreate(wxCommandEvent& event);
	virtual void OnBtnColTilesDelete(wxCommandEvent& event);
	virtual void OnBtnColTilesCleanup(wxCommandEvent& event);
	virtual void OnBtnToolsMapEdit(wxCommandEvent& event);
	virtual void OnBtnToolsMapList(wxCommandEvent& event);
	virtual void OnBtnToolsTiles(wxCommandEvent& event);
	virtual void OnBtnToolsCollisionTiles(wxCommandEvent& event);
	virtual void OnBtnToolsStamps(wxCommandEvent& event);
	virtual void OnBtnToolsPalettes(wxCommandEvent& event);
	virtual void OnBtnToolsGameObjs(wxCommandEvent& event);
	virtual void OnBtnToolsGameObjParams(wxCommandEvent& event);
	virtual void OnBtnToolsTimeline(wxCommandEvent& event);
	virtual void OnBtnMapNew(wxCommandEvent& event);
	virtual void OnBtnMapImport(wxCommandEvent& event);
	virtual void OnBtnMapCopy(wxCommandEvent& event);
	virtual void OnBtnMapDelete(wxCommandEvent& event);
	virtual void OnBtnMapRename(wxCommandEvent& event);
	virtual void OnBtnMapClear(wxCommandEvent& event);
	virtual void OnBtnMapResize(wxCommandEvent& event);
	virtual void OnBtnMapExportBMP(wxCommandEvent& event);
	virtual void OnBtnSaveLayout(wxCommandEvent& event);
	virtual void OnBtnGridShow(wxCommandEvent& event);
	virtual void OnBtnGridSnap(wxCommandEvent& event);
	virtual void OnBtnShowOutlines(wxCommandEvent& event);
	virtual void OnBtnShowCollision(wxCommandEvent& event);
	virtual void OnBtnShowDisplayFrame(wxCommandEvent& event);
	virtual void OnBtnTerrainTileEdit(wxCommandEvent& event);
	virtual void OnBtnGameObjTypes(wxCommandEvent& event);

	void OnBtnTool(wxCommandEvent& event);

	void EventHandlerKeyboard(wxKeyEvent& event);

private:
	//Load project from file
	bool LoadProject(const std::string& filename);

	//Set current project (opens default panels)
	void SetProject(Project* project);

	//Set all panel captions
	void SetPanelCaptions();

	//Save/restore layout
	void SaveWindowLayout();
	void RestoreWindowLayout();
	wxString GetWindowLayoutConfig() const;

	//Recent files
	void AddRecentProject(const std::string& project);
	void RestoreRecentProjects();
	void SaveRecentProjects();
	wxString GetRecentFilesConfig() const;
	void CreateRecentProjectsMenu();
	void OnMenuRecentProjects(wxCommandEvent& event);

	//Build project
	void Build(bool exportProj, bool assemble, bool run);

	//OpenGL attributes for GL canvasses
	static wxGLAttributes s_glAttributes;

	wxAuiManager m_auiManager;

	wxWeakRef<MapToolboxTiles> m_toolboxPanelTiles;
	wxWeakRef<MapToolboxStamps> m_toolboxPanelStamps;
	wxWeakRef<MapToolboxTerrain> m_toolboxPanelTerrain;
	wxWeakRef<MapToolboxGameObjs> m_toolboxPanelGameObjs;
	wxWeakRef<SceneExplorerPanel> m_sceneExplorerPanel;
	wxWeakRef<PropertyPanel> m_propertyPanel;
	wxWeakRef<ScriptCompilePanel> m_scriptCompilePanel;
	wxWeakRef<AssemblerPanel> m_assemblerPanel;
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

	//Recently opened projects
	static const int s_maxRecentProjects = 8;
	int m_recentProjectsMenuCount;
	std::vector<std::string> m_recentProjects;

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
