///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "TimelinePanel.h"
#include "MainWindow.h"

const char* TimelinePanel::s_trackNames[eTrackCount] =
{
	"Position"
};

TimelinePanel::TimelinePanel(MainWindow& mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: TimelinePanelBase(parent, winid, pos, size, style)
	, m_mainWindow(mainWindow)
	, m_project(project)
{
	m_animation = NULL;
}

TimelinePanel::~TimelinePanel()
{

}

void TimelinePanel::PopulateTimeline(const Animation& animation)
{
	//Clear existing
	m_gridTimeline->DeleteRows(0, m_gridTimeline->GetNumberRows());
	m_gridTimeline->DeleteCols(0, m_gridTimeline->GetNumberCols());

	//Create default column
	m_gridTimeline->AppendCols(1);

	//Create all rows
	m_gridTimeline->AppendRows(eTrackCount);

	//Set labels
	for(int i = 0; i < eTrackCount; i++)
	{
		m_gridTimeline->SetRowLabelValue(i, s_trackNames[i]);
	}

	m_gridTimeline->SetColLabelValue(0, "0.0");
}

AnimationId TimelinePanel::GetCurrentAnimation() const
{
	return m_animationId;
}

void TimelinePanel::SyncAllActors()
{
	if(m_animation)
	{
		float frame = m_animation->GetFrame();

		for(TAnimActorMap::iterator it = m_animation->ActorsBegin(), end = m_animation->ActorsEnd(); it != end; ++it)
		{
			if(GameObject* gameObject = m_project.GetMap().GetGameObject(it->first))
			{
				gameObject->SetDrawOffset(it->second.m_trackPosition.GetValue(frame));
			}
		}
	}
}

void TimelinePanel::Refresh(bool eraseBackground, const wxRect *rect)
{

}

void TimelinePanel::OnToolAddAnim(wxCommandEvent& event)
{
	m_animationId = m_project.CreateAnimation();
	m_animation = m_project.GetAnimation(m_animationId);
	PopulateTimeline(*m_animation);
}

void TimelinePanel::OnToolDeleteAnim(wxCommandEvent& event)
{

}

void TimelinePanel::OnToolKeyframe(wxCommandEvent& event)
{
	if(m_animation)
	{
		//Get current frame
		float frame = m_animation->GetFrame();

		//Insert keyframes
		for(TAnimActorMap::iterator it = m_animation->ActorsBegin(), end = m_animation->ActorsEnd(); it != end; ++it)
		{
			if(GameObject* gameObject = m_project.GetMap().GetGameObject(it->first))
			{
				it->second.m_trackPosition.InsertKeyframe(AnimKeyframePosition(frame, gameObject->GetDrawOffset()));
			}
		}

		//Advance frame
		frame += 1.0f;

		//Adjust anim length
		if(m_animation->GetLength() < frame)
		{
			m_animation->SetLength(frame);
		}

		//Set next frame
		m_animation->SetFrame(frame);

		//Sync all actors
		SyncAllActors();

		//Redraw map panel
		m_mainWindow.RedrawPanel(MainWindow::ePanelMap);
	}
}

void TimelinePanel::OnToolPlay(wxCommandEvent& event)
{

}

void TimelinePanel::OnToolStop(wxCommandEvent& event)
{

}

void TimelinePanel::OnToolRewind(wxCommandEvent& event)
{

}

void TimelinePanel::OnToolIsolateObject(wxCommandEvent& event)
{

}

void TimelinePanel::OnSliderTimelineChange(wxScrollEvent& event)
{
	if(m_animation)
	{
		float time = ion::maths::Lerp(0.0f, m_animation->GetLength(), (float)m_sliderTimeline->GetValue() / 100.0f);
		m_animation->SetFrame(time);
		SyncAllActors();
		m_mainWindow.RedrawPanel(MainWindow::ePanelMap);
	}
}

void TimelinePanel::OnContextMenuClick(wxCommandEvent& event)
{

}
