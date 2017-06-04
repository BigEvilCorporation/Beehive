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

#include <ion/beehive/Animation.h>

#include "UIBase.h"
#include "ViewPanel.h"

#include <wx/timer.h>

class TimelinePanel : public TimelinePanelBase
{
public:
	TimelinePanel(MainWindow& mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~TimelinePanel();

	void PopulateAnimations();
	void PopulateTimeline(const Animation& animation, const AnimationActor* actor);

	void SetCurrentActor(GameObjectId actorId);
	AnimationId GetCurrentAnimation() const;

	void SyncActor(AnimationActor& actor);
	void SyncAllActors();

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

	void EventHandlerTimer(wxTimerEvent& event);

protected:

	virtual void OnSpinSpeed(wxSpinEvent& event);
	virtual void OnSelectAnimation(wxCommandEvent& event);
	virtual void OnSelectSpriteAnim(wxCommandEvent& event);
	virtual void OnToolAddAnim(wxCommandEvent& event);
	virtual void OnToolDeleteAnim(wxCommandEvent& event);
	virtual void OnToolKeyframeActor(wxCommandEvent& event);
	virtual void OnToolKeyframeAll(wxCommandEvent& event);
	virtual void OnToolPlay(wxCommandEvent& event);
	virtual void OnToolStop(wxCommandEvent& event);
	virtual void OnToolRewind(wxCommandEvent& event);
	virtual void OnToolIsolateObject(wxCommandEvent& event);
	virtual void OnSliderTimelineChange(wxScrollEvent& event);
	virtual void OnTimelineColResize(wxGridSizeEvent& event);
	virtual void OnResize(wxSizeEvent& event);
	virtual void OnTimelineKeyframeLeftClick(wxGridEvent& event);
	virtual void OnTimelineKeyframeRightClick(wxGridEvent& event);

	//Right-click menu callback
	void OnContextMenuClick(wxCommandEvent& event);

private:
	enum MenuItems
	{
		eMenuInsertKeyframeLeft,
		eMenuInsertKeyframeRight,
		eMenuDeleteKeyframe,
		eMenuSetSpriteAnimLength
	};

	void SetSliderFrame(float frame);
	void Keyframe(AnimationActor* actor);
	void BuildGridColPosCache();
	void SetAnimLength(Animation& animation, const AnimationActor& actor);

	static const char* s_trackNames[eTrackCount];

	MainWindow& m_mainWindow;
	Project& m_project;

	std::vector<AnimationId> m_animCache;
	std::vector<std::pair<SpriteSheetId, SpriteAnimId>> m_spriteSheetCache;
	std::vector<int> m_gridColPosCache;

	AnimationId m_animationId;
	Animation* m_animation;

	GameObjectId m_actorId;
	AnimationActor* m_actor;

	//Timer
	u64 m_prevClock;
	wxTimer m_timer;

	int m_contextMenuColIdx;
};