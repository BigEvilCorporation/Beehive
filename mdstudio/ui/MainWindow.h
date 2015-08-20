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
#include "CollisionTypePanel.h"
#include "CollisionTilesPanel.h"
#include "CollisionEditorPanel.h"

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
		ePanelCollisionTiles,
		ePanelCollisionTypes,
		ePanelCollisionTileEditor
	};

	MainWindow();
	virtual ~MainWindow();

	//Open panels
	void ShowPanelPalettes();
	void ShowPanelTiles();
	void ShowPanelStamps();
	void ShowPanelMap();
	void ShowPanelTileEditor();
	void ShowPanelCollisionTiles();
	void ShowPanelCollisionTypes();
	void ShowPanelCollisionEditor();

	//Open toolbox
	void ShowPanelToolbox();

	//Set current map editing tool
	void SetMapTool(ToolType tool);

	//Sync project settings and their respective UI widget states
	void SyncSettingsWidgets();

	//Refresh/redraw panels
	void RefreshPanel(Panel panel);
	void RedrawPanel(Panel panel);
	void RefreshAll();
	void RedrawAll();

	//Refresh tileset
	void RefreshTileset();

	//Refresh collision tileset
	void RefreshCollisionTileset();

	//Refresh collision types
	void RefreshCollisionTypes();

protected:
	virtual void OnBtnProjNew(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjOpen(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjSave(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjExport(wxRibbonButtonBarEvent& event);
	virtual void OnBtnTilesImport(wxRibbonButtonBarEvent& event);
	virtual void OnBtnTilesCreate(wxRibbonButtonBarEvent& event);
	virtual void OnBtnTilesDelete(wxRibbonButtonBarEvent& event);
	virtual void OnBtnColTilesCreate(wxRibbonButtonBarEvent& event);
	virtual void OnBtnColTilesDelete(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsMapEdit(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsTiles(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsStamps(wxRibbonButtonBarEvent& event) { event.Skip(); }
	virtual void OnBtnToolsPalettes(wxRibbonButtonBarEvent& event);
	virtual void OnBtnMapClear(wxRibbonButtonBarEvent& event);
	virtual void OnBtnMapResize(wxRibbonButtonBarEvent& event);
	virtual void OnBtnGridShow(wxCommandEvent& event);
	virtual void OnBtnGridSnap(wxCommandEvent& event);
	virtual void OnBtnShowOutlines(wxCommandEvent& event);
	virtual void OnBtnCollisionTileEdit(wxRibbonButtonBarEvent& event);

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
	wxWeakRef<CollisionTilesPanel> m_collisionTilesPanel;
	wxWeakRef<CollisionTypePanel> m_collisionTypesPanel;
	wxWeakRef<CollisionEditorPanel> m_collisionEditorPanel;

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
