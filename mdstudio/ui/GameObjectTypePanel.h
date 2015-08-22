///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "MainWindow.h"
#include "UIBase.h"
#include "../Project.h"
#include "../GameObject.h"

class GameObjTypesPanel : public GameObjTypesPanelBase
{
public:
	GameObjTypesPanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);
	virtual void OnGameObjectTypeSelected(wxCommandEvent& event);

private:
	Project& m_project;
	MainWindow* m_mainWindow;
};