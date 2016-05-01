///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <ion/maths/Vector.h>
#include <ion/renderer/Material.h>
#include <ion/renderer/Primitive.h>
#include <ion/renderer/Renderer.h>
#include <ion/renderer/Shader.h>
#include <ion/renderer/Texture.h>

#include "../Project.h"
#include "../BMPReader.h"

class RenderResources
{
public:
	enum ShaderType
	{
		eShaderFlatColour,
		eShaderFlatTextured,
		eShaderMax
	};

	enum MaterialType
	{
		eMaterialFlatColour,
		eMaterialTileset,
		eMaterialCollisionTypes,
		eMaterialTerrainTileset,
		eMaterialSprite,
		eMaterialMax
	};

	enum TextureType
	{
		eTextureTileset,
		eTextureCollisionTypes,
		eTextureTerrainTileset,
		eTextureSpritePreview,
		eTextureMax
	};

	enum ColourType
	{
		eColourHighlight,
		eColourSelected,
		eColourPreview,
		eColourOutline,
		eColourGrid,
		eColourMax
	};

	enum PrimitiveType
	{
		ePrimitiveUnitQuad,
		ePrimitiveUnitLineQuad,
		ePrimitiveMax
	};

	RenderResources();
	~RenderResources();

	//Set project
	void SetProject(Project* project) { m_project = project; }

	//Create and redraw tileset texture
	void CreateTilesetTexture();

	//Create and redraw terrain tileset texture
	void CreateTerrainTilesTexture();

	//Create and redraw collision texture
	void CreateCollisionTypesTexture();

	//Create sprite preview texture
	ion::render::Texture* CreateSpritePreviewTexture(const BMPReader& reader);

	//Get tileset UV coords for tile
	void GetTileTexCoords(TileId tileId, ion::render::TexCoord texCoords[4], u32 flipFlags) const;

	//Get terrain tileset UV coords for collision type
	void GetCollisionTypeTexCoords(u16 collisionFlags, ion::render::TexCoord texCoords[4]) const;

	//Get terrain tileset UV coords for terrain tile
	void GetTerrainTileTexCoords(TerrainTileId tileId, ion::render::TexCoord texCoords[4]) const;

	//Edit tileset texture pixel
	void SetTilesetTexPixel(TileId tileId, const ion::Vector2i& pixel, u8 colourIdx);

	//Edit terrain tileset texture height
	void SetTerrainTileHeight(TerrainTileId terrainTileId, int x, s8 height);

	//Get resources
	ion::render::Material* GetMaterial(MaterialType type) { return m_materials[type]; }
	ion::render::Shader* GetVertexShader(ShaderType type) { return m_vertexShaders[type]; }
	ion::render::Shader* GetPixelhader(ShaderType type) { return m_pixelShaders[type]; }
	ion::render::Primitive* GetPrimitive(PrimitiveType type) const { return m_primitives[type]; }
	const ion::Colour& GetColour(ColourType type) const { return m_colours[type]; }

	//Rendering util functions
	ion::Matrix4 CalcBoxMatrix(const ion::Vector2i& position, const ion::Vector2i& size, const ion::Vector2i& mapSize, float z);
	ion::render::Primitive* CreateBezierPrimitive(const ion::gamekit::BezierPath& bezier);
	ion::render::Primitive* CreateBezierControlsPrimitive(const ion::gamekit::BezierPath& bezier, float handleBoxHalfExtents);

private:
	
	//Map tile IDs to indices
	std::map<TileId, u32> m_tileIndexMap;

	//Tileset size sq
	u32 m_tilesetSizeSq;

	//terrain tileset size sq
	u32 m_terrainTilesetSizeSq;

	//Tileset texture cell size sq
	float m_cellSizeTexSpaceSq;

	//terrain tileset texture cell size sq
	float m_cellSizeTerrainTilesetTexSpaceSq;

	//Current project
	Project* m_project;

	//Resources
	ion::render::Shader* m_vertexShaders[eShaderMax];
	ion::render::Shader* m_pixelShaders[eShaderMax];
	ion::render::Texture* m_textures[eTextureMax];
	ion::render::Material* m_materials[eMaterialMax];
	ion::render::Primitive* m_primitives[ePrimitiveMax];
	ion::Colour m_colours[eColourMax];
};