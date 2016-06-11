///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "RenderResources.h"

#include <ion/core/memory/Memory.h>

RenderResources::RenderResources(Project& project)
 : m_project(project)
{
	m_tilesetSizeSq = 1;
	m_terrainTilesetSizeSq = 1;
	m_cellSizeTexSpaceSq = 1.0f;

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

	//Setup textured collision types material
	m_materials[eMaterialCollisionTypes]->AddDiffuseMap(m_textures[eTextureCollisionTypes]);
	m_materials[eMaterialCollisionTypes]->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f));
	m_materials[eMaterialCollisionTypes]->SetVertexShader(m_vertexShaders[eShaderFlatTextured]);
	m_materials[eMaterialCollisionTypes]->SetPixelShader(m_pixelShaders[eShaderFlatTextured]);

	//Setup textured terrain tileset material
	m_materials[eMaterialTerrainTileset]->AddDiffuseMap(m_textures[eTextureTerrainTileset]);
	m_materials[eMaterialTerrainTileset]->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f));
	m_materials[eMaterialTerrainTileset]->SetVertexShader(m_vertexShaders[eShaderFlatTextured]);
	m_materials[eMaterialTerrainTileset]->SetPixelShader(m_pixelShaders[eShaderFlatTextured]);

	//Setup textured spriteSheet material
	m_materials[eMaterialSpriteSheet]->AddDiffuseMap(m_textures[eTextureSpriteSheetPreview]);
	m_materials[eMaterialSpriteSheet]->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f));
	m_materials[eMaterialSpriteSheet]->SetVertexShader(m_vertexShaders[eShaderFlatTextured]);
	m_materials[eMaterialSpriteSheet]->SetPixelShader(m_pixelShaders[eShaderFlatTextured]);

	//Set colours
	m_colours[eColourHighlight] = ion::Colour(0.1f, 0.2f, 0.5f, 0.4f);
	m_colours[eColourSelected] = ion::Colour(0.1f, 0.5f, 0.7f, 0.8f);
	m_colours[eColourPreview] = ion::Colour(1.0f, 1.0f, 1.0f, 0.8f);
	m_colours[eColourOutline] = ion::Colour(1.0f, 1.0f, 0.0f, 1.0f);
	m_colours[eColourGrid] = ion::Colour(0.0f, 0.0f, 0.0f, 1.0f);

	//Create primitives
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	m_primitives[ePrimitiveUnitQuad] = new ion::render::Quad(ion::render::Quad::xy, ion::Vector2(0.5f, 0.5f));
	m_primitives[ePrimitiveTileQuad] = new ion::render::Quad(ion::render::Quad::xy, ion::Vector2(tileWidth / 2.0f, tileHeight / 2.0f));
	m_primitives[ePrimitiveUnitLineQuad] = new ion::render::LineQuad(ion::render::LineQuad::xy, ion::Vector2(tileWidth / 2.0f, tileHeight / 2.0f));
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
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	const Tileset& tileset = m_project.GetTileset();

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
		Palette* palette = m_project.GetPalette(paletteId);

		u32 x = i % m_tilesetSizeSq;
		u32 y = i / m_tilesetSizeSq;

		for(int pixelY = 0; pixelY < tileHeight; pixelY++)
		{
			for(int pixelX = 0; pixelX < tileWidth; pixelX++)
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
					ion::debug::Assert(dataOffset + 2 < textureSize, "eOut of bounds");
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

void RenderResources::CreateTerrainTilesTexture()
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;
	const int blankRowHeight = tileHeight;

	const TerrainTileset& tileset = m_project.GetTerrainTileset();

	u32 numTiles = tileset.GetCount() + 1;
	m_terrainTilesetSizeSq = max(1, (u32)ion::maths::Ceil(ion::maths::Sqrt((float)numTiles)));
	u32 textureWidth = m_terrainTilesetSizeSq * tileWidth;
	u32 textureHeight = m_terrainTilesetSizeSq * tileHeight;
	u32 bytesPerPixel = 4;
	u32 textureSize = textureWidth * textureHeight * bytesPerPixel;
	m_cellSizeTerrainTilesetTexSpaceSq = 1.0f / (float)m_terrainTilesetSizeSq;

	u8* data = new u8[textureSize];
	ion::memory::MemSet(data, 0, textureSize);

	const ion::Colour setColour(0.0f, 1.0f, 0.0f, 0.7f);
	const ion::Colour unsetColour(0.0f, 0.0f, 0.0f, 0.0f);

	for(int i = 0; i < tileset.GetCount(); i++)
	{
		const TerrainTile& tile = *tileset.GetTerrainTile(i);

		u32 x = i % m_terrainTilesetSizeSq;
		u32 y = i / m_terrainTilesetSizeSq;
		
		for(int pixelX = 0; pixelX < tileWidth; pixelX++)
		{
			//Get height at X
			u16 height = tile.GetHeight(pixelX);

			for(int pixelY = 0; pixelY < tileHeight; pixelY++)
			{
				const ion::Colour& colour = pixelY < height ? setColour : unsetColour;

				int destPixelX = (x * tileWidth) + pixelX;
				int destPixelY = (y * tileHeight) + pixelY;
				u32 pixelIdx = (destPixelY * textureWidth) + destPixelX;
				u32 dataOffset = pixelIdx * bytesPerPixel;
				ion::debug::Assert(dataOffset + 2 < textureSize, "eOut of bounds");
				data[dataOffset] = colour.r * 255;
				data[dataOffset + 1] = colour.g * 255;
				data[dataOffset + 2] = colour.b * 255;
				data[dataOffset + 3] = colour.a * 255;
			}
		}
	}

	m_textures[eTextureTerrainTileset]->Load(textureWidth, textureHeight, ion::render::Texture::eRGBA, ion::render::Texture::eRGBA, ion::render::Texture::eBPP24, false, data);
	m_textures[eTextureTerrainTileset]->SetMinifyFilter(ion::render::Texture::eFilterNearest);
	m_textures[eTextureTerrainTileset]->SetMagnifyFilter(ion::render::Texture::eFilterNearest);
	m_textures[eTextureTerrainTileset]->SetWrapping(ion::render::Texture::eWrapClamp);

	delete data;
}

void RenderResources::CreateCollisionTypesTexture()
{
	const int numCollisionTypes = 4;

	u32 collisionTexSizeSq = max(1, (u32)ion::maths::Ceil(ion::maths::Sqrt((float)numCollisionTypes)));
	u32 textureWidth = collisionTexSizeSq;
	u32 textureHeight = collisionTexSizeSq;
	u32 bytesPerPixel = 4;
	u32 textureSize = textureWidth * textureHeight * bytesPerPixel;

	u8* data = new u8[textureSize];
	ion::memory::MemSet(data, 0, textureSize);

	const ion::Colour colours[numCollisionTypes] =
	{
		ion::Colour(0.0f, 0.0f, 0.0f, 0.0f),
		ion::Colour(1.0f, 0.0f, 0.0f, 0.7f),
		ion::Colour(0.0f, 0.0f, 1.0f, 0.7f),
		ion::Colour(0.0f, 1.0f, 0.0f, 0.7f),
	};

	for(int i = 0; i < numCollisionTypes; i++)
	{
		u32 x = i % collisionTexSizeSq;
		u32 y = i / collisionTexSizeSq;

		const ion::Colour& colour = colours[i];

		u32 pixelIdx = (y * textureWidth) + x;
		u32 dataOffset = pixelIdx * bytesPerPixel;
		ion::debug::Assert(dataOffset + 2 < textureSize, "eOut of bounds");
		data[dataOffset] = colour.r * 255;
		data[dataOffset + 1] = colour.g * 255;
		data[dataOffset + 2] = colour.b * 255;
		data[dataOffset + 3] = colour.a * 255;
	}

	m_textures[eTextureCollisionTypes]->Load(textureWidth, textureHeight, ion::render::Texture::eRGBA, ion::render::Texture::eRGBA, ion::render::Texture::eBPP24, false, data);
	m_textures[eTextureCollisionTypes]->SetMinifyFilter(ion::render::Texture::eFilterNearest);
	m_textures[eTextureCollisionTypes]->SetMagnifyFilter(ion::render::Texture::eFilterNearest);
	m_textures[eTextureCollisionTypes]->SetWrapping(ion::render::Texture::eWrapClamp);

	delete data;
}

void RenderResources::CreateSpriteSheetPreviewTexture(const BMPReader& reader)
{
	u32 textureWidth = reader.GetWidth();
	u32 textureHeight = reader.GetHeight();
	u32 bytesPerPixel = 3;
	u32 textureSize = textureWidth * textureHeight * bytesPerPixel;

	u8* data = new u8[textureSize];
	ion::memory::MemSet(data, 0, textureSize);

	for(int x = 0; x < textureWidth; x++)
	{
		for(int y = 0; y < textureHeight; y++)
		{
			const Colour& colour = reader.GetPixel(x, y);

			u32 pixelIdx = (y * textureWidth) + x;
			u32 dataOffset = pixelIdx * bytesPerPixel;
			ion::debug::Assert(dataOffset + 2 < textureSize, "eOut of bounds");
			data[dataOffset] = colour.GetRed();
			data[dataOffset + 1] = colour.GetGreen();
			data[dataOffset + 2] = colour.GetBlue();
		}
	}

	m_textures[eTextureSpriteSheetPreview]->Load(textureWidth, textureHeight, ion::render::Texture::eRGB, ion::render::Texture::eRGB, ion::render::Texture::eBPP24, false, data);
	m_textures[eTextureSpriteSheetPreview]->SetMinifyFilter(ion::render::Texture::eFilterNearest);
	m_textures[eTextureSpriteSheetPreview]->SetMagnifyFilter(ion::render::Texture::eFilterNearest);
	m_textures[eTextureSpriteSheetPreview]->SetWrapping(ion::render::Texture::eWrapClamp);

	delete data;
}

ion::render::Texture* RenderResources::CreateSpriteSheetPreviewTexture(const SpriteSheet& spriteSheet)
{
	return NULL;
}

void RenderResources::GetTileTexCoords(TileId tileId, ion::render::TexCoord texCoords[4], u32 flipFlags) const
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	if(tileId == InvalidTileId)
	{
		//Use background tile if there is one
		tileId = m_project.GetBackgroundTile();

		if(tileId == InvalidTileId && m_project.GetTileset().GetCount() > 0)
		{
			//If no background tile, and there are tiles available, use first tile
			tileId = 0;
		}
	}

	if(tileId == InvalidTileId)
	{
		//Invalid tile, use top-left pixel
		float onePixelTexSpaceX = m_cellSizeTexSpaceSq / tileWidth;
		float onePixelTexSpaceY = m_cellSizeTexSpaceSq / tileHeight;

		//Top left
		texCoords[0].x = 0.0f;
		texCoords[0].y = 0.0f;
		//Bottom left
		texCoords[1].x = 0.0f;
		texCoords[1].y = onePixelTexSpaceY;
		//Bottom right
		texCoords[2].x = onePixelTexSpaceX;
		texCoords[2].y = onePixelTexSpaceY;
		//Top right
		texCoords[3].x = onePixelTexSpaceX;
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

void RenderResources::GetCollisionTypeTexCoords(u16 collisionFlags, ion::render::TexCoord texCoords[4]) const
{
	const int numCollisionTypes = 4;

	u32 collisionTexSizeSq = max(1, (u32)ion::maths::Ceil(ion::maths::Sqrt((float)numCollisionTypes)));
	float cellSizeCollisionTexSpaceSq = 1.0f / (float)collisionTexSizeSq;

	//TODO: Only allows for one flag type
	int cellX = 0;
	int cellY = 0;

	if(collisionFlags & eCollisionTileFlagSolid)
	{
		cellX = 1;
	}
	
	ion::Vector2 textureBottomLeft(cellSizeCollisionTexSpaceSq * cellX, cellSizeCollisionTexSpaceSq * cellY);

	float top = textureBottomLeft.y + cellSizeCollisionTexSpaceSq;
	float left = textureBottomLeft.x;
	float bottom = textureBottomLeft.y;
	float right = textureBottomLeft.x + cellSizeCollisionTexSpaceSq;

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

void RenderResources::GetTerrainTileTexCoords(TerrainTileId tileId, ion::render::TexCoord texCoords[4]) const
{
	if(tileId == InvalidTerrainTileId)
	{
		//Use default tile if there is one
		tileId = m_project.GetDefaultTerrainTile();

		if(tileId == InvalidTerrainTileId)
		{
			//If no default tile, use blank tile
			tileId = (m_terrainTilesetSizeSq * m_terrainTilesetSizeSq) - 1;
		}
	}

	//Map tile to X/Y on tileset texture
	int tilesetX = (tileId % m_terrainTilesetSizeSq);
	int tilesetY = (tileId / m_terrainTilesetSizeSq);
	ion::Vector2 textureBottomLeft(m_cellSizeTerrainTilesetTexSpaceSq * tilesetX, m_cellSizeTerrainTilesetTexSpaceSq * tilesetY);
	
	float top = textureBottomLeft.y + m_cellSizeTerrainTilesetTexSpaceSq;
	float left = textureBottomLeft.x;
	float bottom = textureBottomLeft.y;
	float right = textureBottomLeft.x + m_cellSizeTerrainTilesetTexSpaceSq;
	
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
	if(m_textures[eTextureTileset])
	{
		if(Tile* tile = m_project.GetTileset().GetTile(tileId))
		{
			if(Palette* palette = m_project.GetPalette(tile->GetPaletteId()))
			{
				const int tileWidth = m_project.GetPlatformConfig().tileWidth;
				const int tileHeight = m_project.GetPlatformConfig().tileHeight;

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

void RenderResources::SetTerrainTileHeight(TerrainTileId terrainTileId, int x, s8 height)
{
	if(m_textures[eTextureTerrainTileset])
	{
		if(TerrainTile* tile = m_project.GetTerrainTileset().GetTerrainTile(terrainTileId))
		{
			ion::Colour setColour(0.0f, 1.0f, 0.0f, 0.7f);
			ion::Colour unsetColour(0.0f, 0.0f, 0.0f, 0.0f);

			const int tileWidth = m_project.GetPlatformConfig().tileWidth;
			const int tileHeight = m_project.GetPlatformConfig().tileHeight;

			const u32 tileX = terrainTileId % m_terrainTilesetSizeSq;
			const u32 tileY = terrainTileId / m_terrainTilesetSizeSq;

			//Draw all pixels up to height
			for(int y = 0; y < tileHeight; y++)
			{
				const ion::Colour& colour = (y < height) ? setColour : unsetColour;
				ion::Vector2i pixelPos((tileX * tileWidth) + x, (tileY * tileHeight) + y);
				m_textures[eTextureTerrainTileset]->SetPixel(pixelPos, colour);
			}
		}
	}
}

ion::Matrix4 RenderResources::CalcBoxMatrix(const ion::Vector2i& position, const ion::Vector2i& size, const ion::Vector2i& mapSize, float z)
{
	ion::Matrix4 matrix;

	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	float bottom = mapSize.y - position.y - size.y;
	ion::Vector2 tileSize(tileWidth, tileHeight);
	ion::Vector3 translation(floor((position.x - (mapSize.x / 2.0f) + (size.x / 2.0f)) * tileSize.x), floor((bottom - (mapSize.y / 2.0f) + (size.y / 2.0f)) * tileSize.y), z);
	matrix.SetTranslation(translation);
	matrix.SetScale(ion::Vector3(size.x, size.y, 1.0f));

	return matrix;
}

ion::render::Primitive* RenderResources::CreateBezierPrimitive(const ion::gamekit::BezierPath& bezier)
{
	const float granularity = 1.0f;
	const int maxPoints = bezier.GetNumPoints();
	const int numPoints = ion::maths::Floor((float)maxPoints / granularity);
	std::vector<ion::Vector2> points2d;
	std::vector<ion::Vector3> points3d;
	points2d.reserve(numPoints);
	points3d.reserve(numPoints);
	bezier.GetPositions(points2d, 0.0f, 1.0f, 100); // numPoints);

	for(int i = 0; i < points2d.size(); i++)
	{
		points3d.push_back(ion::Vector3(points2d[i].x, points2d[i].y, 0.0f));
	}

	return new ion::render::LineStrip(points3d);
}

ion::render::Primitive* RenderResources::CreateBezierControlsPrimitive(const ion::gamekit::BezierPath& bezier, float handleBoxHalfExtents)
{
	int numPoints = bezier.GetNumPoints();
	std::vector<ion::Vector3> points;
	points.reserve(numPoints * 28);

	for(int i = 0; i < numPoints; i++)
	{
		//Ignore first and last control handles (they're meaningless)
		bool drawHandleA = (i != 0);
		bool drawHandleB = (i != (numPoints - 1));

		ion::Vector2 position;
		ion::Vector2 controlA;
		ion::Vector2 controlB;

		bezier.GetPoint(i, position, controlA, controlB);
		controlA += position;
		controlB += position;

		//Lines
		if(drawHandleA)
		{
			points.push_back(ion::Vector3(position.x, position.y, 0.0f));
			points.push_back(ion::Vector3(controlA.x, controlA.y, 0.0f));
		}
		
		if(drawHandleB)
		{
			points.push_back(ion::Vector3(position.x, position.y, 0.0f));
			points.push_back(ion::Vector3(controlB.x, controlB.y, 0.0f));
		}

		//Boxes
		points.push_back(ion::Vector3(position.x - handleBoxHalfExtents, position.y - handleBoxHalfExtents, 0.0f));
		points.push_back(ion::Vector3(position.x + handleBoxHalfExtents, position.y - handleBoxHalfExtents, 0.0f));
		points.push_back(ion::Vector3(position.x - handleBoxHalfExtents, position.y - handleBoxHalfExtents, 0.0f));
		points.push_back(ion::Vector3(position.x - handleBoxHalfExtents, position.y + handleBoxHalfExtents, 0.0f));
		points.push_back(ion::Vector3(position.x + handleBoxHalfExtents, position.y - handleBoxHalfExtents, 0.0f));
		points.push_back(ion::Vector3(position.x + handleBoxHalfExtents, position.y + handleBoxHalfExtents, 0.0f));
		points.push_back(ion::Vector3(position.x - handleBoxHalfExtents, position.y + handleBoxHalfExtents, 0.0f));
		points.push_back(ion::Vector3(position.x + handleBoxHalfExtents, position.y + handleBoxHalfExtents, 0.0f));

		if(drawHandleA)
		{
			points.push_back(ion::Vector3(controlA.x - handleBoxHalfExtents, controlA.y - handleBoxHalfExtents, 0.0f));
			points.push_back(ion::Vector3(controlA.x + handleBoxHalfExtents, controlA.y - handleBoxHalfExtents, 0.0f));
			points.push_back(ion::Vector3(controlA.x - handleBoxHalfExtents, controlA.y - handleBoxHalfExtents, 0.0f));
			points.push_back(ion::Vector3(controlA.x - handleBoxHalfExtents, controlA.y + handleBoxHalfExtents, 0.0f));
			points.push_back(ion::Vector3(controlA.x + handleBoxHalfExtents, controlA.y - handleBoxHalfExtents, 0.0f));
			points.push_back(ion::Vector3(controlA.x + handleBoxHalfExtents, controlA.y + handleBoxHalfExtents, 0.0f));
			points.push_back(ion::Vector3(controlA.x - handleBoxHalfExtents, controlA.y + handleBoxHalfExtents, 0.0f));
			points.push_back(ion::Vector3(controlA.x + handleBoxHalfExtents, controlA.y + handleBoxHalfExtents, 0.0f));
		}

		if(drawHandleB)
		{
			points.push_back(ion::Vector3(controlB.x - handleBoxHalfExtents, controlB.y - handleBoxHalfExtents, 0.0f));
			points.push_back(ion::Vector3(controlB.x + handleBoxHalfExtents, controlB.y - handleBoxHalfExtents, 0.0f));
			points.push_back(ion::Vector3(controlB.x - handleBoxHalfExtents, controlB.y - handleBoxHalfExtents, 0.0f));
			points.push_back(ion::Vector3(controlB.x - handleBoxHalfExtents, controlB.y + handleBoxHalfExtents, 0.0f));
			points.push_back(ion::Vector3(controlB.x + handleBoxHalfExtents, controlB.y - handleBoxHalfExtents, 0.0f));
			points.push_back(ion::Vector3(controlB.x + handleBoxHalfExtents, controlB.y + handleBoxHalfExtents, 0.0f));
			points.push_back(ion::Vector3(controlB.x - handleBoxHalfExtents, controlB.y + handleBoxHalfExtents, 0.0f));
			points.push_back(ion::Vector3(controlB.x + handleBoxHalfExtents, controlB.y + handleBoxHalfExtents, 0.0f));
		}
	}

	return new ion::render::LineSegments(points);
}

RenderResources::SpriteSheetRenderResources::SpriteSheetRenderResources()
{
	m_primitive = NULL;
}

void RenderResources::SpriteSheetRenderResources::Load(const SpriteSheet& spriteSheet, ion::render::Shader* pixelshader, ion::render::Shader* vertexShader, Project* project)
{
	const int tileWidth = project->GetPlatformConfig().tileWidth;
	const int tileHeight = project->GetPlatformConfig().tileHeight;

	m_primitive = new ion::render::Chessboard(ion::render::Chessboard::xy, ion::Vector2((float)spriteSheet.GetWidthTiles() * (tileWidth / 2.0f), (float)spriteSheet.GetHeightTiles() * (tileHeight / 2.0f)), spriteSheet.GetWidthTiles(), spriteSheet.GetHeightTiles(), true);

	u32 widthTiles = spriteSheet.GetWidthTiles();
	u32 heightTiles = spriteSheet.GetHeightTiles();
	u32 textureWidth = widthTiles * tileWidth;
	u32 textureHeight = heightTiles * tileHeight;
	u32 bytesPerPixel = 4;
	u32 textureSize = textureWidth * textureHeight * bytesPerPixel;

	const Palette& palette = spriteSheet.GetPalette();

	for(int i = 0; i < spriteSheet.GetNumFrames(); i++)
	{
		//Get spriteSheet frame
		const SpriteSheetFrame& spriteSheetFrame = spriteSheet.GetFrame(i);

		//Create new render frame
		Frame renderFrame;

		//Create tileset texture for frame
		renderFrame.texture = ion::render::Texture::Create();

		u8* data = new u8[textureSize];
		ion::memory::MemSet(data, 0, textureSize);

		for(int tileX = 0; tileX < spriteSheet.GetWidthTiles(); tileX++)
		{
			for(int tileY = 0; tileY < spriteSheet.GetHeightTiles(); tileY++)
			{
				//Genesis spriteSheet order = column major
				const Tile& tile = spriteSheetFrame[(tileX * heightTiles) + tileY];

				//Invert Y for OpenGL
				int tileY_inv = spriteSheet.GetHeightTiles() - 1 - tileY;

				//Paint tile to texture
				for(int pixelY = 0; pixelY < tileHeight; pixelY++)
				{
					for(int pixelX = 0; pixelX < tileWidth; pixelX++)
					{
						//Invert Y for OpenGL
						int pixelY_OGL = tileHeight - 1 - pixelY;

						u8 colourIdx = tile.GetPixelColour(pixelX, pixelY_OGL);

						const Colour& colour = palette.GetColour(colourIdx);

						int destPixelX = (tileX * tileWidth) + pixelX;
						int destPixelY = (tileY_inv * tileHeight) + pixelY;
						u32 pixelIdx = (destPixelY * textureWidth) + destPixelX;
						u32 dataOffset = pixelIdx * bytesPerPixel;
						ion::debug::Assert(dataOffset + 2 < textureSize, "eOut of bounds");
						data[dataOffset] = colour.GetRed();
						data[dataOffset + 1] = colour.GetGreen();
						data[dataOffset + 2] = colour.GetBlue();
						data[dataOffset + 3] = colourIdx > 0 ? 255 : 0;
					}
				}

				//Set UV coords on primitive
				ion::render::TexCoord coords[4];
				ion::Vector2 textureBottomLeft((1.0f / (float)widthTiles) * tileX, (1.0f / (float)heightTiles) * tileY);

				float top = textureBottomLeft.y + (1.0f / (float)heightTiles);
				float left = textureBottomLeft.x;
				float bottom = textureBottomLeft.y;
				float right = textureBottomLeft.x + (1.0f / (float)widthTiles);

				//Top left
				coords[0].x = left;
				coords[0].y = top;
				//Bottom left
				coords[1].x = left;
				coords[1].y = bottom;
				//Bottom right
				coords[2].x = right;
				coords[2].y = bottom;
				//Top right
				coords[3].x = right;
				coords[3].y = top;

				m_primitive->SetTexCoords((tileY * widthTiles) + tileX, coords);
			}
		}

		renderFrame.texture->Load(textureWidth, textureHeight, ion::render::Texture::eRGBA, ion::render::Texture::eRGBA, ion::render::Texture::eBPP24, false, data);
		renderFrame.texture->SetMinifyFilter(ion::render::Texture::eFilterNearest);
		renderFrame.texture->SetMagnifyFilter(ion::render::Texture::eFilterNearest);
		renderFrame.texture->SetWrapping(ion::render::Texture::eWrapClamp);

		//Create material
		renderFrame.material = new ion::render::Material();
		renderFrame.material->AddDiffuseMap(renderFrame.texture);
		renderFrame.material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f));
		renderFrame.material->SetVertexShader(vertexShader);
		renderFrame.material->SetPixelShader(pixelshader);

		//Insert frame
		m_frames.push_back(renderFrame);

		delete data;
	}
}

RenderResources::SpriteSheetRenderResources::~SpriteSheetRenderResources()
{
	for(int i = 0; i < m_frames.size(); i++)
	{
		delete m_frames[i].material;
		delete m_frames[i].texture;
	}

	if(m_primitive)
	{
		delete m_primitive;
	}
}

void RenderResources::CreateSpriteSheetResources(const Project& project)
{
	m_spriteSheetRenderResources.clear();

	for(TActorMap::const_iterator actorIt = project.ActorsBegin(), actorEnd = project.ActorsEnd(); actorIt != actorEnd; ++actorIt)
	{
		for(TSpriteSheetMap::const_iterator it = actorIt->second.SpriteSheetsBegin(), end = actorIt->second.SpriteSheetsEnd(); it != end; ++it)
		{
			CreateSpriteSheetResources(it->first, it->second);
		}
	}
}

void RenderResources::CreateSpriteSheetResources(SpriteSheetId spriteSheetId, const SpriteSheet& spriteSheet)
{
	m_spriteSheetRenderResources.erase(spriteSheetId);
	std::pair<std::map<SpriteSheetId, SpriteSheetRenderResources>::iterator, bool> it = m_spriteSheetRenderResources.insert(std::make_pair(spriteSheetId, SpriteSheetRenderResources()));
	it.first->second.Load(spriteSheet, m_pixelShaders[eShaderFlatTextured], m_vertexShaders[eShaderFlatTextured], &m_project);
}

void RenderResources::DeleteSpriteSheetRenderResources(SpriteSheetId spriteSheetId)
{
	m_spriteSheetRenderResources.erase(spriteSheetId);
}

RenderResources::SpriteSheetRenderResources* RenderResources::GetSpriteSheetResources(SpriteSheetId spriteSheetId)
{
	SpriteSheetRenderResources* spriteSheetResources = NULL;

	std::map<SpriteSheetId, SpriteSheetRenderResources>::iterator it = m_spriteSheetRenderResources.find(spriteSheetId);
	if(it != m_spriteSheetRenderResources.end())
	{
		spriteSheetResources = &it->second;
	}

	return spriteSheetResources;
}