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
{
	m_textProjectName->SetValue(m_project.GetName());
	m_dirPickerProject->SetPath(m_project.m_settings.projectExportDir);
	m_dirPickerScene->SetPath(m_project.m_settings.sceneExportDir);
	m_dirPickerSprites->SetPath(m_project.m_settings.spritesExportDir);
	m_dirPickerSpriteAnims->SetPath(m_project.m_settings.spriteAnimsExportDir);
	m_dirPickerSpritePalettes->SetPath(m_project.m_settings.spritePalettesExportDir);
	m_filePickerGameObjTypesFile->SetPath(m_project.m_settings.gameObjectsExternalFile);
	m_filePickerSpritesProj->SetPath(m_project.m_settings.spriteActorsExternalFile);
	m_spinStampWidth->SetValue(m_project.GetPlatformConfig().stampWidth);
	m_spinStampHeight->SetValue(m_project.GetPlatformConfig().stampHeight);

	Connect(wxID_OK, wxEVT_BUTTON, wxCommandEventHandler(ProjectSettingsDialog::OnBtnOK));
	Connect(wxID_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(ProjectSettingsDialog::OnBtnCancel));
}

void ProjectSettingsDialog::OnBtnScanProject(wxCommandEvent& event)
{
	std::string projectDir = m_dirPickerProject->GetPath().c_str().AsChar();
	if (projectDir.size() > 0)
	{
		ScanProject(projectDir);
	}
}

void ProjectSettingsDialog::OnBtnOK(wxCommandEvent& event)
{
	m_project.SetName(m_textProjectName->GetValue().c_str().AsChar());
	std::string projectDir = m_dirPickerProject->GetPath().c_str().AsChar();
	m_project.m_settings.sceneExportDir = m_dirPickerScene->GetPath().c_str().AsChar();
	m_project.m_settings.spritesExportDir = m_dirPickerSprites->GetPath().c_str().AsChar();
	m_project.m_settings.spriteAnimsExportDir = m_dirPickerSpriteAnims->GetPath().c_str().AsChar();
	m_project.m_settings.spritePalettesExportDir = m_dirPickerSpritePalettes->GetPath().c_str().AsChar();
	std::string gameObjectsFile = m_filePickerGameObjTypesFile->GetPath().c_str().AsChar();
	std::string spritesFile = m_filePickerSpritesProj->GetPath().c_str().AsChar();
	std::string referenceFile = m_filePickerReference->GetPath().c_str().AsChar();

	bool buildSpriteResources = false;

	if (m_project.m_settings.gameObjectsExternalFile != gameObjectsFile)
	{
		if (!gameObjectsFile.empty())
		{
			//Re-import game objects file
			if (!m_project.ImportGameObjectTypes(gameObjectsFile))
			{
				wxMessageBox("Could not import external game object types, check settings.\nData may be lost if project is saved.", "Error", wxOK | wxICON_ERROR);
			}
		}

		m_project.m_settings.gameObjectsExternalFile = gameObjectsFile;
		buildSpriteResources = true;
	}

	if (m_project.m_settings.spriteActorsExternalFile != spritesFile)
	{
		if (!spritesFile.empty())
		{
			//Re-import sprites file
			if (!m_project.ImportActors(spritesFile))
			{
				wxMessageBox("Could not import external sprite actors, check settings.\nData may be lost if project is saved.", "Error", wxOK | wxICON_ERROR);
			}
		}

		m_project.m_settings.spriteActorsExternalFile = spritesFile;
		buildSpriteResources = true;
	}

	if (referenceFile.size() > 0)
	{
		BMPReader reader;
		if (reader.Read(referenceFile))
		{
			m_renderResources.CreateReferenceImageTexture(reader);
		}
	}

	if (buildSpriteResources)
	{
		m_renderResources.CreateSpriteSheetResources(m_project);
	}

	if (projectDir != m_project.m_settings.projectExportDir)
	{
		if (wxMessageBox("Project directory has changed, would you like to re-scan for entity types?", "Scan for entities", wxOK | wxCANCEL) == wxOK)
		{
			ScanProject(projectDir);
		}

		m_project.m_settings.projectExportDir = projectDir;
	}

	m_project.GetPlatformConfig().stampWidth = m_spinStampWidth->GetValue();
	m_project.GetPlatformConfig().stampHeight = m_spinStampHeight->GetValue();

	EndModal(wxID_OK);
}

void ProjectSettingsDialog::OnBtnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void ProjectSettingsDialog::ScanProject(const std::string directory)
{
#if defined BEEHIVE_PLUGIN_LUMINARY
	luminary::EntityParser entityParser;
	std::vector<luminary::Entity> entities;
	if (entityParser.ParseDirectory(directory, entities))
	{
		for (int i = 0; i < entities.size(); i++)
		{
			//Find or add game object type
			GameObjectType* gameObjectType = m_project.FindGameObjectType(entities[i].name);
			if (!gameObjectType)
			{
				GameObjectTypeId gameObjectTypeId = m_project.AddGameObjectType();
				gameObjectType = m_project.GetGameObjectType(gameObjectTypeId);
				gameObjectType->SetName(entities[i].name);
			}

			//Add all variables from entity and all components,
			//but keep the original values(if they exist) and keep the order
			int numVariables = entities[i].spawnData.params.size();

			for (int j = 0; j < entities[i].components.size(); j++)
			{
				numVariables += entities[i].components[j].spawnData.params.size();
			}

			std::vector<GameObjectVariable> variables;
			variables.resize(numVariables);

			int variableIdx = 0;

			//Add entity variables
			for (int j = 0; j < entities[i].spawnData.params.size(); j++, variableIdx++)
			{
				if (GameObjectVariable* variable = gameObjectType->FindVariable(entities[i].spawnData.params[j].name))
				{
					variables[variableIdx] = *variable;
				}
				else
				{
					variables[variableIdx].m_name = entities[i].spawnData.params[j].name;
				}

				variables[variableIdx].m_tags = entities[i].spawnData.params[j].tags;

				switch (entities[i].spawnData.params[j].size)
				{
				case luminary::ParamSize::Byte:
					variables[variableIdx].m_size = eSizeByte;
					break;
				case luminary::ParamSize::Word:
					variables[variableIdx].m_size = eSizeWord;
					break;
				case luminary::ParamSize::Long:
					variables[variableIdx].m_size = eSizeLong;
					break;
				}
			}

			//Add component variables
			for (int j = 0; j < entities[i].components.size(); j++)
			{
				for (int k = 0; k < entities[i].components[j].spawnData.params.size(); k++, variableIdx++)
				{
					if (GameObjectVariable* variable = gameObjectType->FindVariable(entities[i].components[j].spawnData.params[k].name))
					{
						variables[variableIdx] = *variable;
					}
					else
					{
						variables[variableIdx].m_name = entities[i].components[j].spawnData.params[k].name;
					}

					variables[variableIdx].m_componentIdx = j;
					variables[variableIdx].m_componentName = entities[i].components[j].name;
					variables[variableIdx].m_tags = entities[i].components[j].spawnData.params[k].tags;

					switch (entities[i].components[j].spawnData.params[k].size)
					{
					case luminary::ParamSize::Byte:
						variables[variableIdx].m_size = eSizeByte;
						break;
					case luminary::ParamSize::Word:
						variables[variableIdx].m_size = eSizeWord;
						break;
					case luminary::ParamSize::Long:
						variables[variableIdx].m_size = eSizeLong;
						break;
					}
				}
			}

			gameObjectType->ClearVariables();

			for (int j = 0; j < numVariables; j++)
			{
				gameObjectType->AddVariable() = variables[j];
			}
		}
	}
#endif
}