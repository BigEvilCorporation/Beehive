///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "CollisionTypeDialog.h"

#include "MainWindow.h"
#include "../Project.h"

CollisionTypeDialog::CollisionTypeDialog(MainWindow& mainWindow, Project& project)
	: CollisionTypeDialogBase(&mainWindow)
	, m_mainWindow(mainWindow)
	, m_project(project)
{

}

void CollisionTypeDialog::OnAddType(wxCommandEvent& event)
{
	int numTypes = m_project.GetCollisionTypeCount();
	if(numTypes < 8)
	{
		u8 bit = 0;

		//Find gap in bit
		for(u8 i = 0; i < numTypes && !bit; i++)
		{
			if(!m_project.GetCollisionType(i))
			{
				bit = i;
			}
		}

		if(!bit)
		{
			//No gap, add new
			bit = numTypes;
		}
		
		CollisionType collisionType;
		collisionType.bit = bit;
		
		m_project.AddCollisionType(collisionType);
		PopulateList();
	}
}

void CollisionTypeDialog::OnRemoveType(wxCommandEvent& event)
{

}

void CollisionTypeDialog::OnTypeSelected(wxListEvent& event)
{

}

void CollisionTypeDialog::OnIconChange(wxCommandEvent& event)
{

}

void CollisionTypeDialog::OnNameChange(wxCommandEvent& event)
{

}

void CollisionTypeDialog::OnBitChange(wxSpinEvent& event)
{

}

void CollisionTypeDialog::PopulateList()
{
	m_listCollisionTypes->ClearAll();

	wxListItem col0;
	col0.SetId(0);
	col0.SetText(_("Bit"));
	col0.SetWidth(50);
	m_listCollisionTypes->InsertColumn(0, col0);

	wxListItem col1;
	col1.SetId(1);
	col1.SetText(_("Icon"));
	m_listCollisionTypes->InsertColumn(1, col1);

	wxListItem col2;
	col1.SetId(2);
	col1.SetText(_("Name"));
	m_listCollisionTypes->InsertColumn(2, col2);

	char bitText[1024] = { 0 };

	int row = 0;
	for(TCollisionTypeMap::const_iterator it = m_project.CollisionTypesBegin(), end = m_project.CollisionTypesEnd(); it != end; ++it, ++row)
	{
		sprintf(bitText, "Bit: %u\tName: %s", it->second.bit, it->second.name.c_str());

		wxListItem item;
		item.SetId(row);
		item.SetText(bitText);
		//item.SetData((wxUintPtr)it->second.bit);
		m_listCollisionTypes->InsertItem(item);
	}
}