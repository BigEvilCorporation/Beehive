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

#include "TileEditorPanel.h"
#include "MainWindow.h"

#include <ion/renderer/Texture.h>

const float TileEditorPanel::s_defaultZoom = 3.0f;

TileEditorPanel::TileEditorPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, wxGLAttributes& glAttributes, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, project, renderer, glContext, glAttributes, renderResources, parent, winid, pos, size, style, name)
	, m_project(project)
{
	//No panning
	EnablePan(false);

	u8 tileWidth = project.GetPlatformConfig().tileWidth;
	u8 tileHeight = project.GetPlatformConfig().tileWidth;

	m_canvasSize.x = tileWidth;
	m_canvasSize.y = tileHeight;

	//Create rendering primitive
	m_tilePrimitive = new ion::render::Quad(ion::render::Quad::Axis::xy, ion::Vector2(tileWidth * 4.0f, tileHeight * 4.0f));

	//Create 8x8 grid
	CreateGrid(tileWidth, tileHeight, tileWidth, tileHeight);
}

TileEditorPanel::~TileEditorPanel()
{
	delete m_tilePrimitive;
}

void TileEditorPanel::OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta)
{
	ViewPanel::OnMouse(event, mouseDelta);
}

void TileEditorPanel::OnKeyboard(wxKeyEvent& event)
{
	ViewPanel::OnKeyboard(event);
}

void TileEditorPanel::OnResize(wxSizeEvent& event)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		ViewPanel::OnResize(event);
		CentreCamera();
		SetCameraZoom(s_defaultZoom);
		Refresh();
	}
}

void TileEditorPanel::OnMouseTileEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int x, int y)
{
	const u8 tileWidth = m_project.GetPlatformConfig().tileWidth;
	const u8 tileHeight = m_project.GetPlatformConfig().tileWidth;

	if(m_project.GetPaintTile())
	{
		TileId tileId = m_project.GetPaintTile();

#if !BEEHIVE_FIXED_STAMP_MODE //No tile editing in fixed mode
		if(buttonBits & eMouseLeft)
		{
			if(Tile* tile = m_project.GetTileset().GetTile(tileId))
			{
				if(x >= 0 && x < tileWidth && y >= 0 && y < tileHeight)
				{
					//Get palette
					Palette* palette = m_project.GetPalette(tile->GetPaletteId());

					if(palette)
					{
						//Get colour
						u8 colourIdx = m_project.GetPaintColour();
						const Colour& colour = palette->GetColour(colourIdx);

						//Set colour on tile
						tile->SetPixelColour(x, y, colourIdx);

						//Re-add to hash map
						m_project.GetTileset().HashChanged(tileId);

						//Set colour on tileset texture
						m_renderResources.SetTilesetTexPixel(tileId, ion::Vector2i(x, y), colourIdx);

						//Refresh panel
						Refresh();

						//Refresh tiles, map and stamps panels
						m_mainWindow->RedrawPanel(MainWindow::ePanelTiles);
						m_mainWindow->RedrawPanel(MainWindow::ePanelMap);
						m_mainWindow->RedrawPanel(MainWindow::ePanelStamps);
					}
				}
			}
		}
#endif
	}
}

void TileEditorPanel::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	RenderTile(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	if(m_project.GetShowGrid())
	{
		RenderGrid(renderer, cameraInverseMtx, projectionMtx, z);
	}
}

void TileEditorPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		ion::render::TexCoord texCoords[4];
		m_renderResources.GetTileTexCoords(m_project.GetPaintTile(), texCoords, 0);
		m_tilePrimitive->SetTexCoords(texCoords);
		m_tilePrimitive->GetVertexBuffer().CommitBuffer();

		ViewPanel::Refresh(eraseBackground, rect);
	}
}

void TileEditorPanel::RenderTile(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//Draw tile
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTileset);
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	renderer.BindMaterial(*material, ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_tilePrimitive->GetVertexBuffer(), m_tilePrimitive->GetIndexBuffer());
	renderer.UnbindMaterial(*material);
}