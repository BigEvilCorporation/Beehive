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

#include "UIBase.h"
#include "../Project.h"
#include "../GameObject.h"

class MainWindow;

class GameObjectTypesPanel : public GameObjTypesPanelBase
{
public:
	GameObjectTypesPanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);
	virtual void OnGameObjectTypeSelected(wxCommandEvent& event);

private:
	Project& m_project;
	MainWindow* m_mainWindow;
	std::vector<GameObjectTypeId> m_gameObjectTypeMap;
};