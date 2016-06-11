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

#include <ion/core/Types.h>

#include "UIBase.h"
#include "../GameObject.h"
#include "../SpriteSheet.h"

class MainWindow;
class Project;

class GameObjectTypeDialog : public GameObjTypeDialogBase
{
public:
	GameObjectTypeDialog(MainWindow& mainWindow, Project& project);

protected:
	virtual void OnToolGameObjAdd(wxCommandEvent& event);
	virtual void OnToolGameObjRemove(wxCommandEvent& event);
	virtual void OnToolVariableAdd(wxCommandEvent& event);
	virtual void OnToolVariableRemove(wxCommandEvent& event);
	virtual void OnSelectGameObjType(wxCommandEvent& event);
	virtual void OnSelectVariable(wxListEvent& event);
	virtual void OnBtnApplyChanges(wxCommandEvent& event);
	virtual void OnBtnImport(wxCommandEvent& event);
	virtual void OnBtnExport(wxCommandEvent& event);

private:
	void PopulateTypeList();
	void PopulateTypeFields(GameObjectType* gameObjType);
	void PopulateVarsList(GameObjectType* gameObjType);
	void PopulateVarsFields(GameObjectVariable* variable);

	MainWindow& m_mainWindow;
	Project& m_project;
	GameObjectTypeId m_currentTypeId;
	GameObjectVariable* m_currentVariable;

	std::map<u32, GameObjectTypeId> m_gameObjIndexMap;
	std::vector<SpriteSheetId> m_spriteSheetCache;
};