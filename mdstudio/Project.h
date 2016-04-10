///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <io/Archive.h>
#include <string>
#include <vector>
#include <set>

#include <ion/gamekit/Bezier.h>

#include "Map.h"
#include "Palette.h"
#include "Stamp.h"
#include "Tile.h"
#include "Tileset.h"
#include "CollisionMap.h"
#include "TerrainTile.h"
#include "TerrainTileset.h"
#include "GameObject.h"

typedef std::map<StampId, Stamp> TStampMap;
typedef std::map<GameObjectTypeId, GameObjectType> TGameObjectTypeMap;

class Project
{
	//TODO: Add/remove palettes
	static const int s_maxPalettes = 4;

public:
	enum BMPImportFlags
	{
		eBMPImportClearPalettes	= (1 << 0),
		eBMPImportClearTiles	= (1 << 1),
		eBMPImportClearMap		= (1 << 2),
		eBMPImportDrawToMap		= (1 << 3),
		eBMPImportToStamp		= (1 << 4),
		eBMPImportToSprite		= (1 << 5),
		eBMPImportWholePalette	= (1 << 6),
		eBMPImportInsertBGTile	= (1 << 7),
	};

	Project();

	//Load/save project
	bool Load(const std::string& filename);
	bool Save(const std::string& filename);

	//Get/set name
	const std::string& GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

	//Get filename
	const std::string& GetFilename() { return m_filename; }

	//Clear project (palettes, tiles, map)
	void Clear();

	//Get map
	Map& GetMap() { return m_map; }

	//Get collision map
	CollisionMap& GetCollisionMap() { return m_collisionMap; }

	//Get tileset
	Tileset& GetTileset() { return m_tileset; }
	const Tileset& GetTileset() const { return m_tileset; }

	//Get terrain tileset
	TerrainTileset& GetTerrainTileset() { return m_terrainTileset; }
	const TerrainTileset& GetTerrainTileset() const { return m_terrainTileset; }

	//Get palette
	Palette* GetPalette(PaletteId paletteId) { return &m_palettes[paletteId]; }
	int GetNumPalettes() const { return s_maxPalettes; }

	//Palette slots
	int AddPaletteSlot(Palette& palette);
	Palette* GetPaletteSlot(int slotIndex);
	int GetNumPaletteSlots() const;
	void SetActivePaletteSlot(PaletteId paletteId, int slotIndex);
	void ExportPaletteSlots(const std::string& filename);
	void ImportPaletteSlots(const std::string& filename);

	//Tiles
	void DeleteTile(TileId tileId);
	void SwapTiles(TileId tileId1, TileId tileId2);
	void SetBackgroundTile(TileId tileId);
	TileId GetBackgroundTile() const { return m_backgroundTile; }
	int CleanupTiles();

	//Stamps
	StampId AddStamp(int width, int height);
	void DeleteStamp(StampId stampId);
	Stamp* GetStamp(StampId stampId);
	const Stamp* GetStamp(StampId stampId) const;
	const TStampMap::const_iterator StampsBegin() const;
	const TStampMap::const_iterator StampsEnd() const;
	int GetStampCount() const;

	//Collision tiles
	void DeleteTerrainTile(TerrainTileId tileId);
	void SwapTerrainTiles(TerrainTileId tileId1, TerrainTileId tileId2);
	void SetDefaultTerrainTile(TerrainTileId tileId);
	TerrainTileId GetDefaultTerrainTile() const { return m_defaultTerrainTile; }
	int CleanupTerrainTiles(bool prompt);

	//Collision beziers
	ion::gamekit::BezierPath* AddTerrainBezier();
	ion::gamekit::BezierPath* GetTerrainBezier(u32 index);
	void RemoveTerrainBezier(u32 index);
	int GetNumTerrainBeziers() const;

	//Terrain generation from bezier paths
	void GenerateTerrainFromBeziers();

	//Terrain generation from graphic tiles
	void GenerateTerrain(const std::vector<ion::Vector2i>& graphicTiles);

	//Game object types
	GameObjectTypeId AddGameObjectType();
	void RemoveGameObjectType(GameObjectTypeId typeId);
	GameObjectType* GetGameObjectType(GameObjectTypeId typeId);
	const GameObjectType* GetGameObjectType(GameObjectTypeId typeId) const;
	const TGameObjectTypeMap& GetGameObjectTypes() const;
	void ExportGameObjectTypes(const std::string& filename);
	void ImportGameObjectTypes(const std::string& filename);

	//Set current colour used for editing
	void SetPaintColour(u8 colourIdx);
	u8 GetPaintColour() const;

	//Set current collision tile used for painting
	void SetPaintTerrainTile(TerrainTileId tile);
	TerrainTileId GetPaintTerrainTile() const;

	//Set current tile used for painting
	void SetPaintTile(TileId tile);
	TileId GetPaintTile() const;

	//Set current tile used for erasing
	void SetEraseTile(TileId tile);
	TileId GetEraseTile() const;

	//Set current stamp used for painting
	void SetPaintStamp(StampId stamp);
	StampId GetPaintStamp() const;

	//Set current game object type for placing
	void SetPaintGameObjectType(GameObjectTypeId typeId);
	GameObjectTypeId GetPaintGameObjectType() const;

	//Grid
	int GetGridSize() const { return m_gridSize; }
	void SetGridSize(int gridSize) { m_gridSize = gridSize; }
	bool GetShowGrid() const { return m_showGrid; }
	void SetShowGrid(bool show) { m_showGrid = show; }
	bool GetGridSnap() const { return m_snapToGrid; }
	void SetGridSnap(bool snap) { m_snapToGrid = snap; }

	//Stamp outlines
	bool GetShowStampOutlines() const { return m_showStampOutlines; }
	void SetShowStampOutlines(bool show) { m_showStampOutlines = show; }

	void InvalidateMap(bool invalidate) { m_mapInvalidated = invalidate; }
	void InvalidateTiles(bool invalidate) { m_tilesInvalidated = invalidate; }
	void InvalidateTerrainTiles(bool invalidate) { m_terrainTilesInvalidated = invalidate; }
	void InvalidateTerrainBeziers(bool invalidate) { m_terrainBeziersInvalidated = invalidate; }
	void InvalidateStamps(bool invalidate) { m_stampsInvalidated = invalidate; }
	bool MapIsInvalidated() const { return m_mapInvalidated; }
	bool TilesAreInvalidated() const { return m_tilesInvalidated; }
	bool TerrainTilesAreInvalidated() const { return m_terrainTilesInvalidated; }
	bool TerrainBeziersAreInvalidated() const { return m_terrainBeziersInvalidated; }
	bool StampsAreInvalidated() const { return m_stampsInvalidated; }

	//Import bitmap
	bool ImportBitmap(const std::string& filename, u32 importFlags, u32 paletteBits);

	//Export
	bool ExportPalettes(const std::string& filename) const;
	bool ExportTiles(const std::string& filename, bool binary) const;
	bool ExportMap(const std::string& filename, bool binary) const;
	bool ExportTerrainTiles(const std::string& filename, bool binary) const;
	bool ExportCollisionMap(const std::string& filename, bool binary) const;
	bool ExportGameObjects(const std::string& filename) const;

	//Serialise
	void Serialise(ion::io::Archive& archive);

	//Last used export filenames
	struct ExportFilenames
	{
		std::string palettes;
		std::string tileset;
		std::string map;
		std::string TerrainTiles;
		std::string collisionMap;
		std::string gameObjects;

		void Serialise(ion::io::Archive& archive)
		{
			archive.Serialise(palettes, "exportFNamePalettes");
			archive.Serialise(tileset, "exportFNameTileset");
			archive.Serialise(map, "exportFNameMap");
			archive.Serialise(TerrainTiles, "exportFNameTerrainTiles");
			archive.Serialise(collisionMap, "exportFNameCollisionMap");
			archive.Serialise(gameObjects, "exportFNameGameObjects");
		}
	};
	
	ExportFilenames m_exportFilenames;

private:
	//Find palette matching 8x8 colour grid
	bool FindPalette(Colour* pixels, u32 useablePalettes, PaletteId& paletteId, PaletteId& closestPalette, int& closestColourCount) const;
	bool ImportPalette(Colour* pixels, Palette& palette);
	bool MergePalettes(Palette& dest, const Palette& source);

	void WriteFileHeader(std::stringstream& stream) const;

	//Collapse palette slots to palettes in use
	void CollapsePaletteSlots();

	//Generate heighmap from graphic tile
	void GenerateHeightMap(const Tile& graphicTile, std::vector<u8>& heightMap) const;
	void GenerateCeilingMap(const Tile& graphicTile, std::vector<u8>& ceilingMap) const;

	//Get tile at position (including on stamps)
	TileId GetTileAtPosition(const ion::Vector2i& position);

	//Project name
	std::string m_name;

	//Filename
	std::string m_filename;

	//Tileset
	Tileset m_tileset;

	//terrain tileset
	TerrainTileset m_terrainTileset;

	//Map
	Map m_map;

	//Collision map
	CollisionMap m_collisionMap;

	//Terrain beziers
	std::vector<ion::gamekit::BezierPath> m_terrainBeziers;

	//Active palettes
	std::vector<Palette> m_palettes;

	//Saved palette slots
	std::vector<Palette> m_paletteSlots;

	//Stamps
	TStampMap m_stamps;
	StampId m_nextFreeStampId;

	//Game object types
	TGameObjectTypeMap m_gameObjectTypes;
	GameObjectTypeId m_nextFreeGameObjectTypeId;

	//Colour used for painting
	u8 m_paintColour;

	//Collision tile used for painting
	TerrainTileId m_paintTerrainTile;

	//Tile used for painting
	TileId m_paintTile;

	//Stamp used for painting
	StampId m_paintStamp;

	//Tile used for erasing
	TileId m_eraseTile;

	//Game object used for placing
	GameObjectTypeId m_paintGameObjectType;

	//Background tile (replaced InvalidTileId on export)
	TileId m_backgroundTile;

	//Default collision tile (replaces InvalidTerrainTileId on export)
	TerrainTileId m_defaultTerrainTile;

	//Grid
	int m_gridSize;
	bool m_showGrid;
	bool m_snapToGrid;

	//Stamp outlines
	bool m_showStampOutlines;

	//Map needs redraw
	bool m_mapInvalidated;
	bool m_tilesInvalidated;
	bool m_terrainTilesInvalidated;
	bool m_terrainBeziersInvalidated;
	bool m_stampsInvalidated;
};