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
					m_objectMap.insert(std::make_pair(itemId, object.m_gameObject.GetId()));
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
				mapPanel->SetTool(eToolMoveGameObject);
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
	wxMenu* addMenu = new wxMenu();

	m_objectTypeListSorted.clear();

	for (auto objectType : m_project.GetGameObjectTypes())
	{
		m_objectTypeListSorted.push_back(std::make_pair(objectType.first, objectType.second.GetName()));
	}

	std::sort(m_objectTypeListSorted.begin(), m_objectTypeListSorted.end(), [](const std::pair<GameObjectTypeId, std::string>& lhs, const std::pair<GameObjectTypeId, std::string>& rhs) { return lhs.second < rhs.second; });

	for(int i = 0; i < m_objectTypeListSorted.size(); i++)
	{
		addMenu->Append(ContextMenu::TypeListFirst + i, m_objectTypeListSorted[i].second);
	}

	m_addObjectMenu = contextMenu.AppendSubMenu(addMenu, "Add Object");
	contextMenu.Append(ContextMenu::Rename, "Rename Object");

	contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&SceneExplorerPanel::OnContextMenuClick, NULL, this);
	PopupMenu(&contextMenu);
}

void SceneExplorerPanel::OnContextMenuClick(wxCommandEvent& event)
{
	if (event.GetId() >= ContextMenu::TypeListFirst)
	{
		GameObjectId objectTypeId = m_objectTypeListSorted[event.GetId() - ContextMenu::TypeListFirst].first;

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
