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

#include "PropertyPanel.h"
#include "MainWindow.h"
#include "Dialogs.h"

#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>

#include <ion/io/File.h>
#include <ion/io/FileDevice.h>

#if defined BEEHIVE_PLUGIN_LUMINARY
#include <luminary/Types.h>
#include <luminary/ScriptCompiler.h>
#include <luminary/BeehiveToLuminary.h>
#endif

static const std::string s_defaultVarName = "[ DEFAULT_VALUE ]";

PropertyPanel::PropertyPanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: PropertyPanelBase(parent, id, pos, size, style)
	, m_project(project)
	, m_mainWindow(mainWindow)
{
	m_gameObjectId = InvalidGameObjectId;
	m_gameObjectTypeId = InvalidGameObjectTypeId;
	m_archetypeId = InvalidGameObjectArchetypeId;
	m_contextProperty = nullptr;
}

void PropertyPanel::GetEditingVariables(GameObjectType*& gameObjectType, GameObject*& gameObject, Actor*& actor, std::vector<GameObjectVariable>*& typeVariables, std::vector<GameObjectVariable>*& instanceVariables)
{
	gameObjectType = nullptr;
	actor = nullptr;
	typeVariables = nullptr;
	instanceVariables = nullptr;

	gameObject = m_project.GetEditingMap().GetGameObject(m_gameObjectId);
	if (gameObject)
	{
		gameObjectType = m_project.GetGameObjectType(gameObject->GetTypeId());
		typeVariables = &gameObjectType->GetVariables();
		instanceVariables = &gameObject->GetVariables();

		actor = m_project.GetActor(gameObject->GetSpriteActorId());
		if (!actor)
			actor = m_project.GetActor(gameObjectType->GetSpriteActorId());
	}
	else
	{
		gameObjectType = m_project.GetGameObjectType(m_gameObjectTypeId);
		if (gameObjectType)
		{
			if (GameObjectArchetype* archetype = gameObjectType->GetArchetype(m_archetypeId))
			{
				typeVariables = &gameObjectType->GetVariables();
				instanceVariables = &archetype->variables;

				actor = m_project.GetActor(archetype->spriteActorId);
				if (!actor)
					actor = m_project.GetActor(gameObjectType->GetSpriteActorId());
			}
		}
	}
}

GameObjectVariable* PropertyPanel::FindVariable(std::vector<GameObjectVariable>& variables, const std::string& name, int componentIdx)
{
	for (int i = 0; i < variables.size(); i++)
	{
		if (((componentIdx == -1) || (componentIdx == variables[i].m_componentIdx)) && ion::string::CompareNoCase(variables[i].m_name, name))
		{
			return &variables[i];
		}
	}

	return nullptr;
}

GameObjectVariable* PropertyPanel::FindVariableByTag(std::vector<GameObjectVariable>& variables, const std::string& tag, int componentIdx)
{
	for (int i = 0; i < variables.size(); i++)
	{
		if (((componentIdx == -1) || (componentIdx == variables[i].m_componentIdx)) && variables[i].HasTag(tag))
		{
			return &variables[i];
		}
	}

	return nullptr;
}

GameObjectVariable& PropertyPanel::AddVariable(std::vector<GameObjectVariable>& variables)
{
	variables.push_back(GameObjectVariable());
	return variables[variables.size() - 1];
}

void PropertyPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		m_propertyGrid->Clear();

		GameObjectType* gameObjectType = nullptr;
		GameObject* gameObject = nullptr;
		Actor* actor = nullptr;
		std::vector<GameObjectVariable>* typeVariables = nullptr;
		std::vector<GameObjectVariable>* instanceVariables = nullptr;

		GetEditingVariables(gameObjectType, gameObject, actor, typeVariables, instanceVariables);

		if (typeVariables)
		{
			//If entity has a sprite actor, find it and add property
			for (auto variable : *typeVariables)
			{
				if (variable.HasTag("SPRITE_SHEET"))
				{
					std::string currentActor;

					if (actor)
					{
						currentActor = actor->GetName();
					}

					wxArrayString* list = new wxArrayString();
					int selection = PopulateSpriteActorList(*list, currentActor);
					wxEnumProperty* choiceProp = new wxEnumProperty("Sprite Actor", "Sprite Actor", *list);

					if (selection >= 0 && selection < list->size())
						choiceProp->SetChoiceSelection(selection);

					choiceProp->SetAttribute("isScript", false);

					m_propertyGrid->Append(choiceProp);

					break;
				}
			}

			//Add all properties from all components
			int componentIdx = -1;

#if 0
			if (gameObjectType->GetScriptVariables().size() > 0)
			{
				//Populate all variables, mark unmodifiable ones as read only
				for (auto variable : gameObjectType->GetScriptVariables())
				{
					if (componentIdx != variable.m_componentIdx)
					{
						m_propertyGrid->Append(new wxPropertyCategory(variable.m_componentName));
						componentIdx = variable.m_componentIdx;
					}

					if (gameObjectType->FindVariable(variable.m_name, variable.m_componentIdx))
					{
						//Variable is modifiable
						if (const GameObjectVariable* overriddenVar = gameObject->FindVariable(variable.m_name, variable.m_componentIdx))
						{
							AddProperty(*gameObject, *overriddenVar, variable.m_componentIdx);
						}
						else
						{
							AddProperty(*gameObject, variable, variable.m_componentIdx);
						}
					}
					else
					{
						//Script variable only, show but mark read only
						AddProperty(*gameObject, variable, variable.m_componentIdx, false);
					}
				}
			}
			else
#endif
			{
				for (auto variable : *typeVariables)
				{
					if (componentIdx != variable.m_componentIdx)
					{
						m_propertyGrid->Append(new wxPropertyCategory(variable.m_componentName));
						componentIdx = variable.m_componentIdx;
					}

					//Some properties need the actor and sprite sheet associated with this component, if it has one
					const GameObjectVariable* spriteSheetVar = FindVariableByTag(*instanceVariables, "SPRITE_SHEET", variable.m_componentIdx);

					if(!spriteSheetVar)
						spriteSheetVar = FindVariableByTag(*typeVariables, "SPRITE_SHEET", variable.m_componentIdx);

					const GameObjectVariable* overriddenVar = FindVariable(*instanceVariables, variable.m_name, variable.m_componentIdx);
					if (overriddenVar)
					{
						AddProperty(*gameObjectType, *overriddenVar, variable.m_componentIdx, actor, spriteSheetVar);
					}
					else
					{
						AddProperty(*gameObjectType, variable, variable.m_componentIdx, actor, spriteSheetVar);
					}
				}
			}
		}
	}
}

void PropertyPanel::OnPropertyChanged(wxPropertyGridEvent& event)
{
	if (wxPGProperty* property = event.GetProperty())
	{
		GameObjectType* gameObjectType = nullptr;
		GameObject* gameObject = nullptr;
		Actor* actor = nullptr;
		std::vector<GameObjectVariable>* typeVariables = nullptr;
		std::vector<GameObjectVariable>* instanceVariables = nullptr;

		GetEditingVariables(gameObjectType, gameObject, actor, typeVariables, instanceVariables);

		if (typeVariables)
		{
			wxString variableName = property->GetAttribute("variableName", "");
			wxVariant componentIdx = property->GetAttribute("componentIdx");
			wxString value = property->GetValueAsString();

			if (value != s_defaultVarName)
			{
				//Find or add overridden var on game object
				GameObjectVariable* variable = FindVariable(*instanceVariables, variableName.c_str().AsChar(), componentIdx.GetInteger());
				if (!variable)
				{
					if (const GameObjectVariable* original = FindVariable(*typeVariables, variableName.c_str().AsChar(), componentIdx.GetInteger()))
					{
						variable = &AddVariable(*instanceVariables);
						*variable = *original;
					}
				}

				if (variable)
				{
					variable->m_value = value.c_str().AsChar();
				}

				Refresh();
			}
		}
	}
}

void PropertyPanel::OnRightClick(wxMouseEvent& event)
{
	m_contextProperty = m_propertyGrid->GetItemAtY(event.GetPosition().y);

	if (m_contextProperty)
	{
		if (GameObject* gameObject = m_project.GetEditingMap().GetGameObject(m_gameObjectId))
		{
			wxMenu contextMenu;

			wxMenuItem* defaultItem = contextMenu.Append(ContextMenu::Default, "Revert to Default");
			defaultItem->Enable(gameObject->GetOriginalArchetype() != InvalidGameObjectArchetypeId);

			wxMenuItem* editScriptItem = contextMenu.Append(ContextMenu::EditScript, "Edit Script");
			editScriptItem->Enable(m_contextProperty->GetAttribute("isScript"));

			wxMenuItem* compileScriptItem = contextMenu.Append(ContextMenu::CompileScript, "Compile Script");
			compileScriptItem->Enable(m_contextProperty->GetAttribute("isScript"));

			contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&PropertyPanel::OnContextMenuClick, NULL, this);
			PopupMenu(&contextMenu);
		}
	}
}

void PropertyPanel::OnContextMenuClick(wxCommandEvent& event)
{
	if (m_contextProperty)
	{
		GameObjectType* gameObjectType = nullptr;
		GameObject* gameObject = nullptr;
		Actor* actor = nullptr;
		std::vector<GameObjectVariable>* typeVariables = nullptr;
		std::vector<GameObjectVariable>* instanceVariables = nullptr;

		GetEditingVariables(gameObjectType, gameObject, actor, typeVariables, instanceVariables);

		if(typeVariables)
		{
			std::string variableName = m_contextProperty->GetAttribute("variableName").GetString();
			int componentIdx = m_contextProperty->GetAttribute("componentIdx").GetInteger();

			//Find or add overridden var on game object
			GameObjectVariable* variable = FindVariable(*instanceVariables, variableName, componentIdx);
			if (!variable)
			{
				if (const GameObjectVariable* original = FindVariable(*typeVariables, variableName, componentIdx))
				{
					variable = &gameObject->AddVariable();
					*variable = *original;
				}
			}

			if (variable)
			{
				switch (event.GetId())
				{
					case ContextMenu::Default:
					{
						break;
					}

					case ContextMenu::EditScript:
					{
#if defined BEEHIVE_PLUGIN_LUMINARY
						//TODO: Do this in Beehive-side script compiler
						const std::string engineRootDir = m_project.m_settings.engineRootDir;
						const std::string projectRootDir = m_project.m_settings.projectRootDir;

						const std::string scriptsSourceDir = projectRootDir + "\\SCRIPTS\\";
						const std::string scriptsEngineIncludes = engineRootDir + "\\INCLUDE\\";

						const std::string scriptSourceFilename = gameObjectType->GetName() + ".cpp";
						const std::string scriptSourceFullPath = scriptsSourceDir + "\\" + scriptSourceFilename;

						//Generate header
						luminary::ScriptTranspiler scriptTranspiler;
						luminary::Entity entity;
						luminary::beehive::ConvertScriptEntity(*gameObjectType, entity);
						scriptTranspiler.GenerateEntityCppHeader(entity, scriptsSourceDir);

						//Generate boilerplate
						if (ion::io::FileDevice* device = ion::io::FileDevice::GetDefault())
						{
							if (!device->GetFileExists(scriptSourceFullPath))
							{
								scriptTranspiler.GenerateEntityCppBoilerplate(entity, scriptsSourceDir);
								variable->m_value = scriptSourceFilename;
								Refresh();
							}
						}

						//Open in default shell program
						std::string shellCmd = "rundll32 SHELL32.DLL,ShellExec_RunDLL \"" + scriptSourceFullPath + "\"";
						wxExecute(shellCmd);
#endif
						break;
					}

					case ContextMenu::CompileScript:
					{
						//TODO: Do this in Beehive-side script compiler
						m_mainWindow->ShowPanelScriptCompile();
						if (ScriptCompilePanel* panel = m_mainWindow->GetScriptCompilePanel())
						{
							const std::string engineRootDir = m_project.m_settings.engineRootDir;
							const std::string projectRootDir = m_project.m_settings.projectRootDir;

							const std::string scriptsSourceDir = projectRootDir + "\\SCRIPTS\\";
							const std::string scriptsEngineIncludes = engineRootDir + "\\INCLUDE\\";
							const std::string scriptsExportDir = projectRootDir + "\\DATA\\SCRIPTS\\";

							const std::string scriptSourceFilename = gameObjectType->GetName() + ".cpp";
							const std::string scriptSourceFullPath = scriptsSourceDir + "\\" + scriptSourceFilename;

							//Generate header
							luminary::ScriptTranspiler scriptTranspiler;
							luminary::Entity entity;
							luminary::beehive::ConvertScriptEntity(*gameObjectType, entity);
							scriptTranspiler.GenerateEntityCppHeader(entity, scriptsSourceDir);

							//Generate boilerplate
							if (ion::io::FileDevice* device = ion::io::FileDevice::GetDefault())
							{
								if (!device->GetFileExists(scriptSourceFullPath))
								{
									scriptTranspiler.GenerateEntityCppBoilerplate(entity, scriptsSourceDir);
									variable->m_value = scriptSourceFilename;
									Refresh();
								}
							}

							//Compile
							const std::string scriptOutNameRelease = ion::string::RemoveSubstring(scriptSourceFilename, ".cpp");
							const std::string scriptOutFullPathRelease = scriptsExportDir + scriptOutNameRelease;
							std::vector<std::string> includes;
							std::vector<std::string> defines;
							includes.push_back(scriptsEngineIncludes);
							includes.push_back(scriptsSourceDir);
							defines.push_back("_RELEASE");
							panel->BeginCompileAsync(scriptSourceFullPath, scriptOutFullPathRelease, includes, defines, nullptr);
						}

						break;
					}
				}
			}
		}

		m_contextProperty = nullptr;
	}
}

void PropertyPanel::SetGameObject(GameObjectId gameObjectId)
{
	m_gameObjectId = gameObjectId;
	m_gameObjectTypeId = InvalidGameObjectTypeId;
	m_archetypeId = InvalidGameObjectArchetypeId;
	Refresh();
}

void PropertyPanel::SetArchetype(GameObjectTypeId gameObjectTypeId, GameObjectArchetypeId archetypeId)
{
	m_gameObjectId = InvalidGameObjectId;
	m_gameObjectTypeId = gameObjectTypeId;
	m_archetypeId = archetypeId;
	Refresh();
}

void PropertyPanel::AddProperty(const GameObjectType& gameObjectType, const GameObjectVariable& variable, int componentIdx, const Actor* actor, const GameObjectVariable* spriteSheetVar, bool enabled)
{
	wxPGProperty* property = nullptr;

	const std::string propName = variable.m_componentName + "_" + variable.m_name + "_" + std::to_string(componentIdx);

#if defined BEEHIVE_PLUGIN_LUMINARY
	if (variable.HasTag("SPRITE_SHEET"))
	{
		if (actor)
		{
			wxArrayString* list = new wxArrayString();
			int selection = PopulateSpriteSheetList(*list, *actor, variable.m_value);
			wxEnumProperty* choiceProp = new wxEnumProperty(variable.m_name, propName, *list);
			property = choiceProp;

			if(selection >= 0 && selection < list->size())
				choiceProp->SetChoiceSelection(selection);
		}
	}
	else if (variable.HasTag("SPRITE_ANIM"))
	{
		//Find sprite sheet in this component first
		if (actor)
		{
			SpriteSheetId spriteSheetId = InvalidSpriteSheetId;

			if (!spriteSheetVar)
			{
				spriteSheetVar = gameObjectType.FindVariableByTag("SPRITE_SHEET", variable.m_componentIdx);
			}

			if (spriteSheetVar && actor)
			{
				spriteSheetId = actor->FindSpriteSheetId(spriteSheetVar->m_value);
			}

			wxArrayString* list = new wxArrayString();
			int selection = PopulateSpriteAnimList(*list, *actor, spriteSheetId, variable.m_value);
			wxEnumProperty* choiceProp = new wxEnumProperty(variable.m_name, propName, *list);
			property = choiceProp;

			if (selection >= 0 && selection < list->size())
				choiceProp->SetChoiceSelection(selection);
		}
	}
	else if (variable.HasTag("ENTITY_DESC"))
	{
		/*
		m_choiceValue->Enable(true);
		m_textValue->Enable(false);

		PopulateGameObjectTypeList(*m_choiceValue);
		m_choiceValue->SetStringSelection(variable->m_value);
		*/
	}
	else if (variable.HasTag("ENTITY_ARCHETYPE"))
	{
		/*
		m_choiceValue->Enable(true);
		m_textValue->Enable(false);

		//Find entity type in this component first
		GameObjectTypeId gameObjTypeId = InvalidGameObjectTypeId;

		const GameObjectVariable* gameObjTypeVar = m_gameObject->FindVariableByTag("ENTITY_DESC", variable->m_componentIdx);
		if (gameObjTypeVar)
		{
			if (const GameObjectType* gameObjType = m_project.FindGameObjectType(gameObjTypeVar->m_value))
			{
				gameObjTypeId = gameObjType->GetId();
			}
		}

		PopulateArchetypeList(*m_choiceValue, gameObjTypeId);
		m_choiceValue->SetStringSelection(variable->m_value);
		*/
	}
	else
	{
#endif

		property = new wxStringProperty(variable.m_name, propName, variable.m_value);
	}

	if (property)
	{
		property->Enable(enabled);
		property->SetAttribute("variableName", variable.m_name);
		property->SetAttribute("componentIdx", componentIdx);
		property->SetAttribute("variableSize", variable.m_size);
		property->SetAttribute("variable", wxVariant((void*)&variable));
		property->SetAttribute("isScript", variable.HasTag("SCRIPT_DATA"));
		m_propertyGrid->Append(property);
	}
}

int PropertyPanel::PopulateSpriteActorList(wxArrayString& list, const std::string& selectedValue)
{
	int selection = -1;
	int index = 0;

	for (TActorMap::const_iterator it = m_project.ActorsBegin(), end = m_project.ActorsEnd(); it != end; ++it, ++index)
	{
		list.Add(it->second.GetName());

		if (it->second.GetName() == selectedValue)
			selection = index;
	}

	return selection;
}

int PropertyPanel::PopulateSpriteSheetList(wxArrayString& list, const Actor& actor, const std::string& selectedValue)
{
	int selection = -1;
	int index = 0;

	for (TSpriteSheetMap::const_iterator it = actor.SpriteSheetsBegin(), end = actor.SpriteSheetsEnd(); it != end; ++it, ++index)
	{
		list.Add(it->second.GetName());

		if (it->second.GetName() == selectedValue)
			selection = index;
	}

	return selection;
}

int PropertyPanel::PopulateSpriteAnimList(wxArrayString& list, const Actor& actor, SpriteSheetId spriteSheetId, const std::string& selectedValue)
{
	int selection = -1;
	int index = 0;

	if (const SpriteSheet* spriteSheet = actor.GetSpriteSheet(spriteSheetId))
	{
		for (TSpriteAnimMap::const_iterator it = spriteSheet->AnimationsBegin(), end = spriteSheet->AnimationsEnd(); it != end; ++it)
		{
			list.Add(it->second.GetName());

			if (it->second.GetName() == selectedValue)
				selection = index;
		}
	}

	return selection;
}
