#pragma once

#include "MainWindowTemplate.h"

class MainWindow : public MainWindowTemplate 
{
public:
	MainWindow();
	virtual ~MainWindow();

protected:
	virtual void OnBtnProjNew( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnProjSave( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnProjExport( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnTilesImport( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnTilesDelete( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnToolsMapEdit( wxRibbonButtonBarEvent& event );
	virtual void OnBtnToolsTiles( wxRibbonButtonBarEvent& event ) { event.Skip(); }
	virtual void OnBtnToolsStamps( wxRibbonButtonBarEvent& event ) { event.Skip(); }

private:
	wxWeakRef<ToolboxMapEditTemplate> m_toolboxMapEdit;
};
