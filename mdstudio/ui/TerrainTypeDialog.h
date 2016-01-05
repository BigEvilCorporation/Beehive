///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <ion/core/Types.h>

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
	static const int sIconWidth = 16;
	static const int sIconHeight = 16;

	void PopulateList();
	void OnTypeSelected(u8 collisionTypeBit);
	u8 GetSelection();

	MainWindow& m_mainWindow;
	Project& m_project;
};