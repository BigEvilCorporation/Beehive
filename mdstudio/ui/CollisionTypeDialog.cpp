///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "CollisionTypeDialog.h"
#include "MainWindow.h"
#include "../Project.h"

#include <wx/filedlg.h>
#include <wx/msgdlg.h>

CollisionTypeDialog::CollisionTypeDialog(MainWindow& mainWindow, Project& project)
	: CollisionTypeDialogBase(&mainWindow)
	, m_mainWindow(mainWindow)
	, m_project(project)
{
	//Deselect
	OnTypeSelected(0);
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
			bit = numTypes + 1;
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
	OnTypeSelected(GetSelection());
}

void CollisionTypeDialog::OnIconChange(wxCommandEvent& event)
{
	u8 collisionTypeBits = GetSelection();
	if(collisionTypeBits)
	{
		if(CollisionType* collisionType = m_project.GetCollisionType(collisionTypeBits))
		{
			//Open image file
			wxFileDialog dialogue(this, _("Open Image file"), "", "", "BMP files (*.bmp)|*.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
			if(dialogue.ShowModal() == wxID_OK)
			{
				wxImage image;
				if(image.LoadFile(dialogue.GetPath()))
				{
					if(image.GetWidth() == sIconWidth && image.GetHeight() == sIconHeight)
					{
						//Set icon data
						const int bytesPerPixel = 3;
						int imageSize = sIconWidth * sIconHeight * bytesPerPixel;
						collisionType->iconData.resize(imageSize);
						unsigned char* imageData = image.GetData();
						ion::memory::MemCopy(&collisionType->iconData[0], imageData, imageSize);

						//Repopulate list and re-select current item to display icons
						PopulateList();
						OnTypeSelected(collisionTypeBits);

						//Refresh collision types panel
						m_mainWindow.RedrawPanel(MainWindow::ePanelCollisionTypes);
					}
					else
					{
						wxMessageBox("Image must be 16x16", "Error", wxOK | wxICON_ERROR);
					}
				}
			}
		}
	}
}

void CollisionTypeDialog::OnNameChange(wxCommandEvent& event)
{
	u8 collisionTypeBits = GetSelection();
	if(collisionTypeBits)
	{
		if(CollisionType* collisionType = m_project.GetCollisionType(collisionTypeBits))
		{
			collisionType->name = m_textName->GetValue();
		}
	}

	event.Skip();
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

	//Add all images first
	if(wxImageList* imageList = m_listCollisionTypes->GetImageList(0))
	{
		m_listCollisionTypes->SetImageList(NULL, 0);
		delete imageList;
	}

	wxImageList* imageList = new wxImageList(sIconWidth, sIconHeight);

	wxImage defaultImage = m_buttonIcon->GetBitmap().ConvertToImage();

	for(TCollisionTypeMap::const_iterator it = m_project.CollisionTypesBegin(), end = m_project.CollisionTypesEnd(); it != end; ++it)
	{
		wxIcon icon;

		if(it->second.iconData.size() > 0)
		{
			wxImage image(sIconWidth, sIconHeight, (unsigned char*)&it->second.iconData[0], true);
			icon.CopyFromBitmap(image);
		}
		else
		{
			icon.CopyFromBitmap(defaultImage);
		}
		
		imageList->Add(icon);
	}

	m_listCollisionTypes->SetImageList(imageList, 0);

	int row = 0;
	for(TCollisionTypeMap::const_iterator it = m_project.CollisionTypesBegin(), end = m_project.CollisionTypesEnd(); it != end; ++it, ++row)
	{
		sprintf(bitText, "Bit: %u\tName: %s", it->second.bit, it->second.name.c_str());

		wxListItem item;
		item.SetId(row);
		item.SetImage(row);
		item.SetText(bitText);
		item.SetData((long)it->second.bit);

		m_listCollisionTypes->InsertItem(item);
	}
}

void CollisionTypeDialog::OnTypeSelected(u8 collisionTypeBit)
{
	int index = m_listCollisionTypes->FindItem(-1, (wxUIntPtr)collisionTypeBit);
	if(index >= 0)
	{
		if(const CollisionType* collisionType = m_project.GetCollisionType(collisionTypeBit))
		{
			m_buttonIcon->Enable();
			m_textName->Enable();
			m_spinBit->Enable();

			//Set name, icon, bit
			m_textName->SetValue(collisionType->name);

			if(collisionType->iconData.size() > 0)
			{
				wxImage image(sIconWidth, sIconHeight, (unsigned char*)&collisionType->iconData[0], true);
				m_buttonIcon->SetImageLabel(wxBitmap(image));
			}
		}
	}
	else
	{
		m_buttonIcon->Disable();
		m_textName->Disable();
		m_spinBit->Disable();
	}
}

u8 CollisionTypeDialog::GetSelection()
{
	u8 collisionType = 0;

	int index = m_listCollisionTypes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if(index != wxNOT_FOUND)
	{
		wxListItem item;
		item.SetId(index);
		item.SetMask(wxLIST_MASK_DATA);
		m_listCollisionTypes->GetItem(item);
		collisionType = (u8)item.GetData();
	}

	return collisionType;
}