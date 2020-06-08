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

#include "SceneExplorerPanel.h"
#include "MainWindow.h"
#include "Dialogs.h"

#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>

SceneExplorerPanel::SceneExplorerPanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: SceneExplorerPanelBase(parent, id, pos, size, style)
	, m_project(project)
	, m_mainWindow(mainWindow)
{

}

void SceneExplorerPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		m_tree->DeleteAllItems();
		m_objectMap.clear();

		const Map& editingMap = m_project.GetEditingMap();

		wxTreeItemId rootId = m_tree->AddRoot(editingMap.GetName());

		int labelIdx = 1;

		for(auto objectType : editingMap.GetGameObjects())
		{
			for (auto object : objectType.second)
			{
				if (const GameObjectType* objectType = m_project.GetGameObjectType(object.m_gameObject.GetTypeId()))
				{
					std::string name = object.m_gameObject.GetName();
					if (name.empty())
						name = "[" + objectType->GetName() + "_id" + std::to_string(labelIdx++) + "]";

					wxTreeItemId itemId = m_tree->AppendItem(rootId, name);
					m_tree->SetItemBold(itemId, objectType->IsPrefabType());
					m_objectMap.insert(std::make_pair(itemId, object.m_gameObject.GetId()));

					if (objectType->IsPrefabType())
					{
						for (auto prefabChild : objectType->GetChildren())
						{
							if (const GameObjectType* prefabChildType = m_project.GetGameObjectType(prefabChild.typeId))
							{
								std::string childName = "[" + prefabChildType->GetName() + "_id" + std::to_string(labelIdx++) + "]";
								wxTreeItemId childId = m_tree->AppendItem(itemId, childName);
							}
						}
						
					}
				}
			}
		}

		m_tree->Expand(rootId);
	}
}

void SceneExplorerPanel::OnToolAddMap(wxCommandEvent& event)
{

}

void SceneExplorerPanel::OnToolRemoveMap(wxCommandEvent& event)
{

}

void SceneExplorerPanel::OnItemSelected(wxTreeEvent& event)
{
	std::map<wxTreeItemId, GameObjectId>::const_iterator it = m_objectMap.find(event.GetItem());
	if (it != m_objectMap.end())
	{
		if (MapPanel* mapPanel = m_mainWindow->GetMapPanel())
		{
			if (GameObject* gameObject = m_project.GetEditingMap().GetGameObject(it->second))
			{
				m_mainWindow->SetSelectedGameObject(gameObject);
				mapPanel->SelectGameObject(it->second);
				mapPanel->SetTool(eToolSelectGameObject);
				m_mainWindow->RedrawPanel(MainWindow::ePanelMap);
			}
		}
	}
}

void SceneExplorerPanel::OnItemActivated(wxTreeEvent& event)
{
	std::map<wxTreeItemId, GameObjectId>::const_iterator it = m_objectMap.find(event.GetItem());
	if (it != m_objectMap.end())
	{
		if (MapPanel* mapPanel = m_mainWindow->GetMapPanel())
		{
			if (const GameObject* gameObject = m_project.GetEditingMap().GetGameObject(it->second))
			{
				mapPanel->CameraCentreOnObject(*gameObject);
				mapPanel->Refresh();
			}
		}
	}
}

void SceneExplorerPanel::OnItemDragged(wxTreeEvent& event)
{

}

void SceneExplorerPanel::OnItemRenamed(wxTreeEvent& event)
{
	std::map<wxTreeItemId, GameObjectId>::const_iterator it = m_objectMap.find(event.GetItem());
	if (it != m_objectMap.end())
	{
		if (GameObject* gameObject = m_project.GetEditingMap().GetGameObject(it->second))
		{
			gameObject->SetName(event.GetLabel().c_str().AsChar());
			m_mainWindow->RefreshAnimActors();
		}
	}
}

void SceneExplorerPanel::OnItemContextMenu(wxTreeEvent& event)
{
	//Right-click menu
	m_contextItem = event.GetItem();

	wxMenu contextMenu;
	wxMenu* objMenu = new wxMenu();
	wxMenu* archetypeMenu = new wxMenu();

	m_objectTypeListSorted.clear();
	m_archetypeListSorted.clear();

	for (auto objectType : m_project.GetGameObjectTypes())
	{
		m_objectTypeListSorted.push_back(std::make_pair(objectType.first, objectType.second.GetName()));

		for (auto archetype : objectType.second.GetArchetypes())
		{
			ArchetypeEntry archetypeEntry;
			archetypeEntry.archetypeId = archetype.first;
			archetypeEntry.archetypeName = archetype.second.name;
			archetypeEntry.typeId = objectType.first;
			archetypeEntry.typeName = objectType.second.GetName();

			m_archetypeListSorted.push_back(archetypeEntry);
		}
	}

	m_firstObjectId = ContextMenu::TypeListFirst;
	m_firstArchetypeId = ContextMenu::TypeListFirst + m_objectTypeListSorted.size();

	std::sort(m_objectTypeListSorted.begin(), m_objectTypeListSorted.end(), [](const std::pair<GameObjectTypeId, std::string>& lhs, const std::pair<GameObjectTypeId, std::string>& rhs) { return lhs.second < rhs.second; });
	std::sort(m_archetypeListSorted.begin(), m_archetypeListSorted.end(), [](const ArchetypeEntry& lhs, const ArchetypeEntry& rhs) { return lhs.typeName < rhs.typeName; });

	for(int i = 0; i < m_objectTypeListSorted.size(); i++)
	{
		objMenu->Append(m_firstObjectId + i, m_objectTypeListSorted[i].second);
	}

	for (int i = 0; i < m_archetypeListSorted.size(); i++)
	{
		archetypeMenu->Append(m_firstArchetypeId + i, m_archetypeListSorted[i].typeName + " : " + m_archetypeListSorted[i].archetypeName);
	}

	contextMenu.AppendSubMenu(objMenu, "Add New Object");
	contextMenu.AppendSubMenu(archetypeMenu, "Add Object from Archetype");
	contextMenu.Append(ContextMenu::Rename, "Rename Object");

	contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&SceneExplorerPanel::OnContextMenuClick, NULL, this);
	PopupMenu(&contextMenu);
}

void SceneExplorerPanel::OnContextMenuClick(wxCommandEvent& event)
{
	if (event.GetId() >= ContextMenu::TypeListFirst + m_objectTypeListSorted.size())
	{
		ArchetypeEntry archetypeEntry = m_archetypeListSorted[event.GetId() - m_firstArchetypeId];

		if (const GameObjectType* objectType = m_project.GetGameObjectType(archetypeEntry.typeId))
		{
			if (MapPanel* mapPanel = m_mainWindow->GetMapPanel())
			{
				//Start placement
				m_project.SetPaintGameObjectType(archetypeEntry.typeId, archetypeEntry.archetypeId);
				mapPanel->SetTool(eToolPlaceGameObject);
			}
		}
	}
	else if (event.GetId() >= ContextMenu::TypeListFirst)
	{
		GameObjectId objectTypeId = m_objectTypeListSorted[event.GetId() - m_firstObjectId].first;

		if (const GameObjectType* objectType = m_project.GetGameObjectType(objectTypeId))
		{
			if (MapPanel* mapPanel = m_mainWindow->GetMapPanel())
			{
				//Start placement
				m_project.SetPaintGameObjectType(objectTypeId);
				mapPanel->SetTool(eToolPlaceGameObject);
			}
		}
	}
	else if (event.GetId() == ContextMenu::Rename)
	{
		m_tree->EditLabel(m_contextItem);
	}
}
