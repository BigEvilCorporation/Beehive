///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "CollisionEditorPanel.h"
#include "MainWindow.h"
#include <ion/renderer/Texture.h>

const float CollisionEditorPanel::s_defaultZoom = 3.0f;

CollisionEditorPanel::CollisionEditorPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
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

CollisionEditorPanel::~CollisionEditorPanel()
{
	delete m_tilePrimitive;
	delete m_collisionPrimitive;
}

void CollisionEditorPanel::OnMouse(wxMouseEvent& event, const ion::Vector2& mouseDelta)
{
	ViewPanel::OnMouse(event, mouseDelta);
}

void CollisionEditorPanel::OnKeyboard(wxKeyEvent& event)
{
	ViewPanel::OnKeyboard(event);
}

void CollisionEditorPanel::OnResize(wxSizeEvent& event)
{
	ViewPanel::OnResize(event);
	CentreCamera();
	SetCameraZoom(s_defaultZoom);
	Refresh();
}

void CollisionEditorPanel::OnMouseTileEvent(int buttonBits, int x, int y)
{
	const int tileHeight = 8;

	if(m_project)
	{
		if((buttonBits & eMouseLeft) || (buttonBits & eMouseRight))
		{
			CollisionTileId tileId = m_project->GetPaintCollisionTile();
			if(CollisionTile* tile = m_project->GetCollisionTileset().GetCollisionTile(tileId))
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
						m_renderResources.SetCollisionTileHeight(tileId, x, height);
					}
					else
					{
						//Clear height at X
						tile->ClearHeight(x);

						//Clear collision tile
						m_renderResources.SetCollisionTileHeight(tileId, x, 0);
					}

					//Refresh collision panels
					m_mainWindow->RedrawPanel(MainWindow::ePanelCollisionTiles);
					m_mainWindow->RedrawPanel(MainWindow::ePanelCollisionTileEditor);
					m_mainWindow->RedrawPanel(MainWindow::ePanelMap);
				}
			}
		}
	}
}

void CollisionEditorPanel::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
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

void CollisionEditorPanel::SetProject(Project* project)
{
	ViewPanel::SetProject(project);
}

void CollisionEditorPanel::Refresh(bool eraseBackground, const wxRect *rect)
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

void CollisionEditorPanel::RenderTile(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//Draw tile
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialTileset);
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	material->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_tilePrimitive->GetVertexBuffer(), m_tilePrimitive->GetIndexBuffer());
	material->Unbind();
}

void CollisionEditorPanel::RenderCollision(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//Draw collision tile
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialCollisionTileset);
	renderer.SetAlphaBlending(ion::render::Renderer::Translucent);
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	material->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_collisionPrimitive->GetVertexBuffer(), m_collisionPrimitive->GetIndexBuffer());
	material->Unbind();
	renderer.SetAlphaBlending(ion::render::Renderer::NoBlend);
}

void CollisionEditorPanel::PaintTile()
{
	ion::render::TexCoord texCoords[4];
	m_renderResources.GetTileTexCoords(m_project->GetPaintTile(), texCoords, 0);
	m_tilePrimitive->SetTexCoords(texCoords);
}

void CollisionEditorPanel::PaintCollision()
{
	if(m_project)
	{
		const int tileWidth = 8;
		const int tileHeight = 8;

		//Set tex coords
		ion::render::TexCoord texCoords[4];
		m_renderResources.GetCollisionTileTexCoords(m_project->GetPaintCollisionTile(), texCoords);
		m_collisionPrimitive->SetTexCoords(texCoords);
	}
}