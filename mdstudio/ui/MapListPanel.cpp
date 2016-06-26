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
#include "Dialogs.h"

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

		MapId editingMap = m_project.GetEditingMapId();

		int index = 0;
		for(TMapMap::const_iterator it = m_project.MapsBegin(), end = m_project.MapsEnd(); it != end; ++it, ++index)
		{
			m_mapIds.push_back(it->first);
			m_listMaps->InsertItem(index, wxString(it->second.GetName()));

			if(it->first == editingMap)
			{
				m_listMaps->SetItemState(index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
			}
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
	DialogNewMap dialog(m_mainWindow);

	if(dialog.ShowModal() == wxID_OK)
	{
		MapId newMapId = m_project.CreateMap();
		Map& newMap = m_project.GetMap(newMapId);
		newMap.SetName(dialog.m_textMapName->GetValue().GetData().AsChar());
		newMap.Resize(dialog.m_spinCtrlWidth->GetValue(), dialog.m_spinCtrlHeight->GetValue(), false, false);
		m_project.SetEditingMap(newMapId);

		m_project.InvalidateMap(true);
		m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
		m_project.InvalidateMap(false);

		Refresh(false, NULL);
	}
}

void MapListPanel::OnToolRemoveMap(wxCommandEvent& event)
{
	if(m_mapIds.size() > 1)
	{
		m_project.DeleteMap(m_project.GetEditingMapId());
		Refresh(false, NULL);
		m_project.SetEditingMap(m_mapIds[0]);

		m_project.InvalidateMap(true);
		m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
		m_project.InvalidateMap(false);
	}
}

void MapListPanel::OnToolRenameMap(wxCommandEvent& event)
{

}