///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#include "GameObjectTypePanel.h"
#include "MainWindow.h"

GameObjectTypesPanel::GameObjectTypesPanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: GameObjTypesPanelBase(parent, id, pos, size, style)
	, m_project(project)
	, m_mainWindow(mainWindow)
{

}

void GameObjectTypesPanel::PopulateGameObjects()
{
	m_listGameObjTypes->Clear();
	m_gameObjectTypeMap.clear();

	const TGameObjectTypeMap& types = m_project.GetGameObjectTypes();
	const TGameObjectPosMap& objectPosMap = m_project.GetEditingMap().GetGameObjects();

	int index = 0;
	int selectedIndex = -1;
	for (TGameObjectTypeMap::const_iterator it = types.begin(), end = types.end(); it != end; ++it, ++index)
	{
		m_gameObjectTypeMap.push_back(it->second.GetId());

		int count = 0;
		TGameObjectPosMap::const_iterator mapIt = objectPosMap.find(it->first);
		if (mapIt != objectPosMap.end())
		{
			count = mapIt->second.size();
		}

		std::stringstream nameCount;
		nameCount << it->second.GetName() << " (" << count << ")";
		m_listGameObjTypes->Insert(wxString(nameCount.str()), index);

		if (it->first == m_project.GetPaintGameObjectType())
		{
			selectedIndex = index;
		}
	}

	if(selectedIndex >= 0)
		m_listGameObjTypes->SetSelection(selectedIndex);
}

void GameObjectTypesPanel::PopulateArchetypes(GameObjectTypeId gameObjectTypeId)
{
	m_listGameObjArchetypes->Clear();
	m_archetypeMap.clear();

	if (const GameObjectType* gameObjectType = m_project.GetGameObjectType(gameObjectTypeId))
	{
		const TGameObjectArchetypeMap& archetypes = gameObjectType->GetArchetypes();

		int index = 0;
		int selectedIndex = -1;
		for (TGameObjectArchetypeMap::const_iterator it = archetypes.begin(), end = archetypes.end(); it != end; ++it, ++index)
		{
			m_archetypeMap.push_back(it->second.id);
			m_listGameObjArchetypes->Insert(it->second.name, index);

			if (it->first == m_project.GetPaintGameObjectArchetype())
			{
				selectedIndex = index;
			}
		}

		if(selectedIndex >= 0)
			m_listGameObjArchetypes->SetSelection(selectedIndex);
	}
}

void GameObjectTypesPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		PopulateGameObjects();
		PopulateArchetypes(m_project.GetPaintGameObjectType());
	}
}

void GameObjectTypesPanel::OnGameObjectTypeSelected(wxCommandEvent& event)
{
	GameObjectTypeId typeId = m_gameObjectTypeMap[event.GetSelection()];
	PopulateArchetypes(typeId);
	m_project.SetPaintGameObjectType(typeId, InvalidGameObjectArchetypeId);

	if (PropertyPanel* propertiesPanel = m_mainWindow->GetPropertiesPanel())
	{
		propertiesPanel->SetArchetype(InvalidGameObjectTypeId, InvalidGameObjectArchetypeId);
	}
}

void GameObjectTypesPanel::OnArchetypeSelected(wxCommandEvent& event)
{
	GameObjectArchetypeId archetypeId = m_archetypeMap[event.GetSelection()];
	m_project.SetPaintGameObjectType(m_project.GetPaintGameObjectType(), archetypeId);

	if (PropertyPanel* propertiesPanel = m_mainWindow->GetPropertiesPanel())
	{
		propertiesPanel->SetArchetype(m_project.GetPaintGameObjectType(), archetypeId);
	}
}