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
		eMaterialMax
	};

	enum TextureType
	{
		eTextureTileset,
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
		ePrimitiveMax
	};

	RenderResources();
	~RenderResources();

	//Set project
	void SetProject(Project* project) { m_project = project; }

	//Create and redraw tileset texture
	void CreateTilesetTexture();

	//Get tileset UV coords for tile
	void GetTileTexCoords(TileId tileId, ion::render::TexCoord texCoords[4], u32 flipFlags) const;

	//Edit tileset texture pixel
	void SetTilesetTexPixel(TileId tileId, const ion::Vector2i& pixel, u8 colourIdx);

	//Get resources
	ion::render::Material* GetMaterial(MaterialType type) { return m_materials[type]; }
	ion::render::Shader* GetVertexShader(ShaderType type) { return m_vertexShaders[type]; }
	ion::render::Shader* GetPixelhader(ShaderType type) { return m_pixelShaders[type]; }
	const ion::render::Primitive* GetPrimitive(PrimitiveType type) const { return m_primitives[type]; }
	const ion::Colour& GetColour(ColourType type) const { return m_colours[type]; }

private:
	//Create TileID to index cache
	void CacheTileIndices(const Tileset& tileset);

	//Get tile index into tileset
	int GetTilesetTexTileIndex(TileId tileId) const;

	//Map tile IDs to indices
	std::map<TileId, u32> m_tileIndexMap;

	//Tileset size sq
	u32 m_tilesetSizeSq;

	//Tileset texture cell size sq
	float m_cellSizeTexSpaceSq;

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