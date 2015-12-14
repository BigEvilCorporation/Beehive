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
	m_collisionTilesetSizeSq = 1;
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

	//Create textures
	for(int i = 0; i < eTextureMax; i++)
	{
		m_textures[i] = ion::render::Texture::Create();
	}

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

	//Setup textured collision tileset material
	m_materials[eMaterialCollisionTileset]->AddDiffuseMap(m_textures[eTextureCollisionTileset]);
	m_materials[eMaterialCollisionTileset]->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f));
	m_materials[eMaterialCollisionTileset]->SetVertexShader(m_vertexShaders[eShaderFlatTextured]);
	m_materials[eMaterialCollisionTileset]->SetPixelShader(m_pixelShaders[eShaderFlatTextured]);

	//Set colours
	m_colours[eColourHighlight] = ion::Colour(0.1f, 0.2f, 0.5f, 0.4f);
	m_colours[eColourSelected] = ion::Colour(0.1f, 0.5f, 0.7f, 0.8f);
	m_colours[eColourPreview] = ion::Colour(1.0f, 1.0f, 1.0f, 0.8f);
	m_colours[eColourOutline] = ion::Colour(1.0f, 1.0f, 0.0f, 1.0f);
	m_colours[eColourGrid] = ion::Colour(0.0f, 0.0f, 0.0f, 1.0f);

	//Create primitives
	m_primitives[ePrimitiveUnitQuad] = new ion::render::Quad(ion::render::Quad::xy, ion::Vector2(4.0f, 4.0f));
	m_primitives[ePrimitiveUnitLineQuad] = new ion::render::LineQuad(ion::render::LineQuad::xy, ion::Vector2(4.0f, 4.0f));
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
	m_tilesetSizeSq = max(1, (u32)ion::maths::Ceil(ion::maths::Sqrt((float)numTiles)));
	u32 textureWidth = m_tilesetSizeSq * tileWidth;
	u32 textureHeight = m_tilesetSizeSq * tileHeight;
	u32 bytesPerPixel = 3;
	u32 textureSize = textureWidth * textureHeight * bytesPerPixel;
	m_cellSizeTexSpaceSq = 1.0f / (float)m_tilesetSizeSq;

	u8* data = new u8[textureSize];
	ion::memory::MemSet(data, 255, textureSize);

	for(int i = 0; i < tileset.GetCount(); i++)
	{
		const Tile& tile = *tileset.GetTile(i);
		PaletteId paletteId = tile.GetPaletteId();
		Palette* palette = m_project->GetPalette(paletteId);

		u32 x = i % m_tilesetSizeSq;
		u32 y = i / m_tilesetSizeSq;

		for(int pixelY = 0; pixelY < 8; pixelY++)
		{
			for(int pixelX = 0; pixelX < 8; pixelX++)
			{
				//Invert Y for OpenGL
				int pixelY_OGL = tileHeight - 1 - pixelY;

				u8 colourIdx = tile.GetPixelColour(pixelX, pixelY_OGL);

				//Protect against blank tiles
				if(palette->IsColourUsed(colourIdx))
				{
					const Colour& colour = palette->GetColour(colourIdx);

					int destPixelX = (x * tileWidth) + pixelX;
					int destPixelY = (y * tileHeight) + pixelY;
					u32 pixelIdx = (destPixelY * textureWidth) + destPixelX;
					u32 dataOffset = pixelIdx * bytesPerPixel;
					ion::debug::Assert(dataOffset + 2 < textureSize, "Out of bounds");
					data[dataOffset] = colour.GetRed();
					data[dataOffset + 1] = colour.GetGreen();
					data[dataOffset + 2] = colour.GetBlue();
				}
			}
		}
	}

	m_textures[eTextureTileset]->Load(textureWidth, textureHeight, ion::render::Texture::eRGB, ion::render::Texture::eRGB, ion::render::Texture::eBPP24, false, data);
	m_textures[eTextureTileset]->SetMinifyFilter(ion::render::Texture::eFilterNearest);
	m_textures[eTextureTileset]->SetMagnifyFilter(ion::render::Texture::eFilterNearest);
	m_textures[eTextureTileset]->SetWrapping(ion::render::Texture::eWrapClamp);

	delete data;
}

void RenderResources::CreateCollisionTilesTexture()
{
	const int tileWidth = 8;
	const int tileHeight = 8;
	const int blankRowHeight = 8;

	const CollisionTileset& tileset = m_project->GetCollisionTileset();

	u32 numTiles = tileset.GetCount() + 1;
	m_collisionTilesetSizeSq = max(1, (u32)ion::maths::Ceil(ion::maths::Sqrt((float)numTiles)));
	u32 textureWidth = m_collisionTilesetSizeSq * tileWidth;
	u32 textureHeight = m_collisionTilesetSizeSq * tileHeight;
	u32 bytesPerPixel = 4;
	u32 textureSize = textureWidth * textureHeight * bytesPerPixel;
	m_cellSizeCollisionTilesetTexSpaceSq = 1.0f / (float)m_collisionTilesetSizeSq;

	u8* data = new u8[textureSize];
	ion::memory::MemSet(data, 0, textureSize);

	const ion::Colour setColour(1.0f, 0.0f, 0.0f, 0.7f);
	const ion::Colour unsetColour(0.0f, 0.0f, 0.0f, 0.0f);

	for(int i = 0; i < tileset.GetCount(); i++)
	{
		const CollisionTile& tile = *tileset.GetCollisionTile(i);

		u32 x = i % m_collisionTilesetSizeSq;
		u32 y = i / m_collisionTilesetSizeSq;
		
		for(int pixelX = 0; pixelX < 8; pixelX++)
		{
			//Get height at X
			u16 height = tile.GetHeight(pixelX);

			for(int pixelY = 0; pixelY < 8; pixelY++)
			{
				const ion::Colour& colour = pixelY < height ? setColour : unsetColour;

				int destPixelX = (x * tileWidth) + pixelX;
				int destPixelY = (y * tileHeight) + pixelY;
				u32 pixelIdx = (destPixelY * textureWidth) + destPixelX;
				u32 dataOffset = pixelIdx * bytesPerPixel;
				ion::debug::Assert(dataOffset + 2 < textureSize, "Out of bounds");
				data[dataOffset] = colour.r * 255;
				data[dataOffset + 1] = colour.g * 255;
				data[dataOffset + 2] = colour.b * 255;
				data[dataOffset + 3] = colour.a * 255;
			}
		}
	}

	m_textures[eTextureCollisionTileset]->Load(textureWidth, textureHeight, ion::render::Texture::eRGBA, ion::render::Texture::eRGBA, ion::render::Texture::eBPP24, false, data);
	m_textures[eTextureCollisionTileset]->SetMinifyFilter(ion::render::Texture::eFilterNearest);
	m_textures[eTextureCollisionTileset]->SetMagnifyFilter(ion::render::Texture::eFilterNearest);
	m_textures[eTextureCollisionTileset]->SetWrapping(ion::render::Texture::eWrapClamp);

	delete data;
}

void RenderResources::GetTileTexCoords(TileId tileId, ion::render::TexCoord texCoords[4], u32 flipFlags) const
{
	if(tileId == InvalidTileId)
	{
		//Use background tile if there is one
		tileId = m_project->GetBackgroundTile();

		if(tileId == InvalidTileId && m_project->GetTileset().GetCount() > 0)
		{
			//If no background tile, and there are tiles available, use first tile
			tileId = 0;
		}
	}

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
		//Map tile to X/Y on tileset texture
		int tilesetX = (tileId % m_tilesetSizeSq);
		int tilesetY = (tileId / m_tilesetSizeSq);
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

void RenderResources::GetCollisionTileTexCoords(CollisionTileId tileId, ion::render::TexCoord texCoords[4]) const
{
	if(tileId == InvalidTileId)
	{
		//Use default tile if there is one
		tileId = m_project->GetDefaultCollisionTile();

		if(tileId == InvalidTileId)
		{
			//If no default tile, use blank tile
			tileId = (m_collisionTilesetSizeSq * m_collisionTilesetSizeSq) - 1;
		}
	}

	//Map tile to X/Y on tileset texture
	int tilesetX = (tileId % m_collisionTilesetSizeSq);
	int tilesetY = (tileId / m_collisionTilesetSizeSq);
	ion::Vector2 textureBottomLeft(m_cellSizeCollisionTilesetTexSpaceSq * tilesetX, m_cellSizeCollisionTilesetTexSpaceSq * tilesetY);
	
	float top = textureBottomLeft.y + m_cellSizeCollisionTilesetTexSpaceSq;
	float left = textureBottomLeft.x;
	float bottom = textureBottomLeft.y;
	float right = textureBottomLeft.x + m_cellSizeCollisionTilesetTexSpaceSq;
	
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

				u32 x = tileId % m_tilesetSizeSq;
				u32 y = tileId / m_tilesetSizeSq;

				//Invert Y for OpenGL
				int y_inv = tileHeight - 1 - pixel.y;

				ion::Vector2i pixelPos((x * tileWidth) + pixel.x, (y * tileHeight) + y_inv);

				m_textures[eTextureTileset]->SetPixel(pixelPos, ion::Colour(colour.GetRed(), colour.GetGreen(), colour.GetBlue()));
			}
		}
	}
}

void RenderResources::SetCollisionTileHeight(CollisionTileId tileId, int x, s8 height)
{
	if(m_project && m_textures[eTextureCollisionTileset])
	{
		if(CollisionTile* tile = m_project->GetCollisionTileset().GetCollisionTile(tileId))
		{
			ion::Colour setColour(1.0f, 0.0f, 0.0f, 0.7f);
			ion::Colour unsetColour(0.0f, 0.0f, 0.0f, 0.0f);

			const int tileWidth = 8;
			const int tileHeight = 8;

			const u32 tileX = tileId % m_collisionTilesetSizeSq;
			const u32 tileY = tileId / m_collisionTilesetSizeSq;

			//Draw all pixels up to height
			for(int y = 0; y < tileHeight; y++)
			{
				const ion::Colour& colour = (y < height) ? setColour : unsetColour;
				ion::Vector2i pixelPos((tileX * tileWidth) + x, (tileY * tileHeight) + y);
				m_textures[eTextureCollisionTileset]->SetPixel(pixelPos, colour);
			}
		}
	}
}

ion::Matrix4 RenderResources::CalcBoxMatrix(const ion::Vector2i& position, const ion::Vector2i& size, const ion::Vector2i& mapSize, float z)
{
	ion::Matrix4 matrix;

	float bottom = mapSize.y - position.y - size.y;
	ion::Vector2 tileSize(8, 8);
	ion::Vector3 translation(floor((position.x - (mapSize.x / 2.0f) + (size.x / 2.0f)) * tileSize.x), floor((bottom - (mapSize.y / 2.0f) + (size.y / 2.0f)) * tileSize.y), z);
	matrix.SetTranslation(translation);
	matrix.SetScale(ion::Vector3(size.x, size.y, 1.0f));

	return matrix;
}