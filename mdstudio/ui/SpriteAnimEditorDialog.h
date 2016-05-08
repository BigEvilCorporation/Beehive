///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "UIBase.h"
#include "RenderResources.h"
#include "../Project.h"
#include "../SpriteSheet.h"

#include <ion/renderer/Renderer.h>

#include <wx/glcanvas.h>
#include <wx/timer.h>
#include <wx/dragimag.h>

class SpriteAnimEditorDialog : public SpriteAnimEditorDialogBase
{
public:
	SpriteAnimEditorDialog(wxWindow* parent, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources);

	void EventHandlerTimer(wxTimerEvent& event);
	void EventHandlerDragTimelineBegin(wxListEvent& event);
	void EventHandlerDragTimelineMove(wxMouseEvent& event);
	void EventHandlerDragTimelineEnd(wxMouseEvent& event);

protected:
	virtual void OnActorSelected(wxCommandEvent& event);
	virtual void OnSpriteSheetSelected(wxCommandEvent& event);
	virtual void OnAnimSelected(wxCommandEvent& event);
	virtual void OnBtnActorNew(wxCommandEvent& event);
	virtual void OnBtnActorDelete(wxCommandEvent& event);
	virtual void OnBtnActorExport(wxCommandEvent& event);
	virtual void OnBtnSpriteSheetImport(wxCommandEvent& event);
	virtual void OnButtonSpriteSheetDelete(wxCommandEvent& event);
	virtual void OnBtnAnimNew(wxCommandEvent& event);
	virtual void OnBtnAnimDelete(wxCommandEvent& event);
	virtual void OnSliderMove(wxScrollEvent& event);
	virtual void OnBtnPlay(wxCommandEvent& event);
	virtual void OnBtnStop(wxCommandEvent& event);
	virtual void OnSpinSpeedChange(wxSpinEvent& event);

private:
	void PopulateActorList();
	void PopulateSpriteSheetList(const Actor& actor);
	void PopulateAnimList(const SpriteSheet& spriteSheet);
	void PopulateKeyframes(const SpriteSheetId& spriteSheetId, const SpriteAnimation& anim);
		 
	void SelectActor(int index);
	void SelectSpriteSheet(int index);
	void SelectAnimation(int index);

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

	//Drag and drop
	int m_draggingTimelineItem;
	int m_dragDropTarget;
	int m_dragDropTargetPrev;
	wxDragImage* m_dragImage;
	std::vector<std::pair<u32, wxRect>> m_dragDropKeyframeList;
};