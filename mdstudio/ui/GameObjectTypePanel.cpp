///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "GameObjectTypePanel.h"

GameObjTypesPanel::GameObjTypesPanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: GameObjTypesPanelBase(parent, id, pos, size, style)
	, m_project(project)
	, m_mainWindow(mainWindow)
{

}

void GameObjTypesPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	m_listGameObjTypes->Clear();w
	const TGameObjectTypeMap& types = m_project.GetGameObjectTypes();

	for(int i = 0; i < types.size(); i++)
	{
		m_listGameObjTypes->Insert(wxString(types[i].m_name), i);
	}
}

void GameObjTypesPanel::OnGameObjectTypeSelected(wxCommandEvent& event)
{
	const TGameObjectTypeMap& types = m_project.GetGameObjectTypes();
	GameObjectTypeId selection = types[event.GetSelection()].GetId();
	m_project->SetPaintGameObjectType(selection);
	m_mainWindow->SetMapTool(eToolPlaceGameObject);
}