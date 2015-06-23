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

	//Setup textured collision set material
	m_materials[eMaterialCollisionSet]->AddDiffuseMap(m_textures[eTextureCollisionSet]);
	m_materials[eMaterialCollisionSet]->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f));
	m_materials[eMaterialCollisionSet]->SetVertexShader(m_vertexShaders[eShaderFlatTextured]);
	m_materials[eMaterialCollisionSet]->SetPixelShader(m_pixelShaders[eShaderFlatTextured]);

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

void RenderResources::CreateCollisionTexture()
{
	const int iconWidth = 16;
	const int iconHeight = 16;

	const u32 bytesPerPixelSource = 3;
	const u32 bytesPerPixelDest = 4;

	u32 numCollisionTypes = m_project->GetCollisionTypeCount() + 1;
	m_collisionSetSizeSq = (u32)ion::maths::Ceil(ion::maths::Sqrt((float)numCollisionTypes));
	u32 textureWidth =  m_collisionSetSizeSq * iconWidth;
	u32 textureHeight = m_collisionSetSizeSq * iconHeight;
	u32 textureSize = textureWidth * textureHeight * bytesPerPixelDest;
	m_cellSizeCollisionTexSpaceSq = 1.0f / (float)m_collisionSetSizeSq;

	u8* data = new u8[textureSize];
	ion::memory::MemSet(data, 0, textureSize);

	for(TCollisionTypeMap::const_iterator it = m_project->CollisionTypesBegin(), end = m_project->CollisionTypesEnd(); it != end; ++it)
	{
		const CollisionType& collisionType = it->second;

		int index = -1;

		//Cheap and lazy bit scan
		for(int i = 0; i < sizeof(u8) * 8 && index < 0; i++)
		{
			if((1 << i) & collisionType.bit)
			{
				//First icon is blank
				index = i + 1;
			}
		}

		u32 x = index % m_collisionSetSizeSq;
		u32 y = index / m_collisionSetSizeSq;

		for(int pixelY = 0; pixelY < iconHeight; pixelY++)
		{
			for(int pixelX = 0; pixelX < iconWidth; pixelX++)
			{
				//Invert Y for OpenGL
				int pixelY_OGL = iconHeight - 1 - pixelY;

				int destPixelX = (x * iconWidth) + pixelX;
				int destPixelY = (y * iconHeight) + pixelY;
				u32 pixelIdx = (destPixelY * textureWidth) + destPixelX;
				u32 destOffset = pixelIdx * bytesPerPixelDest;
				u32 sourceOffset = ((pixelY * iconWidth) + pixelX) * bytesPerPixelSource;
				ion::debug::Assert(destOffset + 2 < textureSize, "Out of bounds");
				ion::debug::Assert(sourceOffset + 2 < collisionType.iconData.size(), "Out of bounds");

				const u8& r = collisionType.iconData[sourceOffset];
				const u8& g = collisionType.iconData[sourceOffset + 1];
				const u8& b = collisionType.iconData[sourceOffset + 2];
				data[destOffset] = r;
				data[destOffset + 1] = g;
				data[destOffset + 2] = b;

				//White is transparent
				data[destOffset + 3] = (r == 255 && g == 255 && b == 255) ? 0 : 255;
			}
		}
	}

	m_textures[eTextureCollisionSet]->Load(textureWidth, textureHeight, ion::render::Texture::eRGBA, ion::render::Texture::eRGBA, ion::render::Texture::eBPP24, false, data);
	m_textures[eTextureCollisionSet]->SetMinifyFilter(ion::render::Texture::eFilterNearest);
	m_textures[eTextureCollisionSet]->SetMagnifyFilter(ion::render::Texture::eFilterNearest);
	m_textures[eTextureCollisionSet]->SetWrapping(ion::render::Texture::eWrapClamp);

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

void RenderResources::GetCollisionTypeTexCoords(u8 collisionBit, ion::render::TexCoord texCoords[4]) const
{
	int index = -1;

	if(collisionBit)
	{
		//Cheap and lazy bit scan
		for(int i = 0; i < sizeof(u8) * 8 && index < 0; i++)
		{
			if((1 << i) & collisionBit)
			{
				//First icon is blank
				index = i + 1;
			}
		}
	}
	else
	{
		index = 0;
	}

	//Map bit to X/Y on collision set texture
	int tilesetX = (index % m_collisionSetSizeSq);
	int tilesetY = (index / m_collisionSetSizeSq);
	ion::Vector2 textureBottomLeft(m_cellSizeCollisionTexSpaceSq * tilesetX, m_cellSizeCollisionTexSpaceSq * tilesetY);

	float top = (textureBottomLeft.y + m_cellSizeCollisionTexSpaceSq);
	float left = textureBottomLeft.x;
	float bottom = textureBottomLeft.y;
	float right = (textureBottomLeft.x + m_cellSizeCollisionTexSpaceSq);

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