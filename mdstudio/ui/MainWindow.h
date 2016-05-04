///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <wx/aui/aui.h>

#include <ion/renderer/Renderer.h>
#include <ion/renderer/Texture.h>

#include "UIBase.h"
#include "RenderResources.h"
#include "Tool.h"

#include "PalettesPanel.h"
#include "TilesPanel.h"
#include "StampsPanel.h"
#include "MapPanel.h"
#include "TileEditorPanel.h"
#include "TerrainTilesPanel.h"
#include "TerrainTileEditorPanel.h"
#include "GameObjectTypeDialog.h"
#include "GameObjectTypePanel.h"
#include "GameObjectParamsPanel.h"

#include "../Project.h"

wxDECLARE_SCOPED_PTR(Project, ProjectPtr)

class MainWindow : public MainWindowBase
{
public:
	enum Panel
	{
		ePanelMap,
		ePanelStamps,
		ePanelTiles,
		ePanelPalettes,
		ePanelTileEditor,
		ePanelTerrainTiles,
		ePanelTerrainTileEditor,
		ePanelGameObjectTypes,
		ePanelGameObjectParams
	};

	MainWindow();
	virtual ~MainWindow();

	//Open panels
	void ShowPanelPalettes();
	void ShowPanelTiles();
	void ShowPanelStamps();
	void ShowPanelMap();
	void ShowPanelTileEditor();
	void ShowPanelTerrainTiles();
	void ShowPanelTerrainEditor();
	void ShowPanelGameObjectTypes();
	void ShowPanelGameObjectParams();

	//Open toolbox
	void ShowPanelToolbox();

	//Set current map editing tool
	void SetMapTool(ToolType tool);

	//Set selected game object
	void SetSelectedGameObject(GameObject* gameObject);

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

protected:
	virtual void OnBtnProjNew(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjOpen(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjSave(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjExport(wxRibbonButtonBarEvent& event);
	virtual void OnBtnTilesImport(wxRibbonButtonBarEvent& event);
	virtual void OnBtnTilesCreate(wxRibbonButtonBarEvent& event);
	virtual void OnBtnTilesDelete(wxRibbonButtonBarEvent& event);
	virtual void OnBtnTilesCleanup(wxRibbonButtonBarEvent& event);
	virtual void OnBtnSpriteEditor(wxRibbonButtonBarEvent& event);
	virtual void OnBtnColMapClear(wxRibbonButtonBarEvent& event);
	virtual void OnBtnColGenTerrainBezier(wxRibbonButtonBarEvent& event);
	virtual void OnBtnColTilesCreate(wxRibbonButtonBarEvent& event);
	virtual void OnBtnColTilesDelete(wxRibbonButtonBarEvent& event);
	virtual void OnBtnColTilesCleanup(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsMapEdit(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsTiles(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsStamps(wxRibbonButtonBarEvent& event) { event.Skip(); }
	virtual void OnBtnToolsPalettes(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsGameObjs(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsGameObjParams(wxRibbonButtonBarEvent& event);
	virtual void OnBtnMapClear(wxRibbonButtonBarEvent& event);
	virtual void OnBtnMapResize(wxRibbonButtonBarEvent& event);
	virtual void OnBtnGridShow(wxCommandEvent& event);
	virtual void OnBtnGridSnap(wxCommandEvent& event);
	virtual void OnBtnShowOutlines(wxCommandEvent& event);
	virtual void OnBtnTerrainTileEdit(wxRibbonButtonBarEvent& event);
	virtual void OnBtnGameObjTypes(wxRibbonButtonBarEvent& event);

	void OnBtnTool(wxCommandEvent& event);

	void EventHandlerKeyboard(wxKeyEvent& event);

private:
	//Set current project (opens default panels)
	void SetProject(Project* project);

	//Set all panel captions
	void SetPanelCaptions();

	wxAuiManager m_auiManager;

	wxWeakRef<MapToolbox> m_toolboxPanel;
	wxWeakRef<PalettesPanel> m_palettesPanel;
	wxWeakRef<MapPanel> m_mapPanel;
	wxWeakRef<TilesPanel> m_tilesPanel;
	wxWeakRef<StampsPanel> m_stampsPanel;
	wxWeakRef<TileEditorPanel> m_tileEditorPanel;
	wxWeakRef<TerrainTilesPanel> m_terrainTilesPanel;
	wxWeakRef<TerrainTileEditorPanel> m_TerrainTileEditorPanel;
	wxWeakRef<GameObjectTypesPanel> m_gameObjectTypePanel;
	wxWeakRef<GameObjectParamsPanel> m_gameObjectParamsPanel;

	ProjectPtr m_project;

	//Renderer
	ion::render::Renderer* m_renderer;

	//OpenGL context
	wxGLContext* m_context;

	//Blank canvas for creating gobal DC
	wxWeakRef<wxGLCanvas> m_blankCanvas;

	//Rendering resources
	RenderResources* m_renderResources;
};
