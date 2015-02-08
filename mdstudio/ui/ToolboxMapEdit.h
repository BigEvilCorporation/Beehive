///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "UIBase.h"
#include "../Project.h"

class ToolboxMapEdit : public ToolboxMapEditBase
{
public:
	ToolboxMapEdit(Project& project, wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Map Editing"), const wxPoint& pos = wxPoint(50, 200), const wxSize& size = wxSize( 128,400 ), long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxFRAME_TOOL_WINDOW|wxSIMPLE_BORDER|wxTAB_TRAVERSAL)
		: ToolboxMapEditBase(parent, id, title, pos, size, style)
		, m_project(project)
	{
	}

private:
	Project& m_project;
};