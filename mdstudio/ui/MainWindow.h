#pragma once

#include "UIBase.h"
#include "ToolboxTiles.h"
#include "ToolboxMapEdit.h"

#include "../Project.h"

class MainWindow : public MainWindowBase
{
public:
	MainWindow();
	virtual ~MainWindow();

	void SetProject(Project* project);

	void ShowToolboxTiles();
	void ShowToolboxMapEdit();

protected:
	virtual void OnBtnProjNew( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnProjSave( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnProjExport( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnTilesImport( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnTilesDelete( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnToolsMapEdit( wxRibbonButtonBarEvent& event );
	virtual void OnBtnToolsTiles( wxRibbonButtonBarEvent& event );
	virtual void OnBtnToolsStamps( wxRibbonButtonBarEvent& event ) { event.Skip(); }

private:
	wxWeakRef<ToolboxMapEdit> m_toolboxMapEdit;
	wxWeakRef<ToolboxTiles> m_toolboxTiles;

	Project* m_project;
};
