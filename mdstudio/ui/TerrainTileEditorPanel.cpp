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

#include "TerrainTileEditorPanel.h"
#include "MainWindow.h"
#include <ion/renderer/Texture.h>

const float TerrainTileEditorPanel::s_defaultZoom = 3.0f;

TerrainTileEditorPanel::TerrainTileEditorPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, wxGLAttributes& glAttributes, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, project, renderer, glContext, glAttributes, renderResources, parent, winid, pos, size, style, name)
{
	//No panning
	EnablePan(false);

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	m_canvasSize.x = tileWidth;
	m_canvasSize.y = tileHeight;

	//Create rendering primitives
	m_tilePrimitive = new ion::render::Quad(ion::render::Quad::Axis::xy, ion::Vector2(tileWidth * 4.0f, tileHeight * 4.0f));
	m_collisionPrimitive = new ion::render::Quad(ion::render::Quad::Axis::xy, ion::Vector2(tileWidth * 4.0f, tileHeight * 4.0f));
	m_primitiveDirty = true;

	//Create 8x8 grid
	CreateGrid(tileWidth, tileHeight, tileWidth, tileHeight);
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
	if(!m_mainWindow->IsRefreshLocked())
	{
		ViewPanel::OnResize(event);
		CentreCamera();
		SetCameraZoom(s_defaultZoom);
		Refresh();
	}
}

void TerrainTileEditorPanel::OnMouseTileEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int x, int y)
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

#if !BEEHIVE_FIXED_STAMP_MODE //No collision editing in fixed mode
	if((buttonBits & eMouseLeft) || (buttonBits & eMouseRight))
	{
		TerrainTileId tileId = m_project.GetPaintTerrainTile();
		if(TerrainTile* tile = m_project.GetTerrainTileset().GetTerrainTile(tileId))
		{
			if(x >= 0 && x < tileWidth && y >= 0 && y < tileHeight)
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
#endif
}

void TerrainTileEditorPanel::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	RenderTile(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	RenderCollision(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	if(m_project.GetShowGrid())
	{
		RenderGrid(renderer, cameraInverseMtx, projectionMtx, z);
	}
}

void TerrainTileEditorPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		//Redraw tile and collision
		PaintTile();

		//Redraw collision tile
		PaintCollision();

		if (m_primitiveDirty)
		{
			m_tilePrimitive->GetVertexBuffer().CommitBuffer();
			m_collisionPrimitive->GetVertexBuffer().CommitBuffer();
			m_primitiveDirty = false;
		}

		ViewPanel::Refresh(eraseBackground, rect);
	}
}

void TerrainTileEditorPanel::RenderTile(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//Draw tile
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTileset);
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	renderer.BindMaterial(*material, ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_tilePrimitive->GetVertexBuffer(), m_tilePrimitive->GetIndexBuffer());
	renderer.UnbindMaterial(*material);
}

void TerrainTileEditorPanel::RenderCollision(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//Draw collision tile
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTerrainTilesetHeight);
	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	renderer.BindMaterial(*material, ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_collisionPrimitive->GetVertexBuffer(), m_collisionPrimitive->GetIndexBuffer());
	renderer.UnbindMaterial(*material);
	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);
}

void TerrainTileEditorPanel::PaintTile()
{
	ion::render::TexCoord texCoords[4];
	m_renderResources.GetTileTexCoords(m_project.GetPaintTile(), texCoords, 0);
	m_tilePrimitive->SetTexCoords(texCoords);
	m_primitiveDirty = true;
}

void TerrainTileEditorPanel::PaintCollision()
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	//Set tex coords
	ion::render::TexCoord texCoords[4];
	m_renderResources.GetTerrainTileTexCoords(m_project.GetPaintTerrainTile(), texCoords);
	m_collisionPrimitive->SetTexCoords(texCoords);
	m_primitiveDirty = true;
}