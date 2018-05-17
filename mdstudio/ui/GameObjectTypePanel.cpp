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

void GameObjectTypesPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		m_listGameObjTypes->Clear();
		m_gameObjectTypeMap.clear();
		const TGameObjectTypeMap& types = m_project.GetGameObjectTypes();
		const TGameObjectPosMap& objectPosMap = m_project.GetEditingMap().GetGameObjects();

		int index = 0;
		for(TGameObjectTypeMap::const_iterator it = types.begin(), end = types.end(); it != end; ++it, ++index)
		{
			m_gameObjectTypeMap.push_back(it->second.GetId());

			std::stringstream nameCount;
			nameCount << it->second.GetName() << " (" << objectPosMap.find(it->first)->second.size() << ")";
			m_listGameObjTypes->Insert(wxString(nameCount.str()), index);
		}
	}
}

void GameObjectTypesPanel::OnGameObjectTypeSelected(wxCommandEvent& event)
{
	m_project.SetPaintGameObjectType(m_gameObjectTypeMap[event.GetSelection()]);
}