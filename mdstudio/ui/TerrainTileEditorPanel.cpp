///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "TerrainTileEditorPanel.h"
#include "MainWindow.h"
#include <ion/renderer/Texture.h>

const float TerrainTileEditorPanel::s_defaultZoom = 3.0f;

TerrainTileEditorPanel::TerrainTileEditorPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, renderer, glContext, renderResources, parent, winid, pos, size, style, name)
{
	//No panning
	EnablePan(false);

	m_canvasSize.x = s_tileWidth;
	m_canvasSize.y = s_tileHeight;

	//Create rendering primitives
	m_tilePrimitive = new ion::render::Quad(ion::render::Quad::xy, ion::Vector2(s_tileWidth * 4.0f, s_tileHeight * 4.0f));
	m_collisionPrimitive = new ion::render::Quad(ion::render::Quad::xy, ion::Vector2(s_tileWidth * 4.0f, s_tileHeight * 4.0f));

	//Create 8x8 grid
	CreateGrid(s_tileWidth, s_tileHeight, s_tileWidth, s_tileHeight);
}

TerrainTileEditorPanel::~TerrainTileEditorPanel()
{
	delete m_tilePrimitive;
	delete m_collisionPrimitive;
}

void TerrainTileEditorPanel::OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta)
{
	ViewPanel::OnMouse(event, mouseDelta);
}

void TerrainTileEditorPanel::OnKeyboard(wxKeyEvent& event)
{
	ViewPanel::OnKeyboard(event);
}

void TerrainTileEditorPanel::OnResize(wxSizeEvent& event)
{
	ViewPanel::OnResize(event);
	CentreCamera();
	SetCameraZoom(s_defaultZoom);
	Refresh();
}

void TerrainTileEditorPanel::OnMouseTileEvent(int buttonBits, int x, int y)
{
	const int tileHeight = 8;

	if(m_project)
	{
		if((buttonBits & eMouseLeft) || (buttonBits & eMouseRight))
		{
			TerrainTileId tileId = m_project->GetPaintTerrainTile();
			if(TerrainTile* tile = m_project->GetTerrainTileset().GetTerrainTile(tileId))
			{
				if(x >= 0 && x < s_tileWidth && y >= 0 && y < s_tileHeight)
				{
					if(buttonBits & eMouseLeft)
					{
						//Invert for height
						const int height = tileHeight - y;

						//Set height at X
						tile->SetHeight(x, height);

						//Draw on collision tile
						m_renderResources.SetTerrainTileHeight(tileId, x, height);
					}
					else
					{
						//Clear height at X
						tile->ClearHeight(x);

						//Clear collision tile
						m_renderResources.SetTerrainTileHeight(tileId, x, 0);
					}

					//Refresh collision panels
					m_mainWindow->RedrawPanel(MainWindow::ePanelTerrainTiles);
					m_mainWindow->RedrawPanel(MainWindow::ePanelTerrainTileEditor);
					m_mainWindow->RedrawPanel(MainWindow::ePanelMap);
				}
			}
		}
	}
}

void TerrainTileEditorPanel::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	RenderTile(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	RenderCollision(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	if(m_project->GetShowGrid())
	{
		RenderGrid(renderer, cameraInverseMtx, projectionMtx, z);
	}
}

void TerrainTileEditorPanel::SetProject(Project* project)
{
	ViewPanel::SetProject(project);
}

void TerrainTileEditorPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	ViewPanel::Refresh(eraseBackground, rect);

	if(m_project)
	{
		//Redraw tile and collision
		PaintTile();
		//Redraw collision tile
		PaintCollision();
	}
}

void TerrainTileEditorPanel::RenderTile(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//Draw tile
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTileset);
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	material->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_tilePrimitive->GetVertexBuffer(), m_tilePrimitive->GetIndexBuffer());
	material->Unbind();
}

void TerrainTileEditorPanel::RenderCollision(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//Draw collision tile
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTerrainTileset);
	renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	material->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_collisionPrimitive->GetVertexBuffer(), m_collisionPrimitive->GetIndexBuffer());
	material->Unbind();
	renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);
}

void TerrainTileEditorPanel::PaintTile()
{
	ion::render::TexCoord texCoords[4];
	m_renderResources.GetTileTexCoords(m_project->GetPaintTile(), texCoords, 0);
	m_tilePrimitive->SetTexCoords(texCoords);
}

void TerrainTileEditorPanel::PaintCollision()
{
	if(m_project)
	{
		const int tileWidth = 8;
		const int tileHeight = 8;

		//Set tex coords
		ion::render::TexCoord texCoords[4];
		m_renderResources.GetTerrainTileTexCoords(m_project->GetPaintTerrainTile(), texCoords);
		m_collisionPrimitive->SetTexCoords(texCoords);
	}
}