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

#include "BlocksPanel.h"
#include "MainWindow.h"

#pragma optimize("",off)

BlocksPanel::BlocksPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, project, renderer, glContext, renderResources, parent, winid, pos, size, style, name)
{
	//Custom zoom/pan handling
	//EnableZoom(false);
	//EnablePan(false);
}

BlocksPanel::~BlocksPanel()
{

}

void BlocksPanel::OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta)
{
	ViewPanel::OnMouse(event, mouseDelta);

	/*

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	//Camera pan Y (if canvas is taller than panel)
	//if((m_canvasSize.y * tileHeight) > (m_panelSize.y / m_cameraZoom))
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

	*/
}

void BlocksPanel::OnResize(wxSizeEvent& event)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		ViewPanel::OnResize(event);

		const int tileWidth = m_project.GetPlatformConfig().tileWidth;
		const int tileHeight = m_project.GetPlatformConfig().tileHeight;
		const int blockWidth = m_project.GetPlatformConfig().blockWidth;
		const int blockHeight = m_project.GetPlatformConfig().blockHeight;

		if(m_panelSize.x > tileWidth && m_panelSize.y > tileHeight)
		{
			//Fit canvas to panel
			m_canvasSize.x = ion::maths::Ceil(m_panelSize.x / tileWidth);
			m_canvasSize.y = ion::maths::Ceil(m_panelSize.y / tileHeight);

			//Redraw blocks to canvas
			PaintBlocks();

			//Recreate grid
			//CreateGrid(m_canvasSize.x, m_canvasSize.y, m_canvasSize.x / blockWidth, m_canvasSize.y / blockHeight);

			if(m_gridPrimitive)
				delete m_gridPrimitive;

			m_gridPrimitive = new ion::render::Grid(ion::render::Grid::xy, ion::Vector2((float)m_panelSize.x / 2.0f, (float)m_panelSize.y / 2.0f), m_canvasSize.x /*/ blockWidth*/, m_canvasSize.y /*/ blockHeight*/);

			//Reset zoom/pan
			ResetZoomPan();
		}
	}
}

void BlocksPanel::OnMouseTileEvent(int buttonBits, ion::Vector2i tileDelta, int x, int y)
{
	ViewPanel::OnMouseTileEvent(buttonBits, tileDelta, x, y);

	m_hoverPos.x = ion::maths::RoundDownToNearest(x, m_project.GetPlatformConfig().blockWidth);
	m_hoverPos.y = ion::maths::RoundDownToNearest(y, m_project.GetPlatformConfig().blockHeight);

	if(buttonBits & eMouseLeft)
	{
		const int blockWidth = m_project.GetPlatformConfig().blockWidth;
		const int blockHeight = m_project.GetPlatformConfig().blockHeight;

		int index = ((y / blockHeight) * (m_canvasSize.x / blockHeight)) + (x / blockWidth);

		if(index < m_blockCache.size())
		{
			const Map::Block* block = m_blockCache[index];
			bool breakMe = true;
		}
	}

	//Redraw
	Refresh();
}

void BlocksPanel::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	//Render canvas
	RenderCanvas(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	//Render grid
	if(m_project.GetShowGrid())
	{
		RenderGrid(renderer, cameraInverseMtx, projectionMtx, z);
	}

	z += zOffset;

	//Render selection box
	ion::Vector2 size(m_project.GetPlatformConfig().blockWidth, m_project.GetPlatformConfig().blockHeight);
	const ion::Colour& colour = m_renderResources.GetColour(RenderResources::eColourHighlight);
	RenderBox(m_hoverPos, size, colour, renderer, cameraInverseMtx, projectionMtx, z);
}

void BlocksPanel::RenderBox(const ion::Vector2i& pos, const ion::Vector2& size, const ion::Colour& colour, ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
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
	ion::render::Primitive* primitive = m_renderResources.GetPrimitive(RenderResources::ePrimitiveTileQuad);

	renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
	material->SetDiffuseColour(colour);
	material->Bind(boxMtx, cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
	material->Unbind();
	renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);
}

void BlocksPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		const int tileWidth = m_project.GetPlatformConfig().tileWidth;
		const int tileHeight = m_project.GetPlatformConfig().tileHeight;
		const int blockWidth = m_project.GetPlatformConfig().blockWidth;
		const int blockHeight = m_project.GetPlatformConfig().blockHeight;

		//If stamps invalidated
		if(m_project.BlocksAreInvalidated() &&  m_panelSize.x > tileWidth && m_panelSize.y > tileHeight)
		{
			//Fit canvas to panel
			m_canvasSize.x = ion::maths::Ceil(m_panelSize.x / tileWidth);
			m_canvasSize.y = ion::maths::Ceil(m_panelSize.y / tileHeight);

			//Recreate grid
			//CreateGrid(m_canvasSize.x, m_canvasSize.y, m_canvasSize.x / blockWidth, m_canvasSize.y / blockHeight);

			if(m_gridPrimitive)
				delete m_gridPrimitive;

			m_gridPrimitive = new ion::render::Grid(ion::render::Grid::xy, ion::Vector2((float)m_panelSize.x / 2.0f, (float)m_panelSize.y / 2.0f), m_canvasSize.x /*/ blockWidth*/, m_canvasSize.y /*/ blockHeight*/);

			//Redraw blocks to canvas
			PaintBlocks();

			//Reset zoom/pan
			ResetZoomPan();
		}

		ViewPanel::Refresh(eraseBackground, rect);
	}
}

void BlocksPanel::PaintBlocks()
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;
	const int blockWidth = m_project.GetPlatformConfig().blockWidth;
	const int blockHeight = m_project.GetPlatformConfig().blockHeight;

	//Count blocks
	int blockCount = 0;
	for(TMapMap::const_iterator it = m_project.MapsBegin(), end = m_project.MapsEnd(); it != end; ++it)
	{
		const std::vector<Map::Block>& currBlocks = it->second.GetBlocks();
		for(int i = 0; i < currBlocks.size(); i++)
		{
			if(currBlocks[i].unique)
			{
				blockCount++;
			}
		}
	}

	int canvasWidthBlocks = m_canvasSize.x / blockWidth;
	int canvasSizePixelsX = m_canvasSize.x * tileWidth;
	int canvasSizePixelsY = (ion::maths::RoundUpToNearest(blockCount / canvasWidthBlocks, canvasWidthBlocks)) * blockHeight * tileHeight;
	int canvasSizeTilesX = canvasSizePixelsX / tileWidth;
	int canvasSizeTilesY = canvasSizePixelsY / tileHeight;

	//Recreate canvas
	if(m_canvasPrimitive)
		delete m_canvasPrimitive;

	if(blockCount)
	{
		m_canvasPrimitive = new ion::render::Chessboard(ion::render::Chessboard::xy, ion::Vector2((float)canvasSizePixelsX / 2.0f, (float)canvasSizePixelsY / 2.0f), canvasSizeTilesX, canvasSizeTilesY, true);

		//Fill with invalid tile
		//FillTiles(InvalidTileId, ion::Vector2i(0, 0), ion::Vector2i(canvasSizeTilesX - 1, canvasSizeTilesY - 1));

		m_blockCache.clear();

		//Paint all unique blocks
		int index = 0;
		for(TMapMap::const_iterator it = m_project.MapsBegin(), end = m_project.MapsEnd(); it != end; ++it)
		{
			const std::vector<Map::Block>& currBlocks = it->second.GetBlocks();
			for(int i = 0; i < currBlocks.size(); i++)
			{
				if(currBlocks[i].unique)
				{
					m_blockCache.push_back(&currBlocks[i]);
					PaintBlock(currBlocks[i], index % canvasWidthBlocks, index / canvasWidthBlocks, canvasSizeTilesX, canvasSizeTilesY);
					index++;
				}
			}
		}
	}
}

int prevyinv = 0;

void BlocksPanel::PaintBlock(const Map::Block& block, int x, int y, int canvasSizeTilesX, int canvasSizeTilesY)
{
	int blockWidth = m_project.GetPlatformConfig().blockWidth;
	int blockHeight = m_project.GetPlatformConfig().blockHeight;

	for(int stampX = 0; stampX < blockWidth; stampX++)
	{
		for(int stampY = 0; stampY < blockHeight; stampY++)
		{
			TileId tileId = block.m_tiles[(blockWidth * stampY) + stampX].m_id;
			u32 tileFlags = block.m_tiles[(blockWidth * stampY) + stampX].m_flags;

			int canvasX = stampX + (x * blockWidth);
			int canvasY = stampY + (y * blockHeight);
			int y_inv = canvasSizeTilesY - 1 - canvasY;
			prevyinv = y_inv;

			//Can place stamps outside canvas boundaries, only draw tiles that are inside
			//if(canvasX >= 0 && canvasX < m_canvasSize.x && y_inv >= 0 && y_inv < m_canvasSize.y)
			{
				//Paint on canvas
				//PaintTile(tileId, canvasX, y_inv, tileFlags);

				//Set texture coords for cell
				ion::render::TexCoord coords[4];
				m_renderResources.GetTileTexCoords(tileId, coords, tileFlags);
				m_canvasPrimitive->SetTexCoords((y_inv * canvasSizeTilesX) + canvasX, coords);
			}
		}
	}
}

void BlocksPanel::ResetZoomPan()
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	m_cameraZoom = (float)m_panelSize.x / (m_canvasSize.x * tileWidth);
	ion::Vector3 cameraPos(-(m_panelSize.x / 2.0f / m_cameraZoom), -(m_panelSize.y / 2.0f / m_cameraZoom), 0.0f);

	m_camera.SetZoom(ion::Vector3(m_cameraZoom, m_cameraZoom, 1.0f));
	m_camera.SetPosition(cameraPos);
}

#pragma optimize("",on)