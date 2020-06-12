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

#pragma once

#include <ion/beehive/Project.h>
#include <ion/beehive/Map.h>

#include <map>
#include <vector>

#include "UIBase.h"

class MainWindow;

class SceneExplorerPanel : public SceneExplorerPanelBase
{
public:
	SceneExplorerPanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

	virtual void OnToolAddMap(wxCommandEvent& event);
	virtual void OnToolRemoveMap(wxCommandEvent& event);
	virtual void OnItemActivated(wxTreeEvent& event);
	virtual void OnItemDragged(wxTreeEvent& event);
	virtual void OnItemRenamed(wxTreeEvent& event);
	virtual void OnItemContextMenu(wxTreeEvent& event);
	virtual void OnItemSelected(wxTreeEvent& event);

protected:
	enum ContextMenu
	{
		Add,
		Rename,
		Delete,
		Duplicate,
		Convert,
		TypeListFirst
	};

	void OnContextMenuClick(wxCommandEvent& event);

private:
	struct ArchetypeEntry
	{
		GameObjectTypeId typeId;
		GameObjectArchetypeId archetypeId;
		std::string typeName;
		std::string archetypeName;
	};

	Project& m_project;
	MainWindow* m_mainWindow;
	std::map<wxTreeItemId, GameObjectId> m_objectMap;
	std::vector<std::pair<GameObjectTypeId, std::string>> m_objectTypeListSorted;
	std::vector<std::pair<GameObjectTypeId, std::string>> m_convertTypeListSorted;
	std::vector<ArchetypeEntry> m_archetypeListSorted;
	wxTreeItemId m_contextItem;

	int m_firstObjectId;
	int m_firstArchetypeId;
	int m_firstConvertId;
};