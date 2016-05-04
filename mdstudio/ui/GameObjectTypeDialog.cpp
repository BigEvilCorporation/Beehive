///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "GameObjectTypeDialog.h"
#include "MainWindow.h"
#include "../Project.h"

GameObjectTypeDialog::GameObjectTypeDialog(MainWindow& mainWindow, Project& project)
	: GameObjTypeDialogBase((wxWindow*)&mainWindow)
	, m_mainWindow(mainWindow)
	, m_project(project)
{
	m_currentTypeId = InvalidGameObjectTypeId;
	m_currentVariable = NULL;

	PopulateTypeList();
	PopulateVarsList(NULL);
}

void GameObjectTypeDialog::OnToolGameObjAdd(wxCommandEvent& event)
{
	m_currentTypeId = m_project.AddGameObjectType();

	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		PopulateTypeList();
		PopulateTypeFields(gameObjType);
		PopulateVarsList(gameObjType);
		m_mainWindow.RedrawPanel(MainWindow::ePanelGameObjectTypes);
	}
}

void GameObjectTypeDialog::OnToolGameObjRemove(wxCommandEvent& event)
{
	if(m_currentTypeId != InvalidGameObjectTypeId)
	{
		m_project.RemoveGameObjectType(m_currentTypeId);
		m_currentTypeId = InvalidGameObjectTypeId;
		PopulateTypeList();
		PopulateTypeFields(NULL);
		PopulateVarsList(NULL);
		m_mainWindow.RedrawPanel(MainWindow::ePanelGameObjectTypes);
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

void GameObjectTypeDialog::OnBtnApplyChanges(wxCommandEvent& event)
{
	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		gameObjType->SetName(m_textGameObjName->GetValue().c_str().AsChar());
		gameObjType->SetDimensions(ion::Vector2i(m_spinWidth->GetValue(), m_spinHeight->GetValue()));

		SpriteSheetId prevSpriteSheet = gameObjType->GetPreviewSpriteSheet();

		if(m_spriteSheetCache.size() > 0)
		{
			int spriteSheetIndex = m_choiceSprites->GetCurrentSelection();
			if(spriteSheetIndex >= 0 && spriteSheetIndex < m_spriteSheetCache.size())
			{
				gameObjType->SetPreviewSpriteSheet(m_spriteSheetCache[spriteSheetIndex]);
			}
		}

		if(m_currentVariable)
		{
			m_currentVariable->m_name = m_textVariableName->GetValue().c_str().AsChar();
			m_currentVariable->m_value = m_textValue->GetValue().c_str().AsChar();

			switch(m_choiceSize->GetSelection())
			{
			case 0:
				m_currentVariable->m_size = eSizeByte;
				break;
			case 1:
				m_currentVariable->m_size = eSizeWord;
				break;
			case 2:
				m_currentVariable->m_size = eSizeLong;
				break;
			default:
				ion::debug::Error("GameObjectTypeDialog::OnVariableSizeChanged() - Bad size");
				break;
			}
		}

		PopulateTypeList();
		PopulateVarsList(gameObjType);
		m_mainWindow.RedrawPanel(MainWindow::ePanelGameObjectTypes);
		m_mainWindow.RedrawPanel(MainWindow::ePanelMap);
	}
}

void GameObjectTypeDialog::OnBtnImport(wxCommandEvent& event)
{
	wxFileDialog dialogue(this, _("Open BEE GameObj file"), "", "", "BEE_GameObj files (*.bee_gameobj)|*.bee_gameobj", wxFD_OPEN);
	if(dialogue.ShowModal() == wxID_OK)
	{
		std::string filename = dialogue.GetPath().c_str().AsChar();
		m_project.ImportGameObjectTypes(filename);
		PopulateTypeList();
		PopulateVarsList(NULL);
		m_mainWindow.RedrawPanel(MainWindow::ePanelGameObjectTypes);
	}
}

void GameObjectTypeDialog::OnBtnExport(wxCommandEvent& event)
{
	wxFileDialog dialogue(this, _("Save BEE GameObj file"), "", "", "BEE_GameObj files (*.bee_gameobj)|*.bee_gameobj", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if(dialogue.ShowModal() == wxID_OK)
	{
		std::string filename = dialogue.GetPath().c_str().AsChar();
		m_project.ExportGameObjectTypes(filename);
	}
}

void GameObjectTypeDialog::PopulateTypeList()
{
	m_listGameObjTypes->Clear();
	m_gameObjIndexMap.clear();

	const TGameObjectTypeMap& typesMap = m_project.GetGameObjectTypes();

	int index = 0;
	for(TGameObjectTypeMap::const_iterator it = typesMap.begin(), end = typesMap.end(); it != end; ++it, ++index)
	{
		m_listGameObjTypes->Insert(wxString(it->second.GetName()), index);
		m_gameObjIndexMap.insert(std::make_pair(index, it->second.GetId()));
	}
}

void GameObjectTypeDialog::PopulateTypeFields(GameObjectType* gameObjType)
{
	if(gameObjType)
	{
		m_textGameObjName->SetValue(gameObjType->GetName());
		m_spinWidth->SetValue(gameObjType->GetDimensions().x);
		m_spinHeight->SetValue(gameObjType->GetDimensions().y);

		m_choiceSprites->Clear();
		m_spriteSheetCache.clear();

		for(TActorMap::const_iterator it = m_project.ActorsBegin(), end = m_project.ActorsEnd(); it != end; ++it)
		{
			for(TSpriteSheetMap::const_iterator itSprite = it->second.SpriteSheetsBegin(), endSprite = it->second.SpriteSheetsEnd(); itSprite != endSprite; ++itSprite)
			{
				//Store by index
				m_spriteSheetCache.push_back(itSprite->first);

				//Add to list
				m_choiceSprites->AppendString(itSprite->second.GetName());
			}
		}
	}
	else
	{
		m_textGameObjName->SetValue("");
		m_choiceSprites->Clear();
		m_spinWidth->SetValue(1);
		m_spinHeight->SetValue(1);
	}
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
		const std::vector<GameObjectVariable>& variables = gameObjType->GetVariables();

		for(int i = 0; i < variables.size(); i++)
		{
			wxListItem item;
			item.SetId(i);
			m_listVariables->InsertItem(item);

			std::string sizeText;
			
			switch(variables[i].m_size)
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

			m_listVariables->SetItem(i, 0, wxString(variables[i].m_name));
			m_listVariables->SetItem(i, 1, wxString(sizeText));
			m_listVariables->SetItem(i, 2, wxString(variables[i].m_value));
		}
	}
}

void GameObjectTypeDialog::PopulateVarsFields(GameObjectVariable* variable)
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
	}
	else
	{
		m_textVariableName->SetValue("");
		m_textValue->SetValue("");
		m_choiceSize->SetSelection(0);
	}
}