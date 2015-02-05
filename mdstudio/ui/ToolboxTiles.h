#pragma once

#include "UIBase.h"
#include "TilesPanel.h"
#include "../Project.h"

class ToolboxTiles : public ToolboxTilesBase
{
public:
	ToolboxTiles(Project& project, wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Tiles"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxFRAME_TOOL_WINDOW|wxTAB_TRAVERSAL)
		: ToolboxTilesBase(parent, id, title, pos, size, style)
		, m_project(project)
	{
		m_tilesPanel->SetProject(&project);
	}

private:
	Project& m_project;
};