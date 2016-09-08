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

#include "ExportDialog.h"

ExportDialog::ExportDialog(wxWindow* parent, Project& project)
	: ExportDialogBase(parent)
{
	//Create a map tab for each map
	for(TMapMap::const_iterator it = project.MapsBegin(), end = project.MapsEnd(); it != end; ++it)
	{
		m_mapTabs.push_back(new ExportDialogMapTab(m_notebookMapTabs));
		m_notebookMapTabs->AddPage(m_mapTabs.back(), it->second.GetName());
	}
}

void ExportDialog::OnBtnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void ExportDialog::OnBtnExportAll(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}

void ExportDialog::SetMapFormStrings(int mapIndex, const std::string& mapFilename, const std::string& collisionMapFilename, const std::string& gameObjectsFilename)
{
	m_mapTabs[mapIndex]->m_filePickerMap->SetPath(mapFilename);
	m_mapTabs[mapIndex]->m_filePickerCollisionMap->SetPath(collisionMapFilename);
	m_mapTabs[mapIndex]->m_filePickerGameObj->SetPath(gameObjectsFilename);
}

void ExportDialog::SetMapFormBools(int mapIndex, bool mapExport, bool collisionMapExport, bool gameObjectsExport)
{
	m_mapTabs[mapIndex]->m_chkMap->SetValue(mapExport);
	m_mapTabs[mapIndex]->m_chkCollisionMap->SetValue(collisionMapExport);
	m_mapTabs[mapIndex]->m_chkGameObj->SetValue(gameObjectsExport);
}

void ExportDialog::GetMapFormStrings(int mapIndex, std::string& mapFilename, std::string& collisionMapFilename, std::string& gameObjectsFilename) const
{
	mapFilename = m_mapTabs[mapIndex]->m_filePickerMap->GetPath();
	collisionMapFilename = m_mapTabs[mapIndex]->m_filePickerCollisionMap->GetPath();
	gameObjectsFilename = m_mapTabs[mapIndex]->m_filePickerGameObj->GetPath();
}

void ExportDialog::GetMapFormBools(int mapIndex, bool& mapExport, bool& collisionMapExport, bool& gameObjectsExport) const
{
	mapExport = m_mapTabs[mapIndex]->m_chkMap->GetValue();
	collisionMapExport = m_mapTabs[mapIndex]->m_chkCollisionMap->GetValue();
	gameObjectsExport = m_mapTabs[mapIndex]->m_chkGameObj->GetValue();
}