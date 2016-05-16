///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "TimelinePanel.h"

const char* TimelinePanel::s_trackNames[eTrackCount] =
{
	"Position"
};

TimelinePanel::TimelinePanel(MainWindow& mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: TimelinePanelBase(parent, winid, pos, size, style)
{
	m_animation = NULL;
}

TimelinePanel::~TimelinePanel()
{

}

void TimelinePanel::SetAnimation(Animation* animation)
{
	m_animation = animation;

	//Clear existing
	m_gridTimeline->DeleteCols(0, m_gridTimeline->GetNumberCols());

	for(int i = 0; i < eTrackCount; i++)
	{
		//m_gridTimeline->AppendCol(s_trackNames[i]);
	}
}

void TimelinePanel::Refresh(bool eraseBackground, const wxRect *rect)
{

}

void TimelinePanel::OnContextMenuClick(wxCommandEvent& event)
{

}
