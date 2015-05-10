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
	m_canvasHeight = 1;
	m_stampPosMap.clear();

	if(m_project)
	{
		ion::Vector2i currPos;
		int maxRowHeight = 0;

		for(TStampMap::const_iterator it = m_project->StampsBegin(), end = m_project->StampsEnd(); it != end; ++it)
		{
			const Stamp& stamp = it->second;
			ion::Vector2i stampSize(stamp.GetWidth(), stamp.GetHeight());
			ion::Vector2i stampPos;

			//Grow canvas height
			m_canvasHeight = max(currPos.y + stampSize.y, 1);

			if(currPos.x + stampSize.x > canvasWidth)
			{
				//Can't fit on this line, advance Y by tallest stamp on current row
				currPos.y += maxRowHeight;

				//Reset column
				currPos.x = 0;

				stampPos = currPos;
			}
			else
			{
				stampPos = currPos;

				//Next column
				currPos.x += stampSize.x;
			}

			//Add stamp to position map
			m_stampPosMap.push_back(std::make_pair(stamp.GetId(), stampPos));

			//Record tallest stamp on current row
			maxRowHeight = max(maxRowHeight, stampSize.y);
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