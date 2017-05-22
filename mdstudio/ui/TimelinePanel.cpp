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

#include "TimelinePanel.h"
#include "MainWindow.h"

#include <sstream>

const char* TimelinePanel::s_trackNames[eTrackCount] =
{
	"Position",
	"SpriteAnim"
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
	if(m_animation && m_animation->GetState() == ion::render::Animation::ePlaying)
	{
		float delta = (float)event.GetInterval() / 10.0f;

		m_animation->Update(delta);
		float time = m_animation->GetFrame();
		SetSliderFrame(time);
		m_gridTimeline->GoToCell(0, ion::maths::Floor(time));

		if(m_toolIsolateObject->IsToggled() && m_actor)
		{
			SyncActor(*m_actor);
		}
		else
		{
			SyncAllActors();
		}
	}
}

void TimelinePanel::SetSliderFrame(float frame)
{
	float lerpTime = ion::maths::UnLerp(0.0f, m_animation->GetLength(), frame);
	m_sliderTimeline->SetValue((int)ion::maths::Round(lerpTime * 100.0f));
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

	//Set drop-down
	std::vector<AnimationId>::iterator it = std::find_if(m_animCache.begin(), m_animCache.end(), [&](const AnimationId& animId) { return animId == animation.GetId(); });
	int index = std::distance(it, m_animCache.begin());

	if(m_choiceAnims->GetSelection() != index)
	{
		m_choiceAnims->SetSelection(index);
	}

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

		//Create all rows
		m_gridTimeline->AppendRows(eTrackCount);

		//Set row labels
		for(int i = 0; i < eTrackCount; i++)
		{
			m_gridTimeline->SetRowLabelValue(i, s_trackNames[i]);
		}

		if(GameObject* gameObject = m_project.GetEditingMap().GetGameObject(actor->GetGameObjectId()))
		{
			if(const GameObjectType* gameObjectType = m_project.GetGameObjectType(gameObject->GetTypeId()))
			{
				//Populate position track
				for(int i = 0; i < numKeyframes; i++)
				{
					std::stringstream label;
					ion::Vector2i value = actor->m_trackPosition.GetKeyframe(i).GetValue();
					label << "(" << value.x << "," << value.y << ")";
					m_gridTimeline->SetCellValue(label.str(), eTrackPosition, i);
				}

				//Populate sprite anim track
				for(int i = 0; i < numKeyframes; i++)
				{
					std::stringstream label;
					std::pair<SpriteSheetId, SpriteAnimId> value = actor->m_trackSpriteAnim.GetKeyframe(i).GetValue();
					if(Actor* spriteActor = m_project.GetActor(gameObjectType->GetSpriteActorId()))
					{
						if(SpriteSheet* spriteSheet = spriteActor->GetSpriteSheet(value.first))
						{
							if(SpriteAnimation* spriteAnim = spriteSheet->GetAnimation(value.second))
							{
								std::stringstream label;
								label << spriteSheet->GetName() << ":" << spriteAnim->GetName();
								m_gridTimeline->SetCellValue(label.str(), eTrackSpriteAnim, i);
							}
						}
					}
				}
			}
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

		//Create all rows
		m_gridTimeline->AppendRows(eTrackCount);

		//Set row labels
		for(int i = 0; i < eTrackCount; i++)
		{
			m_gridTimeline->SetRowLabelValue(i, s_trackNames[i]);
		}

		//Set game object label
		m_textCurrentActor->SetLabel("[All objects]");
	}
}

void TimelinePanel::SetCurrentActor(GameObjectId actorId)
{
	if(actorId != m_actorId)
	{
		if(const GameObject* gameObject = m_project.GetEditingMap().GetGameObject(actorId))
		{
			m_actorId = actorId;

			if(m_animation)
			{
				m_actor = m_animation->GetActor(actorId);

				if(m_actor)
				{
					//Populate timeline
					PopulateTimeline(*m_animation, m_actor);

					//Set game object label
					std::stringstream label;

					std::string objName = gameObject->GetName();
					if(objName.empty())
					{
						objName = "<No Name>";
					}

					label << objName << " (object " << gameObject->GetId() << ")";
					m_textCurrentActor->SetLabel(label.str());

					//Populate sprite actor choices
					m_choiceSpriteAnim->Clear();
					m_spriteSheetCache.clear();

					if(const GameObjectType* gameObjectType = m_project.GetGameObjectType(gameObject->GetTypeId()))
					{
						ActorId spriteActorId = gameObjectType->GetSpriteActorId();
						if(Actor* spriteActor = m_project.GetActor(spriteActorId))
						{
							for(TSpriteSheetMap::iterator spriteSheetIt = spriteActor->SpriteSheetsBegin(), spriteSheetEnd = spriteActor->SpriteSheetsEnd(); spriteSheetIt != spriteSheetEnd; ++spriteSheetIt)
							{
								for(TSpriteAnimMap::iterator animIt = spriteSheetIt->second.AnimationsBegin(), animEnd = spriteSheetIt->second.AnimationsEnd(); animIt != animEnd; ++animIt)
								{
									std::stringstream label;
									label << spriteSheetIt->second.GetName() << ":" << animIt->second.GetName();
									m_choiceSpriteAnim->AppendString(label.str());
									m_spriteSheetCache.push_back(std::make_pair(spriteSheetIt->first, animIt->first));
								}
							}
						}
					}
				}
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
		if(GameObject* gameObject = m_project.GetEditingMap().GetGameObject(actor.GetGameObjectId()))
		{
			float frame = m_animation->GetFrame();
			SpriteAnimId originalSpriteAnim = gameObject->GetSpriteAnim();

			//Get all track values
			ion::Vector2i position = actor.m_trackPosition.GetValue(frame);
			std::pair<SpriteSheetId, SpriteAnimId> spriteAnim = actor.m_trackSpriteAnim.GetValue(frame);

			//Apply to game object
			m_project.GetEditingMap().MoveGameObject(actor.GetGameObjectId(), position.x, position.y);
			gameObject->SetPosition(position);
			gameObject->SetSpriteSheetId(spriteAnim.first);
			gameObject->SetSpriteAnim(spriteAnim.second);

			//Advance current sprite anim
			if(const GameObjectType* gameObjectType = m_project.GetGameObjectType(gameObject->GetTypeId()))
			{
				if(Actor* spriteActor = m_project.GetActor(gameObjectType->GetSpriteActorId()))
				{
					if(SpriteSheet* spriteSheet = spriteActor->GetSpriteSheet(spriteAnim.first))
					{
						if(SpriteAnimation* spriteAnimation = spriteSheet->GetAnimation(spriteAnim.second))
						{
							spriteAnimation->SetFrame(ion::maths::Fmod(frame * spriteAnimation->GetSpeed(), spriteAnimation->GetLength()));
						}
					}
				}
			}

			//Refresh map panel
			m_mainWindow.RedrawPanel(MainWindow::ePanelMap);
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
	if(!m_mainWindow.IsRefreshLocked())
	{
	}
}

void TimelinePanel::OnSpinSpeed(wxSpinEvent& event)
{
	if(m_animation)
	{
		m_animation->SetPlaybackSpeed((float)m_spinSpeed->GetValue());
	}
}

void TimelinePanel::OnSelectAnimation(wxCommandEvent& event)
{
	int index = event.GetSelection();
	m_animationId = m_animCache[index];
	m_animation = m_project.GetAnimation(m_animationId);
	if(m_animation)
	{
		m_animation->SetState(ion::render::Animation::eStopped);
		PopulateTimeline(*m_animation, NULL);
	}
}

void TimelinePanel::OnSelectSpriteAnim(wxCommandEvent& event)
{
	if(m_animation && m_actor)
	{
		if(GameObject* gameObject = m_project.GetEditingMap().GetGameObject(m_actor->GetGameObjectId()))
		{
			gameObject->SetSpriteSheetId(m_spriteSheetCache[m_choiceSpriteAnim->GetSelection()].first);
			gameObject->SetSpriteAnim(m_spriteSheetCache[m_choiceSpriteAnim->GetSelection()].second);

			PopulateTimeline(*m_animation, m_actor);
			m_mainWindow.RefreshPanel(MainWindow::ePanelMap);
		}
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
		m_actorId = InvalidActorId;

		PopulateAnimations();
	}
}

namespace
{
	template <typename T> void AddOrEditKeyframe(const T& value, ion::render::AnimationTrack<T>& track, float frame)
	{
		if(ion::render::Keyframe<T>* keyframe = track.FindKeyframe(frame))
		{
			keyframe->SetValue(value);
		}
		else
		{
			track.InsertKeyframe(ion::render::Keyframe<T>(frame, value));
		}
	}

	void KeyframeObjectCurrentState(Project& project, AnimationActor& actor, float frame)
	{
		if(GameObject* gameObject = project.GetEditingMap().GetGameObject(actor.GetGameObjectId()))
		{
			//Add/edit keyframes for all tracks
			AddOrEditKeyframe(gameObject->GetPosition(), actor.m_trackPosition, frame);
			AddOrEditKeyframe(std::make_pair(gameObject->GetSpriteSheetId(), gameObject->GetSpriteAnim()), actor.m_trackSpriteAnim, frame);
		}
	}
}

void TimelinePanel::Keyframe(AnimationActor* actor)
{
	if(m_animation)
	{
		//Get current frame
		float frame = m_animation->GetFrame();

		if(actor)
		{
			//Edit or insert keyframe for current actor
			KeyframeObjectCurrentState(m_project, *actor, frame);
		}
		else
		{
			//Edit or insert keyframe for all actors
			for(TAnimActorMap::iterator it = m_animation->ActorsBegin(), end = m_animation->ActorsEnd(); it != end; ++it)
			{
				KeyframeObjectCurrentState(m_project, it->second, frame);
			}
		}

		//If at end, advance to next keyframe
		if(frame >= m_animation->GetLength() - 1.0f)
		{
			//Advance frame
			frame += 1.0f;

			//Adjust anim length
			if(m_animation->GetLength() < frame)
			{
				m_animation->SetLength(frame);
			}

			//Set next frame
			m_animation->SetFrame(frame);

			//Update sider
			SetSliderFrame(frame);
		}

		if(m_toolIsolateObject->IsToggled() && actor)
		{
			SyncActor(*actor);
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

void TimelinePanel::OnToolKeyframeActor(wxCommandEvent& event)
{
	if(m_actor)
	{
		Keyframe(m_actor);
	}
}

void TimelinePanel::OnToolKeyframeAll(wxCommandEvent& event)
{
	Keyframe(NULL);
}

void TimelinePanel::OnToolPlay(wxCommandEvent& event)
{
	if(m_animation)
	{
		m_animation->SetState(ion::render::Animation::ePlaying);
	}
}

void TimelinePanel::OnToolStop(wxCommandEvent& event)
{
	if(m_animation)
	{
		m_animation->SetState(ion::render::Animation::eStopped);
	}
}

void TimelinePanel::OnToolRewind(wxCommandEvent& event)
{
	if(m_animation)
	{
		m_animation->SetFrame(0.0f);
	}

	SetSliderFrame(0.0f);

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
		
		std::stringstream frameText;
		frameText << "Frame: " << time;
		m_textFrame->SetLabel(frameText.str());

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
