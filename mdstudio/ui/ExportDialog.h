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

#include <ion/beehive/Project.h>

#include "UIBase.h"

#include <vector>

class ExportDialog : public ExportDialogBase
{
public:
	ExportDialog(wxWindow* parent, Project& project);

	virtual void OnBtnCancel(wxCommandEvent& event);
	virtual void OnBtnExportAll(wxCommandEvent& event);

	void SetMapFormStrings(int mapIndex, const std::string& mapFilename, const std::string& stampMapFilename, const std::string& collisionMapFilename, const std::string& gameObjectsFilename);
	void SetMapFormBools(int mapIndex, bool mapExport, bool stampMapExport, bool collisionMapExport, bool gameObjectsExport);

	void GetMapFormStrings(int mapIndex, std::string& mapFilename, std::string& stampMapFilename, std::string& collisionMapFilename, std::string& gameObjectsFilename) const;
	void GetMapFormBools(int mapIndex, bool& mapExport, bool& stampMapExport, bool& collisionMapExport, bool& gameObjectsExport) const;

private:
	std::vector<ExportDialogMapTab*> m_mapTabs;
};