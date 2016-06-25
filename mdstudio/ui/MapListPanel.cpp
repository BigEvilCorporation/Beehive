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

#include "MapListPanel.h"
#include "MainWindow.h"

MapListPanel::MapListPanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: MapListPanelBase(parent, id, pos, size, style)
	, m_project(project)
	, m_mainWindow(mainWindow)
{

}

void MapListPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		m_listMaps->ClearAll();
		m_mapIds.clear();

		int index = 0;
		for(TMapMap::const_iterator it = m_project.MapsBegin(), end = m_project.MapsEnd(); it != end; ++it, ++index)
		{
			m_mapIds.push_back(it->first);
			m_listMaps->InsertItem(index, wxString(it->second.GetName()));
		}
	}
}

void MapListPanel::OnMapSelected(wxListEvent& event)
{
	m_project.SetEditingMap(m_mapIds[event.GetIndex()]);

	m_project.InvalidateMap(true);
	m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
	m_project.InvalidateMap(false);
}

void MapListPanel::OnToolAddMap(wxCommandEvent& event)
{
	Map& currentMap = m_project.GetEditingMap();

	MapId newMapId = m_project.CreateMap();
	Map& newMap = m_project.GetMap(newMapId);
	newMap.Resize(currentMap.GetWidth(), currentMap.GetHeight(), false, false);
	m_project.SetEditingMap(newMapId);

	m_project.InvalidateMap(true);
	m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
	m_project.InvalidateMap(false);

	Refresh(false, NULL);
}

void MapListPanel::OnToolRemoveMap(wxCommandEvent& event)
{

}