#include "MainWindow.h"

MainWindow::MainWindow()
	: MainWindowTemplate(NULL)
{
	m_toolboxMapEdit = NULL;
	m_toolboxTiles = new ToolboxTilesTemplate(this);
	m_toolboxTiles->Show();
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnBtnToolsMapEdit( wxRibbonButtonBarEvent& event )
{
	if(!m_toolboxMapEdit)
	{
		m_toolboxMapEdit = new ToolboxMapEditTemplate(this);
	}

	if(!m_toolboxMapEdit->IsShown())
	{
		m_toolboxMapEdit->Show();
	}
}

void MainWindow::OnBtnToolsTiles( wxRibbonButtonBarEvent& event )
{
	if(!m_toolboxTiles)
	{
		m_toolboxTiles = new ToolboxTilesTemplate(this);
	}

	if(!m_toolboxTiles->IsShown())
	{
		m_toolboxTiles->Show();
	}
}