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
		m_prefabChildMap.clear();

		const Map& editingMap = m_project.GetEditingMap();

		wxTreeItemId rootId = m_tree->AddRoot(editingMap.GetName());

		int labelIdx = 1;

		for(auto objectTypeList : editingMap.GetGameObjects())
		{
			for (auto object : objectTypeList.second)
			{
				if (const GameObjectType* objectType = m_project.GetGameObjectType(object.m_gameObject.GetTypeId()))
				{
					std::string name = object.m_gameObject.GetName();
					if (name.empty())
						name = "[" + objectType->GetName() + "_id" + std::to_string(object.m_gameObject.GetId()) + "]";

					wxTreeItemId itemId = m_tree->AppendItem(rootId, name);
					m_tree->SetItemBold(itemId, objectType->IsPrefabType());
					m_objectMap.insert(std::make_pair(itemId, object.m_gameObject.GetId()));

					if (objectType->IsPrefabType())
					{
						for (auto prefabChild : objectType->GetPrefabChildren())
						{
							if (const GameObjectType* prefabChildType = m_project.GetGameObjectType(prefabChild.typeId))
							{
								std::string childName = prefabChild.name;
								if(childName.size() == 0)
									childName = "[" + prefabChildType->GetName() + "_id" + std::to_string(labelIdx++) + "]";
								wxTreeItemId childId = m_tree->AppendItem(itemId, childName);

								PrefabChildEntry entry;
								entry.rootObjectId = object.m_gameObject.GetId();
								entry.childTypeId = prefabChild.typeId;
								entry.childInstanceId = prefabChild.instanceId;
								m_prefabChildMap.insert(std::make_pair(childId, entry));
							}
						}
					}
				}
			}
		}

		m_tree->SortChildren(rootId);
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
		if (GameObject* gameObject = m_project.GetEditingMap().GetGameObject(it->second))
		{
			m_mainWindow->SetSelectedGameObject(gameObject);

			if (MapPanel* mapPanel = m_mainWindow->GetMapPanel())
			{
				mapPanel->SetTool(eToolSelectGameObject);
				mapPanel->SelectGameObject(it->second);
				m_mainWindow->RedrawPanel(MainWindow::ePanelMap);
			}
		}
	}
	else
	{
		std::map<wxTreeItemId, PrefabChildEntry>::const_iterator prefabIt = m_prefabChildMap.find(event.GetItem());
		if (prefabIt != m_prefabChildMap.end())
		{
			if (GameObject* rootObject = m_project.GetEditingMap().GetGameObject(prefabIt->second.rootObjectId))
			{
				if (GameObjectType* rootType = m_project.GetGameObjectType(rootObject->GetTypeId()))
				{
					if (GameObjectType* childType = m_project.GetGameObjectType(prefabIt->second.childTypeId))
					{
						m_mainWindow->SetSelectedPrefabChild(rootType, rootObject, childType, prefabIt->second.childInstanceId);

						if (MapPanel* mapPanel = m_mainWindow->GetMapPanel())
						{
							mapPanel->SetTool(eToolSelectGameObject);
							mapPanel->SelectPrefabChild(rootObject->GetId(), prefabIt->second.childInstanceId);
							m_mainWindow->RedrawPanel(MainWindow::ePanelMap);
						}
					}
				}
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

	//Set current as selected
	m_tree->SelectItem(m_contextItem);

	wxMenu contextMenu;
	wxMenu* objMenu = new wxMenu();
	wxMenu* convertMenu = new wxMenu();
	wxMenu* archetypeMenu = new wxMenu();

	m_objectTypeListSorted.clear();
	m_archetypeListSorted.clear();
	m_convertTypeListSorted.clear();

	for (auto objectType : m_project.GetGameObjectTypes())
	{
		m_objectTypeListSorted.push_back(std::make_pair(objectType.first, objectType.second.GetName()));
		m_convertTypeListSorted.push_back(std::make_pair(objectType.first, objectType.second.GetName()));

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
	m_firstArchetypeId = m_firstObjectId + m_objectTypeListSorted.size();
	m_firstConvertId = m_firstArchetypeId + m_archetypeListSorted.size();

	std::sort(m_objectTypeListSorted.begin(), m_objectTypeListSorted.end(), [](const std::pair<GameObjectTypeId, std::string>& lhs, const std::pair<GameObjectTypeId, std::string>& rhs) { return lhs.second < rhs.second; });
	std::sort(m_archetypeListSorted.begin(), m_archetypeListSorted.end(), [](const ArchetypeEntry& lhs, const ArchetypeEntry& rhs) { return lhs.typeName < rhs.typeName; });
	std::sort(m_convertTypeListSorted.begin(), m_convertTypeListSorted.end(), [](const std::pair<GameObjectTypeId, std::string>& lhs, const std::pair<GameObjectTypeId, std::string>& rhs) { return lhs.second < rhs.second; });

	for (int i = 0; i < m_objectTypeListSorted.size(); i++)
	{
		objMenu->Append(m_firstObjectId + i, m_objectTypeListSorted[i].second);
	}

	for (int i = 0; i < m_archetypeListSorted.size(); i++)
	{
		archetypeMenu->Append(m_firstArchetypeId + i, m_archetypeListSorted[i].typeName + " : " + m_archetypeListSorted[i].archetypeName);
	}

	for (int i = 0; i < m_convertTypeListSorted.size(); i++)
	{
		convertMenu->Append(m_firstConvertId + i, m_convertTypeListSorted[i].second);
	}

	contextMenu.AppendSubMenu(objMenu, "Add New Object");
	contextMenu.AppendSubMenu(archetypeMenu, "Add Object from Archetype");
	contextMenu.AppendSubMenu(convertMenu, "Convert Object");
	contextMenu.Append(ContextMenu::Delete, "Delete Object");
	contextMenu.Append(ContextMenu::Rename, "Rename Object");
	contextMenu.Append(ContextMenu::Duplicate, "Duplicate Object");

	std::map<wxTreeItemId, GameObjectId>::const_iterator it = m_objectMap.find(m_contextItem);
	if (it != m_objectMap.end())
	{
		const GameObject* gameObject = m_project.GetEditingMap().GetGameObject(it->second);
		if (gameObject)
		{
			GameObjectId objectTypeId = gameObject->GetTypeId();
			if (const GameObjectType* objectType = m_project.GetGameObjectType(objectTypeId))
			{
				if (objectType->IsPrefabType())
				{
					contextMenu.Append(ContextMenu::EditPrefab, "Edit Prefab");
				}
			}
		}
	}

	contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&SceneExplorerPanel::OnContextMenuClick, NULL, this);
	PopupMenu(&contextMenu);
}

void SceneExplorerPanel::OnContextMenuClick(wxCommandEvent& event)
{
	if (event.GetId() >= m_firstConvertId)
	{
		std::map<wxTreeItemId, GameObjectId>::const_iterator it = m_objectMap.find(m_tree->GetSelection());
		if (it != m_objectMap.end())
		{
			GameObjectId objectTypeId = m_convertTypeListSorted[event.GetId() - m_firstConvertId].first;

			if (const GameObjectType* objectType = m_project.GetGameObjectType(objectTypeId))
			{
				if (const GameObject* originalObj = m_project.GetEditingMap().GetGameObject(it->second))
				{
					if (const GameObjectType* originalType = m_project.GetGameObjectType(originalObj->GetTypeId()))
					{
						//Copy obj
						GameObject newObj(originalObj->GetId(), objectTypeId, *originalObj);

						//Generate name
						std::string name = newObj.GetName();
						if (name.size() == 0)
						{
							std::vector<char> stripChars = { '[', ']' };
							name = ion::string::Strip(m_tree->GetItemText(m_tree->GetSelection()).c_str().AsChar(), stripChars);
						}

						newObj.SetName(name);

						//Copy size and sprite params to instance, new type might not support default types
						newObj.SetDimensions(originalObj->GetDimensions().x == 0 ? originalType->GetDimensions() : originalObj->GetDimensions());
						newObj.SetSpriteActorId(m_project.GetActor(originalObj->GetSpriteActorId()) ? originalObj->GetSpriteActorId() : originalType->GetSpriteActorId());
						newObj.SetSpriteSheetId(originalObj->GetSpriteSheetId() == InvalidSpriteSheetId ? originalType->GetSpriteSheetId() : originalObj->GetSpriteSheetId());
						newObj.SetSpriteAnim(originalObj->GetSpriteAnim() == InvalidSpriteAnimId ? originalType->GetSpriteAnim() : originalObj->GetSpriteAnim());

						//Remove original and place new
						m_project.GetEditingMap().RemoveGameObject(it->second);
						m_project.GetEditingMap().PlaceGameObject(newObj);

						Refresh();
					}
				}
			}
		}
	}
	else if (event.GetId() >= m_firstArchetypeId)
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
	else if (event.GetId() >= m_firstObjectId)
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
	else if (event.GetId() == ContextMenu::Delete)
	{
		std::map<wxTreeItemId, GameObjectId>::const_iterator it = m_objectMap.find(m_tree->GetSelection());
		if (it != m_objectMap.end())
		{
			m_project.GetEditingMap().RemoveGameObject(it->second);
			m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
			m_mainWindow->RefreshPanel(MainWindow::ePanelProperties);
			Refresh();
		}
	}
	else if (event.GetId() == ContextMenu::Duplicate)
	{
		//TODO: multiple selection
		//for (auto gameObjId : m_selectedGameObjects)
		std::map<wxTreeItemId, GameObjectId>::const_iterator it = m_objectMap.find(m_tree->GetSelection());
		if (it != m_objectMap.end())
		{
			if (const GameObject* original = m_project.GetEditingMap().GetGameObject(it->second))
			{
				if (GameObjectType* gameObjectType = m_project.GetGameObjectType(original->GetTypeId()))
				{
					std::string name = ion::string::AddNumericPostfix(original->GetName(), 1);
					while (m_project.GetEditingMap().FindGameObject(name))
					{
						name = ion::string::AddNumericPostfix(name, 1);
					}

					GameObjectId newObject = m_project.GetEditingMap().PlaceGameObject(original->GetPosition().x, original->GetPosition().y, *gameObjectType, *original, name);
					m_mainWindow->RedrawPanel(MainWindow::ePanelMap);
					m_mainWindow->RedrawPanel(MainWindow::ePanelGameObjectTypes);
					Refresh();
				}
			}
		}
	}
	else if (event.GetId() == ContextMenu::Rename)
	{
		m_tree->EditLabel(m_contextItem);
	}
	else if (event.GetId() == ContextMenu::EditPrefab)
	{
	}
}
