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

#include "UIBase.h"
#include "RenderResources.h"
#include "GridCellRenderer.h"
#include "Project.h"
#include "StampAnimation.h"

#include <ion/renderer/Renderer.h>

#include <wx/glcanvas.h>
#include <wx/timer.h>
#include <wx/dragimag.h>
#include <wx/weakref.h>

class MainWindow;

class StampAnimEditorDialog : public StampAnimEditorDialogBase
{
public:
	static const int s_iconHeight = 64;
	static const int s_iconBorderX = 16;
	static const int s_iconBorderY = 2;

	enum ContextMenu
	{
		eMenuDeleteKeyframe = 1,
		eMenuAddKeyframe
	};

	StampAnimEditorDialog(MainWindow& mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources);
	virtual ~StampAnimEditorDialog();

	void SetAnimation(StampAnimId animId);

	void EventHandlerTimer(wxTimerEvent& event);
	void EventHandlerDragTimelineBegin(wxGridEvent& event);
	void EventHandlerDragTimelineMove(wxMouseEvent& event);
	void EventHandlerDragTimelineEnd(wxMouseEvent& event);
	void EventHandlerTimelineRightClick(wxGridEvent& event);
	void EventHandlerContextMenuClick(wxCommandEvent& event);

protected:
	virtual void OnAnimSelected(wxCommandEvent& event);
	virtual void OnBtnAnimDelete(wxCommandEvent& event);
	virtual void OnTimelineCellChange(wxGridEvent& event);
	virtual void OnSliderMove(wxScrollEvent& event);
	virtual void OnBtnPlay(wxCommandEvent& event);
	virtual void OnBtnStop(wxCommandEvent& event);
	virtual void OnSpinSpeedChange(wxSpinEvent& event);

private:
	void PopulateAnimList();
	void PopulateKeyframes(const StampAnimation& animation);

	void SelectAnimation(int index);

	void FindAndDrawDropTarget(wxPoint mousePosDropSource, wxPoint mousePosDropTarget);

	void ApplyAnimFrame(int frame);

	Project& m_project;
	ion::render::Renderer& m_renderer;
	RenderResources& m_renderResources;
	wxGLContext& m_glContext;
	MainWindow& m_mainWindow;

	StampAnimId m_selectedAnimId;
	StampAnimation* m_selectedAnim;

	std::vector<StampAnimId> m_animCache;

	wxTimer m_timer;

	//Timeline
	wxSharedPtr<wxImageList> m_timelineImageList;

	//Drag and drop
	int m_draggingTimelineItem;
	int m_dragDropTarget;
	int m_dragDropTargetPrev;
	wxDragImage* m_dragImage;
	std::vector<std::pair<u32, wxRect>> m_dragDropKeyframeList;

	//Right-click menu
	int m_contextMenuKeyframeIndex;

	//System scrollbar height
	int m_scrollbarHeight;
};