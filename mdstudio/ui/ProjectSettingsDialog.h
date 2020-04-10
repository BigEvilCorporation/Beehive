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

#include "UIBase.h"

class Project;
class MainWindow;
class RenderResources;

class ProjectSettingsDialog : public ProjectSettingsDialogBase
{
public:
	ProjectSettingsDialog(MainWindow& mainWindow, Project& project, RenderResources& renderResources);

protected:
	virtual void OnBtnScanProject(wxCommandEvent& event);
	virtual void OnBtnOK(wxCommandEvent& event);
	virtual void OnBtnCancel(wxCommandEvent& event);

private:
	void ScanProject(const std::string& engineDir, const std::string& projectDir);
	Project& m_project;
	RenderResources& m_renderResources;
};
