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

#include <ion/renderer/Renderer.h>
#include <ion/beehive/Project.h>
#include <ion/beehive/SpriteSheet.h>

#include "UIBase.h"
#include "RenderResources.h"
#include "GridCellRenderer.h"

#include <wx/glcanvas.h>
#include <wx/timer.h>
#include <wx/dragimag.h>
#include <wx/weakref.h>

class SpriteAnimEditorDialog : public SpriteAnimEditorDialogBase
{
public:
	static const int s_iconHeight = 64;
	static const int s_iconBorderX = 16;
	static const int s_iconBorderY = 2;

	enum ContextMenu
	{
		eMenuDeleteKeyframe = 1
	};

	enum TimelineTrack
	{
		eTrackSpriteFrame,
		eTrackPositionX,
		eTrackPositionY,
		eTrack_MAX
	};

	SpriteAnimEditorDialog(wxWindow* parent, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources);
	virtual ~SpriteAnimEditorDialog();

	void EventHandlerTimer(wxTimerEvent& event);
	void EventHandlerDragFrameListBegin(wxGridEvent& event);
	void EventHandlerDragFrameListMove(wxMouseEvent& event);
	void EventHandlerDragFrameListEnd(wxMouseEvent& event);
	void EventHandlerDragTimelineBegin(wxGridEvent& event);
	void EventHandlerDragTimelineMove(wxMouseEvent& event);
	void EventHandlerDragTimelineEnd(wxMouseEvent& event);
	void EventHandlerTimelineRightClick(wxGridEvent& event);
	void EventHandlerContextMenuClick(wxCommandEvent& event);

protected:
	virtual void OnActorSelected(wxCommandEvent& event);
	virtual void OnSpriteSheetSelected(wxCommandEvent& event);
	virtual void OnAnimSelected(wxCommandEvent& event);
	virtual void OnBtnActorNew(wxCommandEvent& event);
	virtual void OnBtnActorDelete(wxCommandEvent& event);
	virtual void OnBtnActorExport(wxCommandEvent& event);
	virtual void OnBtnSpriteSheetImport(wxCommandEvent& event);
	virtual void OnBtnSpriteSheetDelete(wxCommandEvent& event);
	virtual void OnBtnSpriteSheetUsePalette(wxCommandEvent& event);
	virtual void OnBtnAnimNew(wxCommandEvent& event);
	virtual void OnBtnAnimDelete(wxCommandEvent& event);
	virtual void OnTimelineCellChange(wxGridEvent& event);
	virtual void OnSliderMove(wxScrollEvent& event);
	virtual void OnBtnPlay(wxCommandEvent& event);
	virtual void OnBtnStop(wxCommandEvent& event);
	virtual void OnSpinSpeedChange(wxSpinEvent& event);
	virtual void OnRadioBlendLerp(wxCommandEvent& event);
	virtual void OnRadioBlendSnap(wxCommandEvent& event);

private:
	void PopulateActorList();
	void PopulateSpriteSheetList(const Actor& actor);
	void PopulateSpriteFrames(const SpriteSheetId& spriteSheetId);
	void PopulateAnimList(const SpriteSheet& spriteSheet);
	void PopulateKeyframes(const SpriteSheetId& spriteSheetId, const SpriteAnimation& anim);
		 
	void SelectActor(int index);
	void SelectSpriteSheet(int index);
	void SelectAnimation(int index);

	void FindAndDrawDropTarget(wxPoint mousePosDropSource, wxPoint mousePosDropTarget);

	Project& m_project;
	ion::render::Renderer& m_renderer;
	RenderResources& m_renderResources;
	wxGLContext& m_glContext;

	ActorId m_selectedActorId;
	Actor* m_selectedActor;
	SpriteSheetId m_selectedSpriteSheetId;
	SpriteSheet* m_selectedSpriteSheet;
	SpriteAnimId m_selectedAnimId;
	SpriteAnimation* m_selectedAnim;

	std::vector<ActorId> m_actorCache;
	std::vector<SpriteSheetId> m_spriteSheetCache;
	std::vector<SpriteAnimId> m_animCache;

	wxTimer m_timer;

	//Sprite frame list
	wxSharedPtr<wxImageList> m_spriteFrameImageList;

	//Timeline
	wxSharedPtr<wxImageList> m_timelineImageList;

	//Drag and drop
	int m_draggingSpriteFrameItem;
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