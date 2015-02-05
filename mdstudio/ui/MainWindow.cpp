#pragma once

#include "MainWindow.h"
#include "MapPanel.h"

MainWindow::MainWindow()
	: MainWindowBase(NULL)
{
	m_project = NULL;
	m_toolboxMapEdit = NULL;
}

MainWindow::~MainWindow()
{
}

void MainWindow::SetProject(Project* project)
{
	if(project && project != m_project)
	{
		m_project = project;
		m_mapPanel->SetProject(project);

		//New project, open default toolboxes
		ShowToolboxTiles();
		ShowToolboxMapEdit();
	}
	else
	{
		//Project closed, close toolboxes
		if(m_toolboxMapEdit)
			delete m_toolboxMapEdit;

		if(m_toolboxTiles)
			delete m_toolboxTiles;

		m_mapPanel->SetProject(NULL);
		m_project = NULL;
	}
}

void MainWindow::ShowToolboxTiles()
{
	if(m_project)
	{
		if(!m_toolboxTiles)
		{
			m_toolboxTiles = new ToolboxTiles(*m_project, this);
		}

		if(!m_toolboxTiles->IsShown())
		{
			m_toolboxTiles->Show();
		}
	}
}

void MainWindow::ShowToolboxMapEdit()
{
	if(m_project)
	{
		if(!m_toolboxMapEdit)
		{
			m_toolboxMapEdit = new ToolboxMapEdit(*m_project, this);
		}

		if(!m_toolboxMapEdit->IsShown())
		{
			m_toolboxMapEdit->Show();
		}
	}
}

void MainWindow::OnBtnToolsMapEdit( wxRibbonButtonBarEvent& event )
{
	ShowToolboxMapEdit();
}

void MainWindow::OnBtnToolsTiles( wxRibbonButtonBarEvent& event )
{
	ShowToolboxTiles();
}