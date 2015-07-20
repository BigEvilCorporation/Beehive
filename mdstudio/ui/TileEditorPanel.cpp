///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "TileEditorPanel.h"
#include "MainWindow.h"

#include <ion/renderer/Texture.h>

const float TileEditorPanel::s_defaultZoom = 3.0f;

TileEditorPanel::TileEditorPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, renderer, glContext, renderResources, parent, winid, pos, size, style, name)
{
	//No panning
	EnablePan(false);

	m_canvasSize.x = s_tileWidth;
	m_canvasSize.y = s_tileHeight;

	//Create rendering primitive
	m_tilePrimitive = new ion::render::Quad(ion::render::Quad::xy, ion::Vector2(s_tileWidth * 4.0f, s_tileHeight * 4.0f));

	//Create 8x8 grid
	CreateGrid(s_tileWidth, s_tileHeight, s_tileWidth, s_tileHeight);
}

TileEditorPanel::~TileEditorPanel()
{
	delete m_tilePrimitive;
}

void TileEditorPanel::OnMouse(wxMouseEvent& event, const ion::Vector2& mouseDelta)
{
	ViewPanel::OnMouse(event, mouseDelta);
}

void TileEditorPanel::OnKeyboard(wxKeyEvent& event)
{
	ViewPanel::OnKeyboard(event);
}

void TileEditorPanel::OnResize(wxSizeEvent& event)
{
	ViewPanel::OnResize(event);
	CentreCamera();
	SetCameraZoom(s_defaultZoom);
	Refresh();
}

void TileEditorPanel::OnMouseTileEvent(ion::Vector2 mouseDelta, int buttonBits, int x, int y)
{
	const int tileHeight = 8;

	if(m_project && m_project->GetPaintTile())
	{
		TileId tileId = m_project->GetPaintTile();

		if(buttonBits & eMouseLeft)
		{
			if(Tile* tile = m_project->GetTileset().GetTile(tileId))
			{
				if(x >= 0 && x < s_tileWidth && y >= 0 && y < s_tileHeight)
				{
					//Get palette
					Palette* palette = m_project->GetPalette(tile->GetPaletteId());

					if(palette)
					{
						//Get colour
						u8 colourIdx = m_project->GetPaintColour();
						const Colour& colour = palette->GetColour(colourIdx);

						//Set colour on tile
						tile->SetPixelColour(x, y, colourIdx);

						//Re-add to hash map
						m_project->GetTileset().HashChanged(tileId);

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
	}
}

void TileEditorPanel::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	RenderTile(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	if(m_project->GetShowGrid())
	{
		RenderGrid(renderer, cameraInverseMtx, projectionMtx, z);
	}
}

void TileEditorPanel::SetProject(Project* project)
{
	ViewPanel::SetProject(project);
}

void TileEditorPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	ViewPanel::Refresh(eraseBackground, rect);

	if(m_project)
	{
		ion::render::TexCoord texCoords[4];
		m_renderResources.GetTileTexCoords(m_project->GetPaintTile(), texCoords, 0);
		m_tilePrimitive->SetTexCoords(texCoords);
	}
}

void TileEditorPanel::RenderTile(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//Draw tile
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTileset);
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	material->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_tilePrimitive->GetVertexBuffer(), m_tilePrimitive->GetIndexBuffer());
	material->Unbind();
}