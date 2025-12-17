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

#include "StampsPanel.h"
#include "MainWindow.h"
#include "SpriteAnimEditorDialog.h"
#include "UpdateStampDialog.h"
#include "EditStampCollisionDialog.h"

#include <wx/menu.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>

#include <ion/core/utils/STL.h>

StampsPanel::StampsPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, wxGLAttributes& glAttributes, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, project, renderer, glContext, glAttributes, renderResources, parent, winid, pos, size, style, name)
{
	m_selectedStamp = InvalidStampId;
	m_hoverStamp = InvalidStampId;
	m_stampToSubstitute = InvalidStampId;

	m_mode = eModeSelect;

	//Custom zoom/pan handling
	EnableZoom(false);
	EnablePan(false);

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	//Create selection quad
	m_selectionPrimitive = new ion::render::Quad(ion::render::Quad::Axis::xy, ion::Vector2(tileWidth / 2.0f, tileHeight / 2.0f));
}

StampsPanel::~StampsPanel()
{

}

void StampsPanel::OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta)
{
	ViewPanel::OnMouse(event, mouseDelta);

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	//Camera pan Y (if canvas is taller than panel)
	if((m_canvasSize.y * tileHeight) > (m_panelSize.y / m_cameraZoom))
	{
		float panDeltaY = 0.0f;

		if(event.Dragging() && event.ButtonIsDown(wxMOUSE_BTN_MIDDLE))
		{
			panDeltaY = mouseDelta.y;
		}
		else if(event.GetWheelRotation() != 0)
		{
			panDeltaY = (float)event.GetWheelRotation();
		}

		if(panDeltaY != 0.0f)
		{
			ion::Vector3 cameraPos = m_camera.GetPosition();
			cameraPos.y += panDeltaY * m_cameraPanSpeed / m_cameraZoom;

			//Clamp to size
			float halfCanvas = (m_canvasSize.y * (tileHeight / 2.0f));
			float minY = -halfCanvas;
			float maxY = halfCanvas - ((float)m_panelSize.y / m_cameraZoom);

			if(cameraPos.y > maxY)
				cameraPos.y = maxY;
			else if(cameraPos.y < minY)
				cameraPos.y = minY;

			m_camera.SetPosition(cameraPos);

			Refresh();
		}
	}
}

void StampsPanel::OnKeyboard(wxKeyEvent& event)
{
	ViewPanel::OnKeyboard(event);
}

void StampsPanel::OnResize(wxSizeEvent& event)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		if (event.GetSize().x != m_panelSize.x || event.GetSize().y != m_panelSize.y)
		{
			const int tileWidth = m_project.GetPlatformConfig().tileWidth;
			const int tileHeight = m_project.GetPlatformConfig().tileHeight;

			ViewPanel::OnResize(event);

			//Rearrange stamps (calculates canvas size)
			ArrangeStamps(ion::Vector2(m_panelSize.x, m_panelSize.y));

			//Recreate canvas
			CreateCanvas(m_canvasSize.x, m_canvasSize.y);

			//Fill with invalid tile
			FillTiles(InvalidTileId, ion::Vector2i(0, 0), ion::Vector2i(m_canvasSize.x - 1, m_canvasSize.y - 1));

			//Redraw stamps on canvas
			PaintStamps();

			//Recreate grid
			CreateGrid(m_canvasSize.x, m_canvasSize.y, m_canvasSize.x / m_project.GetGridSize(), m_canvasSize.y / m_project.GetGridSize());

			//Reset zoom/pan
			ResetZoomPan();

			Refresh();
		}
	}
}

void StampsPanel::OnMouseTileEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int x, int y)
{
	ViewPanel::OnMouseTileEvent(mousePos, mouseDelta, tileDelta, buttonBits, x, y);

	StampId selectedStamp = InvalidStampId;
	ion::Vector2i stampTopLeft;

	//If in range, get stamp under mouse cursor
	if(x >= 0 && y >= 0 && x < m_canvasSize.x && y < m_canvasSize.y)
	{
		//TODO: Per-tile stamp map
		//Brute force search for stamp ID
		ion::Vector2i mousePos(x, y);

		for(int i = 0; i < m_stampPosMap.size() && !selectedStamp; i++)
		{
			StampId stampId = m_stampPosMap[i].first;
			if(Stamp* stamp = m_project.GetStamp(stampId))
			{
				stampTopLeft = m_stampPosMap[i].second;
				const ion::Vector2i& stampBottomRight = stampTopLeft + ion::Vector2i(stamp->GetWidth(), stamp->GetHeight());
				if(mousePos.x >= stampTopLeft.x && mousePos.y >= stampTopLeft.y
					&& mousePos.x < stampBottomRight.x && mousePos.y < stampBottomRight.y)
				{
					selectedStamp = stampId;
				}
			}
		}
	}

	//Set mouse hover stamp
	m_hoverStamp = selectedStamp;
	m_hoverStampPos = stampTopLeft;

	if (buttonBits == 0)
	{
		const int tileWidth = m_project.GetPlatformConfig().tileWidth;
		const int tileHeight = m_project.GetPlatformConfig().tileHeight;

		if (const Stamp* stamp = m_project.GetStamp(selectedStamp))
		{
			std::stringstream tipStr;
			tipStr << "Stamp " << stamp->GetName() << std::endl;
			tipStr << "Index 0x" << SSTREAM_HEX4(selectedStamp) << " (" << selectedStamp << ")" << std::endl;
			tipStr << "Size: " << stamp->GetWidth() << ", " << stamp->GetHeight() << std::endl;
			tipStr << "Addr: 0x" << SSTREAM_HEX8(selectedStamp * tileWidth * tileHeight * 2) << std::endl;
			SetToolTip(tipStr.str().c_str());
		}
		else
		{
			UnsetToolTip();
		}
	}

	if((buttonBits & eMouseLeft) && !(m_prevMouseBits & eMouseLeft))
	{
		//Left click, set current stamp
		m_selectedStamp = selectedStamp;
		m_selectedStampPos = stampTopLeft;

		if(m_mode == eModeSelect)
		{
			//Set as current painting stamp
			m_project.SetPaintStamp(selectedStamp);

			//Set stamp paint tool
			m_mainWindow->SetMapTool(eToolPaintStamp);
		}
		else if(m_mode == eModeSubstitute)
		{
			const Stamp* stampA = m_project.GetStamp(m_stampToSubstitute);
			const Stamp* stampB = m_project.GetStamp(m_selectedStamp);

			if(stampA && stampB && stampA != stampB)
			{
				if(stampA->GetWidth() == stampB->GetWidth() && stampA->GetHeight() == stampB->GetHeight())
				{
					//Substitute stamp
					m_project.SubstituteStamp(m_stampToSubstitute, m_selectedStamp);

					//Delete stamp
					m_project.DeleteStamp(m_stampToSubstitute);

					//Redraw stamps and map panels
					m_project.InvalidateStamps(true);
					m_mainWindow->RefreshPanel(MainWindow::ePanelStamps);
					m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
					m_project.InvalidateStamps(false);
				}
				else
				{
					wxMessageBox("Substitute stamp's width/height does not match the original", "Error", wxOK);
				}
			}

			m_stampToSubstitute = InvalidStampId;
			m_mode = eModeSelect;
		}
	}

	if(buttonBits & eMouseRight)
	{
		//if(m_hoverStamp != InvalidStampId)
		{
			//Right-click menu
			wxMenu contextMenu;

			contextMenu.Append(eMenuRenameStamp, wxString("Rename stamp"));
			contextMenu.Append(eMenuUpdateStamp, wxString("Update stamp"));
#if !BEEHIVE_LEAN_UI
			contextMenu.Append(eMenuUpdatePalette, wxString("Update palette"));
			contextMenu.Append(eMenuSubstituteStamp, wxString("Substitute stamp"));
#endif
			contextMenu.Append(eMenuDeleteStamp, wxString("Delete stamp"));
			contextMenu.Append(eMenuEditCollision, wxString("Edit collision"));
			contextMenu.Append(eMenuSetBackground, wxString("Set as background stamp"));
#if !BEEHIVE_LEAN_UI
			contextMenu.Append(eMenuSortTilesSequentially, wxString("Sort tiles sequentially"));
			contextMenu.Append(eMenuOpenInAnimEditor, wxString("Open in animation editor"));
#endif
#if !BEEHIVE_FIXED_STAMP_MODE
			contextMenu.Append(eMenuSetStampLowDrawPrio, wxString("Set stamp low draw priority"));
			contextMenu.Append(eMenuSetStampHighDrawPrio, wxString("Set stamp high draw priority"));
#endif
			contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&StampsPanel::OnContextMenuClick, NULL, this);
			PopupMenu(&contextMenu);
		}
	}

	//Redraw
	Refresh();
}

void StampsPanel::OnContextMenuClick(wxCommandEvent& event)
{
	if(event.GetId() == eMenuRenameStamp)
	{
		Stamp* stamp = m_project.GetStamp(m_hoverStamp);
		if(stamp)
		{
			wxTextEntryDialog dialog(m_mainWindow, "Rename Stamp");
			if(dialog.ShowModal() == wxID_OK)
			{
				stamp->SetName(dialog.GetValue().c_str().AsChar());
			}
		}
	}
	else if(event.GetId() == eMenuUpdateStamp)
	{
		Stamp* stamp = m_project.GetStamp(m_hoverStamp);
		if(stamp)
		{
			DialogUpdateStamp dialog(this, *stamp, m_project, m_renderer, *m_glContext, m_renderResources);
			if(dialog.ShowModal() == wxID_OK)
			{
				m_mainWindow->RefreshAll();
			}
		}
	}
#if !BEEHIVE_LEAN_UI
	else if(event.GetId() == eMenuUpdatePalette)
	{
		Stamp* stamp = m_project.GetStamp(m_hoverStamp);
		if(stamp)
		{
			wxFileDialog dialog(this, _("Open BMP file"), "", "", "PNG files (*.png)|*.png|BMP files (*.bmp)|*.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
			if(dialog.ShowModal() == wxID_OK)
			{
				std::string filename = dialog.GetPath().c_str().AsChar();
				std::string referenceFile = m_project.m_settings.Get("referenceFile");
				ion::ImageFormat* reader = ion::ImageFormat::CreateReader(ion::string::GetFileExtension(referenceFile));
				if(reader && reader->Read(filename))
				{
					const int tileWidth = m_project.GetPlatformConfig().tileWidth;
					const int tileHeight = m_project.GetPlatformConfig().tileHeight;

					if(reader->GetWidth() != (stamp->GetWidth() * tileWidth) || (reader->GetHeight() != stamp->GetHeight() * tileHeight))
					{
						ion::debug::log << "Bitmap width / height " << reader->GetWidth() << "x" << reader->GetHeight() << " does not match original stamp: " << filename << ion::debug::end;
						return;
					}

					//Get original palette
					int paletteIdx = 0;
					TileId firstTile = stamp->GetTile(0, 0);
					if(Tile* tile = m_project.GetTileset().GetTile(firstTile))
					{
						paletteIdx = (int)tile->GetPaletteId();
					}

					if(Palette* originalPalette = m_project.GetPalette(paletteIdx))
					{
						//New palette
						Palette newPalette;

						for(int x = 0; x < reader->GetWidth(); x++)
						{
							for(int y = 0; y < reader->GetHeight(); y++)
							{
								//Get original tile
								TileId tileId = stamp->GetTile(x / tileWidth, y / tileHeight);

								if(const Tile* tile = m_project.GetTileset().GetTile(tileId))
								{
									//Get colour idx of old pixel
									u8 originalIdx = tile->GetPixelColour(x % tileWidth, y % tileHeight);

									//Get colour of new pixel
									ion::ImageFormat::Colour colour = reader->GetPixel(x, y);

									//Replace colour in original palette
									newPalette.SetColour(originalIdx, Colour(colour.GetRed(), colour.GetGreen(), colour.GetBlue()));
								}
							}
						}

						//Replace palette
						*originalPalette = newPalette;

						//Refresh
						m_mainWindow->RefreshAll();
					}

					delete reader;
				}
			}
		}
	}
	else if(event.GetId() == eMenuSubstituteStamp)
	{
		Stamp* stamp = m_project.GetStamp(m_hoverStamp);
		if(stamp)
		{
			m_stampToSubstitute = m_hoverStamp;
			m_mode = eModeSubstitute;
		}
	}
#endif
	else if(event.GetId() == eMenuDeleteStamp)
	{
		//Delete stamp
		m_project.DeleteStamp(m_hoverStamp);

		//Invalidate hover/selected stamp
		m_hoverStamp = InvalidStampId;
		m_selectedStamp = InvalidStampId;

		//Refresh
		m_mainWindow->RefreshAll();
	}
	else if (event.GetId() == eMenuEditCollision)
	{
		//Show collision editor dialog
		if (Stamp* stamp = m_project.GetStamp(m_hoverStamp))
		{
			DialogEditStampCollision dialog(*m_mainWindow, *stamp, m_project, m_renderer, *m_glContext, m_renderResources);
			dialog.ShowModal();
		}
	}
	else if (event.GetId() == eMenuSetBackground)
	{
		m_project.SetBackgroundStamp(m_hoverStamp);
	}
#if !BEEHIVE_FIXED_STAMP_MODE
	else if(event.GetId() == eMenuSetStampLowDrawPrio)
	{
		Stamp* stamp = m_project.GetStamp(m_hoverStamp);
		if(stamp)
		{
			for(int x = 0; x < stamp->GetWidth(); x++)
			{
				for(int y = 0; y < stamp->GetHeight(); y++)
				{
					stamp->SetTileFlags(x, y, stamp->GetTileFlags(x, y) & ~Map::eHighPlane);
				}
			}
		}
	}
	else if(event.GetId() == eMenuSetStampHighDrawPrio)
	{
		Stamp* stamp = m_project.GetStamp(m_hoverStamp);
		if(stamp)
		{
			for(int x = 0; x < stamp->GetWidth(); x++)
			{
				for(int y = 0; y < stamp->GetHeight(); y++)
				{
					stamp->SetTileFlags(x, y, stamp->GetTileFlags(x, y) | Map::eHighPlane);
				}
			}
		}
	}
#endif
#if !BEEHIVE_LEAN_UI
	else if(event.GetId() == eMenuSortTilesSequentially)
	{
		Stamp* stamp = m_project.GetStamp(m_hoverStamp);
		if(stamp)
		{
			//Re-arrange tiles sequentially to prepare for animation use
			m_project.SortStampTilesSequentially(stamp);

			//Refresh
			m_mainWindow->RefreshAll();
		}
	}
	else if(event.GetId() == eMenuOpenInAnimEditor)
	{
		Stamp* stamp = m_project.GetStamp(m_hoverStamp);
		if(stamp)
		{
			//Check if sorted first
			bool sequential = stamp->CheckTilesBatched();

			if(!sequential)
			{
				//Warn
				if(wxMessageBox("Tiles are not sorted sequentially, this stamp isn't animation compatible. Sort tiles now?", "Stamp not suitable for animation", wxYES | wxNO | wxICON_WARNING) == wxYES)
				{
					//Sort
					m_project.SortStampTilesSequentially(stamp);

					//Refresh
					m_mainWindow->RefreshAll();

					sequential = true;
				}
			}

			if(sequential)
			{
				//Show anim editor in stamp mode
				SpriteAnimEditorDialog animEditorDialog(m_mainWindow, SpriteAnimEditorDialog::eAnimEditModeStampAnim, m_project, m_renderer, *m_glContext, m_renderResources);
				animEditorDialog.SetSelectedStamp(m_hoverStamp);
				animEditorDialog.ShowModal();
			}
		}
	}
#endif
}

void StampsPanel::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	//Render canvas
	RenderCanvas(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	//Render selected stamp
	if(m_selectedStamp)
	{
		if(Stamp* stamp = m_project.GetStamp(m_selectedStamp))
		{
			ion::debug::Assert(stamp, "Invalid stamp");
			ion::Vector2 size(stamp->GetWidth(), stamp->GetHeight());
			const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourSelected);
			RenderBox(m_selectedStampPos, size, colour, renderer, cameraInverseMtx, projectionMtx, z);
		}
	}

	z += zOffset;

	//Render mouse hover stamp
	if(m_hoverStamp && m_hoverStamp != m_selectedStamp)
	{
		if(Stamp* stamp = m_project.GetStamp(m_hoverStamp))
		{
			ion::debug::Assert(stamp, "Invalid stamp");
			ion::Vector2 size(stamp->GetWidth(), stamp->GetHeight());
			const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourHighlight);
			RenderBox(m_hoverStampPos, size, colour, renderer, cameraInverseMtx, projectionMtx, z);
		}
	}

	z += zOffset;

	//Render grid
	if(m_project.GetShowGrid())
	{
		RenderGrid(renderer, cameraInverseMtx, projectionMtx, z);
	}

	z += zOffset;

	//Render outlines
	if(m_project.GetShowStampOutlines())
	{
		RenderStampOutlines(renderer, cameraInverseMtx, projectionMtx, z);
	}
}

void StampsPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		const int tileWidth = m_project.GetPlatformConfig().tileWidth;
		const int tileHeight = m_project.GetPlatformConfig().tileHeight;

		//If stamps invalidated
		if(m_project.StampsAreInvalidated() && m_panelSize.x > tileWidth && m_panelSize.y > tileHeight)
		{
			//Rearrange stamps (calculates canvas size)
			ArrangeStamps(ion::Vector2(m_panelSize.x, m_panelSize.y));

			//Recreate canvas
			CreateCanvas(m_canvasSize.x, m_canvasSize.y);

			//Fill with invalid tile
			FillTiles(InvalidTileId, ion::Vector2i(0, 0), ion::Vector2i(m_canvasSize.x - 1, m_canvasSize.y - 1));

			//Recreate grid
			CreateGrid(m_canvasSize.x, m_canvasSize.y, m_canvasSize.x / m_project.GetGridSize(), m_canvasSize.y / m_project.GetGridSize());

			//Redraw stamps on canvas
			PaintStamps();

			//Reset zoom/pan
			ResetZoomPan();
		}

		ViewPanel::Refresh(eraseBackground, rect);
	}
}

void StampsPanel::ArrangeStamps(const ion::Vector2& panelSize)
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	//Fit canvas to panel
	m_canvasSize.x = ion::maths::Ceil(panelSize.x / tileWidth);
	m_canvasSize.y = ion::maths::Ceil(panelSize.y / tileHeight);

	//Clear stamp position map
	m_stampPosMap.clear();

	//Sort by size, and find widest stamp
	std::vector<std::pair<int, const Stamp*>> stampsSorted;
	stampsSorted.reserve(m_project.GetStampCount());

	for(TStampMap::const_iterator it = m_project.StampsBegin(), end = m_project.StampsEnd(); it != end; ++it)
	{
		const Stamp& stamp = it->second;
		int size = stamp.GetWidth() * stamp.GetHeight();
		stampsSorted.push_back(std::make_pair(size, &stamp));

		//If wider than current canvas width, grow canvas
		m_canvasSize.x = ion::maths::Max(m_canvasSize.x, it->second.GetWidth());
	}

	std::sort(stampsSorted.begin(), stampsSorted.end(), [](const std::pair<int, const Stamp*>& a, const std::pair<int, const Stamp*>& b) { return a.first < b.first; });

	ion::Vector2i currPos;
	int rowHeight = 1;

	for(int i = 0; i < stampsSorted.size(); i++)
	{
		const Stamp& stamp = *stampsSorted[i].second;
		ion::Vector2i stampSize(stamp.GetWidth(), stamp.GetHeight());
		ion::Vector2i stampPos;

		if(currPos.x + stampSize.x > m_canvasSize.x)
		{
			//Can't fit on this line, advance Y by tallest stamp on current row
			currPos.y += rowHeight;

			//Reset column
			currPos.x = stampSize.x;

			//Reset current row height
			rowHeight = stampSize.y;

			//Set stamp pos
			stampPos.x = 0;
			stampPos.y = currPos.y;
		}
		else
		{
			//Set stamp pos
			stampPos = currPos;

			//Next column
			currPos.x += stampSize.x;
		}

		//Record tallest stamp on current row
		rowHeight = ion::maths::Max(rowHeight, stampSize.y);

		//If Y pos + height extends beyond canvas height, grow canvas
		m_canvasSize.y = ion::maths::Max(m_canvasSize.y, currPos.y + rowHeight);

		//Add stamp to position map
		m_stampPosMap.push_back(std::make_pair(stamp.GetId(), stampPos));

		//If this is the currently selected stamp, update position
		if(stamp.GetId() == m_selectedStamp)
		{
			m_selectedStampPos = stampPos;
		}
	}
}

void StampsPanel::PaintStamps()
{
	for(int i = 0; i < m_stampPosMap.size(); i++)
	{
		Stamp* stamp = m_project.GetStamp(m_stampPosMap[i].first);
		ion::debug::Assert(stamp, "Invalid stamp");
		PaintStamp(*stamp, m_stampPosMap[i].second.x, m_stampPosMap[i].second.y, 0);
	}
}

void StampsPanel::RenderBox(const ion::Vector2i& pos, const ion::Vector2& size, const ion::Colour& colour, ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;
	const int quadHalfExtentsX = 4;
	const int quadHalfExtentsY = 4;

	float bottom = (m_canvasSize.y - (pos.y + size.y));

	ion::Matrix4 boxMtx;
	ion::Vector3 boxScale(size.x, size.y, 0.0f);
	ion::Vector3 boxPos(floor((pos.x - (m_canvasSize.x / 2.0f) + (boxScale.x / 2.0f)) * tileWidth),
						floor((bottom - (m_canvasSize.y / 2.0f) + (boxScale.y / 2.0f)) * tileHeight), z);

	boxMtx.SetTranslation(boxPos);
	boxMtx.SetScale(boxScale);

	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);

	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
	material->SetDiffuseColour(colour);
	renderer.BindMaterial(*material, boxMtx, cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_selectionPrimitive->GetVertexBuffer(), m_selectionPrimitive->GetIndexBuffer());
	renderer.UnbindMaterial(*material);
	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);
}

void StampsPanel::RenderStampOutlines(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	ion::Matrix4 worldViewProjMtx;
	ion::Matrix4 outlineMtx;
	ion::render::Shader* shader = m_renderResources.GetShader(RenderResources::eShaderFlatColour);
	ion::render::Shader::ParamHndl<ion::Matrix4> worldViewProjParamV = shader->CreateParamHndl<ion::Matrix4>("gWorldViewProjectionMatrix");

	ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileLineQuad);
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
	const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourOutline);

	material->SetDiffuseColour(colour);
	renderer.BindMaterial(*material, outlineMtx, cameraInverseMtx, projectionMtx);

	for(int i = 0; i < m_stampPosMap.size(); i++)
	{
		Stamp* stamp = m_project.GetStamp(m_stampPosMap[i].first);
		if(stamp)
		{
			outlineMtx = m_renderResources.CalcBoxMatrix(m_stampPosMap[i].second, ion::Vector2i(stamp->GetWidth(), stamp->GetHeight()), m_canvasSize, z);
			worldViewProjMtx = outlineMtx * cameraInverseMtx * projectionMtx;
			worldViewProjParamV.SetValue(worldViewProjMtx);

			renderer.DrawVertexBuffer(primitive->GetVertexBuffer());
		}
	}

	renderer.UnbindMaterial(*material);
}

void StampsPanel::ResetZoomPan()
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	m_cameraZoom = (float)m_panelSize.x / (m_canvasSize.x * tileWidth);
	ion::Vector3 cameraPos(-(m_panelSize.x / 2.0f / m_cameraZoom), -(m_panelSize.y / 2.0f / m_cameraZoom), 0.0f);

	m_camera.SetZoom(ion::Vector3(m_cameraZoom, m_cameraZoom, 1.0f));
	m_camera.SetPosition(cameraPos);
}