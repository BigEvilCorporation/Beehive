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

	void SetMapFormValues(const std::string& projectRoot, int mapIndex, const Map::ExportFilenames& values);
	void GetMapFormValues(const std::string& projectRoot, int mapIndex, Map::ExportFilenames& values) const;

private:
	std::vector<ExportDialogMapTab*> m_mapTabs;
};