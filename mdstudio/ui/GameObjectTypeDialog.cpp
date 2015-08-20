///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "GameObjectTypeDialog.h"

GameObjectTypeDialog::GameObjectTypeDialog(MainWindow& mainWindow, Project& project)
	: GameObjTypeDialogBase((wxWindow*)&mainWindow)
	, m_mainWindow(mainWindow)
	, m_project(project)
{
	m_currentType = NULL;
}

void GameObjectTypeDialog::OnToolGameObjAdd(wxCommandEvent& event)
{
	m_currentType = &m_project->AddGameObjectType("GameObj_");
	PopulateList();
}

void GameObjectTypeDialog::OnToolGameObjRemove(wxCommandEvent& event)
{
	if(m_currentType)
	{
		m_project->RemoveGameObjectType(*m_currentType);
		m_currentType = NULL;
		PopulateList();
	}
}

void GameObjectTypeDialog::OnToolVariableAdd(wxCommandEvent& event)
{

}

void GameObjectTypeDialog::OnToolVariableRemove(wxCommandEvent& event)
{

}

void GameObjectTypeDialog::OnGameObjNameChange(wxCommandEvent& event)
{
	if(m_currentType)
	{

	}
}

void GameObjectTypeDialog::OnVariableNameChanged(wxCommandEvent& event)
{

}

void GameObjectTypeDialog::OnVariableSizeChanged(wxCommandEvent& event)
{

}

void GameObjectTypeDialog::OnValueChanged(wxCommandEvent& event)
{

}

void GameObjectTypeDialog::OnWidthChanged(wxSpinEvent& event)
{

}

void GameObjectTypeDialog::OnHeightChanged(wxSpinEvent& event)
{

}

void GameObjectTypeDialog::OnBtnImport(wxCommandEvent& event)
{

}

void GameObjectTypeDialog::OnBtnExport(wxCommandEvent& event)
{

}

void GameObjectTypeDialog::PopulateList()
{

}