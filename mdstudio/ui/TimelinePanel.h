///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "UIBase.h"
#include "ViewPanel.h"
#include "../Animation.h"

class TimelinePanel : public TimelinePanelBase
{
public:
	TimelinePanel(MainWindow& mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~TimelinePanel();

	void PopulateTimeline(const Animation& animation);
	AnimationId GetCurrentAnimation() const;

	void SyncAllActors();

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

protected:

	virtual void OnToolAddAnim(wxCommandEvent& event);
	virtual void OnToolDeleteAnim(wxCommandEvent& event);
	virtual void OnToolKeyframe(wxCommandEvent& event);
	virtual void OnToolPlay(wxCommandEvent& event);
	virtual void OnToolStop(wxCommandEvent& event);
	virtual void OnToolRewind(wxCommandEvent& event);
	virtual void OnToolIsolateObject(wxCommandEvent& event);
	virtual void OnSliderTimelineChange(wxScrollEvent& event);

	//Right-click menu callback
	void OnContextMenuClick(wxCommandEvent& event);

private:
	enum MenuItems
	{
		eMenuInsertKeyframe,
		eMenuDeleteKeyframe,
	};

	static const char* s_trackNames[eTrackCount];

	MainWindow& m_mainWindow;
	Project& m_project;

	AnimationId m_animationId;
	Animation* m_animation;
};