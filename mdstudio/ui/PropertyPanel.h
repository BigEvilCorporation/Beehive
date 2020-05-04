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
#include <ion/beehive/Map.h>

#include <map>
#include <vector>

#include "UIBase.h"

class MainWindow;

class PropertyPanel : public PropertyPanelBase
{
public:
	PropertyPanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);
	virtual void OnPropertyChanged(wxPropertyGridEvent& event);
	virtual void OnRightClick(wxMouseEvent& event);

	void SetGameObject(GameObjectId gameObjectId);
	void SetArchetype(GameObjectTypeId gameObjectTypeId, GameObjectArchetypeId archetypeId);

protected:

private:
	enum ContextMenu
	{
		Default,
		EditScript,
		CompileScript
	};

	void GetEditingVariables(GameObjectType*& gameObjectType, GameObject*& gameObject, Actor*& actor, std::vector<GameObjectVariable>*& variables);
	GameObjectVariable* FindVariableByTag(std::vector<GameObjectVariable>& variables, const std::string& tag, int componentIdx);

	void OnContextMenuClick(wxCommandEvent& event);

	void AddProperty(const GameObjectType& gameObjectType, const GameObjectVariable& variable, int componentIdx, const Actor* actor, const GameObjectVariable* spriteSheetVar, bool enabled = true);
	int PopulateSpriteActorList(wxArrayString& list, const std::string& selectedValue);
	int PopulateSpriteSheetList(wxArrayString& list, const Actor& actor, const std::string& selectedValue);
	int PopulateSpriteAnimList(wxArrayString& list, const Actor& actor, SpriteSheetId spriteSheetId, const std::string& selectedValue);

	Project& m_project;
	MainWindow* m_mainWindow;
	GameObjectId m_gameObjectId;
	GameObjectTypeId m_gameObjectTypeId;
	GameObjectArchetypeId m_archetypeId;

	wxPGProperty* m_contextProperty;
};