///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "GameObjectParamsPanel.h"
#include "MainWindow.h"

GameObjectParamsPanel::GameObjectParamsPanel(MainWindow& mainWindow, Project& project, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: GameObjParamsPanelBase(parent, id, pos, size, style)
	, m_project(project)
	, m_mainWindow(mainWindow)
{
	m_gameObject = NULL;
	m_currentVariable = NULL;
	PopulateVarsList();
}

void GameObjectParamsPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	PopulateVarsList();
}

void GameObjectParamsPanel::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
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
		m_currentVariable = m_gameObject->GetVariable(event.GetIndex());
		PopulateVarsFields(m_currentVariable);
	}
}

void GameObjectParamsPanel::OnBtnApplyChanges(wxCommandEvent& event)
{
	if(m_gameObject)
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
				ion::debug::Error("GameObjectParamsPanel::OnVariableSizeChanged() - Bad size");
				break;
			}
		}

		PopulateVarsList();
		m_mainWindow.RedrawPanel(MainWindow::ePanelGameObjectParams);
	}
}

void GameObjectParamsPanel::PopulateVarsList()
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

	if(m_gameObject)
	{
		const std::vector<GameObjectVariable>& variables = m_gameObject->GetVariables();

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

void GameObjectParamsPanel::PopulateVarsFields(GameObjectVariable* variable)
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