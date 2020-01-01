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

#include "GameObjectParamsPanel.h"
#include "MainWindow.h"

static const std::string s_defaultVarName = "[ DEFAULT_VALUE ]";

GameObjectParamsPanel::GameObjectParamsPanel(MainWindow& mainWindow, Project& project, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: GameObjParamsPanelBase(parent, id, pos, size, style)
	, m_project(project)
	, m_mainWindow(mainWindow)
{
	m_gameObject = NULL;
	m_currentVariable = NULL;
	PopulateVarsList();

#if defined BEEHIVE_PLUGIN_LUMINARY
	// TODO: Remove from UI, uses game object type layout now
	m_toolAddVariable->Enable(false);
	m_toolRemoveVariable->Enable(false);
	m_textVariableName->Enable(false);
	m_choiceSize->Enable(false);

	//One sprite sheet+anim per ECSprite component
	m_choiceSpriteSheet->Enable(false);
	m_choiceSpriteAnim->Enable(false);
#endif
}

void GameObjectParamsPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(!m_mainWindow.IsRefreshLocked())
	{
		PopulateVarsList();
	}
}

void GameObjectParamsPanel::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;

	if(gameObject)
	{
		m_textObjectName->ChangeValue(m_gameObject->GetName());
		PopulateSpriteActorList(*m_choiceSpriteActor);
		PopulateArchetypeList(*m_choiceArchetypes, gameObject->GetTypeId());

#if !defined BEEHIVE_PLUGIN_LUMINARY
		//One sprite sheet+anim per ECSprite component
		PopulateSpriteSheetList(*m_choiceSpriteSheet);
		PopulateSpriteAnimList(*m_choiceSpriteAnim, m_gameObject->GetSpriteSheetId());
#endif

		PopulateVarsList();
	}
}

void GameObjectParamsPanel::OnToolVariableAdd(wxCommandEvent& event)
{
	if(m_gameObject)
	{
		m_currentVariable = &m_gameObject->AddVariable();
		PopulateVarsList();
	}
}

void GameObjectParamsPanel::OnToolVariableRemove(wxCommandEvent& event)
{
	if(m_currentVariable)
	{
		if(m_gameObject)
		{
			m_gameObject->RemoveVariable(*m_currentVariable);
			PopulateVarsList();
		}
	}
}

void GameObjectParamsPanel::OnSelectVariable(wxListEvent& event)
{
	if(m_gameObject)
	{
#if defined BEEHIVE_PLUGIN_LUMINARY
		if (const GameObjectType* gameObjectType = m_project.GetGameObjectType(m_gameObject->GetTypeId()))
		{
			if (const GameObjectVariable* variable = gameObjectType->GetVariable(event.GetIndex()))
			{
				//Find overridden variable
				m_currentVariable = m_gameObject->FindVariable(variable->m_name, variable->m_componentIdx);
				if (!m_currentVariable)
				{
					//No overridden variable, show archetype's value
					m_currentVariable = variable;
				}
			}
		}
#else
		m_currentVariable = m_gameObject->GetVariable(event.GetIndex());
#endif
		
		PopulateVarsFields(m_currentVariable);
	}
}

void GameObjectParamsPanel::OnButtonApplyObjectName(wxCommandEvent& event)
{
	if(m_gameObject)
	{
		m_gameObject->SetName(m_textObjectName->GetValue().c_str().AsChar());
		m_mainWindow.RefreshAnimActors();
	}
}

void GameObjectParamsPanel::OnButtonNewArchetype(wxCommandEvent& event)
{
	if (m_gameObject)
	{
		if (GameObjectType* gameObjectType = m_project.GetGameObjectType(m_gameObject->GetTypeId()))
		{
			GameObjectArchetypeId archetypeId = gameObjectType->CreateArchetypeFromState(*m_gameObject);

			PopulateArchetypeList(*m_choiceArchetypes, m_gameObject->GetTypeId());

			if (const GameObjectArchetype* archetype = gameObjectType->GetArchetype(archetypeId))
			{
				m_choiceArchetypes->SetStringSelection(archetype->name);
			}
		}
	}
}

void GameObjectParamsPanel::OnEnterTextVariable(wxCommandEvent& event)
{
#if !BEEHIVE_PLUGIN_LUMINARY
	if (m_gameObject && m_currentVariable)
	{
		if (m_currentVariable->m_name != s_defaultVarName)
		{
			//Find or add overridden var on game object
			GameObjectVariable* variable = m_gameObject->FindVariable(m_currentVariable->m_name, m_currentVariable->m_componentIdx);
			if (!variable)
			{
				if (const GameObjectType* gameObjectType = m_project.GetGameObjectType(m_gameObject->GetTypeId()))
				{
					if (const GameObjectVariable* original = gameObjectType->FindVariable(m_currentVariable->m_name, m_currentVariable->m_componentIdx))
					{
						variable = &m_gameObject->AddVariable();
						*variable = *original;
					}
				}
			}

			variable->m_name = m_textVariableName->GetValue().c_str().AsChar();
			PopulateVarsList();
		}
	}
#endif
}

void GameObjectParamsPanel::OnVariableSizeChanged(wxCommandEvent& event)
{
#if !BEEHIVE_PLUGIN_LUMINARY
	if (m_gameObject && m_currentVariable)
	{
		//Find or add overridden var on game object
		GameObjectVariable* variable = m_gameObject->FindVariable(m_textVariableName->GetValue().c_str().AsChar());
		if (!variable)
		{
			variable = &m_gameObject->AddVariable();
			variable->m_name = m_textVariableName->GetValue().c_str().AsChar();
		}

		switch (m_choiceSize->GetSelection())
		{
		case 0:
			variable->m_size = eSizeByte;
			break;
		case 1:
			variable->m_size = eSizeWord;
			break;
		case 2:
			variable->m_size = eSizeLong;
			break;
		default:
			ion::debug::Error("GameObjectParamsPanel::OnVariableSizeChanged() - Bad size");
		}
	}
#endif
}

void GameObjectParamsPanel::OnEnterTextValue(wxCommandEvent& event)
{
	if (m_gameObject && m_currentVariable)
	{
		if (m_textValue->GetValue() != s_defaultVarName)
		{
			//Find or add overridden var on game object
			GameObjectVariable* variable = m_gameObject->FindVariable(m_currentVariable->m_name, m_currentVariable->m_componentIdx);
			if (!variable)
			{
				if (const GameObjectType* gameObjectType = m_project.GetGameObjectType(m_gameObject->GetTypeId()))
				{
					if (const GameObjectVariable* original = gameObjectType->FindVariable(m_currentVariable->m_name, m_currentVariable->m_componentIdx))
					{
						variable = &m_gameObject->AddVariable();
						*variable = *original;
					}
				}
			}

			variable->m_value = m_textValue->GetValue().c_str().AsChar();
			PopulateVarsList();
		}
	}
}

void GameObjectParamsPanel::OnSelectArchetype(wxCommandEvent& event)
{
	if (m_gameObject)
	{
		if (const GameObjectType* gameObjectType = m_project.GetGameObjectType(m_gameObject->GetTypeId()))
		{
			if (const GameObjectArchetype* archetype = gameObjectType->GetArchetype(m_choiceArchetypes->GetStringSelection().c_str().AsChar()))
			{
				m_gameObject->ApplyArchetype(*archetype);
			}
		}
	}
}

void GameObjectParamsPanel::OnSelectValue(wxCommandEvent& event)
{
	if (m_gameObject && m_currentVariable)
	{
		if (m_textValue->GetValue() != s_defaultVarName)
		{
			//Find or add overridden var on game object
			GameObjectVariable* variable = m_gameObject->FindVariable(m_currentVariable->m_name, m_currentVariable->m_componentIdx);
			if (!variable)
			{
				if (const GameObjectType* gameObjectType = m_project.GetGameObjectType(m_gameObject->GetTypeId()))
				{
					if (const GameObjectVariable* original = gameObjectType->FindVariable(m_currentVariable->m_name, m_currentVariable->m_componentIdx))
					{
						variable = &m_gameObject->AddVariable();
						*variable = *original;
					}
				}
			}

			variable->m_value = m_choiceValue->GetStringSelection().c_str().AsChar();
			PopulateVarsList();
		}
	}
}

void GameObjectParamsPanel::OnSelectSpriteActor(wxCommandEvent& event)
{
	m_gameObject->SetSpriteActorId(m_project.FindActorId(m_choiceSpriteActor->GetStringSelection().c_str().AsChar()));

#if !defined BEEHIVE_PLUGIN_LUMINARY
	//One sprite sheet+anim per ECSprite component
	PopulateSpriteSheetList(*m_choiceSpriteSheet);
	PopulateSpriteAnimList(*m_choiceSpriteAnim, m_gameObject->GetSpriteSheetId());
#endif
}

void GameObjectParamsPanel::OnSelectSpriteSheet(wxCommandEvent& event)
{
	if (const Actor* actor = m_project.GetActor(m_gameObject->GetSpriteActorId()))
	{
		m_gameObject->SetSpriteSheetId(actor->FindSpriteSheetId(m_choiceSpriteSheet->GetStringSelection().c_str().AsChar()));
		PopulateSpriteAnimList(*m_choiceSpriteAnim, m_gameObject->GetSpriteSheetId());
	}
}

void GameObjectParamsPanel::OnSelectSpriteAnim(wxCommandEvent& event)
{
	if (const Actor* actor = m_project.GetActor(m_gameObject->GetSpriteActorId()))
	{
		if (const SpriteSheet* spriteSheet = actor->GetSpriteSheet(m_gameObject->GetSpriteSheetId()))
		{
			m_gameObject->SetSpriteAnim(spriteSheet->FindAnimationId(m_choiceSpriteAnim->GetStringSelection().c_str().AsChar()));
		}
	}
}

void GameObjectParamsPanel::PopulateArchetypeList(wxChoice& list, GameObjectTypeId gameObjectTypeId)
{
	list.Clear();
	list.SetSelection(-1);

	if (const GameObjectType* gameObjectType = m_project.GetGameObjectType(gameObjectTypeId))
	{
		for (TGameObjectArchetypeMap::const_iterator it = gameObjectType->GetArchetypes().begin(), end = gameObjectType->GetArchetypes().end(); it != end; ++it)
		{
			list.AppendString(it->second.name);
		}
	}
}

void GameObjectParamsPanel::PopulateSpriteActorList(wxChoice& list)
{
	list.Clear();
	list.SetSelection(-1);

	for (TActorMap::const_iterator it = m_project.ActorsBegin(), end = m_project.ActorsEnd(); it != end; ++it)
	{
		list.AppendString(it->second.GetName());
	}

	if (const Actor* actor = m_project.GetActor(m_gameObject->GetSpriteActorId()))
	{
		list.SetStringSelection(actor->GetName());
	}
}

void GameObjectParamsPanel::PopulateSpriteSheetList(wxChoice& list)
{
	list.Clear();
	list.SetSelection(-1);

	if (const Actor* actor = m_project.GetActor(m_gameObject->GetSpriteActorId()))
	{
		for (TSpriteSheetMap::const_iterator it = actor->SpriteSheetsBegin(), end = actor->SpriteSheetsEnd(); it != end; ++it)
		{
			list.AppendString(it->second.GetName());
		}

		if (const SpriteSheet* spriteSheet = actor->GetSpriteSheet(m_gameObject->GetSpriteSheetId()))
		{
			list.SetStringSelection(spriteSheet->GetName());
		}
	}
}

void GameObjectParamsPanel::PopulateSpriteAnimList(wxChoice& list, SpriteSheetId spriteSheetId)
{
	list.Clear();
	list.SetSelection(-1);

	if (const Actor* actor = m_project.GetActor(m_gameObject->GetSpriteActorId()))
	{
		if (const SpriteSheet* spriteSheet = actor->GetSpriteSheet(spriteSheetId))
		{
			for (TSpriteAnimMap::const_iterator it = spriteSheet->AnimationsBegin(), end = spriteSheet->AnimationsEnd(); it != end; ++it)
			{
				list.AppendString(it->second.GetName());
			}

			if (const SpriteAnimation* spriteAnim = spriteSheet->GetAnimation(m_gameObject->GetSpriteAnim()))
			{
				list.SetStringSelection(spriteAnim->GetName());
			}
		}
	}
}

void GameObjectParamsPanel::PopulateGameObjectTypeList(wxChoice& list)
{
	list.Clear();
	list.SetSelection(-1);

	for (TGameObjectTypeMap::const_iterator it = m_project.GetGameObjectTypes().begin(), end = m_project.GetGameObjectTypes().end(); it != end; ++it)
	{
		list.AppendString(it->second.GetName());
	}
}

void GameObjectParamsPanel::PopulateVarsList()
{
	m_listVariables->ClearAll();

	wxListItem col0;
	col0.SetId((int)Columns::Name);
	col0.SetText(_("Name"));
	m_listVariables->InsertColumn((int)Columns::Name, col0);

	wxListItem col1;
	col1.SetId((int)Columns::Size);
	col1.SetText(_("Size"));
	m_listVariables->InsertColumn((int)Columns::Size, col1);

	wxListItem col2;
	col2.SetId((int)Columns::Value);
	col2.SetText(_("Value"));
	m_listVariables->InsertColumn((int)Columns::Value, col2);

	wxListItem col3;
	col3.SetId((int)Columns::Tags);
	col3.SetText(_("Tags"));
	m_listVariables->InsertColumn((int)Columns::Tags, col3);

	if(m_gameObject)
	{
#if defined BEEHIVE_PLUGIN_LUMINARY
		if (const GameObjectType* gameObjectType = m_project.GetGameObjectType(m_gameObject->GetTypeId()))
		{
			//Populate var names and layout from game object type, but take values from game object
			const std::vector<GameObjectVariable>& variables = gameObjectType->GetVariables();
#else
		const std::vector<GameObjectVariable>& variables = m_gameObject->GetVariables();
		{
#endif

			for (int i = 0; i < variables.size(); i++)
			{
				wxListItem item;
				item.SetId(i);
				m_listVariables->InsertItem(item);

				std::string sizeText;

				switch (variables[i].m_size)
				{
				case eSizeByte:
					sizeText = "Byte";
					break;
				case eSizeWord:
					sizeText = "Word";
					break;
				case eSizeLong:
					sizeText = "Long";
					break;
				}

				m_listVariables->SetItem(i,(int)Columns::Name, wxString(variables[i].m_name));
				m_listVariables->SetItem(i, (int)Columns::Size, wxString(sizeText));
				m_listVariables->SetItemData(i, variables[i].m_componentIdx);

				if (const GameObjectVariable* overriddenVar = m_gameObject->FindVariable(variables[i].m_name, variables[i].m_componentIdx))
				{
					m_listVariables->SetItem(i, (int)Columns::Value, wxString(overriddenVar->m_value));
				}
				else
				{
					m_listVariables->SetItem(i, (int)Columns::Value, wxString(s_defaultVarName));
				}

				std::string tags;
				for (int j = 0; j < variables[i].m_tags.size(); j++)
				{
					tags += variables[i].m_tags[j] + " ";
				}

				m_listVariables->SetItem(i, (int)Columns::Tags, wxString(tags));
			}
		}
	}
}

void GameObjectParamsPanel::PopulateVarsFields(const GameObjectVariable* variable)
{
	if(variable)
	{
		m_textVariableName->ChangeValue(variable->m_name);

#if defined BEEHIVE_PLUGIN_LUMINARY
		if (variable->HasTag("SPRITE_SHEET"))
		{
			m_choiceValue->Enable(true);
			m_textValue->Enable(false);
			PopulateSpriteSheetList(*m_choiceValue);
			m_choiceValue->SetStringSelection(variable->m_value);
		}
		else if (variable->HasTag("SPRITE_ANIM"))
		{
			m_choiceValue->Enable(true);
			m_textValue->Enable(false);

			//Find sprite sheet in this component first
			const Actor* actor = m_project.GetActor(m_gameObject->GetSpriteActorId());
			SpriteSheetId spriteSheetId = InvalidSpriteSheetId;
			const GameObjectVariable* spriteSheetVar = m_gameObject->FindVariableByTag("SPRITE_SHEET", variable->m_componentIdx);

			if (!spriteSheetVar)
			{
				if (const GameObjectType* gameObjectType = m_project.GetGameObjectType(m_gameObject->GetTypeId()))
				{
					spriteSheetVar = gameObjectType->FindVariableByTag("SPRITE_SHEET", variable->m_componentIdx);
				}
			}

			if (spriteSheetVar && actor)
			{
				spriteSheetId = actor->FindSpriteSheetId(spriteSheetVar->m_value);
			}

			PopulateSpriteAnimList(*m_choiceValue, spriteSheetId);
			m_choiceValue->SetStringSelection(variable->m_value);
		}
		else if (variable->HasTag("ENTITY_DESC"))
		{
			m_choiceValue->Enable(true);
			m_textValue->Enable(false);

			PopulateGameObjectTypeList(*m_choiceValue);
			m_choiceValue->SetStringSelection(variable->m_value);
		}
		else if (variable->HasTag("ENTITY_ARCHETYPE"))
		{
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
		}
		else
#endif
		{
			m_choiceValue->Enable(false);
			m_textValue->Enable(true);
			m_textValue->ChangeValue(variable->m_value);
		}

		int choice = 0;

		switch(variable->m_size)
		{
		case eSizeByte:
			choice = 0;
			break;
		case eSizeWord:
			choice = 1;
			break;
		case eSizeLong:
			choice = 2;
			break;
		}

		m_choiceSize->SetSelection(choice);

		//Vars with tags can't be edited, they're auto-populated by exporter
		m_textValue->Enable(variable->m_tags.size() == 0);

#if defined BEEHIVE_PLUGIN_LUMINARY
		m_textVariableName->Enable(false);
#endif
	}
	else
	{
		m_textVariableName->ChangeValue("");
		m_textValue->ChangeValue("");
		m_choiceSize->SetSelection(0);
	}
}