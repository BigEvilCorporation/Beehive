///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "GameObjectTypeDialog.h"
#include "../Project.h"

GameObjectTypeDialog::GameObjectTypeDialog(MainWindow& mainWindow, Project& project)
	: GameObjTypeDialogBase((wxWindow*)&mainWindow)
	, m_mainWindow(mainWindow)
	, m_project(project)
{
	m_currentTypeId = InvalidGameObjTypeId;
	m_currentVariable = NULL;
}

void GameObjectTypeDialog::OnToolGameObjAdd(wxCommandEvent& event)
{
	m_currentTypeId = m_project.AddGameObjectType();

	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		PopulateTypeList();
		PopulateTypeFields(gameObjType);
		PopulateVarsList(gameObjType);
	}
}

void GameObjectTypeDialog::OnToolGameObjRemove(wxCommandEvent& event)
{
	if(m_currentTypeId != InvalidGameObjTypeId)
	{
		m_project.RemoveGameObjectType(m_currentTypeId);
		m_currentTypeId = InvalidGameObjTypeId;
		PopulateTypeList();
		PopulateTypeFields(NULL);
		PopulateVarsList(NULL);
	}
}

void GameObjectTypeDialog::OnToolVariableAdd(wxCommandEvent& event)
{
	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		m_currentVariable = &gameObjType->AddVariable();
		PopulateVarsList(gameObjType);
	}
}

void GameObjectTypeDialog::OnToolVariableRemove(wxCommandEvent& event)
{
	if(m_currentVariable)
	{
		if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
		{
			gameObjType->RemoveVariable(*m_currentVariable);
			PopulateVarsList(gameObjType);
		}
	}
}

void GameObjectTypeDialog::OnSelectGameObjType(wxCommandEvent& event)
{
	m_currentTypeId = m_gameObjIndexMap[event.GetInt()];

	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		PopulateTypeFields(gameObjType);
		PopulateVarsList(gameObjType);
		PopulateVarsFields(NULL);
	}
}

void GameObjectTypeDialog::OnSelectVariable(wxListEvent& event)
{
	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		m_currentVariable = gameObjType->GetVariable(event.GetIndex());
		PopulateVarsFields(m_currentVariable);
	}
}

void GameObjectTypeDialog::OnGameObjNameChange(wxCommandEvent& event)
{
	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		gameObjType->SetName(event.GetString().c_str().AsChar());
		PopulateTypeList();
	}
}

void GameObjectTypeDialog::OnVariableNameChanged(wxCommandEvent& event)
{
	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		if(m_currentVariable)
		{
			m_currentVariable->m_name = event.GetString();
			PopulateVarsList(gameObjType);
		}
	}
}

void GameObjectTypeDialog::OnVariableSizeChanged(wxCommandEvent& event)
{
	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		if(m_currentVariable)
		{
			switch(event.GetSelection())
			{
			case 0:
				m_currentVariable->m_size = GameObjectType::eSizeByte;
				break;
			case 1:
				m_currentVariable->m_size = GameObjectType::eSizeWord;
				break;
			case 2:
				m_currentVariable->m_size = GameObjectType::eSizeLong;
				break;
			default:
				ion::debug::Error("GameObjectTypeDialog::OnVariableSizeChanged() - Bad size");
				break;
			}

			PopulateVarsList(gameObjType);
		}
	}
}

void GameObjectTypeDialog::OnValueChanged(wxCommandEvent& event)
{
	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		if(m_currentVariable)
		{
			m_currentVariable->m_value = event.GetString();
			PopulateVarsList(gameObjType);
		}
	}
}

void GameObjectTypeDialog::OnWidthChanged(wxSpinEvent& event)
{
	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		gameObjType->SetDimensions(ion::Vector2i(event.GetInt(), gameObjType->GetDimensions().y));
	}
}

void GameObjectTypeDialog::OnHeightChanged(wxSpinEvent& event)
{
	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		gameObjType->SetDimensions(ion::Vector2i(gameObjType->GetDimensions().x, event.GetInt()));
	}
}

void GameObjectTypeDialog::OnBtnImport(wxCommandEvent& event)
{

}

void GameObjectTypeDialog::OnBtnExport(wxCommandEvent& event)
{

}

void GameObjectTypeDialog::PopulateTypeList()
{
	m_listGameObjTypes->Clear();
	m_gameObjIndexMap.clear();

	const TGameObjTypeMap& typesMap = m_project.GetGameObjectTypes();

	int index = 0;
	for(TGameObjTypeMap::const_iterator it = typesMap.begin(), end = typesMap.end(); it != end; ++it, ++index)
	{
		m_listGameObjTypes->Insert(wxString(it->second.GetName()), index);
		m_gameObjIndexMap.insert(std::make_pair(index, it->second.GetId()));
	}
}

void GameObjectTypeDialog::PopulateTypeFields(GameObjectType* gameObjType)
{
	m_textGameObjName->SetValue(gameObjType->GetName());
	m_spinWidth->SetValue(gameObjType->GetDimensions().x);
	m_spinHeight->SetValue(gameObjType->GetDimensions().y);
}

void GameObjectTypeDialog::PopulateVarsList(GameObjectType* gameObjType)
{
	m_listVariables->ClearAll();

	wxListItem col0;
	col0.SetId(0);
	col0.SetText(_("Name"));
	col0.SetWidth(50);
	m_listVariables->InsertColumn(0, col0);

	wxListItem col1;
	col1.SetId(1);
	col1.SetText(_("Size"));
	m_listVariables->InsertColumn(1, col1);

	wxListItem col2;
	col1.SetId(2);
	col1.SetText(_("Value"));
	m_listVariables->InsertColumn(2, col2);

	if(gameObjType)
	{
		const std::vector<GameObjectType::Variable>& variables = gameObjType->GetVariables();

		for(int i = 0; i < variables.size(); i++)
		{
			wxListItem item;
			item.SetId(i);
			m_listVariables->InsertItem(item);

			std::string sizeText;
			
			switch(variables[i].m_size)
			{
			case GameObjectType::eSizeByte:
				sizeText = "Byte";
				break;
			case GameObjectType::eSizeWord:
				sizeText = "Word";
				break;
			case GameObjectType::eSizeLong:
				sizeText = "Long";
				break;
			}

			m_listVariables->SetItem(i, 0, wxString(variables[i].m_name));
			m_listVariables->SetItem(i, 1, wxString(sizeText));
			m_listVariables->SetItem(i, 2, wxString(variables[i].m_value));
		}
	}
}

void GameObjectTypeDialog::PopulateVarsFields(GameObjectType::Variable* variable)
{
	if(variable)
	{
		m_textVariableName->SetValue(variable->m_name);
		m_textValue->SetValue(variable->m_value);

		int choice = 0;

		switch(variable->m_size)
		{
		case GameObjectType::eSizeByte:
			choice = 0;
			break;
		case GameObjectType::eSizeWord:
			choice = 1;
			break;
		case GameObjectType::eSizeLong:
			choice = 2;
			break;
		}

		m_choiceSize->SetSelection(choice);
	}
	else
	{
		m_textVariableName->SetValue("");
		m_textValue->SetValue("");
		m_choiceSize->SetSelection(0);
	}
}