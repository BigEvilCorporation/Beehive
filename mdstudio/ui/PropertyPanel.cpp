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

static const std::string s_defaultVarName = "[ DEFAULT_VALUE ]";

PropertyPanel::PropertyPanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: PropertyPanelBase(parent, id, pos, size, style)
	, m_project(project)
	, m_mainWindow(mainWindow)
{
	m_gameObjectId = InvalidGameObjectId;
}

void PropertyPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		m_propertyGrid->Clear();

		if (const GameObject* gameObject = m_project.GetEditingMap().GetGameObject(m_gameObjectId))
		{
			if (const GameObjectType* gameObjectType = m_project.GetGameObjectType(gameObject->GetTypeId()))
			{
				//If entity has a sprite actor, find it and add property
				for (auto variable : gameObjectType->GetVariables())
				{
					if (variable.HasTag("SPRITE_SHEET"))
					{
						std::string currentActor;

						if (const Actor* actor = m_project.GetActor(gameObject->GetSpriteActorId()))
						{
							currentActor = actor->GetName();
						}

						wxArrayString* list = new wxArrayString();
						int selection = PopulateSpriteActorList(*list, currentActor);
						wxEnumProperty* choiceProp = new wxEnumProperty("Sprite Actor", "Sprite Actor", *list);

						if (selection >= 0 && selection < list->size())
							choiceProp->SetChoiceSelection(selection);

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
					for (auto variable : gameObjectType->GetVariables())
					{
						if (componentIdx != variable.m_componentIdx)
						{
							m_propertyGrid->Append(new wxPropertyCategory(variable.m_componentName));
							componentIdx = variable.m_componentIdx;
						}

						if (const GameObjectVariable* overriddenVar = gameObject->FindVariable(variable.m_name, variable.m_componentIdx))
						{
							AddProperty(*gameObject, *overriddenVar, variable.m_componentIdx);
						}
						else
						{
							AddProperty(*gameObject, variable, variable.m_componentIdx);
						}
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
		if (GameObject* gameObject = m_project.GetEditingMap().GetGameObject(m_gameObjectId))
		{
			wxString variableName = property->GetAttribute("variableName", "");
			wxVariant componentIdx = property->GetAttribute("componentIdx");
			wxString value = property->GetValueAsString();

			if (value != s_defaultVarName)
			{
				//Find or add overridden var on game object
				GameObjectVariable* variable = gameObject->FindVariable(variableName.c_str().AsChar(), componentIdx.GetInteger());
				if (!variable)
				{
					if (const GameObjectType* gameObjectType = m_project.GetGameObjectType(gameObject->GetTypeId()))
					{
						if (const GameObjectVariable* original = gameObjectType->FindVariable(variableName.c_str().AsChar(), componentIdx.GetInteger()))
						{
							variable = &gameObject->AddVariable();
							*variable = *original;
						}
					}
				}

				variable->m_value = value.c_str().AsChar();
				Refresh();
			}
		}
	}
}

void PropertyPanel::SetGameObject(GameObjectId gameObjectId)
{
	m_gameObjectId = gameObjectId;
	Refresh();
}

void PropertyPanel::AddProperty(const GameObject& gameObject, const GameObjectVariable& variable, int componentIdx, bool enabled)
{
	wxPGProperty* property = nullptr;

	const std::string propName = variable.m_componentName + "_" + variable.m_name + "_" + std::to_string(componentIdx);

#if defined BEEHIVE_PLUGIN_LUMINARY
	if (variable.HasTag("SPRITE_SHEET"))
	{
		if (const Actor* actor = m_project.GetActor(gameObject.GetSpriteActorId()))
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
		if (const Actor* actor = m_project.GetActor(gameObject.GetSpriteActorId()))
		{
			SpriteSheetId spriteSheetId = InvalidSpriteSheetId;
			const GameObjectVariable* spriteSheetVar = gameObject.FindVariableByTag("SPRITE_SHEET", variable.m_componentIdx);

			if (!spriteSheetVar)
			{
				if (const GameObjectType* gameObjectType = m_project.GetGameObjectType(gameObject.GetTypeId()))
				{
					spriteSheetVar = gameObjectType->FindVariableByTag("SPRITE_SHEET", variable.m_componentIdx);
				}
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
