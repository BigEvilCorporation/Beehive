///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "StampsPanel.h"

StampsPanel::StampsPanel(ion::render::Renderer& renderer, wxGLContext* glContext, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(renderer, glContext, parent, winid, pos, size, style, name)
{
	//Fixed canvas width
	m_canvasWidth = 10;

	//Dynamic canvas height
	m_canvasHeight = 0;
}

StampsPanel::~StampsPanel()
{

}

void StampsPanel::OnMouse(wxMouseEvent& event)
{
	ViewPanel::OnMouse(event);
}

void StampsPanel::OnKeyboard(wxKeyEvent& event)
{
	ViewPanel::OnKeyboard(event);
}

void StampsPanel::OnResize(wxSizeEvent& event)
{
	ViewPanel::OnResize(event);
}

void StampsPanel::OnMouseTileEvent(ion::Vector2 mouseDelta, int buttonBits, int x, int y)
{
	ViewPanel::OnMouseTileEvent(mouseDelta, buttonBits, x, y);

	if((buttonBits & eMouseLeft) && !(m_prevMouseBits & eMouseLeft))
	{
		//TODO: Per-tile stamp map
		StampId selectedStamp = InvalidStampId;

		//Brute force search for stamp ID
		ion::Vector2i mousePos(x, y);

		for(int i = 0; i < m_stampPosMap.size() && !selectedStamp; i++)
		{
			StampId stampId = m_stampPosMap[i].first;
			if(Stamp* stamp = m_project->GetStamp(stampId))
			{
				const ion::Vector2i& stampTopLeft = m_stampPosMap[i].second;
				const ion::Vector2i& stampBottomRight = stampTopLeft + ion::Vector2i(stamp->GetWidth(), stamp->GetHeight());
				if(mousePos.x >= stampTopLeft.x && mousePos.y >= stampTopLeft.y
					&& mousePos.x <= stampBottomRight.x && mousePos.y <= stampBottomRight.y)
				{
					selectedStamp = stampId;
				}
			}
		}

		if(selectedStamp)
		{
			m_project->SetPaintStamp(selectedStamp);
		}
	}
}

void StampsPanel::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	//Render canvas
	RenderCanvas(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	//Render grid
	if(m_project->GetShowGrid())
	{
		RenderGrid(m_renderer, cameraInverseMtx, projectionMtx, z);
	}
}

void StampsPanel::SetProject(Project* project)
{
	//Creates tileset texture
	ViewPanel::SetProject(project);

	//Rearrange stamps
	ArrangeStamps(m_canvasWidth);

	//Recreate canvas
	CreateCanvas(m_canvasWidth, m_canvasHeight);

	//Fill with invalid tile
	FillTiles(InvalidTileId, ion::Vector2i(0, 0), ion::Vector2i(m_canvasWidth - 1, m_canvasHeight - 1));

	//Recreate grid
	CreateGrid(m_canvasWidth, m_canvasHeight, m_canvasWidth / m_project->GetGridSize(), m_canvasHeight / m_project->GetGridSize());
}

void StampsPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(m_project)
	{
		//If stamps invalidated
		if(m_project->StampsAreInvalidated())
		{
			//Rearrange stamps
			ArrangeStamps(m_canvasWidth);

			//Recreate canvas
			CreateCanvas(m_canvasWidth, m_canvasHeight);

			//Fill with invalid tile
			FillTiles(InvalidTileId, ion::Vector2i(0, 0), ion::Vector2i(m_canvasWidth - 1, m_canvasHeight - 1));

			//Recreate grid
			CreateGrid(m_canvasWidth, m_canvasHeight, m_canvasWidth / m_project->GetGridSize(), m_canvasHeight / m_project->GetGridSize());

			//Recreate tileset texture
			CreateTilesetTexture(m_project->GetMap().GetTileset());

			//Recreate index cache
			CacheTileIndices();

			//Redraw stamps on canvas
			PaintStamps();

			m_project->InvalidateStamps(false);
		}
	}

	ViewPanel::Refresh(eraseBackground, rect);
}

void StampsPanel::ArrangeStamps(int canvasWidth)
{
	m_canvasWidth = 1;
	m_canvasHeight = 1;
	m_stampPosMap.clear();

	if(m_project)
	{
		//Find widest stamp first
		for(TStampMap::const_iterator it = m_project->StampsBegin(), end = m_project->StampsEnd(); it != end; ++it)
		{
			//If wider than provided canvas width, grow canvas
			canvasWidth = max(canvasWidth, it->second.GetWidth());
		}

		m_canvasWidth = canvasWidth;

		ion::Vector2i currPos;
		int rowHeight = 1;

		for(TStampMap::const_iterator it = m_project->StampsBegin(), end = m_project->StampsEnd(); it != end; ++it)
		{
			const Stamp& stamp = it->second;
			ion::Vector2i stampSize(stamp.GetWidth(), stamp.GetHeight());
			ion::Vector2i stampPos;

			if(currPos.x + stampSize.x > canvasWidth)
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
			rowHeight = max(rowHeight, stampSize.y);

			//If Y pos + height extends beyond canvas height, grow canvas
			m_canvasHeight = max(m_canvasHeight, currPos.y + rowHeight);

			//Add stamp to position map
			m_stampPosMap.push_back(std::make_pair(stamp.GetId(), stampPos));
		}
	}

	m_canvasSize.x = m_canvasWidth;
	m_canvasSize.y = m_canvasHeight;
}

void StampsPanel::PaintStamps()
{
	for(int i = 0; i < m_stampPosMap.size(); i++)
	{
		Stamp* stamp = m_project->GetStamp(m_stampPosMap[i].first);
		PaintStamp(*stamp, m_stampPosMap[i].second.x, m_stampPosMap[i].second.y);
	}
}