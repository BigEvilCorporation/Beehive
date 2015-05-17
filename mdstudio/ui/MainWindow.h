///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <wx/aui/aui.h>

#include <ion/renderer/Renderer.h>

#include "UIBase.h"

#include "PalettesPanel.h"
#include "TilesPanel.h"
#include "StampsPanel.h"
#include "MapPanel.h"
#include "TileEditorPanel.h"

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
		ePanelTileEditor
	};

	MainWindow();
	virtual ~MainWindow();

	//Open panels
	void ShowPanelPalettes();
	void ShowPanelTiles();
	void ShowPanelStamps();
	void ShowPanelMap();

	//Open toolbox
	void ShowPanelToolbox();

	//Open tile editor
	void EditTile(TileId tileId);

	//Set current map editing tool
	void SetMapTool(MapPanel::Tool tool);

	//Sync project settings and their respective UI widget states
	void SyncSettingsWidgets();

	//Refresh panels
	void RefreshPanel(Panel panel);
	void RefreshAll();

protected:
	virtual void OnBtnProjNew(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjOpen(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjSave(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjSaveAs(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjExport(wxRibbonButtonBarEvent& event);
	virtual void OnBtnTilesImport( wxRibbonButtonBarEvent& event );
	virtual void OnBtnTilesDelete( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnToolsMapEdit( wxRibbonButtonBarEvent& event );
	virtual void OnBtnToolsTiles( wxRibbonButtonBarEvent& event );
	virtual void OnBtnToolsStamps( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnToolsPalettes( wxRibbonButtonBarEvent& event );
	virtual void OnBtnMapClear(wxRibbonButtonBarEvent& event);
	virtual void OnBtnMapResize(wxRibbonButtonBarEvent& event);
	virtual void OnBtnGridShow(wxCommandEvent& event);
	virtual void OnBtnGridSnap(wxCommandEvent& event);

	void OnBtnTool(wxCommandEvent& event);

private:
	void SetProject(Project* project);

	wxAuiManager m_auiManager;

	wxWeakRef<PalettesPanel> m_palettesPanel;
	wxWeakRef<TilesPanel> m_tilesPanel;
	wxWeakRef<StampsPanel> m_stampsPanel;
	wxWeakRef<MapPanel> m_mapPanel;
	wxWeakRef<TileEditorPanel> m_tileEditorPanel;
	wxWeakRef<MapToolbox> m_toolboxPanel;

	ProjectPtr m_project;

	//Renderer
	ion::render::Renderer* m_renderer;

	//OpenGL context
	wxGLContext* m_context;

	//Blank canvas for creating gobal DC
	wxWeakRef<wxGLCanvas> m_blankCanvas;
};
