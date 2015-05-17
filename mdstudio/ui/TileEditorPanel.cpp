///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "TileEditorPanel.h"

#include <ion/renderer/Texture.h>

TileEditorPanel::TileEditorPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, renderer, glContext, parent, winid, pos, size, style, name)
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
	if(m_project && m_tileId)
	{
		if(buttonBits & ViewPanel::eMouseLeft)
		{
			if(Tile* tile = m_project->GetTileset().GetTile(m_tileId))
			{
				if(x >= 0 && x < s_tileWidth && y >= 0 && y < s_tileHeight)
				{
					//Set colour on tile
					tile->SetPixelColour(x, y, m_project->GetPaintColour());

					//TODO: Slow, edit existing texture
					CreateTexture();

					//Refresh panel
					Refresh();

					//TODO: Refresh tiles and map (figure out a way to do it in realtime)
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
	
	//Create and draw texture
	CreateTexture();
}

void TileEditorPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	ViewPanel::Refresh(eraseBackground, rect);

	if(m_project && m_project->TilesAreInvalidated() && m_tileId)
	{
		CreateTexture();
	}
}

void TileEditorPanel::CreateTexture()
{
	if(m_texture)
		delete m_texture;

	m_texture = ion::render::Texture::Create();

	u32 bytesPerPixel = 3;
	u32 textureSize = s_tileWidth * s_tileHeight * bytesPerPixel;

	u8* data = new u8[textureSize];
	ion::memory::MemSet(data, 0, textureSize);

	Colour colour;
	Tile* tile = NULL;
	Palette* palette = NULL;

	if(m_project)
	{
		colour = m_project->GetPalette(0)->GetColour(0);

		if(m_tileId)
		{
			tile = m_project->GetTileset().GetTile(m_tileId);

			if(tile)
			{
				palette = m_project->GetPalette(tile->GetPaletteId());
			}
		}
	}

	for(int pixelY = 0; pixelY < s_tileHeight; pixelY++)
	{
		for(int pixelX = 0; pixelX < s_tileWidth; pixelX++)
		{
			if(tile && palette)
			{
				colour = palette->GetColour(tile->GetPixelColour(pixelY, pixelX));
			}

			//Invert Y for OpenGL
			int pixelY_OGL = s_tileHeight - 1 - pixelY;

			u32 pixelIdx = (pixelY_OGL * s_tileWidth) + pixelX;
			u32 dataOffset = pixelIdx * bytesPerPixel;
			ion::debug::Assert(dataOffset + 2 < textureSize, "Out of bounds");
			data[dataOffset] = colour.r;
			data[dataOffset + 1] = colour.g;
			data[dataOffset + 2] = colour.b;
		}
	}

	m_texture->Load(s_tileWidth, s_tileHeight, ion::render::Texture::eRGB, ion::render::Texture::eBPP24, data);
	m_texture->SetMinifyFilter(ion::render::Texture::eFilterNearest);
	m_texture->SetMagnifyFilter(ion::render::Texture::eFilterNearest);
	m_texture->SetWrapping(ion::render::Texture::eWrapClamp);

	//Set texture on material
	m_tileMaterial->SetDiffuseMap(m_texture, 0);

	delete data;
}

void TileEditorPanel::RenderTile(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//Draw map
	m_tileMaterial->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_tilePrimitive->GetVertexBuffer(), m_tilePrimitive->GetIndexBuffer());
	m_tileMaterial->Unbind();
}