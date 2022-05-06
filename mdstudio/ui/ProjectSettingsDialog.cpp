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
	m_dirPickerProject->SetPath(m_project.m_settings.Get("projectRootDir"));
	m_dirPickerEngine->SetPath(m_project.m_settings.Get("engineRootDir"));
	m_filePickerGameObjTypesFile->SetPath(m_project.m_settings.Get("gameObjectsExternalFile"));
	m_filePickerSpritesProj->SetPath(m_project.m_settings.Get("spriteActorsExternalFile"));
	m_filePickerAssembler->SetPath(m_project.m_settings.Get("assembler"));
	m_filePickerAssemblyFile->SetPath(m_project.m_settings.Get("assemblyFile"));
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
		ScanProject(projectDir, engineDir);
	}
}

void ProjectSettingsDialog::OnBtnOK(wxCommandEvent& event)
{
	m_project.SetName(m_textProjectName->GetValue().c_str().AsChar());
	std::string projectDir = m_dirPickerProject->GetPath().c_str().AsChar();
	std::string engineDir = m_dirPickerEngine->GetPath().c_str().AsChar();
	std::string gameObjectsFile = m_filePickerGameObjTypesFile->GetPath().c_str().AsChar();
	std::string spritesFile = m_filePickerSpritesProj->GetPath().c_str().AsChar();
	std::string referenceFile = m_filePickerReference->GetPath().c_str().AsChar();

	m_project.m_settings.Set("assembler", m_filePickerAssembler->GetPath().c_str().AsChar());
	m_project.m_settings.Set("assemblyFile", m_filePickerAssemblyFile->GetPath().c_str().AsChar());

	bool buildSpriteResources = false;

	if (m_project.m_settings.Get("gameObjectsExternalFile") != gameObjectsFile)
	{
		if (!gameObjectsFile.empty())
		{
			//Re-import game objects file
			if (!m_project.ImportGameObjectTypes(gameObjectsFile, true))
			{
				wxMessageBox("Could not import external game object types, check settings.\nData may be lost if project is saved.", "Error", wxOK | wxICON_ERROR);
			}
		}

		m_project.m_settings.Set("gameObjectsExternalFile", gameObjectsFile);
		buildSpriteResources = true;
	}

	if (m_project.m_settings.Get("spriteActorsExternalFile") != spritesFile)
	{
		if (!spritesFile.empty())
		{
			//Re-import sprites file
			if (!m_project.ImportActors(spritesFile, true))
			{
				wxMessageBox("Could not import external sprite actors, check settings.\nData may be lost if project is saved.", "Error", wxOK | wxICON_ERROR);
			}
		}

		m_project.m_settings.Set("spriteActorsExternalFile", spritesFile);
		buildSpriteResources = true;
	}

	if (referenceFile.size() > 0)
	{
		ion::ImageFormat* reader = ion::ImageFormat::CreateReader(ion::string::GetFileExtension(referenceFile));
		if (reader && reader->Read(referenceFile))
		{
			m_renderResources.CreateReferenceImageTexture(*reader);
			delete reader;
		}
	}

	if (buildSpriteResources)
	{
		m_renderResources.CreateSpriteSheetResources(m_project);
	}

	if (projectDir != m_project.m_settings.Get("projectRootDir") || engineDir != m_project.m_settings.Get("engineRootDir"))
	{
		if (wxMessageBox("Engine or project directory has changed, would you like to re-scan for entity types?", "Scan for entities", wxOK | wxCANCEL) == wxOK)
		{
			ScanProject(engineDir, projectDir);
		}

		m_project.m_settings.Set("projectRootDir", projectDir);
		m_project.m_settings.Set("engineRootDir", engineDir);
	}

	m_project.GetPlatformConfig().stampWidth = m_spinStampWidth->GetValue();
	m_project.GetPlatformConfig().stampHeight = m_spinStampHeight->GetValue();

	EndModal(wxID_OK);
}

void ProjectSettingsDialog::OnBtnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void ProjectSettingsDialog::ScanProject(const std::string& engineDir, const std::string& projectDir)
{
#if defined BEEHIVE_PLUGIN_LUMINARY
	luminary::EntityParser entityParser;
	std::vector<luminary::Entity> entities;
	std::vector<std::string> directories;

	directories.push_back(engineDir);
	directories.push_back(projectDir);

	if (entityParser.ParseDirectories(directories, entities))
	{
		for (int i = 0; i < entities.size(); i++)
		{
			//Find or add game object type
			GameObjectType* gameObjectType = m_project.FindGameObjectType(entities[i].typeName);
			if (!gameObjectType)
			{
				GameObjectTypeId gameObjectTypeId = m_project.AddGameObjectType();
				gameObjectType = m_project.GetGameObjectType(gameObjectTypeId);
				gameObjectType->SetName(entities[i].typeName);
				gameObjectType->SetStatic(entities[i].isStatic);
			}

			//Clear script funcs
			gameObjectType->ClearScriptFunctions();

			//Add all variables as script variables, keep structures intact
			int numScriptVariables = 0;

			//Add all spawn data variables from entity and all components,
			//but keep the original values (if they exist) and keep the order
			int numConfigurableVariables = 0;
			
			if (entities[i].isStatic)
			{
				//Static entities have no components
				numConfigurableVariables = entities[i].params.size();
				numScriptVariables = entities[i].params.size();
			}
			else
			{
				numConfigurableVariables = entities[i].spawnData.params.size();
				numScriptVariables = entities[i].params.size();

				for (int j = 0; j < entities[i].components.size(); j++)
				{
					numConfigurableVariables += entities[i].components[j].spawnData.params.size();
					numScriptVariables += entities[i].components[j].params.size();
				}
			}

			std::vector<GameObjectVariable> configurableVariables;
			std::vector<GameObjectVariable> scriptVariables;
			configurableVariables.resize(numConfigurableVariables);
			scriptVariables.resize(numScriptVariables);

			int configurableVarIdx = 0;
			int scriptVarIdx = 0;

			//Add entity script variables
			for (int j = 0; j < entities[i].params.size(); j++, scriptVarIdx++)
			{
				scriptVariables[scriptVarIdx].m_name = entities[i].params[j].name;
				scriptVariables[scriptVarIdx].m_tags = entities[i].params[j].tags;

				switch (entities[i].params[j].size)
				{
				case luminary::ParamSize::Byte:
					scriptVariables[scriptVarIdx].m_size = eSizeByte;
					break;
				case luminary::ParamSize::Word:
					scriptVariables[scriptVarIdx].m_size = eSizeWord;
					break;
				case luminary::ParamSize::Long:
					scriptVariables[scriptVarIdx].m_size = eSizeLong;
					break;
				}
			}

			//Add entity script functions
			for (int j = 0; j < entities[i].scriptFuncs.size(); j++)
			{
				GameObjectScriptFunc scriptFunc;
				scriptFunc.componentIdx = -1;
				scriptFunc.name = entities[i].scriptFuncs[j].name;
				scriptFunc.params = entities[i].scriptFuncs[j].params;
				scriptFunc.returnType = entities[i].scriptFuncs[j].returnType;
				scriptFunc.routine = entities[i].scriptFuncs[j].routine;

				gameObjectType->AddScriptFunction(scriptFunc);
			}

			//Add entity spawn data variables
			if (entities[i].isStatic)
			{
				//Static entity is just data
				for (int j = 0; j < entities[i].params.size(); j++, configurableVarIdx++)
				{
					if (GameObjectVariable* variable = gameObjectType->FindVariable(entities[i].params[j].name))
					{
						configurableVariables[configurableVarIdx] = *variable;
					}
					else
					{
						configurableVariables[configurableVarIdx].m_name = entities[i].params[j].name;
					}

					configurableVariables[configurableVarIdx].m_tags = entities[i].params[j].tags;

					switch (entities[i].params[j].size)
					{
					case luminary::ParamSize::Byte:
						configurableVariables[configurableVarIdx].m_size = eSizeByte;
						break;
					case luminary::ParamSize::Word:
						configurableVariables[configurableVarIdx].m_size = eSizeWord;
						break;
					case luminary::ParamSize::Long:
						configurableVariables[configurableVarIdx].m_size = eSizeLong;
						break;
					}
				}
			}
			else
			{
				//Dynamic entity has separate spawn data structure
				for (int j = 0; j < entities[i].spawnData.params.size(); j++, configurableVarIdx++)
				{
					if (GameObjectVariable* variable = gameObjectType->FindVariable(entities[i].spawnData.params[j].name))
					{
						configurableVariables[configurableVarIdx] = *variable;
					}
					else
					{
						configurableVariables[configurableVarIdx].m_name = entities[i].spawnData.params[j].name;
					}

					configurableVariables[configurableVarIdx].m_tags = entities[i].spawnData.params[j].tags;

					switch (entities[i].spawnData.params[j].size)
					{
					case luminary::ParamSize::Byte:
						configurableVariables[configurableVarIdx].m_size = eSizeByte;
						break;
					case luminary::ParamSize::Word:
						configurableVariables[configurableVarIdx].m_size = eSizeWord;
						break;
					case luminary::ParamSize::Long:
						configurableVariables[configurableVarIdx].m_size = eSizeLong;
						break;
					}
				}
			}

			for (int j = 0; j < entities[i].components.size(); j++)
			{
				//Add component script variables
				for (int k = 0; k < entities[i].components[j].params.size(); k++, scriptVarIdx++)
				{
					scriptVariables[scriptVarIdx].m_name = entities[i].components[j].params[k].name;
					scriptVariables[scriptVarIdx].m_componentIdx = j;
					scriptVariables[scriptVarIdx].m_componentName = entities[i].components[j].name;
					scriptVariables[scriptVarIdx].m_tags = entities[i].components[j].params[k].tags;

					switch (entities[i].components[j].params[k].size)
					{
					case luminary::ParamSize::Byte:
						scriptVariables[scriptVarIdx].m_size = eSizeByte;
						break;
					case luminary::ParamSize::Word:
						scriptVariables[scriptVarIdx].m_size = eSizeWord;
						break;
					case luminary::ParamSize::Long:
						scriptVariables[scriptVarIdx].m_size = eSizeLong;
						break;
					}
				}

				//Add component script functions
				for (int k = 0; k < entities[i].components[j].scriptFuncs.size(); k++)
				{
					GameObjectScriptFunc scriptFunc;
					scriptFunc.componentIdx = j;
					scriptFunc.name = entities[i].components[j].scriptFuncs[k].name;
					scriptFunc.params = entities[i].components[j].scriptFuncs[k].params;
					scriptFunc.returnType = entities[i].components[j].scriptFuncs[k].returnType;
					scriptFunc.routine = entities[i].components[j].scriptFuncs[k].routine;

					if (std::find_if(gameObjectType->GetScriptFunctions().begin(), gameObjectType->GetScriptFunctions().end(),
						[&](const GameObjectScriptFunc& lhs) { return lhs == scriptFunc; }) == gameObjectType->GetScriptFunctions().end())
					{
						gameObjectType->AddScriptFunction(scriptFunc);
					}
				}
			}

			//Add component spawn data variables
			for (int j = 0; j < entities[i].components.size(); j++)
			{
				for (int k = 0; k < entities[i].components[j].spawnData.params.size(); k++, configurableVarIdx++, scriptVarIdx++)
				{
					if (GameObjectVariable* variable = gameObjectType->FindVariable(entities[i].components[j].spawnData.params[k].name))
					{
						configurableVariables[configurableVarIdx] = *variable;
					}
					else
					{
						configurableVariables[configurableVarIdx].m_name = entities[i].components[j].spawnData.params[k].name;
					}

					configurableVariables[configurableVarIdx].m_componentIdx = j;
					configurableVariables[configurableVarIdx].m_componentName = entities[i].components[j].name;
					configurableVariables[configurableVarIdx].m_tags = entities[i].components[j].spawnData.params[k].tags;

					switch (entities[i].components[j].spawnData.params[k].size)
					{
					case luminary::ParamSize::Byte:
						configurableVariables[configurableVarIdx].m_size = eSizeByte;
						break;
					case luminary::ParamSize::Word:
						configurableVariables[configurableVarIdx].m_size = eSizeWord;
						break;
					case luminary::ParamSize::Long:
						configurableVariables[configurableVarIdx].m_size = eSizeLong;
						break;
					}
				}
			}

			gameObjectType->ClearVariables();
			gameObjectType->ClearScriptVariables();

			for (int j = 0; j < numScriptVariables; j++)
			{
				gameObjectType->AddScriptVariable(scriptVariables[j]);
			}

			for (int j = 0; j < numConfigurableVariables; j++)
			{
				gameObjectType->AddVariable() = configurableVariables[j];
			}
		}
	}
#endif
}