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
#include <ion/io/FileDevice.h>

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

void ExportDialog::SetMapFormValues(const std::string& projectRootDir, int mapIndex, const Map::ExportFilenames& values)
{
	ion::io::FileDevice* fileDevice = ion::io::FileDevice::GetDefault();

	m_mapTabs[mapIndex]->m_filePickerMap->SetPath(fileDevice->CombinePath(projectRootDir, values.map));
	m_mapTabs[mapIndex]->m_filePickerStampMap->SetPath(fileDevice->CombinePath(projectRootDir, values.stampMap));
	m_mapTabs[mapIndex]->m_filePickerCollisionMap->SetPath(fileDevice->CombinePath(projectRootDir, values.collisionMap));
	m_mapTabs[mapIndex]->m_filePickerGameObj->SetPath(fileDevice->CombinePath(projectRootDir, values.gameObjects));
	m_mapTabs[mapIndex]->m_filePickerBlockMap->SetPath(fileDevice->CombinePath(projectRootDir, values.blockMap));
	m_mapTabs[mapIndex]->m_filePickerCollisionBlockMap->SetPath(fileDevice->CombinePath(projectRootDir, values.terrainBlockMap));
	m_mapTabs[mapIndex]->m_filePickerSceneAnimations->SetPath(fileDevice->CombinePath(projectRootDir, values.sceneAnims));

	m_mapTabs[mapIndex]->m_chkMap->SetValue(values.mapExportEnabled);
	m_mapTabs[mapIndex]->m_chkStampMap->SetValue(values.stampMapExportEnabled);
	m_mapTabs[mapIndex]->m_chkCollisionMap->SetValue(values.collisionMapExportEnabled);
	m_mapTabs[mapIndex]->m_chkGameObj->SetValue(values.gameObjectsExportEnabled);
	m_mapTabs[mapIndex]->m_chkBlockMap->SetValue(values.blockMapExportEnabled);
	m_mapTabs[mapIndex]->m_chkCollisionBlockMap->SetValue(values.terrainBlockMapExportEnabled);
	m_mapTabs[mapIndex]->m_chkSceneAnims->SetValue(values.sceneAnimExportEnabled);
}

void ExportDialog::GetMapFormValues(const std::string& projectRootDir, int mapIndex, Map::ExportFilenames& values) const
{
	values.map = ion::string::RemoveSubstring(m_mapTabs[mapIndex]->m_filePickerMap->GetPath().c_str().AsChar(), projectRootDir);
	values.stampMap = ion::string::RemoveSubstring(m_mapTabs[mapIndex]->m_filePickerStampMap->GetPath().c_str().AsChar(), projectRootDir);
	values.collisionMap = ion::string::RemoveSubstring(m_mapTabs[mapIndex]->m_filePickerCollisionMap->GetPath().c_str().AsChar(), projectRootDir);
	values.gameObjects = ion::string::RemoveSubstring(m_mapTabs[mapIndex]->m_filePickerGameObj->GetPath().c_str().AsChar(), projectRootDir);
	values.blockMap = ion::string::RemoveSubstring(m_mapTabs[mapIndex]->m_filePickerBlockMap->GetPath().c_str().AsChar(), projectRootDir);
	values.terrainBlockMap = ion::string::RemoveSubstring(m_mapTabs[mapIndex]->m_filePickerCollisionBlockMap->GetPath().c_str().AsChar(), projectRootDir);
	values.sceneAnims = ion::string::RemoveSubstring(m_mapTabs[mapIndex]->m_filePickerSceneAnimations->GetPath().c_str().AsChar(), projectRootDir);

	values.mapExportEnabled = m_mapTabs[mapIndex]->m_chkMap->GetValue();
	values.stampMapExportEnabled = m_mapTabs[mapIndex]->m_chkStampMap->GetValue();
	values.collisionMapExportEnabled = m_mapTabs[mapIndex]->m_chkCollisionMap->GetValue();
	values.gameObjectsExportEnabled = m_mapTabs[mapIndex]->m_chkGameObj->GetValue();
	values.blockMapExportEnabled = m_mapTabs[mapIndex]->m_chkBlockMap->GetValue();
	values.terrainBlockMapExportEnabled = m_mapTabs[mapIndex]->m_chkCollisionBlockMap->GetValue();
	values.sceneAnimExportEnabled = m_mapTabs[mapIndex]->m_chkSceneAnims->GetValue();
}
