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

#include <ion/beehive/Project.h>

#include <wx/msgdlg.h>

#include "ProjectSettingsDialog.h"
#include "MainWindow.h"
#include "RenderResources.h"

#if defined BEEHIVE_PLUGIN_LUMINARY
#include <luminary/Types.h>
#include <luminary/EntityParser.h>
#include <luminary/SceneExporter.h>
#endif

ProjectSettingsDialog::ProjectSettingsDialog(MainWindow& mainWindow, Project& project, RenderResources& renderResources)
	: ProjectSettingsDialogBase((wxWindow*)&mainWindow)
	, m_project(project)
	, m_renderResources(renderResources)
	, m_mainWindow(mainWindow)
{
	m_textProjectName->SetValue(m_project.GetName());

	//Absolute paths
	m_dirPickerProject->SetPath(m_project.m_settings.Get("projectRootDir"));
	m_dirPickerEngine->SetPath(m_project.m_settings.Get("engineRootDir"));
	m_filePickerAssembler->SetPath(m_project.m_settings.Get("assembler"));
	m_filePickerAssemblyFile->SetPath(m_project.m_settings.Get("assemblyFile"));
	m_filePickerEmulator->SetPath(m_project.m_settings.Get("emulator"));

	//Relative paths
	m_dirPickerStamps->SetPath(m_project.m_settings.GetAbsolutePath("stampsDir"));
	m_filePickerGameObjTypesFile->SetPath(m_project.m_settings.GetAbsolutePath("gameObjectsExternalFile"));
	m_filePickerSpritesProj->SetPath(m_project.m_settings.GetAbsolutePath("spriteActorsExternalFile"));

	m_spinStampWidth->SetValue(m_project.GetPlatformConfig().stampWidth);
	m_spinStampHeight->SetValue(m_project.GetPlatformConfig().stampHeight);

	Connect(wxID_OK, wxEVT_BUTTON, wxCommandEventHandler(ProjectSettingsDialog::OnBtnOK));
	Connect(wxID_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(ProjectSettingsDialog::OnBtnCancel));
}

void ProjectSettingsDialog::OnBtnScanProject(wxCommandEvent& event)
{
	std::string projectDir = m_dirPickerProject->GetPath().c_str().AsChar();
	std::string engineDir = m_dirPickerEngine->GetPath().c_str().AsChar();
	if (projectDir.size() > 0 && engineDir.size())
	{
		m_mainWindow.ScanProject(projectDir, engineDir);
	}
}

void ProjectSettingsDialog::OnBtnOK(wxCommandEvent& event)
{
	m_project.SetName(m_textProjectName->GetValue().c_str().AsChar());

	//Absolute paths
	std::string projectDir = m_dirPickerProject->GetPath().c_str().AsChar();
	std::string engineDir = m_dirPickerEngine->GetPath().c_str().AsChar();
	std::string referenceFile = m_filePickerReference->GetPath().c_str().AsChar();

	if (projectDir != m_project.m_settings.Get("projectRootDir") || engineDir != m_project.m_settings.Get("engineRootDir"))
	{
		if (wxMessageBox("Engine or project directory has changed, would you like to re-scan for entity types?", "Scan for entities", wxOK | wxCANCEL) == wxOK)
		{
			m_mainWindow.ScanProject(engineDir, projectDir);
		}

		m_project.m_settings.Set("projectRootDir", projectDir);
		m_project.m_settings.Set("engineRootDir", engineDir);
	}

	m_project.m_settings.Set("assembler", m_filePickerAssembler->GetPath().c_str().AsChar());
	m_project.m_settings.Set("assemblyFile", m_filePickerAssemblyFile->GetPath().c_str().AsChar());
	m_project.m_settings.Set("emulator", m_filePickerEmulator->GetPath().c_str().AsChar());

	if (referenceFile.size() > 0)
	{
		ion::ImageFormat* reader = ion::ImageFormat::CreateReader(ion::string::GetFileExtension(referenceFile));
		if (reader && reader->Read(referenceFile))
		{
			m_renderResources.CreateReferenceImageTexture(*reader);
			delete reader;
		}
	}

	//Relative paths (to project directory)
	std::string stampsDir = m_dirPickerStamps->GetPath().c_str().AsChar();
	std::string gameObjectsFile = m_filePickerGameObjTypesFile->GetPath().c_str().AsChar();
	std::string spritesFile = m_filePickerSpritesProj->GetPath().c_str().AsChar();

	if (stampsDir.size() && !m_project.m_settings.IsRelativePath(stampsDir))
		wxMessageBox("Stamps directory is not relative to project directory, this will cause errors", "Settings error", wxOK);
	if (gameObjectsFile.size() && !m_project.m_settings.IsRelativePath(gameObjectsFile))
		wxMessageBox("Game objects file is not relative to project directory, this will cause errors", "Settings error", wxOK);
	if (spritesFile.size() && !m_project.m_settings.IsRelativePath(spritesFile))
		wxMessageBox("Sprite actors file is not relative to project directory, this will cause errors", "Settings error", wxOK);

	bool buildSpriteResources = false;

	if (m_project.m_settings.GetAbsolutePath("gameObjectsExternalFile") != gameObjectsFile)
	{
		if (!gameObjectsFile.empty())
		{
			//Re-import game objects file
			if (!m_project.ImportGameObjectTypes(gameObjectsFile, true))
			{
				wxMessageBox("Could not import external game object types, check settings.\nData may be lost if project is saved.", "Error", wxOK | wxICON_ERROR);
			}
		}

		m_project.m_settings.SetAbsolutePath("gameObjectsExternalFile", gameObjectsFile);
		buildSpriteResources = true;
	}

	if (m_project.m_settings.GetAbsolutePath("spriteActorsExternalFile") != spritesFile)
	{
		if (!spritesFile.empty())
		{
			//Re-import sprites file
			if (!m_project.ImportActors(spritesFile, true))
			{
				wxMessageBox("Could not import external sprite actors, check settings.\nData may be lost if project is saved.", "Error", wxOK | wxICON_ERROR);
			}
		}

		m_project.m_settings.SetAbsolutePath("spriteActorsExternalFile", spritesFile);
		buildSpriteResources = true;
	}

	if (stampsDir != m_project.m_settings.GetAbsolutePath("stampsDir"))
	{
		if (wxMessageBox("Stamps directory has changed, would you like to re-scan for stamps?", "Scan for stamps", wxOK | wxCANCEL) == wxOK)
		{
			m_mainWindow.ScanStamps(stampsDir);
		}

		m_project.m_settings.SetAbsolutePath("stampsDir", stampsDir);
	}

	m_project.GetPlatformConfig().stampWidth = m_spinStampWidth->GetValue();
	m_project.GetPlatformConfig().stampHeight = m_spinStampHeight->GetValue();

	if (buildSpriteResources)
	{
		m_renderResources.CreateSpriteSheetResources(m_project);
	}

	EndModal(wxID_OK);
}

void ProjectSettingsDialog::OnBtnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}
