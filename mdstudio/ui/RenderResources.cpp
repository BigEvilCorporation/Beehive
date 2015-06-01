///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "RenderResources.h"

#include <ion/core/memory/Memory.h>

RenderResources::RenderResources()
{
	m_tilesetSizeSq = 1;
	m_cellSizeTexSpaceSq = 1.0f;
	m_project = NULL;

	//Create shaders
	for(int i = 0; i < eShaderMax; i++)
	{
		m_vertexShaders[i] = ion::render::Shader::Create();
		m_pixelShaders[i] = ion::render::Shader::Create();
	}

	//Load shaders
	if(!m_vertexShaders[eShaderFlatColour]->Load("shaders/flat_v.ion.shader"))
		ion::debug::Error("Error loading vertex shader");

	if(!m_pixelShaders[eShaderFlatColour]->Load("shaders/flat_p.ion.shader"))
		ion::debug::Error("Error loading pixel shader");

	if(!m_vertexShaders[eShaderFlatTextured]->Load("shaders/flattextured_v.ion.shader"))
		ion::debug::Error("Error loading vertex shader");

	if(!m_pixelShaders[eShaderFlatTextured]->Load("shaders/flattextured_p.ion.shader"))
		ion::debug::Error("Error loading pixel shader");

	//Create tileset texture
	m_textures[eTextureTileset] = ion::render::Texture::Create();

	//Create materials
	for(int i = 0; i < eMaterialMax; i++)
	{
		m_materials[i] = new ion::render::Material();
	}

	//Setup flat material
	m_materials[eMaterialFlatColour]->SetDiffuseColour(ion::Colour(0.0f, 0.0f, 0.0f));
	m_materials[eMaterialFlatColour]->SetVertexShader(m_vertexShaders[eShaderFlatColour]);
	m_materials[eMaterialFlatColour]->SetPixelShader(m_pixelShaders[eShaderFlatColour]);

	//Setup textured tileset material
	m_materials[eMaterialTileset]->AddDiffuseMap(m_textures[eTextureTileset]);
	m_materials[eMaterialTileset]->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f));
	m_materials[eMaterialTileset]->SetVertexShader(m_vertexShaders[eShaderFlatTextured]);
	m_materials[eMaterialTileset]->SetPixelShader(m_pixelShaders[eShaderFlatTextured]);

	//Set colours
	m_colours[eColourHighlight] = ion::Colour(0.1f, 0.2f, 0.5f, 0.4f);
	m_colours[eColourSelected] = ion::Colour(0.1f, 0.5f, 0.7f, 0.8f);
	m_colours[eColourPreview] = ion::Colour(1.0f, 1.0f, 1.0f, 0.8f);
	m_colours[eColourOutline] = ion::Colour(1.0f, 1.0f, 0.0f, 1.0f);
	m_colours[eColourGrid] = ion::Colour(0.0f, 0.0f, 0.0f, 1.0f);
}

RenderResources::~RenderResources()
{
	for(int i = 0; i < ePrimitiveMax; i++)
	{
		delete m_primitives[i];
	}

	for(int i = 0; i < eTextureMax; i++)
	{
		delete m_textures[i];
	}

	for(int i = 0; i < eMaterialMax; i++)
	{
		delete m_materials[i];
	}

	for(int i = 0; i < eShaderMax; i++)
	{
		delete m_vertexShaders[i];
		delete m_pixelShaders[i];
	}
}

void RenderResources::CreateTilesetTexture()
{
	const int tileWidth = 8;
	const int tileHeight = 8;

	const Tileset& tileset = m_project->GetTileset();

	u32 numTiles = tileset.GetCount();
	m_tilesetSizeSq = (u32)ion::maths::Ceil(ion::maths::Sqrt((float)numTiles));
	u32 textureWidth = m_tilesetSizeSq * tileWidth;
	u32 textureHeight = m_tilesetSizeSq * tileHeight;
	u32 bytesPerPixel = 3;
	u32 textureSize = textureWidth * textureHeight * bytesPerPixel;
	m_cellSizeTexSpaceSq = 1.0f / (float)m_tilesetSizeSq;

	u8* data = new u8[textureSize];
	ion::memory::MemSet(data, 0, textureSize);

	u32 tileIndex = 0;

	for(TTileMap::const_iterator it = tileset.Begin(), end = tileset.End(); it != end; ++it, ++tileIndex)
	{
		const Tile& tile = it->second;
		PaletteId paletteId = tile.GetPaletteId();
		Palette* palette = m_project->GetPalette(paletteId);

		u32 x = tileIndex % m_tilesetSizeSq;
		u32 y = tileIndex / m_tilesetSizeSq;

		for(int pixelY = 0; pixelY < 8; pixelY++)
		{
			for(int pixelX = 0; pixelX < 8; pixelX++)
			{
				//Invert Y for OpenGL
				int pixelY_OGL = tileHeight - 1 - pixelY;
				const Colour& colour = palette->GetColour(tile.GetPixelColour(pixelX, pixelY_OGL));

				int destPixelX = (x * tileWidth) + pixelX;
				int destPixelY = (y * tileHeight) + pixelY;
				u32 pixelIdx = (destPixelY * textureWidth) + destPixelX;
				u32 dataOffset = pixelIdx * bytesPerPixel;
				ion::debug::Assert(dataOffset + 2 < textureSize, "Out of bounds");
				data[dataOffset] = colour.r;
				data[dataOffset + 1] = colour.g;
				data[dataOffset + 2] = colour.b;
			}
		}
	}

	m_textures[eTextureTileset]->Load(textureWidth, textureHeight, ion::render::Texture::eRGB, ion::render::Texture::eRGB, ion::render::Texture::eBPP24, false, data);
	m_textures[eTextureTileset]->SetMinifyFilter(ion::render::Texture::eFilterNearest);
	m_textures[eTextureTileset]->SetMagnifyFilter(ion::render::Texture::eFilterNearest);
	m_textures[eTextureTileset]->SetWrapping(ion::render::Texture::eWrapClamp);

	//Rebuild tileset index cache
	CacheTileIndices(tileset);

	delete data;
}

void RenderResources::CacheTileIndices(const Tileset& tileset)
{
	m_tileIndexMap.clear();

	u32 tileIndex = 0;

	for(TTileMap::const_iterator it = tileset.Begin(), end = tileset.End(); it != end; ++it, ++tileIndex)
	{
		//Map ID to index
		m_tileIndexMap.insert(std::make_pair(it->first, tileIndex));
	}
}

void RenderResources::GetTileTexCoords(TileId tileId, ion::render::TexCoord texCoords[4], u32 flipFlags) const
{
	if(tileId == InvalidTileId)
	{
		//Invalid tile, use top-left pixel
		float onePixelTexSpace = m_cellSizeTexSpaceSq / 8.0f;

		//Top left
		texCoords[0].x = 0.0f;
		texCoords[0].y = 0.0f;
		//Bottom left
		texCoords[1].x = 0.0f;
		texCoords[1].y = onePixelTexSpace;
		//Bottom right
		texCoords[2].x = onePixelTexSpace;
		texCoords[2].y = onePixelTexSpace;
		//Top right
		texCoords[3].x = onePixelTexSpace;
		texCoords[3].y = 0.0f;
	}
	else
	{
		//Map tile ID to index
		u32 tileIndex = GetTilesetTexTileIndex(tileId);

		//Map tile to X/Y on tileset texture
		int tilesetX = (tileIndex % m_tilesetSizeSq);
		int tilesetY = (tileIndex / m_tilesetSizeSq);
		ion::Vector2 textureBottomLeft(m_cellSizeTexSpaceSq * tilesetX, m_cellSizeTexSpaceSq * tilesetY);

		bool flipX = (flipFlags & Map::eFlipX) != 0;
		bool flipY = (flipFlags & Map::eFlipY) != 0;

		float top = flipY ? (textureBottomLeft.y) : (textureBottomLeft.y + m_cellSizeTexSpaceSq);
		float left = flipX ? (textureBottomLeft.x + m_cellSizeTexSpaceSq) : (textureBottomLeft.x);
		float bottom = flipY ? (textureBottomLeft.y + m_cellSizeTexSpaceSq) : (textureBottomLeft.y);
		float right = flipX ? (textureBottomLeft.x) : (textureBottomLeft.x + m_cellSizeTexSpaceSq);

		//Top left
		texCoords[0].x = left;
		texCoords[0].y = top;
		//Bottom left
		texCoords[1].x = left;
		texCoords[1].y = bottom;
		//Bottom right
		texCoords[2].x = right;
		texCoords[2].y = bottom;
		//Top right
		texCoords[3].x = right;
		texCoords[3].y = top;
	}
}

int RenderResources::GetTilesetTexTileIndex(TileId tileId) const
{
	std::map<TileId, u32>::const_iterator it = m_tileIndexMap.find(tileId);
	ion::debug::Assert(it != m_tileIndexMap.end(), "TileId not found in tileset");
	return it->second;
}

void RenderResources::SetTilesetTexPixel(TileId tileId, const ion::Vector2i& pixel, u8 colourIdx)
{
	if(m_project && m_textures[eTextureTileset])
	{
		if(Tile* tile = m_project->GetTileset().GetTile(tileId))
		{
			if(Palette* palette = m_project->GetPalette(tile->GetPaletteId()))
			{
				const int tileWidth = 8;
				const int tileHeight = 8;

				const Colour& colour = palette->GetColour(colourIdx);

				int tileIndex = GetTilesetTexTileIndex(tileId);
				u32 x = tileIndex % m_tilesetSizeSq;
				u32 y = tileIndex / m_tilesetSizeSq;

				//Invert Y for OpenGL
				int y_inv = tileHeight - 1 - pixel.y;

				ion::Vector2i pixelPos((x * tileWidth) + pixel.x, (y * tileHeight) + y_inv);

				m_textures[eTextureTileset]->SetPixel(pixelPos, ion::Colour(colour.r, colour.g, colour.b));
			}
		}
	}
}