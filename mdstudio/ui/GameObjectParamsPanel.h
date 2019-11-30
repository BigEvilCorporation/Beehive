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

#pragma once

#include <ion/beehive/Project.h>
#include <ion/beehive/GameObject.h>

#include "UIBase.h"

class MainWindow;

class GameObjectParamsPanel : public GameObjParamsPanelBase
{
public:
	GameObjectParamsPanel(MainWindow& mainWindow, Project& project, wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);
	virtual void OnToolVariableAdd(wxCommandEvent& event);
	virtual void OnToolVariableRemove(wxCommandEvent& event);
	virtual void OnSelectVariable(wxListEvent& event);
	virtual void OnButtonApplyObjectName(wxCommandEvent& event);
	virtual void OnSelectSpriteActor(wxCommandEvent& event);
	virtual void OnSelectSpriteSheet(wxCommandEvent& event);
	virtual void OnSelectSpriteAnim(wxCommandEvent& event);
	virtual void OnEnterTextVariable(wxCommandEvent& event);
	virtual void OnVariableSizeChanged(wxCommandEvent& event);
	virtual void OnEnterTextValue(wxCommandEvent& event);
	virtual void OnSelectValue(wxCommandEvent& event);
	
	void SetGameObject(GameObject* gameObject);

private:
	enum class Columns
	{
		Name,
		Size,
		Value,
		Tags
	};

	void PopulateSpriteActorList(wxChoice& list);
	void PopulateSpriteSheetList(wxChoice& list);
	void PopulateSpriteAnimList(wxChoice& list, SpriteSheetId spriteSheetId);
	void PopulateVarsList();
	void PopulateVarsFields(const GameObjectVariable* variable);

	Project& m_project;
	MainWindow& m_mainWindow;

	GameObject* m_gameObject;
	const GameObjectVariable* m_currentVariable;
};