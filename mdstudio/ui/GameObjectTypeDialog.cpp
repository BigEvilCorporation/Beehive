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

#include "GameObjectTypeDialog.h"
#include "MainWindow.h"

GameObjectTypeDialog::GameObjectTypeDialog(MainWindow& mainWindow, Project& project, RenderResources& renderResources)
	: GameObjTypeDialogBase((wxWindow*)&mainWindow)
	, m_mainWindow(mainWindow)
	, m_renderResources(renderResources)
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

void GameObjectTypeDialog::OnToolOrderUp(wxCommandEvent& event)
{
	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		if(gameObjType->GetInitPriority() > 0)
		{
			gameObjType->SetInitPriority(gameObjType->GetInitPriority() - 1);
			PopulateTypeList();
		}
	}
}

void GameObjectTypeDialog::OnToolOrderDown(wxCommandEvent& event)
{
	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		if(gameObjType->GetInitPriority() < 9999)
		{
			gameObjType->SetInitPriority(gameObjType->GetInitPriority() + 1);
			PopulateTypeList();
		}
	}
}

void GameObjectTypeDialog::OnSelectGameObjType(wxCommandEvent& event)
{
	m_currentTypeId = m_gameObjIndexMap[event.GetInt()].second;

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

void GameObjectTypeDialog::OnBtnLoadSprite(wxCommandEvent& event)
{
	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		wxFileDialog dialog(this, _("Open BMP files"), "", "", "BMP files (*.bmp)|*.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		if(dialog.ShowModal() == wxID_OK)
		{
			std::string filename = dialog.GetPath().GetData().AsChar();
			if(gameObjType->LoadPreviewSprite(filename))
			{
				m_renderResources.DeleteSpriteSheetRenderResources(gameObjType->GetPreviewSpriteSheetId());
				m_renderResources.CreateSpriteSheetResources(gameObjType->GetPreviewSpriteSheetId(), gameObjType->GetPreviewSpriteSheet());
				m_mainWindow.RefreshPanel(MainWindow::ePanelMap);
			}
		}
	}
}

void GameObjectTypeDialog::OnBtnApplyObjChanges(wxCommandEvent& event)
{
	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
		gameObjType->SetName(m_textGameObjName->GetValue().c_str().AsChar());
		gameObjType->SetDimensions(ion::Vector2i(m_spinWidth->GetValue(), m_spinHeight->GetValue()));
		
		if(m_actorCache.size() > 0)
		{
			int index = m_choiceSpriteActor->GetSelection();
			if(index >= 0)
			{
				gameObjType->SetSpriteActorId(m_actorCache[index]);
			}
		}

		PopulateTypeList();
	}
}

void GameObjectTypeDialog::OnBtnApplyVarChanges(wxCommandEvent& event)
{
	if(GameObjectType* gameObjType = m_project.GetGameObjectType(m_currentTypeId))
	{
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

		m_renderResources.CreateSpriteSheetResources(m_project);

		m_mainWindow.RedrawPanel(MainWindow::ePanelMap);
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

	//Sort by init priority
	for(TGameObjectTypeMap::const_iterator it = typesMap.begin(), end = typesMap.end(); it != end; ++it)
	{
		m_gameObjIndexMap.push_back(std::make_pair(it->second.GetInitPriority(), it->second.GetId()));
	}

	std::sort(m_gameObjIndexMap.begin(), m_gameObjIndexMap.end(), [](const std::pair<int, GameObjectTypeId>& elemA, const std::pair<int, GameObjectTypeId>& elemB) { return elemA.first < elemB.first; });

	for(int i = 0; i < m_gameObjIndexMap.size(); i++)
	{
		if(const GameObjectType* objType = m_project.GetGameObjectType(m_gameObjIndexMap[i].second))
		{
			m_listGameObjTypes->Insert(wxString(objType->GetName()), i);
		}
	}
}

void GameObjectTypeDialog::PopulateTypeFields(GameObjectType* gameObjType)
{
	if(gameObjType)
	{
		m_textGameObjName->Enable(true);
		m_spinWidth->Enable(true);
		m_spinHeight->Enable(true);
		m_listVariables->Enable(true);
		m_btnApplyObjSettings->Enable(true);
		m_btnLoadSprite->Enable(true);
		m_choiceSpriteActor->Enable(true);

		m_textGameObjName->SetValue(gameObjType->GetName());
		m_spinWidth->SetValue(gameObjType->GetDimensions().x);
		m_spinHeight->SetValue(gameObjType->GetDimensions().y);

		m_choiceSpriteActor->Clear();
		m_actorCache.clear();

		int selectedSpriteIdx = -1;

		for(TActorMap::const_iterator it = m_project.ActorsBegin(), end = m_project.ActorsEnd(); it != end; ++it)
		{
			if(it->first == gameObjType->GetSpriteActorId())
			{
				selectedSpriteIdx = m_actorCache.size();
			}

			//Store by index
			m_actorCache.push_back(it->first);

			//Add to list
			m_choiceSpriteActor->AppendString(it->second.GetName());
		}

		if(selectedSpriteIdx != -1)
		{
			m_choiceSpriteActor->SetSelection(selectedSpriteIdx);
		}
	}
	else
	{
		m_textGameObjName->SetValue("");
		m_spinWidth->SetValue(1);
		m_spinHeight->SetValue(1);
		m_choiceSpriteActor->Clear();

		m_textGameObjName->Enable(false);
		m_spinWidth->Enable(false);
		m_spinHeight->Enable(false);
		m_listVariables->Enable(false);
		m_btnApplyObjSettings->Enable(false);
		m_btnLoadSprite->Enable(false);
		m_choiceSpriteActor->Enable(false);
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

		m_textVariableName->Enable(true);
		m_textValue->Enable(true);
		m_choiceSize->Enable(true);
		m_btnApplyVarSettings->Enable(true);
	}
	else
	{
		m_textVariableName->SetValue("");
		m_textValue->SetValue("");
		m_choiceSize->SetSelection(0);

		m_textVariableName->Enable(false);
		m_textValue->Enable(false);
		m_choiceSize->Enable(false);
		m_btnApplyVarSettings->Enable(false);
	}
}