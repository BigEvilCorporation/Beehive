///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "SpriteAnimEditorDialog.h"
#include "SpriteCanvas.h"
#include "Dialogs.h"

#include <wx/msgdlg.h>

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

	m_canvas->SetupRendering(&renderer, &glContext, &renderResources);

	PopulateActorList();

	//Subscribe to events
	Bind(wxEVT_TIMER, &SpriteAnimEditorDialog::EventHandlerTimer, this, m_timer.GetId());
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
		ImportDialogSpriteSheet dialog(this, m_renderer, m_glContext, m_renderResources);
		if(dialog.ShowModal() == wxID_OK)
		{
			//Create new spriteSheet
			SpriteSheetId spriteSheetId = m_selectedActor->CreateSpriteSheet();
			SpriteSheet* spriteSheet = m_selectedActor->GetSpriteSheet(spriteSheetId);

			//Import bitmap
			if(spriteSheet->ImportBitmap(dialog.m_filePicker->GetPath().GetData().AsChar(), dialog.m_textName->GetValue().GetData().AsChar(), dialog.m_spinWidthCells->GetValue(), dialog.m_spinHeightCells->GetValue(), dialog.m_spinCellCount->GetValue()))
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

void SpriteAnimEditorDialog::OnButtonSpriteSheetDelete(wxCommandEvent& event)
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
		DialogNewAnim dialog(this);
		dialog.m_textName->SetValue(m_selectedSpriteSheet->GetName());

		if(dialog.ShowModal() == wxID_OK)
		{
			SpriteAnimId animId = m_selectedSpriteSheet->AddAnimation();
			SpriteAnimation* anim = m_selectedSpriteSheet->GetAnimation(animId);

			anim->SetName(dialog.m_textName->GetValue().GetData().AsChar());

			if(dialog.m_chkAutoGenerate->GetValue())
			{
				for(int i = 0; i < m_selectedSpriteSheet->GetNumFrames(); i++)
				{
					anim->m_trackSpriteFrame.AddKeyframe(AnimKeyframeSpriteFrame((float)i, i));
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

void SpriteAnimEditorDialog::OnSliderMove(wxScrollEvent& event)
{
	if(m_selectedAnim && m_selectedAnim->GetState() == ion::render::Animation::Stopped)
	{
		float time = ion::maths::Lerp(0.0f, m_selectedAnim->GetLength(), (float)m_sliderTimeline->GetValue() / 100.0f);
		int frame = m_selectedAnim->m_trackSpriteFrame.GetValue(time);

		m_selectedAnim->SetFrame(time);
		m_canvas->SetDrawSpriteSheet(m_selectedSpriteSheetId, frame);
	}
}

void SpriteAnimEditorDialog::OnBtnPlay(wxCommandEvent& event)
{
	if(m_selectedAnim)
	{
		m_selectedAnim->SetState(ion::render::Animation::Playing);
		m_timer.Start(1);
	}
}

void SpriteAnimEditorDialog::OnBtnStop(wxCommandEvent& event)
{
	m_timer.Stop();

	if(m_selectedAnim)
	{
		m_selectedAnim->SetState(ion::render::Animation::Stopped);
	}
}

void SpriteAnimEditorDialog::OnSpinSpeedChange(wxSpinEvent& event)
{

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

void SpriteAnimEditorDialog::SelectActor(int index)
{
	if(m_selectedAnim)
	{
		m_selectedAnim->SetState(ion::render::Animation::Stopped);
	}

	m_selectedActorId = InvalidActorId;
	m_selectedActor = NULL;

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
		m_selectedAnim->SetState(ion::render::Animation::Stopped);
	}

	m_selectedSpriteSheetId = InvalidSpriteSheetId;
	m_selectedSpriteSheet = NULL;

	if(m_selectedActor && m_spriteSheetCache.size() > 0)
	{
		if(index >= 0 && index < m_spriteSheetCache.size())
		{
			m_selectedSpriteSheetId = m_spriteSheetCache[index];
			m_selectedSpriteSheet = m_selectedActor->GetSpriteSheet(m_selectedSpriteSheetId);
			ion::debug::Assert(m_selectedSpriteSheet, "SpriteAnimEditorDialog::OnSpriteSheetSelected() - Invalid spriteSheet ID");
			m_canvas->SetSpriteSheetDimentionsPixels(ion::Vector2i(m_selectedSpriteSheet->GetWidthTiles() * 8, m_selectedSpriteSheet->GetHeightTiles() * 8));
			m_canvas->SetDrawSpriteSheet(m_selectedSpriteSheetId, 0);

			PopulateAnimList(*m_selectedSpriteSheet);
		}
	}
}

void SpriteAnimEditorDialog::SelectAnimation(int index)
{
	if(m_selectedAnim)
	{
		m_selectedAnim->SetState(ion::render::Animation::Stopped);
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
			}
		}
	}
}

void SpriteAnimEditorDialog::EventHandlerTimer(wxTimerEvent& event)
{
	if(m_selectedAnim)
	{
		float delta = (float)event.GetInterval() * ((float)m_spinCtrlSpeed->GetValue() / 100.0f);
		float time = m_selectedAnim->GetFrame();
		float lerpTime = ion::maths::UnLerp(0.0f, m_selectedAnim->GetLength(), time);
		int frame = m_selectedAnim->m_trackSpriteFrame.GetValue(time);

		m_selectedAnim->Update(delta);
		m_canvas->SetDrawSpriteSheet(m_selectedSpriteSheetId, frame);
		m_sliderTimeline->SetValue((int)ion::maths::Round(lerpTime * 100.0f));
	}
}