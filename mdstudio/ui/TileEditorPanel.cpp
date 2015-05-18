///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "TileEditorPanel.h"
#include "MainWindow.h"

#include <ion/renderer/Texture.h>

const float TileEditorPanel::s_defaultZoom = 4.0f;

TileEditorPanel::TileEditorPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, ion::render::Texture* tilesetTexture, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, renderer, glContext, tilesetTexture, parent, winid, pos, size, style, name)
{
	m_texture = NULL;
	m_tileId = InvalidTileId;

	//No panning
	EnablePan(false);

	m_canvasSize.x = s_tileWidth;
	m_canvasSize.y = s_tileHeight;

	//Create rendering primitive
	m_tilePrimitive = new ion::render::Quad(ion::render::Quad::xy, ion::Vector2(s_tileWidth * 4.0f, s_tileHeight * 4.0f));

	//Create selection material
	m_tileMaterial = new ion::render::Material();
	m_tileMaterial->AddDiffuseMap(tilesetTexture);
	m_tileMaterial->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	m_tileMaterial->SetVertexShader(m_vertexShader);
	m_tileMaterial->SetPixelShader(m_pixelShader);

	//Create 8x8 grid
	CreateGrid(s_tileWidth, s_tileHeight, s_tileWidth, s_tileHeight);

	//Centre camera
	CentreCamera();
}

TileEditorPanel::~TileEditorPanel()
{
	delete m_tilePrimitive;
	delete m_tileMaterial;

	if(m_texture)
		delete m_texture;
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
	Refresh();
}

void TileEditorPanel::OnMouseTileEvent(ion::Vector2 mouseDelta, int buttonBits, int x, int y)
{
	const int tileHeight = 8;

	if(m_project && m_tileId)
	{
		if(buttonBits & ViewPanel::eMouseLeft)
		{
			if(Tile* tile = m_project->GetTileset().GetTile(m_tileId))
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

						//Set colour on tileset texture
						m_mainWindow->SetTilesetTexPixel(m_tileId, ion::Vector2i(x, y), colourIdx);

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

void TileEditorPanel::SetTile(TileId tileId)
{
	m_tileId = tileId;
	
	ion::render::TexCoord texCoords[4];
	m_mainWindow->GetTileTexCoords(m_tileId, texCoords, 0);
	m_tilePrimitive->SetTexCoords(texCoords);

	//Centre camera and reset zoom
	CentreCamera();
	SetCameraZoom(s_defaultZoom);

	//Refresh panel
	Refresh();
}

void TileEditorPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	ViewPanel::Refresh(eraseBackground, rect);

	if(m_project && m_project->TilesAreInvalidated() && m_tileId)
	{
		ion::render::TexCoord texCoords[4];
		m_mainWindow->GetTileTexCoords(m_tileId, texCoords, 0);
		m_tilePrimitive->SetTexCoords(texCoords);
	}
}

void TileEditorPanel::RenderTile(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//Draw tile
	m_tileMaterial->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_tilePrimitive->GetVertexBuffer(), m_tilePrimitive->GetIndexBuffer());
	m_tileMaterial->Unbind();
}