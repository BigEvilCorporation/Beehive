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

SpriteAnimEditorDialog::SpriteAnimEditorDialog(wxWindow* parent, AnimEditMode animEditMode, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources)
	: SpriteAnimEditorDialogBase(parent)
	, m_animEditMode(animEditMode)
	, m_project(project)
	, m_renderer(renderer)
	, m_renderResources(renderResources)
	, m_glContext(glContext)
	, m_timer(this)
{
	m_selectedActorId = InvalidActorId;
	m_selectedActor = NULL;
	m_selectedStampId = InvalidStampId;
	m_selectedStamp = NULL;
	m_selectedSpriteSheetId = InvalidSpriteSheetId;
	m_selectedAnimId = InvalidSpriteAnimId;
	m_selectedSpriteSheet = NULL;
	m_selectedAnim = NULL;

	m_draggingSpriteFrameItem = -1;
	m_draggingTimelineItem = -1;
	m_dragImage = NULL;

	m_canvas->SetProject(&project);
	m_canvas->SetupRendering(&renderer, &glContext, &renderResources);

	if(m_animEditMode == eAnimEditModeSpriteAnim)
	{
		PopulateActorList();
	}
	else if(m_animEditMode == eAnimEditModeStampAnim)
	{
		PopulateStampList();
	}

	//Subscribe to events
	Bind(wxEVT_TIMER, &SpriteAnimEditorDialog::EventHandlerTimer, this, m_timer.GetId());
	m_gridSpriteFrames->Bind(wxEVT_GRID_CELL_BEGIN_DRAG, &SpriteAnimEditorDialog::EventHandlerDragFrameListBegin, this, m_gridSpriteFrames->GetId());
	m_gridSpriteFrames->Bind(wxEVT_MOTION, &SpriteAnimEditorDialog::EventHandlerDragFrameListMove, this, m_gridSpriteFrames->GetId());
	m_gridSpriteFrames->Bind(wxEVT_LEFT_UP, &SpriteAnimEditorDialog::EventHandlerDragFrameListEnd, this, m_gridSpriteFrames->GetId());
	m_gridTimeline->Bind(wxEVT_GRID_CELL_BEGIN_DRAG, &SpriteAnimEditorDialog::EventHandlerDragTimelineBegin, this, m_gridTimeline->GetId());
	m_gridTimeline->Bind(wxEVT_MOTION, &SpriteAnimEditorDialog::EventHandlerDragTimelineMove, this, m_gridTimeline->GetId());
	m_gridTimeline->Bind(wxEVT_LEFT_UP, &SpriteAnimEditorDialog::EventHandlerDragTimelineEnd, this, m_gridTimeline->GetId());
	m_gridTimeline->Bind(wxEVT_GRID_CELL_RIGHT_CLICK, &SpriteAnimEditorDialog::EventHandlerTimelineRightClick, this, m_gridTimeline->GetId());
	m_gridTimeline->Bind(wxEVT_GRID_CELL_CHANGING, &SpriteAnimEditorDialog::OnTimelineCellChange, this, m_gridTimeline->GetId());

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

	//Append 1 row per keyframe track
	m_gridTimeline->AppendRows(eTrack_MAX - m_gridTimeline->GetRows());

	Maximize();
}

SpriteAnimEditorDialog::~SpriteAnimEditorDialog()
{

}

void SpriteAnimEditorDialog::SetSelectedStamp(StampId stampId)
{
	for(int i = 0; i < m_stampCache.size(); i++)
	{
		if(m_stampCache[i] == stampId)
		{
			m_listActors->SetSelection(i);
			SelectStamp(i);
			break;
		}
	}
}

void SpriteAnimEditorDialog::OnActorSelected(wxCommandEvent& event)
{
	if(m_animEditMode == eAnimEditModeSpriteAnim)
	{
		SelectActor(event.GetSelection());
	}
	else if(m_animEditMode == eAnimEditModeStampAnim)
	{
		SelectStamp(event.GetSelection());
	}
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

		m_canvas->SetDrawSpriteSheet(InvalidSpriteSheetId, 0, ion::Vector2i());

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

void SpriteAnimEditorDialog::OnBtnImport(wxCommandEvent& event)
{
	wxFileDialog dialogue(this, _("Open Beehive GameObj file"), "", "", "Bee_Sprite files (*.bee_sprites)|*.bee_sprites", wxFD_OPEN);
	if(dialogue.ShowModal() == wxID_OK)
	{
		std::string filename = dialogue.GetPath().c_str().AsChar();
		m_project.ImportActors(filename);
		PopulateActorList();
		m_renderResources.CreateSpriteSheetResources(m_project);
	}
}

void SpriteAnimEditorDialog::OnBtnExport(wxCommandEvent& event)
{
	wxFileDialog dialogue(this, _("Save Beehive Sprites file"), "", "", "Bee_Sprites files (*.bee_sprites)|*.bee_sprites", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if(dialogue.ShowModal() == wxID_OK)
	{
		std::string filename = dialogue.GetPath().c_str().AsChar();
		m_project.ExportActors(filename);
	}
}

void SpriteAnimEditorDialog::OnBtnSpriteSheetImport(wxCommandEvent& event)
{
	if(m_selectedActor || m_selectedStamp)
	{
		ImportDialogSpriteSheet dialog(this, m_project, m_renderer, m_glContext, m_renderResources);
		if(dialog.ShowModal() == wxID_OK)
		{
			//Create new spriteSheet
			SpriteSheetId spriteSheetId = m_selectedActor ? m_selectedActor->CreateSpriteSheet() : m_selectedStamp->CreateStampAnimSheet();
			SpriteSheet* spriteSheet = m_selectedActor ? m_selectedActor->GetSpriteSheet(spriteSheetId) : m_selectedStamp->GetStampAnimSheet(spriteSheetId);

			const int tileWidth = m_project.GetPlatformConfig().tileWidth;
			const int tileHeight = m_project.GetPlatformConfig().tileHeight;

			//Import bitmap
			if(spriteSheet->ImportBitmap(dialog.m_filePicker->GetPath().GetData().AsChar(), dialog.m_textName->GetValue().GetData().AsChar(), tileWidth, tileHeight, dialog.m_spinWidthCells->GetValue(), dialog.m_spinHeightCells->GetValue(), dialog.m_spinCellCount->GetValue()))
			{
				//Create render resources
				m_renderResources.CreateSpriteSheetResources(spriteSheetId, *spriteSheet);

				//Populate spriteSheet list
				if(m_selectedActor)
				{
					PopulateSpriteSheetList(*m_selectedActor);
				}
				else if(m_selectedStamp)
				{
					PopulateStampAnimSheetList(*m_selectedStamp);
				}

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
		if(m_selectedActor)
		{
			m_selectedActor->DeleteSpriteSheet(m_selectedSpriteSheetId);
			PopulateSpriteSheetList(*m_selectedActor);
		}
		else if(m_selectedStamp)
		{
			m_selectedStamp->DeleteStampAnimSheet(m_selectedSpriteSheetId);
			PopulateStampAnimSheetList(*m_selectedStamp);
		}

		m_renderResources.DeleteSpriteSheetRenderResources(m_selectedSpriteSheetId);
		m_canvas->SetDrawSpriteSheet(InvalidSpriteSheetId, 0, ion::Vector2i());

		m_listAnimations->Clear();
		m_animCache.clear();

		m_selectedSpriteSheetId = InvalidSpriteSheetId;
		m_selectedSpriteSheet = NULL;
		m_selectedAnimId = InvalidSpriteAnimId;
		m_selectedAnim = NULL;

		Refresh();
	}
}

void SpriteAnimEditorDialog::OnBtnSpriteSheetUsePalette(wxCommandEvent& event)
{
	if(m_selectedSpriteSheetId != InvalidSpriteSheetId && m_selectedActor)
	{
		m_selectedActor->SetMasterPalette(m_selectedSpriteSheetId);
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

				anim->SetLength((float)m_selectedSpriteSheet->GetNumFrames());
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

void SpriteAnimEditorDialog::OnTimelineCellChange(wxGridEvent& event)
{
	if(m_selectedAnim)
	{
		int frame = event.GetCol();
		TimelineTrack trackType = (TimelineTrack)event.GetRow();

		switch(trackType)
		{
			case eTrackPositionX:
			case eTrackPositionY:
			{
				//Resize position track to match sprite frame track
				int numExistingFrames = m_selectedAnim->m_trackPosition.GetNumKeyframes();
				for(int i = numExistingFrames; i < m_selectedAnim->m_trackSpriteFrame.GetNumKeyframes(); i++)
				{
					m_selectedAnim->m_trackPosition.InsertKeyframe(ion::render::Keyframe<ion::Vector2i>((float)i, ion::Vector2i(0, 0)));
				}

				//Set keyframe
				ion::render::Keyframe<ion::Vector2i>& keyframe = m_selectedAnim->m_trackPosition.GetKeyframe(frame);
				ion::Vector2i position = keyframe.GetValue();
				if(trackType == eTrackPositionX)
					event.GetString().ToLong((long*)&position.x, 10);
				else
					event.GetString().ToLong((long*)&position.y, 10);

				keyframe.SetValue(position);

				break;
			}

			case eTrackSFX:
			{
				//Resize SFX track to match sprite frame track
				int numExistingFrames = m_selectedAnim->m_trackSFX.GetNumKeyframes();
				for(int i = numExistingFrames; i < m_selectedAnim->m_trackSpriteFrame.GetNumKeyframes(); i++)
				{
					m_selectedAnim->m_trackSFX.InsertKeyframe(ion::render::Keyframe<std::string>((float)i, std::string()));
				}

				//Set keyframe
				ion::render::Keyframe<std::string>& keyframe = m_selectedAnim->m_trackSFX.GetKeyframe(frame);
				keyframe.SetValue(event.GetString().c_str().AsChar());

				break;
			}

			default:
				break;
		}
	}
}

void SpriteAnimEditorDialog::OnSliderMove(wxScrollEvent& event)
{
	if(m_selectedAnim && m_selectedAnim->GetState() == ion::render::Animation::eStopped)
	{
		float time = ion::maths::Lerp(0.0f, m_selectedAnim->GetLength(), (float)m_sliderTimeline->GetValue() / 100.0f);
		int frame = m_selectedAnim->m_trackSpriteFrame.GetValue(time);
		const ion::Vector2i& offset = m_selectedAnim->m_trackPosition.GetValue(time);

		m_selectedAnim->SetFrame(time);
		m_canvas->SetDrawSpriteSheet(m_selectedSpriteSheetId, frame, offset);
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

void SpriteAnimEditorDialog::OnRadioBlendLerp(wxCommandEvent& event)
{
	if(m_selectedAnim)
	{
		m_selectedAnim->m_trackPosition.SetBlendMode(AnimTrackSpritePosition::eLinear);
	}
}

void SpriteAnimEditorDialog::OnRadioBlendSnap(wxCommandEvent& event)
{
	if(m_selectedAnim)
	{
		m_selectedAnim->m_trackPosition.SetBlendMode(AnimTrackSpritePosition::eSnap);
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

void SpriteAnimEditorDialog::PopulateStampList()
{
	m_listActors->Clear();
	m_stampCache.clear();
	m_selectedStampId = InvalidStampId;
	m_selectedStamp = NULL;

	typedef std::pair<std::string, ActorId> TNameIDPair;
	typedef std::vector<TNameIDPair> TNameList;
	TNameList nameList;

	for(TStampMap::const_iterator it = m_project.StampsBegin(), end = m_project.StampsEnd(); it != end; ++it)
	{
		//Check if tiles are sorted sequentially
		if(it->second.CheckTilesBatched())
		{
			std::string name = it->second.GetName().size() ? it->second.GetName() : "<Unnamed>";
			nameList.push_back(std::make_pair(name, it->first));
		}
	}

	std::sort(nameList.begin(), nameList.end(), [](TNameIDPair& a, TNameIDPair& b) { return a.first < b.first; });

	for(int i = 0; i < nameList.size(); i++)
	{
		//Store by index
		m_stampCache.push_back(nameList[i].second);

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

void SpriteAnimEditorDialog::PopulateStampAnimSheetList(const Stamp& stamp)
{
	m_listSpriteSheets->Clear();
	m_spriteSheetCache.clear();
	m_selectedSpriteSheetId = InvalidSpriteSheetId;
	m_selectedSpriteSheet = NULL;

	typedef std::pair<std::string, SpriteSheetId> TNameIDPair;
	typedef std::vector<TNameIDPair> TNameList;
	TNameList nameList;

	for(TSpriteSheetMap::const_iterator it = stamp.StampAnimSheetsBegin(), end = stamp.StampAnimSheetsEnd(); it != end; ++it)
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

	//TEMP - for stamps
	if(!spriteResources && m_selectedStamp)
	{
		m_renderResources.CreateSpriteSheetResources(spriteSheetId, *m_selectedStamp->GetStampAnimSheet(spriteSheetId));
		spriteResources = m_renderResources.GetSpriteSheetResources(spriteSheetId);
	}

	ion::debug::Assert(spriteResources, "SpriteAnimEditorDialog::PopulateSpriteFrames() - No sprite resources");

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
	if(m_gridTimeline->GetNumberCols() > 0)
	{
		m_gridTimeline->DeleteCols(0, m_gridTimeline->GetNumberCols());
	}

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
		m_gridTimeline->SetRowSize(eTrackSpriteFrame, s_iconHeight + (s_iconBorderY * 2));
		m_gridTimeline->DisableRowResize(eTrackSpriteFrame);

		for(int i = 0; i < numKeyframes; i++)
		{
			//Setup column
			//Set all row content types to number
			m_gridTimeline->SetColFormatNumber(i);

			m_gridTimeline->SetCellRenderer(eTrackSFX, i, new wxGridCellAutoWrapStringRenderer);
			m_gridTimeline->SetCellEditor(eTrackSFX, i, new wxGridCellAutoWrapStringEditor);

			m_gridTimeline->SetColSize(i, iconWidth + (s_iconBorderX * 2));
			m_gridTimeline->DisableColResize(i);
			m_gridTimeline->SetCellRenderer(eTrackSpriteFrame, i, new GridCellBitmapRenderer(m_timelineImageList.get()));

			//Get sprite keyframe
			const int spriteFrame = anim.m_trackSpriteFrame.GetKeyframe(i).GetValue();
			 
			//Fetch texture, convert to wxImage
			const ion::render::Texture* texture = spriteResources->m_frames[spriteFrame].texture;
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

			//Get position
			if(i < anim.m_trackPosition.GetNumKeyframes())
			{
				ion::Vector2i position = anim.m_trackPosition.GetKeyframe(i).GetValue();
				wxString textX;
				wxString textY;
				textX << position.x;
				textY << position.y;
				m_gridTimeline->SetCellValue(textX, eTrackPositionX, i);
				m_gridTimeline->SetCellValue(textY, eTrackPositionY, i);
			}

			//Get SFX
			if(i < anim.m_trackSFX.GetNumKeyframes())
			{
				std::string sfxName = anim.m_trackSFX.GetKeyframe(i).GetValue();
				m_gridTimeline->SetCellValue(sfxName, eTrackSFX, i);
			}
		}

		//Set timeline height
		m_gridTimeline->SetMinSize(wxSize(m_gridTimeline->GetMinSize().x, s_iconHeight + (s_iconBorderY * 2) + m_scrollbarHeight));
		m_gridTimeline->SetMaxSize(wxSize(m_gridTimeline->GetMaxSize().x, s_iconHeight + (s_iconBorderY * 2) + m_scrollbarHeight));
		m_gridTimeline->GetContainingSizer()->Layout();
		m_gridTimeline->Refresh();
	}

	//Set blend mode radio buttons
	m_radioBlendLerp->SetValue(anim.m_trackPosition.GetBlendMode() == AnimTrackSpritePosition::eLinear);
	m_radioBlendLerp->SetValue(anim.m_trackPosition.GetBlendMode() == AnimTrackSpritePosition::eSnap);
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

void SpriteAnimEditorDialog::SelectStamp(int index)
{
	if(m_selectedAnim)
	{
		m_selectedAnim->SetState(ion::render::Animation::eStopped);
	}

	m_selectedStampId = InvalidStampId;
	m_selectedStamp = NULL;

	m_listSpriteSheets->Clear();
	m_listAnimations->Clear();

	if(m_stampCache.size() > 0)
	{
		if(index >= 0 && index < m_stampCache.size())
		{
			m_selectedStampId = m_stampCache[index];
			m_selectedStamp = m_project.GetStamp(m_selectedStampId);
			ion::debug::Assert(m_selectedStamp, "SpriteAnimEditorDialog::OnStampSelected() - Invalid stamp ID");
			PopulateStampAnimSheetList(*m_selectedStamp);
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

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	if((m_selectedActor || m_selectedStamp) && m_spriteSheetCache.size() > 0)
	{
		if(index >= 0 && index < m_spriteSheetCache.size())
		{
			m_selectedSpriteSheetId = m_spriteSheetCache[index];
			m_selectedSpriteSheet = m_selectedActor ? m_selectedActor->GetSpriteSheet(m_selectedSpriteSheetId) : m_selectedStamp->GetStampAnimSheet(m_selectedSpriteSheetId);
			ion::debug::Assert(m_selectedSpriteSheet, "SpriteAnimEditorDialog::OnSpriteSheetSelected() - Invalid spriteSheet ID");
			m_canvas->SetSpriteSheetDimentionsPixels(ion::Vector2i(m_selectedSpriteSheet->GetWidthTiles() * tileWidth, m_selectedSpriteSheet->GetHeightTiles() * tileHeight));
			m_canvas->SetDrawSpriteSheet(m_selectedSpriteSheetId, 0, ion::Vector2i());
			m_canvas->CreateGrid(m_selectedSpriteSheet->GetWidthTiles() * tileWidth, m_selectedSpriteSheet->GetHeightTiles() * tileHeight, m_selectedSpriteSheet->GetWidthTiles(), m_selectedSpriteSheet->GetHeightTiles());
			m_canvas->SetGridColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
			m_canvas->SetDrawGrid(true);

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
		const ion::Vector2i& offset = m_selectedAnim->m_trackPosition.GetValue(time);

		m_selectedAnim->Update(delta);
		m_canvas->SetDrawSpriteSheet(m_selectedSpriteSheetId, frame, offset);
		m_sliderTimeline->SetValue((int)ion::maths::Round(lerpTime * 100.0f));
		m_gridTimeline->GoToCell(0, frame);
	}
}

void SpriteAnimEditorDialog::EventHandlerDragFrameListBegin(wxGridEvent& event)
{
	m_draggingSpriteFrameItem = event.GetCol();

	wxImageList* imageList = m_spriteFrameImageList.get();
	if(imageList)
	{
		if(m_draggingSpriteFrameItem >= 0 && m_draggingSpriteFrameItem < imageList->GetImageCount())
		{
			//Create drag image
			m_dragImage = new wxDragImage(imageList->GetBitmap(m_draggingSpriteFrameItem));
			m_dragImage->BeginDrag(wxPoint(0, 0), m_gridSpriteFrames, m_gridTimeline);
			m_dragImage->Show();

			//Create target drop rect list
			m_dragDropKeyframeList.clear();

			if(m_selectedAnim)
			{
				//Backup all keyframes and list entry rects
				for(int i = 0; i < m_selectedAnim->m_trackSpriteFrame.GetNumKeyframes(); i++)
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
	TimelineTrack trackType = (TimelineTrack)event.GetRow();

	if(trackType == eTrackSpriteFrame)
	{
		wxImageList* imageList = m_timelineImageList.get();
		if(imageList)
		{
			if(m_selectedAnim && m_draggingTimelineItem >= 0 && m_draggingTimelineItem < imageList->GetImageCount())
			{
				//Create drag image
				m_dragImage = new wxDragImage(imageList->GetBitmap(m_draggingTimelineItem));
				m_dragImage->BeginDrag(wxPoint(0, 0), m_gridTimeline, m_gridTimeline);
				m_dragImage->Show();

				//Create target drop rect list
				m_dragDropKeyframeList.clear();

				for(int i = 0; i < m_selectedAnim->m_trackSpriteFrame.GetNumKeyframes(); i++)
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
			m_selectedAnim->SetLength(0.0f);

			for(int i = 0; i < keyframes.size(); i++)
			{
				m_selectedAnim->m_trackSpriteFrame.InsertKeyframe(AnimKeyframeSpriteFrame((float)i, keyframes[i]));
			}

			m_selectedAnim->SetLength((float)m_selectedAnim->m_trackSpriteFrame.GetNumKeyframes());

			//Re-populate keyframe view
			PopulateKeyframes(m_selectedSpriteSheetId, *m_selectedAnim);
		}
	}
}

void SpriteAnimEditorDialog::FindAndDrawDropTarget(wxPoint mousePosDropSource, wxPoint mousePosDropTarget)
{
	m_dragImage->Move(mousePosDropSource);

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

		if(m_dragDropKeyframeList.size() > 0)
		{
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
		}
		else
		{
			//No frames in list, there's only one drop target
			m_dragDropTarget = 0;
		}

		if(m_dragDropTarget != m_dragDropTargetPrev)
		{
			//De-select previous cell
			if(m_dragDropTargetPrev >= 0)
			{
				int drawIndex = (m_dragDropKeyframeList.size() > 0 && m_dragDropTargetPrev == m_dragDropKeyframeList.size()) ? m_dragDropTargetPrev - 1 : m_dragDropTargetPrev;
				GridCellBitmapRenderer* cellRenderer = (GridCellBitmapRenderer*)m_gridTimeline->GetCellRenderer(0, drawIndex);
				cellRenderer->SetDrawSelectionEdge(GridCellBitmapRenderer::eDrawSelectionEdgeNone);

				wxRect refreshRect = (m_dragDropKeyframeList.size() > 0) ? m_dragDropKeyframeList[drawIndex].second : m_gridTimeline->GetRect();
				refreshRect.Offset(-scrollOffset);
				m_gridTimeline->Refresh(true, &refreshRect);
			}

			//Set selected cell
			if(m_dragDropTarget >= 0)
			{
				if(m_dragDropTarget == 0 && m_dragDropKeyframeList.size() == 0)
				{
					//No existing cells, use far left of timeline
					GridCellBitmapRenderer* cellRenderer = (GridCellBitmapRenderer*)m_gridTimeline->GetCellRenderer(0, 0);
					cellRenderer->SetDrawSelectionEdge(GridCellBitmapRenderer::eDrawSelectionEdgeLeft);

					wxRect refreshRect = m_gridTimeline->GetRect();
					refreshRect.Offset(-scrollOffset);
					m_gridTimeline->Refresh(true, &refreshRect);
				}
				else if(m_dragDropTarget == m_dragDropKeyframeList.size())
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
