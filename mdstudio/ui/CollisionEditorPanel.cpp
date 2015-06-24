///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "CollisionEditorPanel.h"
#include "MainWindow.h"
#include <ion/renderer/Texture.h>

const float CollisionEditorPanel::s_defaultZoom = 4.0f;

CollisionEditorPanel::CollisionEditorPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, renderer, glContext, renderResources, parent, winid, pos, size, style, name)
{
	//No panning
	EnablePan(false);

	m_canvasSize.x = s_tileWidth;
	m_canvasSize.y = s_tileHeight;

	//Create rendering primitives
	m_tilePrimitive = new ion::render::Quad(ion::render::Quad::xy, ion::Vector2(s_tileWidth * 4.0f, s_tileHeight * 4.0f));

	for(int i = 0; i < s_numCollisionTypes; i++)
	{
		m_collisionPrimitives[i] = new ion::render::Chessboard(ion::render::Chessboard::xy, ion::Vector2(s_tileWidth * 4.0f, s_tileHeight * 4.0f), 8, 8, true);
	}

	//Create 8x8 grid
	CreateGrid(s_tileWidth, s_tileHeight, s_tileWidth, s_tileHeight);
}

CollisionEditorPanel::~CollisionEditorPanel()
{
	delete m_tilePrimitive;

	for(int i = 0; i < s_numCollisionTypes; i++)
	{
		delete m_collisionPrimitives[i];
	}
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

void CollisionEditorPanel::OnMouseTileEvent(ion::Vector2 mouseDelta, int buttonBits, int x, int y)
{
	const int tileHeight = 8;

	if(m_project)
	{
		if(const CollisionType* collisionType = m_project->GetPaintCollisionType())
		{
			if((buttonBits & eMouseLeft) && !m_prevMouseBits & eMouseLeft)
			{
				if(Tile* tile = m_project->GetTileset().GetTile(m_project->GetPaintTile()))
				{
					if(x >= 0 && x < s_tileWidth && y >= 0 && y < s_tileHeight)
					{
						u8 collisionBit = collisionType->bit;
						u8 existingBits = tile->GetPixelCollisionBits(x, y);

						if(existingBits & collisionBit)
						{
							//Bit is set, remove
							tile->ClearPixelCollisionBits(x, y, collisionBit);
						}
						else
						{
							//Bit is not set, add
							tile->AddPixelCollisionBits(x, y, collisionType->bit);
						}

						//Invalidate collision types
						m_project->InvalidateCollisionTypes(true);

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
	ion::render::Material* material = m_renderResources.GetMaterial(RenderResources::eMaterialCollisionSet);
	renderer.SetAlphaBlending(ion::render::Renderer::Translucent);
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	material->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);

	for(int i = 0; i < s_numCollisionTypes; i++)
	{
		renderer.DrawVertexBuffer(m_collisionPrimitives[i]->GetVertexBuffer(), m_collisionPrimitives[i]->GetIndexBuffer());
	}

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

		//Reset
		ion::render::TexCoord blankCoords[4];
		m_renderResources.GetCollisionTypeTexCoords(0, blankCoords);

		for(int x = 0; x < tileWidth; x++)
		{
			for(int y = 0; y < tileHeight; y++)
			{
				for(int i = 0; i < s_numCollisionTypes; i++)
				{
					m_collisionPrimitives[i]->SetTexCoords((y * tileWidth) + x, blankCoords);
				}
			}
		}

		//Draw collision icons
		TileId tileId = m_project->GetPaintTile();
		if(Tile* tile = m_project->GetTileset().GetTile(tileId))
		{
			for(int x = 0; x < tileWidth; x++)
			{
				for(int y = 0; y < tileHeight; y++)
				{
					u8 collisionBits = tile->GetPixelCollisionBits(x, y);

					for(int i = 0; i < s_numCollisionTypes; i++)
					{
						u8 collisionBit = (1 << i);
						if(collisionBits & collisionBit)
						{
							if(const CollisionType* collisionType = m_project->GetCollisionType(1 << i))
							{
								//Pixel uses this collision type, draw
								int yInv = tileHeight - 1 - y;

								//Set texture coords for cell
								ion::render::TexCoord coords[4];
								m_renderResources.GetCollisionTypeTexCoords(collisionBit, coords);
								m_collisionPrimitives[i]->SetTexCoords((yInv * tileWidth) + x, coords);
							}
						}
					}
				}
			}
		}
	}
}