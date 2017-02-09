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

void ExportDialog::SetMapFormValues(int mapIndex, const Map::ExportFilenames& values)
{
	m_mapTabs[mapIndex]->m_filePickerMap->SetPath(values.map);
	m_mapTabs[mapIndex]->m_filePickerStampMap->SetPath(values.stampMap);
	m_mapTabs[mapIndex]->m_filePickerCollisionMap->SetPath(values.collisionMap);
	m_mapTabs[mapIndex]->m_filePickerGameObj->SetPath(values.gameObjects);
	m_mapTabs[mapIndex]->m_filePickerBlocks->SetPath(values.blocks);
	m_mapTabs[mapIndex]->m_filePickerBlockMap->SetPath(values.blockMap);
	m_mapTabs[mapIndex]->m_filePickerCollisionBlocks->SetPath(values.terrainBlocks);
	m_mapTabs[mapIndex]->m_filePickerCollisionBlockMap->SetPath(values.terrainBlockMap);
	m_mapTabs[mapIndex]->m_chkMap->SetValue(values.mapExportEnabled);
	m_mapTabs[mapIndex]->m_chkStampMap->SetValue(values.stampMapExportEnabled);
	m_mapTabs[mapIndex]->m_chkCollisionMap->SetValue(values.collisionMapExportEnabled);
	m_mapTabs[mapIndex]->m_chkGameObj->SetValue(values.gameObjectsExportEnabled);
	m_mapTabs[mapIndex]->m_chkBlocks->SetValue(values.blocksExportEnabled);
	m_mapTabs[mapIndex]->m_chkBlockMap->SetValue(values.blockMapExportEnabled);
	m_mapTabs[mapIndex]->m_chkCollisionBlocks->SetValue(values.terrainBlocksExportEnabled);
	m_mapTabs[mapIndex]->m_chkCollisionBlockMap->SetValue(values.terrainBlockMapExportEnabled);
}

void ExportDialog::GetMapFormValues(int mapIndex, Map::ExportFilenames& values) const
{
	values.map = m_mapTabs[mapIndex]->m_filePickerMap->GetPath();
	values.stampMap = m_mapTabs[mapIndex]->m_filePickerStampMap->GetPath();
	values.collisionMap = m_mapTabs[mapIndex]->m_filePickerCollisionMap->GetPath();
	values.gameObjects = m_mapTabs[mapIndex]->m_filePickerGameObj->GetPath();
	values.blocks = m_mapTabs[mapIndex]->m_filePickerBlocks->GetPath();
	values.blockMap = m_mapTabs[mapIndex]->m_filePickerBlockMap->GetPath();
	values.terrainBlocks = m_mapTabs[mapIndex]->m_filePickerCollisionBlocks->GetPath();
	values.terrainBlockMap = m_mapTabs[mapIndex]->m_filePickerCollisionBlockMap->GetPath();
	values.mapExportEnabled = m_mapTabs[mapIndex]->m_chkMap->GetValue();
	values.stampMapExportEnabled = m_mapTabs[mapIndex]->m_chkStampMap->GetValue();
	values.collisionMapExportEnabled = m_mapTabs[mapIndex]->m_chkCollisionMap->GetValue();
	values.gameObjectsExportEnabled = m_mapTabs[mapIndex]->m_chkGameObj->GetValue();
	values.blocksExportEnabled = m_mapTabs[mapIndex]->m_chkBlocks->GetValue();
	values.blockMapExportEnabled = m_mapTabs[mapIndex]->m_chkBlockMap->GetValue();
	values.terrainBlocksExportEnabled = m_mapTabs[mapIndex]->m_chkCollisionBlocks->GetValue();
	values.terrainBlockMapExportEnabled = m_mapTabs[mapIndex]->m_chkCollisionBlockMap->GetValue();
}
