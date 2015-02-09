///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "UIBase.h"
#include "ToolboxTiles.h"
#include "ToolboxMapEdit.h"
#include "ToolboxPalettes.h"

#include "../Project.h"

class MainWindow : public MainWindowBase
{
public:
	MainWindow();
	virtual ~MainWindow();

	void SetProject(Project* project);

	void ShowToolboxTiles();
	void ShowToolboxMapEdit();
	void ShowToolboxPalettes();

protected:
	virtual void OnBtnProjNew( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnProjSave( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnProjExport( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnTilesImport( wxRibbonButtonBarEvent& event );
	virtual void OnBtnTilesDelete( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnToolsMapEdit( wxRibbonButtonBarEvent& event );
	virtual void OnBtnToolsTiles( wxRibbonButtonBarEvent& event );
	virtual void OnBtnToolsStamps( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnToolsPalettes( wxRibbonButtonBarEvent& event );

private:
	wxWeakRef<ToolboxMapEdit> m_toolboxMapEdit;
	wxWeakRef<ToolboxTiles> m_toolboxTiles;
	wxWeakRef<ToolboxPalettes> m_toolboxPalettes;

	Project* m_project;
};
