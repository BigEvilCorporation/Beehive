///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <wx/aui/aui.h>

#include <ion/io/ResourceManager.h>

#include "UIBase.h"

#include "PalettesPanel.h"
#include "TilesPanel.h"
#include "MapPanel.h"

#include "../Project.h"

class MainWindow : public MainWindowBase
{
public:
	MainWindow(ion::io::ResourceManager& resourceManager);
	virtual ~MainWindow();

	void SetProject(Project* project);

	void ShowPanelPalettes();
	void ShowPanelTiles();
	void ShowPanelMap();
	void ShowPanelToolbox();

	void SyncSettingsWidgets();
	void RefreshAll();

protected:
	virtual void OnBtnProjNew( wxRibbonButtonBarEvent& event ) { event.Skip(); }
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
	virtual void OnBtnGridShow(wxCommandEvent& event);
	virtual void OnBtnGridSnap(wxCommandEvent& event);

	void OnBtnTool(wxCommandEvent& event);

private:
	ion::io::ResourceManager& m_resourceManager;

	wxAuiManager m_auiManager;

	wxWeakRef<PalettesPanel> m_palettesPanel;
	wxWeakRef<TilesPanel> m_tilesPanel;
	wxWeakRef<MapPanel> m_mapPanel;
	wxWeakRef<MapToolbox> m_toolboxPanel;

	Project* m_project;
};
