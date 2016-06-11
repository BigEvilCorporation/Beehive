///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "SpriteAnimEditorDialog.h"
#include "SpriteCanvas.h"
#include "Dialogs.h"

#include <wx/msgdlg.h>
#include <wx/imaglist.h>
#include <wx/dc.h>
#include <wx/menu.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/wglext.h>

SpriteAnimEditorDialog::SpriteAnimEditorDialog(wxWindow* parent, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources)
	: SpriteAnimEditorDialogBase(parent)
	, m_project(project)
	, m_renderer(renderer)
	, m_renderResources(renderResources)
	, m_glContext(glContext)
	, m_timer(this)
{
	m_selectedSpriteSheetId = InvalidSpriteSheetId;
	m_selectedAnimId = InvalidSpriteAnimId;
	m_selectedSpriteSheet = NULL;
	m_selectedAnim = NULL;

	m_draggingSpriteFrameItem = -1;
	m_draggingTimelineItem = -1;
	m_dragImage = NULL;

	m_canvas->SetProject(&project);
	m_canvas->SetupRendering(&renderer, &glContext, &renderResources);

	PopulateActorList();

	//Subscribe to events
	Bind(wxEVT_TIMER, &SpriteAnimEditorDialog::EventHandlerTimer, this, m_timer.GetId());
	m_gridSpriteFrames->Bind(wxEVT_GRID_CELL_BEGIN_DRAG, &SpriteAnimEditorDialog::EventHandlerDragFrameListBegin, this, m_gridSpriteFrames->GetId());
	m_gridSpriteFrames->Bind(wxEVT_MOTION, &SpriteAnimEditorDialog::EventHandlerDragFrameListMove, this, m_gridSpriteFrames->GetId());
	m_gridSpriteFrames->Bind(wxEVT_LEFT_UP, &SpriteAnimEditorDialog::EventHandlerDragFrameListEnd, this, m_gridSpriteFrames->GetId());
	m_gridTimeline->Bind(wxEVT_GRID_CELL_BEGIN_DRAG, &SpriteAnimEditorDialog::EventHandlerDragTimelineBegin, this, m_gridTimeline->GetId());
	m_gridTimeline->Bind(wxEVT_MOTION, &SpriteAnimEditorDialog::EventHandlerDragTimelineMove, this, m_gridTimeline->GetId());
	m_gridTimeline->Bind(wxEVT_LEFT_UP, &SpriteAnimEditorDialog::EventHandlerDragTimelineEnd, this, m_gridTimeline->GetId());
	m_gridTimeline->Bind(wxEVT_GRID_CELL_RIGHT_CLICK, &SpriteAnimEditorDialog::EventHandlerTimelineRightClick, this, m_gridTimeline->GetId());

	//Get system scrollbar height
	m_scrollbarHeight = wxSystemSettings::GetMetric(wxSYS_HSCROLL_Y, m_gridTimeline);

	//Default timeline size
	m_gridSpriteFrames->SetMinSize(wxSize(m_gridSpriteFrames->GetMinSize().x, s_iconHeight + (s_iconBorderY * 2) + m_scrollbarHeight));
	m_gridSpriteFrames->SetMaxSize(wxSize(m_gridSpriteFrames->GetMaxSize().x, s_iconHeight + (s_iconBorderY * 2) + m_scrollbarHeight));
	m_gridTimeline->SetMinSize(wxSize(m_gridTimeline->GetMinSize().x, s_iconHeight + (s_iconBorderY * 2) + m_scrollbarHeight));
	m_gridTimeline->SetMaxSize(wxSize(m_gridTimeline->GetMaxSize().x, s_iconHeight + (s_iconBorderY * 2) + m_scrollbarHeight));

	m_gridSpriteFrames->DisableDragColMove();
	m_gridSpriteFrames->DisableDragColSize();
	m_gridSpriteFrames->DisableDragGridSize();
	m_gridSpriteFrames->DisableDragRowSize();
	m_gridSpriteFrames->EnableDragCell(true);

	m_gridTimeline->DisableDragColMove();
	m_gridTimeline->DisableDragColSize();
	m_gridTimeline->DisableDragGridSize();
	m_gridTimeline->DisableDragRowSize();
	m_gridTimeline->EnableDragCell(true);

	Maximize();
}

SpriteAnimEditorDialog::~SpriteAnimEditorDialog()
{

}

void SpriteAnimEditorDialog::OnActorSelected(wxCommandEvent& event)
{
	SelectActor(event.GetSelection());
}

void SpriteAnimEditorDialog::OnSpriteSheetSelected(wxCommandEvent& event)
{
	SelectSpriteSheet(event.GetSelection());
}

void SpriteAnimEditorDialog::OnAnimSelected(wxCommandEvent& event)
{
	SelectAnimation(event.GetSelection());
}

void SpriteAnimEditorDialog::OnBtnActorNew(wxCommandEvent& event)
{
	DialogNewActor dialog(this);
	if(dialog.ShowModal() == wxID_OK)
	{
		ActorId actorId = m_project.CreateActor();
		Actor* actor = m_project.GetActor(actorId);
		actor->SetName(dialog.m_textName->GetValue().GetData().AsChar());

		//Populate list
		PopulateActorList();

		//Select in list
		int index = m_listActors->FindString(actor->GetName());
		m_listActors->SetSelection(index);
		SelectActor(index);
	}
}

void SpriteAnimEditorDialog::OnBtnActorDelete(wxCommandEvent& event)
{
	if(m_selectedActor)
	{
		for(TSpriteSheetMap::const_iterator it = m_selectedActor->SpriteSheetsBegin(), end = m_selectedActor->SpriteSheetsEnd(); it != end; ++it)
		{
			m_renderResources.DeleteSpriteSheetRenderResources(it->first);
		}

		m_canvas->SetDrawSpriteSheet(InvalidSpriteSheetId, 0);

		m_project.DeleteActor(m_selectedActorId);

		PopulateActorList();
		m_listSpriteSheets->Clear();
		m_listAnimations->Clear();
		m_spriteSheetCache.clear();
		m_animCache.clear();

		m_selectedActorId = InvalidActorId;
		m_selectedActor = NULL;
		m_selectedSpriteSheetId = InvalidSpriteSheetId;
		m_selectedSpriteSheet = NULL;
		m_selectedAnimId = InvalidSpriteAnimId;
		m_selectedAnim = NULL;

		Refresh();
	}
}

void SpriteAnimEditorDialog::OnBtnActorExport(wxCommandEvent& event)
{

}

void SpriteAnimEditorDialog::OnBtnSpriteSheetImport(wxCommandEvent& event)
{
	if(m_selectedActor)
	{
		ImportDialogSpriteSheet dialog(this, m_project, m_renderer, m_glContext, m_renderResources);
		if(dialog.ShowModal() == wxID_OK)
		{
			//Create new spriteSheet
			SpriteSheetId spriteSheetId = m_selectedActor->CreateSpriteSheet();
			SpriteSheet* spriteSheet = m_selectedActor->GetSpriteSheet(spriteSheetId);

			const int tileWidth = m_project.GetPlatformConfig().tileWidth;
			const int tileHeight = m_project.GetPlatformConfig().tileHeight;

			//Import bitmap
			if(spriteSheet->ImportBitmap(dialog.m_filePicker->GetPath().GetData().AsChar(), dialog.m_textName->GetValue().GetData().AsChar(), tileWidth, tileHeight, dialog.m_spinWidthCells->GetValue(), dialog.m_spinHeightCells->GetValue(), dialog.m_spinCellCount->GetValue()))
			{
				//Create render resources
				m_renderResources.CreateSpriteSheetResources(spriteSheetId, *spriteSheet);

				//Populate spriteSheet list
				PopulateSpriteSheetList(*m_selectedActor);

				//Select in list
				int index = m_listSpriteSheets->FindString(spriteSheet->GetName());
				m_listSpriteSheets->SetSelection(index);
				SelectSpriteSheet(index);
			}
			else
			{
				//Failed, remove spriteSheet
				wxMessageBox("Error importing spriteSheet", "Error", wxOK);
				m_selectedActor->DeleteSpriteSheet(spriteSheetId);
			}
		}
	}
}

void SpriteAnimEditorDialog::OnBtnSpriteSheetDelete(wxCommandEvent& event)
{
	if(m_selectedSpriteSheetId != InvalidSpriteSheetId)
	{
		m_selectedActor->DeleteSpriteSheet(m_selectedSpriteSheetId);
		m_renderResources.DeleteSpriteSheetRenderResources(m_selectedSpriteSheetId);
		m_canvas->SetDrawSpriteSheet(InvalidSpriteSheetId, 0);

		PopulateSpriteSheetList(*m_selectedActor);
		m_listAnimations->Clear();
		m_animCache.clear();

		m_selectedSpriteSheetId = InvalidSpriteSheetId;
		m_selectedSpriteSheet = NULL;
		m_selectedAnimId = InvalidSpriteAnimId;
		m_selectedAnim = NULL;

		Refresh();
	}
}

void SpriteAnimEditorDialog::OnBtnAnimNew(wxCommandEvent& event)
{
	if(m_selectedSpriteSheet)
	{
		DialogNewSpriteAnim dialog(this);
		dialog.m_textName->SetValue(m_selectedSpriteSheet->GetName());

		if(dialog.ShowModal() == wxID_OK)
		{
			SpriteAnimId animId = m_selectedSpriteSheet->AddAnimation();
			SpriteAnimation* anim = m_selectedSpriteSheet->GetAnimation(animId);

			anim->SetName(dialog.m_textName->GetValue().GetData().AsChar());

			if(dialog.m_chkAutoGenerate->GetValue())
			{
				for(u32 i = 0; i < m_selectedSpriteSheet->GetNumFrames(); i++)
				{
					anim->m_trackSpriteFrame.InsertKeyframe(AnimKeyframeSpriteFrame((float)i, i));
				}

				anim->SetLength((float)m_selectedSpriteSheet->GetNumFrames() );
			}

			//Populate list
			PopulateAnimList(*m_selectedSpriteSheet);

			//Select in list
			int index = m_listAnimations->FindString(anim->GetName());
			m_listAnimations->SetSelection(index);
			SelectAnimation(index);
		}
	}
}

void SpriteAnimEditorDialog::OnBtnAnimDelete(wxCommandEvent& event)
{
	if(m_selectedSpriteSheet && m_selectedAnimId != InvalidSpriteAnimId)
	{
		m_selectedSpriteSheet->DeleteAnimation(m_selectedAnimId);
		PopulateAnimList(*m_selectedSpriteSheet);
		m_selectedAnimId = InvalidSpriteAnimId;
		m_selectedAnim = NULL;
		Refresh();
	}
}

void SpriteAnimEditorDialog::OnSliderMove(wxScrollEvent& event)
{
	if(m_selectedAnim && m_selectedAnim->GetState() == ion::render::Animation::eStopped)
	{
		float time = ion::maths::Lerp(0.0f, m_selectedAnim->GetLength(), (float)m_sliderTimeline->GetValue() / 100.0f);
		int frame = m_selectedAnim->m_trackSpriteFrame.GetValue(time);

		m_selectedAnim->SetFrame(time);
		m_canvas->SetDrawSpriteSheet(m_selectedSpriteSheetId, frame);
		m_gridTimeline->GoToCell(0, frame);
	}
}

void SpriteAnimEditorDialog::OnBtnPlay(wxCommandEvent& event)
{
	if(m_selectedAnim)
	{
		m_selectedAnim->SetState(ion::render::Animation::ePlaying);
		m_timer.Start(1);
	}
}

void SpriteAnimEditorDialog::OnBtnStop(wxCommandEvent& event)
{
	m_timer.Stop();

	if(m_selectedAnim)
	{
		m_selectedAnim->SetState(ion::render::Animation::eStopped);
	}
}

void SpriteAnimEditorDialog::OnSpinSpeedChange(wxSpinEvent& event)
{
	if(m_selectedAnim)
	{
		m_selectedAnim->SetSpeed(event.GetValue());
	}
}

void SpriteAnimEditorDialog::PopulateActorList()
{
	m_listActors->Clear();
	m_actorCache.clear();
	m_selectedActorId = InvalidActorId;
	m_selectedActor = NULL;

	typedef std::pair<std::string, ActorId> TNameIDPair;
	typedef std::vector<TNameIDPair> TNameList;
	TNameList nameList;

	for(TActorMap::const_iterator it = m_project.ActorsBegin(), end = m_project.ActorsEnd(); it != end; ++it)
	{
		nameList.push_back(std::make_pair(it->second.GetName(), it->first));
	}

	std::sort(nameList.begin(), nameList.end(), [](TNameIDPair& a, TNameIDPair& b) { return a.first < b.first; });

	for(int i = 0; i < nameList.size(); i++)
	{
		//Store by index
		m_actorCache.push_back(nameList[i].second);

		//Add to list
		m_listActors->AppendString(nameList[i].first);
	}
}

void SpriteAnimEditorDialog::PopulateSpriteSheetList(const Actor& actor)
{
	m_listSpriteSheets->Clear();
	m_spriteSheetCache.clear();
	m_selectedSpriteSheetId = InvalidSpriteSheetId;
	m_selectedSpriteSheet = NULL;

	if(m_selectedActor)
	{
		typedef std::pair<std::string, SpriteSheetId> TNameIDPair;
		typedef std::vector<TNameIDPair> TNameList;
		TNameList nameList;

		for(TSpriteSheetMap::const_iterator it = actor.SpriteSheetsBegin(), end = actor.SpriteSheetsEnd(); it != end; ++it)
		{
			nameList.push_back(std::make_pair(it->second.GetName(), it->first));
		}

		std::sort(nameList.begin(), nameList.end(), [](TNameIDPair& a, TNameIDPair& b) { return a.first < b.first; });

		for(int i = 0; i < nameList.size(); i++)
		{
			//Store by index
			m_spriteSheetCache.push_back(nameList[i].second);

			//Add to list
			m_listSpriteSheets->AppendString(nameList[i].first);
		}
	}
}

void SpriteAnimEditorDialog::PopulateAnimList(const SpriteSheet& spriteSheet)
{
	m_listAnimations->Clear();
	m_animCache.clear();
	m_selectedAnimId = InvalidSpriteAnimId;
	m_selectedAnim = NULL;

	if(m_selectedSpriteSheet)
	{
		typedef std::pair<std::string, SpriteAnimId> TNameIDPair;
		typedef std::vector<TNameIDPair> TNameList;
		TNameList nameList;

		for(TSpriteAnimMap::const_iterator it = m_selectedSpriteSheet->AnimationsBegin(), end = m_selectedSpriteSheet->AnimationsEnd(); it != end; ++it)
		{
			nameList.push_back(std::make_pair(it->second.GetName(), it->first));
		}

		std::sort(nameList.begin(), nameList.end(), [](TNameIDPair& a, TNameIDPair& b) { return a.first < b.first; });

		for(int i = 0; i < nameList.size(); i++)
		{
			//Store by index
			m_animCache.push_back(nameList[i].second);

			//Add to list
			m_listAnimations->AppendString(nameList[i].first);
		}
	}
}

void SpriteAnimEditorDialog::PopulateSpriteFrames(const SpriteSheetId& spriteSheetId)
{
	//Get sprite resources
	const RenderResources::SpriteSheetRenderResources* spriteResources = m_renderResources.GetSpriteSheetResources(spriteSheetId);

	//Clear existing
	m_gridSpriteFrames->DeleteCols(0, m_gridSpriteFrames->GetNumberCols());

	int numFrames = spriteResources->m_frames.size();

	if(numFrames > 0)
	{
		//Create new image list based on size of first texture
		const ion::render::Texture* texture0 = spriteResources->m_frames[0].texture;

		const float iconAspect = (float)texture0->GetWidth() / (float)texture0->GetHeight();
		const int iconWidth = s_iconHeight * iconAspect;

		m_spriteFrameImageList = new wxImageList(iconWidth, s_iconHeight, numFrames);

		//Create cells
		m_gridSpriteFrames->AppendCols(numFrames);

		//Reset timeline row height
		m_gridSpriteFrames->SetRowSize(0, s_iconHeight + (s_iconBorderY * 2));
		m_gridSpriteFrames->DisableRowResize(0);

		for(int i = 0; i < numFrames; i++)
		{
			//Setup column
			m_gridSpriteFrames->SetColSize(i, iconWidth + (s_iconBorderX * 2));
			m_gridSpriteFrames->DisableColResize(i);
			m_gridSpriteFrames->SetCellRenderer(0, i, new GridCellBitmapRenderer(m_spriteFrameImageList.get()));

			//Fetch texture, convert to wxImage
			const ion::render::Texture* texture = spriteResources->m_frames[i].texture;
			wxImage image(texture->GetWidth(), texture->GetHeight());
			texture->GetPixels(ion::Vector2i(0, 0), ion::Vector2i(texture->GetWidth(), texture->GetHeight()), ion::render::Texture::eRGB, ion::render::Texture::eBPP24, image.GetData());

			//Invert Y for OpenGL
			image = image.Mirror(false);

			//Scale
			image.Rescale(iconWidth, s_iconHeight);

			//To bitmap
			wxBitmap bitmap(image);

			//Add to image list
			m_spriteFrameImageList->Add(bitmap);
		}

		//Set sprite frame list height
		m_gridSpriteFrames->SetMinSize(wxSize(m_gridSpriteFrames->GetMinSize().x, s_iconHeight + (s_iconBorderY * 2) + m_scrollbarHeight));
		m_gridSpriteFrames->SetMaxSize(wxSize(m_gridSpriteFrames->GetMaxSize().x, s_iconHeight + (s_iconBorderY * 2) + m_scrollbarHeight));
		m_gridSpriteFrames->GetContainingSizer()->Layout();
		m_gridSpriteFrames->Refresh();
	}
}

void SpriteAnimEditorDialog::PopulateKeyframes(const SpriteSheetId& spriteSheetId, const SpriteAnimation& anim)
{
	//Get sprite resources
	const RenderResources::SpriteSheetRenderResources* spriteResources = m_renderResources.GetSpriteSheetResources(spriteSheetId);

	//Clear existing
	m_gridTimeline->DeleteCols(0, m_gridTimeline->GetNumberCols());

	const u32 numKeyframes = anim.m_trackSpriteFrame.GetNumKeyframes();

	if(numKeyframes > 0 && spriteResources->m_frames.size() > 0)
	{
		//Create new image list based on size of first texture
		const ion::render::Texture* texture0 = spriteResources->m_frames[0].texture;

		const float iconAspect = (float)texture0->GetWidth() / (float)texture0->GetHeight();
		const int iconWidth = s_iconHeight * iconAspect;

		m_timelineImageList = new wxImageList(iconWidth, s_iconHeight, numKeyframes);

		//Create timeline cells
		m_gridTimeline->AppendCols(numKeyframes);
		
		//Reset timeline row height
		m_gridTimeline->SetRowSize(0, s_iconHeight + (s_iconBorderY * 2));
		m_gridTimeline->DisableRowResize(0);

		for(int i = 0; i < numKeyframes; i++)
		{
			//Setup column
			m_gridTimeline->SetColSize(i, iconWidth + (s_iconBorderX * 2));
			m_gridTimeline->DisableColResize(i);
			m_gridTimeline->SetCellRenderer(0, i, new GridCellBitmapRenderer(m_timelineImageList.get()));

			//Get keyframe
			const int frame = anim.m_trackSpriteFrame.GetKeyframe(i).GetValue();
			 
			//Fetch texture, convert to wxImage
			const ion::render::Texture* texture = spriteResources->m_frames[frame].texture;
			wxImage image(texture->GetWidth(), texture->GetHeight());
			texture->GetPixels(ion::Vector2i(0, 0), ion::Vector2i(texture->GetWidth(), texture->GetHeight()), ion::render::Texture::eRGB, ion::render::Texture::eBPP24, image.GetData());

			//Invert Y for OpenGL
			image = image.Mirror(false);

			//Scale
			image.Rescale(iconWidth, s_iconHeight);

			//To bitmap
			wxBitmap bitmap(image);

			//Add to image list
			m_timelineImageList->Add(bitmap);
		}

		//Set timeline height
		m_gridTimeline->SetMinSize(wxSize(m_gridTimeline->GetMinSize().x, s_iconHeight + (s_iconBorderY * 2) + m_scrollbarHeight));
		m_gridTimeline->SetMaxSize(wxSize(m_gridTimeline->GetMaxSize().x, s_iconHeight + (s_iconBorderY * 2) + m_scrollbarHeight));
		m_gridTimeline->GetContainingSizer()->Layout();
		m_gridTimeline->Refresh();
	}
}

void SpriteAnimEditorDialog::SelectActor(int index)
{
	if(m_selectedAnim)
	{
		m_selectedAnim->SetState(ion::render::Animation::eStopped);
	}

	m_selectedActorId = InvalidActorId;
	m_selectedActor = NULL;

	m_listSpriteSheets->Clear();
	m_listAnimations->Clear();
	//m_listSpriteFrames->ClearAll();

	if(m_actorCache.size() > 0)
	{
		if(index >= 0 && index < m_actorCache.size())
		{
			m_selectedActorId = m_actorCache[index];
			m_selectedActor = m_project.GetActor(m_selectedActorId);
			ion::debug::Assert(m_selectedActor, "SpriteAnimEditorDialog::OnActorSelected() - Invalid actor ID");
			PopulateSpriteSheetList(*m_selectedActor);
		}
	}
}

void SpriteAnimEditorDialog::SelectSpriteSheet(int index)
{
	if(m_selectedAnim)
	{
		m_selectedAnim->SetState(ion::render::Animation::eStopped);
	}

	m_selectedSpriteSheetId = InvalidSpriteSheetId;
	m_selectedSpriteSheet = NULL;

	m_listAnimations->Clear();
	//m_listSpriteFrames->ClearAll();

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	if(m_selectedActor && m_spriteSheetCache.size() > 0)
	{
		if(index >= 0 && index < m_spriteSheetCache.size())
		{
			m_selectedSpriteSheetId = m_spriteSheetCache[index];
			m_selectedSpriteSheet = m_selectedActor->GetSpriteSheet(m_selectedSpriteSheetId);
			ion::debug::Assert(m_selectedSpriteSheet, "SpriteAnimEditorDialog::OnSpriteSheetSelected() - Invalid spriteSheet ID");
			m_canvas->SetSpriteSheetDimentionsPixels(ion::Vector2i(m_selectedSpriteSheet->GetWidthTiles() * tileWidth, m_selectedSpriteSheet->GetHeightTiles() * tileHeight));
			m_canvas->SetDrawSpriteSheet(m_selectedSpriteSheetId, 0);

			PopulateSpriteFrames(m_selectedSpriteSheetId);
			PopulateAnimList(*m_selectedSpriteSheet);
		}
	}
}

void SpriteAnimEditorDialog::SelectAnimation(int index)
{
	if(m_selectedAnim)
	{
		m_selectedAnim->SetState(ion::render::Animation::eStopped);
	}

	m_selectedAnimId = InvalidSpriteAnimId;
	m_selectedAnim = NULL;

	//m_listSpriteFrames->ClearAll();

	if(m_selectedSpriteSheet)
	{
		if(m_animCache.size() > 0)
		{
			if(index >= 0 && index < m_animCache.size())
			{
				m_selectedAnimId = m_animCache[index];
				m_selectedAnim = m_selectedSpriteSheet->GetAnimation(m_selectedAnimId);
				ion::debug::Assert(m_selectedAnim, "SpriteAnimEditorDialog::OnAnimSelected() - Invalid animation ID");

				PopulateKeyframes(m_selectedSpriteSheetId, *m_selectedAnim);

				m_spinCtrlSpeed->SetValue(m_selectedAnim->GetSpeed());
			}
		}
	}
}

void SpriteAnimEditorDialog::EventHandlerTimer(wxTimerEvent& event)
{
	if(m_selectedAnim)
	{
		float frameRate = 24.0f;
		float frameRateMul = 1.0f / (frameRate / 10.0f);
		float delta = (float)event.GetInterval() * ((float)m_spinCtrlSpeed->GetValue() / 100.0f) * frameRateMul;
		float time = m_selectedAnim->GetFrame();
		float lerpTime = ion::maths::UnLerp(0.0f, m_selectedAnim->GetLength(), time);
		int frame = m_selectedAnim->m_trackSpriteFrame.GetValue(time);

		m_selectedAnim->Update(delta);
		m_canvas->SetDrawSpriteSheet(m_selectedSpriteSheetId, frame);
		m_sliderTimeline->SetValue((int)ion::maths::Round(lerpTime * 100.0f));
		m_gridTimeline->GoToCell(0, frame);
	}
}

void SpriteAnimEditorDialog::EventHandlerDragFrameListBegin(wxGridEvent& event)
{
	m_draggingSpriteFrameItem = event.GetCol();

	wxImageList* imageList = m_timelineImageList.get();
	if(imageList)
	{
		if(m_draggingSpriteFrameItem >= 0 && m_draggingSpriteFrameItem < imageList->GetImageCount())
		{
			m_dragImage = new wxDragImage(imageList->GetBitmap(m_draggingSpriteFrameItem));
			m_dragImage->BeginDrag(wxPoint(0, 0), m_gridSpriteFrames, m_gridTimeline);
			m_dragImage->Show();

			m_dragDropKeyframeList.clear();

			if(m_selectedAnim)
			{
				//Backup all keyframes and list entry rects
				for(int i = 0; i < imageList->GetImageCount(); i++)
				{
					wxRect rect = m_gridTimeline->CellToRect(0, i);

					u32 frame = m_selectedAnim->m_trackSpriteFrame.GetKeyframe(i).GetValue();

					m_dragDropKeyframeList.push_back(std::make_pair(frame, rect));
				}
			}

			m_dragDropTarget = -1;
			m_dragDropTargetPrev = -1;
		}
	}
}

void SpriteAnimEditorDialog::EventHandlerDragFrameListMove(wxMouseEvent& event)
{
	if(m_dragImage && m_draggingSpriteFrameItem >= 0)
	{
		//Get mouse position relative to drag source
		wxPoint positionSource = event.GetPosition();

		//Get mouse position relative to timeline grid
		wxPoint positionTarget = positionSource + m_gridSpriteFrames->GetPosition();
		positionTarget -= m_gridTimeline->GetPosition();

		FindAndDrawDropTarget(positionSource, positionTarget);
	}
}

void SpriteAnimEditorDialog::EventHandlerDragFrameListEnd(wxMouseEvent& event)
{
	if(m_dragImage)
	{
		m_dragImage->Hide();
		m_dragImage->EndDrag();
		delete m_dragImage;
		m_dragImage = NULL;

		if(m_draggingSpriteFrameItem >= 0 && m_dragDropTarget >= 0)
		{
			//Valid drop target, insert into animation
			m_dragDropKeyframeList.insert(m_dragDropKeyframeList.begin() + m_dragDropTarget, std::make_pair(m_draggingSpriteFrameItem, wxRect()));

			//Re-populate animation
			m_selectedAnim->m_trackSpriteFrame.Clear();

			for(int i = 0; i < m_dragDropKeyframeList.size(); i++)
			{
				m_selectedAnim->m_trackSpriteFrame.InsertKeyframe(AnimKeyframeSpriteFrame((float)i, m_dragDropKeyframeList[i].first));
			}

			//Set new anim length
			m_selectedAnim->SetLength((float)m_dragDropKeyframeList.size());

			//Re-populate keyframe timeline
			PopulateKeyframes(m_selectedSpriteSheetId, *m_selectedAnim);
		}

		m_draggingSpriteFrameItem = -1;
		m_draggingTimelineItem = -1;
		m_dragDropKeyframeList.clear();
	}
}

void SpriteAnimEditorDialog::EventHandlerDragTimelineBegin(wxGridEvent& event)
{
	m_draggingTimelineItem = event.GetCol();

	wxImageList* imageList = m_timelineImageList.get();
	if(imageList)
	{
		if(m_selectedAnim && m_draggingTimelineItem >= 0 && m_draggingTimelineItem < imageList->GetImageCount())
		{
			m_dragImage = new wxDragImage(imageList->GetBitmap(m_draggingTimelineItem));
			m_dragImage->BeginDrag(wxPoint(0, 0), m_gridTimeline, m_gridTimeline);
			m_dragImage->Show();
	
			m_dragDropKeyframeList.clear();
	
			//Backup all keyframes and list entry rects
			for(int i = 0; i < imageList->GetImageCount(); i++)
			{
				wxRect rect = m_gridTimeline->CellToRect(0, i);
	
				u32 frame = m_selectedAnim->m_trackSpriteFrame.GetKeyframe(i).GetValue();
	
				m_dragDropKeyframeList.push_back(std::make_pair(frame, rect));
			}
	
			m_dragDropTarget = -1;
			m_dragDropTargetPrev = -1;
		}
	}
}

void SpriteAnimEditorDialog::EventHandlerDragTimelineMove(wxMouseEvent& event)
{
	if(m_dragImage && m_draggingTimelineItem >= 0)
	{
		wxPoint position = event.GetPosition();

		m_dragImage->Move(position);

		FindAndDrawDropTarget(position, position);
	}
}

void SpriteAnimEditorDialog::EventHandlerDragTimelineEnd(wxMouseEvent& event)
{
	if(m_dragImage)
	{
		m_dragImage->Hide();
		m_dragImage->EndDrag();
		delete m_dragImage;
		m_dragImage = NULL;

		if(m_draggingTimelineItem >= 0 && m_dragDropTarget  >= 0 && m_dragDropTarget != m_draggingTimelineItem)
		{
			//Valid drop target, insert into list
			m_dragDropKeyframeList.insert(m_dragDropKeyframeList.begin() + m_dragDropTarget, m_dragDropKeyframeList[m_draggingTimelineItem]);

			//If original item was after drop target in list, its index will have incremented
			if(m_draggingTimelineItem >= m_dragDropTarget)
			{
				m_draggingTimelineItem++;
			}

			//Delete original
			m_dragDropKeyframeList.erase(m_dragDropKeyframeList.begin() + m_draggingTimelineItem);

			//Re-populate animation
			m_selectedAnim->m_trackSpriteFrame.Clear();

			for(int i = 0; i < m_dragDropKeyframeList.size(); i++)
			{
				m_selectedAnim->m_trackSpriteFrame.InsertKeyframe(AnimKeyframeSpriteFrame((float)i, m_dragDropKeyframeList[i].first));
			}

			//Set new anim length
			m_selectedAnim->SetLength((float)m_dragDropKeyframeList.size());

			//Re-populate keyframe timeline
			PopulateKeyframes(m_selectedSpriteSheetId, *m_selectedAnim);
		}

		m_draggingSpriteFrameItem = -1;
		m_draggingTimelineItem = -1;
		m_dragDropKeyframeList.clear();
	}
}

void SpriteAnimEditorDialog::EventHandlerTimelineRightClick(wxGridEvent& event)
{
	if(m_selectedAnim)
	{
		//Right-click menu
		m_contextMenuKeyframeIndex = event.GetCol();

		wxMenu contextMenu;
		contextMenu.Append(eMenuDeleteKeyframe, wxString("Delete keyframe"));
		contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&SpriteAnimEditorDialog::EventHandlerContextMenuClick, NULL, this);
		PopupMenu(&contextMenu);
	}
}

void SpriteAnimEditorDialog::EventHandlerContextMenuClick(wxCommandEvent& event)
{
	if(m_selectedAnim)
	{
		if(event.GetId() == eMenuDeleteKeyframe)
		{
			//Rebuild animation
			std::vector<u32> keyframes;

			for(int i = 0; i < m_selectedAnim->m_trackSpriteFrame.GetNumKeyframes(); i++)
			{
				if(i != m_contextMenuKeyframeIndex)
				{
					keyframes.push_back(m_selectedAnim->m_trackSpriteFrame.GetKeyframe(i).GetValue());
				}
			}

			m_selectedAnim->m_trackSpriteFrame.Clear();

			for(int i = 0; i < keyframes.size(); i++)
			{
				m_selectedAnim->m_trackSpriteFrame.InsertKeyframe(AnimKeyframeSpriteFrame((float)i, keyframes[i]));
			}

			//Re-populate keyframe view
			PopulateKeyframes(m_selectedSpriteSheetId, *m_selectedAnim);
		}
	}
}

void SpriteAnimEditorDialog::FindAndDrawDropTarget(wxPoint mousePosDropSource, wxPoint mousePosDropTarget)
{
	m_dragImage->Move(mousePosDropSource);

	if(m_dragDropKeyframeList.size() > 0)
	{
		m_dragDropTarget = -1;

		//Check within timeline bounds
		if(mousePosDropTarget.y >= 0 && mousePosDropTarget.y <= m_gridTimeline->GetSize().GetHeight())
		{
			//Offset scroll
			int pixelsPerScrollUnitX = 0;
			int pixelsPerScrollUnitY = 0;
			m_gridTimeline->GetScrollPixelsPerUnit(&pixelsPerScrollUnitX, &pixelsPerScrollUnitY);
			wxPoint scrollOffset(m_gridTimeline->GetViewStart().x * pixelsPerScrollUnitX, m_gridTimeline->GetViewStart().y * pixelsPerScrollUnitY);
			mousePosDropTarget.x += scrollOffset.x;

			if(mousePosDropTarget.x < m_dragDropKeyframeList.front().second.GetLeft() + (m_dragDropKeyframeList.front().second.GetWidth() / 2))
			{
				//Drag target is left of first item
				m_dragDropTarget = 0;
			}
			else if(mousePosDropTarget.x > m_dragDropKeyframeList.back().second.GetRight() - (m_dragDropKeyframeList.back().second.GetWidth() / 2))
			{
				//Drag target is right of last item
				m_dragDropTarget = m_dragDropKeyframeList.size();
			}
			else
			{
				for(int i = 0; i < m_dragDropKeyframeList.size() - 1 && m_dragDropTarget == -1; i++)
				{
					int posMin = m_dragDropKeyframeList[i].second.GetRight() - (m_dragDropKeyframeList[i].second.GetWidth() / 2);
					int posMax = m_dragDropKeyframeList[i + 1].second.GetLeft() + (m_dragDropKeyframeList[i + 1].second.GetWidth() / 2);

					if(mousePosDropTarget.x > posMin && mousePosDropTarget.x < posMax)
					{
						//Drop target is between item i and item i+1
						m_dragDropTarget = i + 1;
					}
				}
			}

			if(m_dragDropTarget != m_dragDropTargetPrev)
			{
				//De-select previous cell
				if(m_dragDropTargetPrev >= 0)
				{
					int drawIndex = (m_dragDropTargetPrev == m_dragDropKeyframeList.size()) ? m_dragDropTargetPrev - 1 : m_dragDropTargetPrev;
					GridCellBitmapRenderer* cellRenderer = (GridCellBitmapRenderer*)m_gridTimeline->GetCellRenderer(0, drawIndex);
					cellRenderer->SetDrawSelectionEdge(GridCellBitmapRenderer::eDrawSelectionEdgeNone);

					wxRect refreshRect = m_dragDropKeyframeList[drawIndex].second;
					refreshRect.Offset(-scrollOffset);
					m_gridTimeline->Refresh(true, &refreshRect);
				}

				//Set selected cell
				if(m_dragDropTarget >= 0)
				{
					if(m_dragDropTarget == m_dragDropKeyframeList.size())
					{
						//Far right of last cell
						GridCellBitmapRenderer* cellRenderer = (GridCellBitmapRenderer*)m_gridTimeline->GetCellRenderer(0, m_dragDropTarget - 1);
						cellRenderer->SetDrawSelectionEdge(GridCellBitmapRenderer::eDrawSelectionEdgeRight);

						wxRect refreshRect = m_dragDropKeyframeList[m_dragDropTarget - 1].second;
						refreshRect.Offset(-scrollOffset);
						m_gridTimeline->Refresh(true, &refreshRect);
					}
					else
					{
						//Left of current cell
						GridCellBitmapRenderer* cellRenderer = (GridCellBitmapRenderer*)m_gridTimeline->GetCellRenderer(0, m_dragDropTarget);
						cellRenderer->SetDrawSelectionEdge(GridCellBitmapRenderer::eDrawSelectionEdgeLeft);

						wxRect refreshRect = m_dragDropKeyframeList[m_dragDropTarget].second;
						refreshRect.Offset(-scrollOffset);
						m_gridTimeline->Refresh(true, &refreshRect);
					}
				}

				//Drop target changed
				m_dragDropTargetPrev = m_dragDropTarget;
			}
		}
	}
}

GridCellBitmapRenderer::GridCellBitmapRenderer(wxImageList* imageList)
{
	m_imageList = imageList;
	m_selectionEdge = eDrawSelectionEdgeNone;
}

GridCellBitmapRenderer::~GridCellBitmapRenderer()
{

}

void GridCellBitmapRenderer::SetImageList(wxImageList* imageList)
{
	m_imageList = imageList;
}

wxImageList* GridCellBitmapRenderer::GetImageList() const
{
	return m_imageList;
}

void GridCellBitmapRenderer::SetDrawSelectionEdge(SelectionEdge selectionEdge)
{
	m_selectionEdge = selectionEdge;
}

void GridCellBitmapRenderer::Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected)
{
	wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);

	if(m_imageList && col < m_imageList->GetImageCount())
	{
		dc.DrawBitmap(m_imageList->GetBitmap(col), rect.x + SpriteAnimEditorDialog::s_iconBorderX, rect.y + SpriteAnimEditorDialog::s_iconBorderY);
	}

	const int penWidth = 4;

	wxPen pen;
	pen.SetColour(wxColour(0, 0, 0, 255));
	pen.SetWidth(penWidth);
	dc.SetPen(pen);

	if(m_selectionEdge == eDrawSelectionEdgeLeft)
	{
		dc.DrawLine(rect.x + (penWidth / 2), rect.y, rect.x + (penWidth / 2), rect.y + rect.GetHeight());
	}
	else if(m_selectionEdge == eDrawSelectionEdgeRight)
	{
		dc.DrawLine(rect.x + rect.GetWidth() - (penWidth / 2), rect.y, rect.x + rect.GetWidth() - (penWidth / 2), rect.y + rect.GetHeight());
	}
}