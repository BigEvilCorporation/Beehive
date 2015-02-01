#include "MainWindow.h"

MainWindow::MainWindow()
	: MainWindowTemplate(NULL)
{
	m_toolboxMapEdit = NULL;
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