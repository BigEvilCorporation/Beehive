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

	// TODO: Remove from UI, uses game object type layout now
	m_toolAddVariable->Enable(false);
	m_toolRemoveVariable->Enable(false);
	m_textVariableName->Enable(false);
	m_choiceSize->Enable(false);
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
		m_textObjectName->SetValue(m_gameObject->GetName());
	}

	PopulateSpriteActorList();
	PopulateVarsList();
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
		if (const GameObjectType* gameObjectType = m_project.GetGameObjectType(m_gameObject->GetTypeId()))
		{
			m_currentVariable = gameObjectType->GetVariable(event.GetIndex());
		}
		
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

void GameObjectParamsPanel::OnBtnApplyVariableChanges(wxCommandEvent& event)
{
	if(m_gameObject)
	{
		if(m_currentVariable)
		{
			if (m_textValue->GetValue() != s_defaultVarName)
			{
				//Find or add overridden var on game object
				GameObjectVariable* variable = m_gameObject->FindVariable(m_textVariableName->GetValue().c_str().AsChar());
				if (!variable)
				{
					variable = &m_gameObject->AddVariable();
					variable->m_name = m_textVariableName->GetValue().c_str().AsChar();
				}

				variable->m_value = m_textValue->GetValue().c_str().AsChar();
			}
		}

		PopulateVarsList();
		m_mainWindow.RedrawPanel(MainWindow::ePanelGameObjectParams);
	}
}

void GameObjectParamsPanel::OnSelectSpriteActor(wxCommandEvent& event)
{
	m_gameObject->SetSpriteActorId(m_project.FindActorId(m_choiceSpriteActor->GetStringSelection().c_str().AsChar()));
	PopulateSpriteSheetList();
}

void GameObjectParamsPanel::OnSelectSpriteSheet(wxCommandEvent& event)
{
	if (const Actor* actor = m_project.GetActor(m_gameObject->GetSpriteActorId()))
	{
		m_gameObject->SetSpriteSheetId(actor->FindSpriteSheetId(m_choiceSpriteSheet->GetStringSelection().c_str().AsChar()));
		PopulateSpriteAnimList();
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

void GameObjectParamsPanel::PopulateSpriteActorList()
{
	m_choiceSpriteActor->Clear();
	m_choiceSpriteSheet->Clear();
	m_choiceSpriteAnim->Clear();
	m_choiceSpriteActor->SetSelection(-1);
	m_choiceSpriteSheet->SetSelection(-1);
	m_choiceSpriteAnim->SetSelection(-1);

	for (TActorMap::const_iterator it = m_project.ActorsBegin(), end = m_project.ActorsEnd(); it != end; ++it)
	{
		m_choiceSpriteActor->AppendString(it->second.GetName());
	}

	if (const Actor* actor = m_project.GetActor(m_gameObject->GetSpriteActorId()))
	{
		m_choiceSpriteActor->SetStringSelection(actor->GetName());
		PopulateSpriteSheetList();
	}
}

void GameObjectParamsPanel::PopulateSpriteSheetList()
{
	m_choiceSpriteSheet->Clear();
	m_choiceSpriteAnim->Clear();
	m_choiceSpriteSheet->SetSelection(-1);
	m_choiceSpriteAnim->SetSelection(-1);

	if (const Actor* actor = m_project.GetActor(m_gameObject->GetSpriteActorId()))
	{
		for (TSpriteSheetMap::const_iterator it = actor->SpriteSheetsBegin(), end = actor->SpriteSheetsEnd(); it != end; ++it)
		{
			m_choiceSpriteSheet->AppendString(it->second.GetName());
		}

		if (const SpriteSheet* spriteSheet = actor->GetSpriteSheet(m_gameObject->GetSpriteSheetId()))
		{
			m_choiceSpriteSheet->SetStringSelection(spriteSheet->GetName());
			PopulateSpriteAnimList();
		}
	}
}

void GameObjectParamsPanel::PopulateSpriteAnimList()
{
	m_choiceSpriteAnim->Clear();
	m_choiceSpriteAnim->SetSelection(-1);

	if (const Actor* actor = m_project.GetActor(m_gameObject->GetSpriteActorId()))
	{
		if (const SpriteSheet* spriteSheet = actor->GetSpriteSheet(m_gameObject->GetSpriteSheetId()))
		{
			for (TSpriteAnimMap::const_iterator it = spriteSheet->AnimationsBegin(), end = spriteSheet->AnimationsEnd(); it != end; ++it)
			{
				m_choiceSpriteAnim->AppendString(it->second.GetName());
			}

			if (const SpriteAnimation* spriteAnim = spriteSheet->GetAnimation(m_gameObject->GetSpriteAnim()))
			{
				m_choiceSpriteAnim->SetStringSelection(spriteAnim->GetName());
			}
		}
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
		if (const GameObjectType* gameObjectType = m_project.GetGameObjectType(m_gameObject->GetTypeId()))
		{
			//Populate var names and layout from game object type, but take values from game object
			const std::vector<GameObjectVariable>& variables = gameObjectType->GetVariables();

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

				if (const GameObjectVariable* overriddenVar = m_gameObject->FindVariable(variables[i].m_name))
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
		m_textVariableName->SetValue(variable->m_name);
		m_textValue->SetValue(variable->m_value);

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
		m_btnApplyVarParams->Enable(variable->m_tags.size() == 0);
	}
	else
	{
		m_textVariableName->SetValue("");
		m_textValue->SetValue("");
		m_choiceSize->SetSelection(0);
	}
}