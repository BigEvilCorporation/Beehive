///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "TimelinePanel.h"
#include "MainWindow.h"

#include <sstream>

const char* TimelinePanel::s_trackNames[eTrackCount] =
{
	"Position"
};

TimelinePanel::TimelinePanel(MainWindow& mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: TimelinePanelBase(parent, winid, pos, size, style)
	, m_mainWindow(mainWindow)
	, m_project(project)
	, m_timer(this)
{
	m_animationId = InvalidAnimationId;
	m_animation = NULL;
	m_actorId = InvalidActorId;
	m_actor = NULL;

	PopulateAnimations();

	m_gridTimeline->DeleteRows(0, m_gridTimeline->GetNumberRows());
	m_gridTimeline->DeleteCols(0, m_gridTimeline->GetNumberCols());

	Bind(wxEVT_TIMER, &TimelinePanel::EventHandlerTimer, this, m_timer.GetId());

	m_timer.Start(1);
}

TimelinePanel::~TimelinePanel()
{

}

void TimelinePanel::EventHandlerTimer(wxTimerEvent& event)
{
	if(m_animation && m_animation->GetState() == ion::render::Animation::Playing)
	{
		float frameRate = 24.0f;
		float frameRateMul = 1.0f / (frameRate / 10.0f);
		float delta = (float)event.GetInterval() * ((float)m_spinSpeed->GetValue() / 100.0f) * frameRateMul;
		float time = m_animation->GetFrame();
		float lerpTime = ion::maths::UnLerp(0.0f, m_animation->GetLength(), time);

		m_animation->Update(delta);
		m_sliderTimeline->SetValue((int)ion::maths::Round(lerpTime * 100.0f));
		m_gridTimeline->GoToCell(0, ion::maths::Floor(time));

		if(m_toolIsolateObject->IsToggled() && m_actor)
		{
			SyncActor(*m_actor);
		}
		else
		{
			SyncAllActors();
		}

		m_mainWindow.RedrawPanel(MainWindow::ePanelMap);
	}
}

void TimelinePanel::PopulateAnimations()
{
	m_choiceAnims->Clear();
	m_animCache.clear();

	typedef std::pair<std::string, AnimationId> TNameIDPair;
	typedef std::vector<TNameIDPair> TNameList;
	TNameList nameList;

	for(TAnimationMap::const_iterator it = m_project.AnimationsBegin(), end = m_project.AnimationsEnd(); it != end; ++it)
	{
		nameList.push_back(std::make_pair(it->second.GetName(), it->first));
	}

	std::sort(nameList.begin(), nameList.end(), [](TNameIDPair& a, TNameIDPair& b) { return a.first < b.first; });

	for(int i = 0; i < nameList.size(); i++)
	{
		//Store by index
		m_animCache.push_back(nameList[i].second);

		//Add to list
		m_choiceAnims->AppendString(nameList[i].first);
	}
}

void TimelinePanel::PopulateTimeline(const Animation& animation, const AnimationActor* actor)
{
	//Clear existing
	m_gridTimeline->DeleteRows(0, m_gridTimeline->GetNumberRows());
	m_gridTimeline->DeleteCols(0, m_gridTimeline->GetNumberCols());

	if(actor)
	{
		//Populate keyframes from selected actor
		int numKeyframes = actor->m_trackPosition.GetNumKeyframes();

		//Create all columns
		m_gridTimeline->AppendCols(numKeyframes);

		//Set column labels
		for(int i = 0; i < numKeyframes; i++)
		{
			std::stringstream label;
			label << actor->m_trackPosition.GetKeyframe(i).GetTime();
			m_gridTimeline->SetColLabelValue(i, label.str());
		}

		//Set label
		if(GameObject* gameObject = m_project.GetMap().GetGameObject(actor->GetGameObjectId()))
		{
			std::stringstream label;
			label << "Object" << gameObject->GetId();
			m_textCurrentActor->SetLabel(label.str());
		}
	}
	else
	{
		//Populate frames from selected animation
		int numFrames = ion::maths::Ceil(animation.GetLength());

		//Create all columns
		m_gridTimeline->AppendCols(numFrames);

		//Set column labels
		for(int i = 0; i < numFrames; i++)
		{
			std::stringstream label;
			label << (float)i;
			m_gridTimeline->SetColLabelValue(i, label.str());
		}

		//Set label
		m_textCurrentActor->SetLabel("[All keyframes]");
	}


	//Create all rows
	m_gridTimeline->AppendRows(eTrackCount);

	//Set row labels
	for(int i = 0; i < eTrackCount; i++)
	{
		m_gridTimeline->SetRowLabelValue(i, s_trackNames[i]);
	}

	m_gridTimeline->SetColLabelValue(0, "0.0");
}

void TimelinePanel::SetCurrentActor(GameObjectId actorId)
{
	if(actorId != m_actorId)
	{
		m_actorId = actorId;

		if(m_animation)
		{
			m_actor = m_animation->GetActor(actorId);

			if(m_actor)
			{
				PopulateTimeline(*m_animation, m_actor);
			}
		}
	}
}

AnimationId TimelinePanel::GetCurrentAnimation() const
{
	return m_animationId;
}

void TimelinePanel::SyncActor(AnimationActor& actor)
{
	if(m_animation)
	{
		if(GameObject* gameObject = m_project.GetMap().GetGameObject(actor.GetGameObjectId()))
		{
			float frame = m_animation->GetFrame();
			gameObject->SetAnimDrawOffset(actor.m_trackPosition.GetValue(frame));
		}
	}
}

void TimelinePanel::SyncAllActors()
{
	if(m_animation)
	{
		for(TAnimActorMap::iterator it = m_animation->ActorsBegin(), end = m_animation->ActorsEnd(); it != end; ++it)
		{
			SyncActor(it->second);
		}
	}
}

void TimelinePanel::Refresh(bool eraseBackground, const wxRect *rect)
{

}

void TimelinePanel::OnSelectAnimation(wxCommandEvent& event)
{
	int index = event.GetSelection();
	m_animationId = m_animCache[index];
	m_animation = m_project.GetAnimation(m_animationId);
	if(m_animation)
	{
		m_animation->SetState(ion::render::Animation::Stopped);
		PopulateTimeline(*m_animation, NULL);
	}
}

void TimelinePanel::OnToolAddAnim(wxCommandEvent& event)
{
	DialogNewAnim dialog(this);

	if(dialog.ShowModal() == wxID_OK)
	{
		m_animationId = m_project.CreateAnimation();
		m_animation = m_project.GetAnimation(m_animationId);
		m_animation->SetName(dialog.m_textName->GetValue().GetData().AsChar());
		PopulateAnimations();
		PopulateTimeline(*m_animation, NULL);
	}
}

void TimelinePanel::OnToolDeleteAnim(wxCommandEvent& event)
{
	if(m_animation)
	{
		m_gridTimeline->DeleteRows(0, m_gridTimeline->GetNumberRows());
		m_gridTimeline->DeleteCols(0, m_gridTimeline->GetNumberCols());

		m_project.DeleteAnimation(m_animationId);
		m_animationId = InvalidAnimationId;
		m_animation = NULL;

		PopulateAnimations();
	}
}

void TimelinePanel::OnToolKeyframeActor(wxCommandEvent& event)
{
	if(m_animation && m_actor)
	{
		//Get current frame
		float frame = m_animation->GetFrame();

		//Insert keyframe
		if(GameObject* gameObject = m_project.GetMap().GetGameObject(m_actorId))
		{
			m_actor->m_trackPosition.InsertKeyframe(AnimKeyframePosition(frame, gameObject->GetAnimDrawOffset()));
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

		if(m_toolIsolateObject->IsToggled() && m_actor)
		{
			SyncActor(*m_actor);
		}
		else
		{
			SyncAllActors();
		}

		//Redraw map panel
		m_mainWindow.RedrawPanel(MainWindow::ePanelMap);

		//Populate timeline
		PopulateTimeline(*m_animation, m_actor);
	}
}

void TimelinePanel::OnToolKeyframeAll(wxCommandEvent& event)
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
				it->second.m_trackPosition.InsertKeyframe(AnimKeyframePosition(frame, gameObject->GetAnimDrawOffset()));
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

		if(m_toolIsolateObject->IsToggled() && m_actor)
		{
			SyncActor(*m_actor);
		}
		else
		{
			SyncAllActors();
		}

		//Redraw map panel
		m_mainWindow.RedrawPanel(MainWindow::ePanelMap);

		//Populate timeline
		PopulateTimeline(*m_animation, m_actor);
	}
}

void TimelinePanel::OnToolPlay(wxCommandEvent& event)
{
	if(m_animation)
	{
		m_animation->SetState(ion::render::Animation::Playing);
	}
}

void TimelinePanel::OnToolStop(wxCommandEvent& event)
{
	if(m_animation)
	{
		m_animation->SetState(ion::render::Animation::Stopped);
	}
}

void TimelinePanel::OnToolRewind(wxCommandEvent& event)
{
	if(m_animation)
	{
		m_animation->SetFrame(0.0f);
	}

	m_sliderTimeline->SetValue(0);

	if(m_toolIsolateObject->IsToggled() && m_actor)
	{
		SyncActor(*m_actor);
	}
	else
	{
		SyncAllActors();
	}

	m_mainWindow.RedrawPanel(MainWindow::ePanelMap);
}

void TimelinePanel::OnToolIsolateObject(wxCommandEvent& event)
{
	if(event.IsChecked() && !m_actor)
	{
		m_toolIsolateObject->SetToggle(false);
	}
}

void TimelinePanel::OnSliderTimelineChange(wxScrollEvent& event)
{
	if(m_animation)
	{
		float time = ion::maths::Lerp(0.0f, m_animation->GetLength(), (float)m_sliderTimeline->GetValue() / 100.0f);
		m_animation->SetFrame(time);

		if(m_toolIsolateObject->IsToggled() && m_actor)
		{
			SyncActor(*m_actor);
		}
		else
		{
			SyncAllActors();
		}
		
		m_mainWindow.RedrawPanel(MainWindow::ePanelMap);
	}
}

void TimelinePanel::OnContextMenuClick(wxCommandEvent& event)
{

}
