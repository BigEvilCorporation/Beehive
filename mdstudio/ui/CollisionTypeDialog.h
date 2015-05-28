///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "UIBase.h"

class MainWindow;
class Project;

class CollisionTypeDialog : public CollisionTypeDialogBase
{
public:
	CollisionTypeDialog(MainWindow& mainWindow, Project& project);

protected:
	virtual void OnAddType(wxCommandEvent& event);
	virtual void OnRemoveType(wxCommandEvent& event);
	virtual void OnTypeSelected(wxListEvent& event);
	virtual void OnIconChange(wxCommandEvent& event);
	virtual void OnNameChange(wxCommandEvent& event);
	virtual void OnBitChange(wxSpinEvent& event);

private:
	void PopulateList();
	MainWindow& m_mainWindow;
	Project& m_project;
};