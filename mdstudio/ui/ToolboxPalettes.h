///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "UIBase.h"
#include "PalettesPanel.h"
#include "../Project.h"

class ToolboxPalettes : public ToolboxPalettesBase
{
public:
	ToolboxPalettes(Project& project, wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Palettes"), const wxPoint& pos = wxPoint(400, 900), const wxSize& size = wxSize( 530,180 ), long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxFRAME_TOOL_WINDOW|wxTAB_TRAVERSAL)
		: ToolboxPalettesBase(parent, id, title, pos, size, style)
		, m_project(project)
	{
		m_palettesPanel->SetProject(&project);
	}

private:
	Project& m_project;
};