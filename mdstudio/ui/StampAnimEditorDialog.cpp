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

#include "StampAnimEditorDialog.h"
#include "SpriteCanvas.h"
#include "Dialogs.h"
#include "MainWindow.h"

#include <wx/msgdlg.h>
#include <wx/imaglist.h>
#include <wx/dc.h>
#include <wx/menu.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/wglext.h>

StampAnimEditorDialog::StampAnimEditorDialog(MainWindow& mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources)
	: StampAnimEditorDialogBase(&mainWindow)
	, m_project(project)
	, m_renderer(renderer)
	, m_renderResources(renderResources)
	, m_glContext(glContext)
	, m_timer(this)
	, m_mainWindow(mainWindow)
{
	m_selectedAnimId = InvalidStampAnimId;
	m_selectedAnim = NULL;

	m_draggingTimelineItem = -1;
	m_dragImage = NULL;

	m_canvas->SetProject(&project);
	m_canvas->SetupRendering(&renderer, &glContext, &renderResources);

	PopulateAnimList();

	//Subscribe to events
	Bind(wxEVT_TIMER, &StampAnimEditorDialog::EventHandlerTimer, this, m_timer.GetId());
	m_gridTimeline->Bind(wxEVT_GRID_CELL_BEGIN_DRAG, &StampAnimEditorDialog::EventHandlerDragTimelineBegin, this, m_gridTimeline->GetId());
	m_gridTimeline->Bind(wxEVT_MOTION, &StampAnimEditorDialog::EventHandlerDragTimelineMove, this, m_gridTimeline->GetId());
	m_gridTimeline->Bind(wxEVT_LEFT_UP, &StampAnimEditorDialog::EventHandlerDragTimelineEnd, this, m_gridTimeline->GetId());
	m_gridTimeline->Bind(wxEVT_GRID_CELL_RIGHT_CLICK, &StampAnimEditorDialog::EventHandlerTimelineRightClick, this, m_gridTimeline->GetId());
	m_gridTimeline->Bind(wxEVT_GRID_CELL_CHANGING, &StampAnimEditorDialog::OnTimelineCellChange, this, m_gridTimeline->GetId());

	//Get system scrollbar height
	m_scrollbarHeight = wxSystemSettings::GetMetric(wxSYS_HSCROLL_Y, m_gridTimeline);

	//Default timeline size
	m_gridTimeline->SetMinSize(wxSize(m_gridTimeline->GetMinSize().x, s_iconHeight + (s_iconBorderY * 2) + m_scrollbarHeight));
	m_gridTimeline->SetMaxSize(wxSize(m_gridTimeline->GetMaxSize().x, s_iconHeight + (s_iconBorderY * 2) + m_scrollbarHeight));

	m_gridTimeline->DisableDragColMove();
	m_gridTimeline->DisableDragColSize();
	m_gridTimeline->DisableDragGridSize();
	m_gridTimeline->DisableDragRowSize();
	m_gridTimeline->EnableDragCell(true);

	//Append 1 row per keyframe track
	m_gridTimeline->AppendRows(1);

	Maximize();
}

StampAnimEditorDialog::~StampAnimEditorDialog()
{

}

void StampAnimEditorDialog::OnAnimSelected(wxCommandEvent& event)
{
	SelectAnimation(event.GetSelection());
}

void StampAnimEditorDialog::OnBtnAnimDelete(wxCommandEvent& event)
{
	if(m_selectedAnimId != InvalidStampAnimId)
	{
		m_project.DeleteStampAnimation(m_selectedAnimId);
		PopulateAnimList();
		m_selectedAnimId = InvalidStampAnimId;
		m_selectedAnim = NULL;
		Refresh();
	}
}

void StampAnimEditorDialog::OnTimelineCellChange(wxGridEvent& event)
{

}

void StampAnimEditorDialog::OnSliderMove(wxScrollEvent& event)
{
	if(m_selectedAnim && m_selectedAnim->GetState() == ion::render::Animation::eStopped)
	{
		float time = ion::maths::Lerp(0.0f, m_selectedAnim->GetLength(), (float)m_sliderTimeline->GetValue() / 100.0f);
		int frame = m_selectedAnim->m_trackTileFrame.GetValue(time);

		m_selectedAnim->SetFrame(time);
		m_canvas->SetDrawTileFrame(m_selectedAnim->GetTileFrame(frame));
		ApplyAnimFrame(frame);
		m_gridTimeline->GoToCell(0, frame);
	}
}

void StampAnimEditorDialog::OnBtnPlay(wxCommandEvent& event)
{
	if(m_selectedAnim)
	{
		m_selectedAnim->SetState(ion::render::Animation::ePlaying);
		m_timer.Start(1);
	}
}

void StampAnimEditorDialog::OnBtnStop(wxCommandEvent& event)
{
	m_timer.Stop();

	if(m_selectedAnim)
	{
		m_selectedAnim->SetState(ion::render::Animation::eStopped);
	}
}

void StampAnimEditorDialog::OnSpinSpeedChange(wxSpinEvent& event)
{
	if(m_selectedAnim)
	{
		m_selectedAnim->SetSpeed(event.GetValue());
	}
}

void StampAnimEditorDialog::SetAnimation(StampAnimId animId)
{
	m_selectedAnimId = animId;
	m_selectedAnim = m_project.GetStampAnimation(animId);
}

void StampAnimEditorDialog::PopulateAnimList()
{
	m_listAnimations->Clear();
	m_animCache.clear();
	m_selectedAnimId = InvalidStampAnimId;
	m_selectedAnim = NULL;

	typedef std::pair<std::string, StampAnimId> TNameIDPair;
	typedef std::vector<TNameIDPair> TNameList;
	TNameList nameList;

	for(TStampAnimationMap::const_iterator it = m_project.StampAnimationsBegin(), end = m_project.StampAnimationsEnd(); it != end; ++it)
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

void StampAnimEditorDialog::PopulateKeyframes(const StampAnimation& anim)
{
	//Get sprite resources
	const RenderResources::SpriteSheetRenderResources* spriteResources = NULL; // m_renderResources.GetSpriteSheetResources(spriteSheetId);

	//Clear existing
	if(m_gridTimeline->GetNumberCols() > 0)
	{
		m_gridTimeline->DeleteCols(0, m_gridTimeline->GetNumberCols());
	}

	const u32 numKeyframes = anim.m_trackTileFrame.GetNumKeyframes();

	if(numKeyframes > 0)
	{
		int iconWidth = s_iconHeight;

		if(spriteResources && spriteResources->m_frames.size() > 0)
		{
			//Create new image list based on size of first texture
			const ion::render::Texture* texture0 = spriteResources->m_frames[0].texture;

			const float iconAspect = (float)texture0->GetWidth() / (float)texture0->GetHeight();
			iconWidth = s_iconHeight * iconAspect;
		}

		m_timelineImageList = new wxImageList(iconWidth, s_iconHeight, numKeyframes);

		//Create timeline cells
		m_gridTimeline->AppendCols(numKeyframes);

		//Reset timeline row height
		m_gridTimeline->SetRowSize(0, s_iconHeight + (s_iconBorderY * 2));
		m_gridTimeline->DisableRowResize(0);

		for(int i = 0; i < numKeyframes; i++)
		{
			//Setup column
			//Set all row content types to number
			m_gridTimeline->SetColFormatNumber(i);
			m_gridTimeline->SetColSize(i, iconWidth + (s_iconBorderX * 2));
			m_gridTimeline->DisableColResize(i);

			if(spriteResources && spriteResources->m_frames.size() > 0)
			{
				m_gridTimeline->SetCellRenderer(0, i, new GridCellBitmapRenderer(m_timelineImageList.get()));

				//Get sprite keyframe
				const int spriteFrame = anim.m_trackTileFrame.GetKeyframe(i).GetValue();

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
			}
		}

		//Set timeline height
		m_gridTimeline->SetMinSize(wxSize(m_gridTimeline->GetMinSize().x, s_iconHeight + (s_iconBorderY * 2) + m_scrollbarHeight));
		m_gridTimeline->SetMaxSize(wxSize(m_gridTimeline->GetMaxSize().x, s_iconHeight + (s_iconBorderY * 2) + m_scrollbarHeight));
		m_gridTimeline->GetContainingSizer()->Layout();
		m_gridTimeline->Refresh();
	}
}

void StampAnimEditorDialog::SelectAnimation(int index)
{
	if(m_selectedAnim)
	{
		m_selectedAnim->SetState(ion::render::Animation::eStopped);
	}

	m_selectedAnimId = InvalidStampAnimId;
	m_selectedAnim = NULL;

	if(m_animCache.size() > 0)
	{
		if(index >= 0 && index < m_animCache.size())
		{
			m_selectedAnimId = m_animCache[index];
			m_selectedAnim = m_project.GetStampAnimation(m_selectedAnimId);

			PopulateKeyframes(*m_selectedAnim);

			m_spinCtrlSpeed->SetValue(m_selectedAnim->GetSpeed());
		}
	}
}

void StampAnimEditorDialog::ApplyAnimFrame(int frame)
{
	if(m_selectedAnim)
	{
		Tileset& tileset = m_project.GetTileset();

		const TileFrame& tileFrame = m_selectedAnim->GetTileFrame(frame);
		for(int i = 0; i < tileFrame.m_tiles.size(); i++)
		{
			TileId tileId = tileFrame.m_tiles[i].first;
			if(Tile* tile = tileset.GetTile(tileId))
			{
				for(int x = 0; x < tile->GetWidth(); x++)
				{
					for(int y = 0; y < tile->GetHeight(); y++)
					{
						int colourIdx = tileFrame.m_tiles[i].second.GetPixelColour(x, y);
						tile->SetPixelColour(x, y, colourIdx);
						m_renderResources.SetTilesetTexPixel(tileId, ion::Vector2i(x, y), colourIdx);
					}
				}
			}
		}

		//Refresh panel
		Refresh();

		//Refresh tiles, map and stamps panels
		m_mainWindow.RedrawPanel(MainWindow::ePanelTiles);
		m_mainWindow.RedrawPanel(MainWindow::ePanelMap);
		m_mainWindow.RedrawPanel(MainWindow::ePanelStamps);
	}
}

void StampAnimEditorDialog::EventHandlerTimer(wxTimerEvent& event)
{
	if(m_selectedAnim)
	{
		float frameRate = 24.0f;
		float frameRateMul = 1.0f / (frameRate / 10.0f);
		float delta = (float)event.GetInterval() * ((float)m_spinCtrlSpeed->GetValue() / 100.0f) * frameRateMul;
		float time = m_selectedAnim->GetFrame();
		float lerpTime = ion::maths::UnLerp(0.0f, m_selectedAnim->GetLength(), time);
		int frame = m_selectedAnim->m_trackTileFrame.GetValue(time);

		m_selectedAnim->Update(delta);
		m_canvas->SetDrawTileFrame(m_selectedAnim->GetTileFrame(frame));
		ApplyAnimFrame(frame);
		m_sliderTimeline->SetValue((int)ion::maths::Round(lerpTime * 100.0f));
		m_gridTimeline->GoToCell(0, frame);
	}
}

void StampAnimEditorDialog::EventHandlerDragTimelineBegin(wxGridEvent& event)
{
	m_draggingTimelineItem = event.GetCol();

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

			for(int i = 0; i < m_selectedAnim->m_trackTileFrame.GetNumKeyframes(); i++)
			{
				wxRect rect = m_gridTimeline->CellToRect(0, i);

				u32 frame = m_selectedAnim->m_trackTileFrame.GetKeyframe(i).GetValue();

				m_dragDropKeyframeList.push_back(std::make_pair(frame, rect));
			}

			m_dragDropTarget = -1;
			m_dragDropTargetPrev = -1;
		}
	}
}

void StampAnimEditorDialog::EventHandlerDragTimelineMove(wxMouseEvent& event)
{
	if(m_dragImage && m_draggingTimelineItem >= 0)
	{
		wxPoint position = event.GetPosition();

		m_dragImage->Move(position);

		FindAndDrawDropTarget(position, position);
	}
}

void StampAnimEditorDialog::EventHandlerDragTimelineEnd(wxMouseEvent& event)
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
			m_selectedAnim->m_trackTileFrame.Clear();

			for(int i = 0; i < m_dragDropKeyframeList.size(); i++)
			{
				m_selectedAnim->m_trackTileFrame.InsertKeyframe(AnimKeyframeSpriteFrame((float)i, m_dragDropKeyframeList[i].first));
			}

			//Set new anim length
			m_selectedAnim->SetLength((float)m_dragDropKeyframeList.size());

			//Re-populate keyframe timeline
			PopulateKeyframes(*m_selectedAnim);
		}

		m_draggingTimelineItem = -1;
		m_dragDropKeyframeList.clear();
	}
}

void StampAnimEditorDialog::EventHandlerTimelineRightClick(wxGridEvent& event)
{
	if(m_selectedAnim)
	{
		//Right-click menu
		m_contextMenuKeyframeIndex = event.GetCol();

		wxMenu contextMenu;
		contextMenu.Append(eMenuAddKeyframe, wxString("Insert keyframe"));
		contextMenu.Append(eMenuDeleteKeyframe, wxString("Delete keyframe"));
		contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&StampAnimEditorDialog::EventHandlerContextMenuClick, NULL, this);
		PopupMenu(&contextMenu);
	}
}

void StampAnimEditorDialog::EventHandlerContextMenuClick(wxCommandEvent& event)
{
	if(m_selectedAnim)
	{
		if(event.GetId() == eMenuAddKeyframe)
		{
			const int numExistingFrames = m_selectedAnim->m_trackTileFrame.GetNumKeyframes();

			//Create new tile frame using prev as template
			TileFrame tileFrame(m_selectedAnim->GetTileFrame(numExistingFrames - 1));

			//Import tile frame
			wxFileDialog dialog(this, _("Open BMP file"), "", "", "BMP files (*.bmp)|*.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
			if(dialog.ShowModal() == wxID_OK)
			{
				if(tileFrame.Import(dialog.GetPath().c_str().AsChar(), m_project))
				{
					//Insert tile frame
					m_selectedAnim->AddTileFrame(tileFrame);

					//Add new keyframe
					m_selectedAnim->m_trackTileFrame.InsertKeyframe(AnimKeyframeSpriteFrame((float)numExistingFrames, numExistingFrames));

					//Re-populate keyframe view
					PopulateKeyframes(*m_selectedAnim);
				}
			}
		}
		else if(event.GetId() == eMenuDeleteKeyframe)
		{
			//Rebuild animation
			std::vector<u32> keyframes;

			for(int i = 0; i < m_selectedAnim->m_trackTileFrame.GetNumKeyframes(); i++)
			{
				if(i != m_contextMenuKeyframeIndex)
				{
					keyframes.push_back(m_selectedAnim->m_trackTileFrame.GetKeyframe(i).GetValue());
				}
			}

			m_selectedAnim->m_trackTileFrame.Clear();

			for(int i = 0; i < keyframes.size(); i++)
			{
				m_selectedAnim->m_trackTileFrame.InsertKeyframe(AnimKeyframeSpriteFrame((float)i, keyframes[i]));
			}

			//Re-populate keyframe view
			PopulateKeyframes(*m_selectedAnim);
		}
	}
}

void StampAnimEditorDialog::FindAndDrawDropTarget(wxPoint mousePosDropSource, wxPoint mousePosDropTarget)
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
