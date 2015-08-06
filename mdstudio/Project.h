///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <io/Archive.h>
#include <string>
#include <vector>

#include "Map.h"
#include "Palette.h"
#include "Stamp.h"
#include "Tile.h"
#include "Tileset.h"
#include "CollisionMap.h"
#include "CollisionTile.h"
#include "CollisionTileset.h"
#include "CollisionType.h"

typedef std::map<StampId, Stamp> TStampMap;
typedef std::map<u8, CollisionType> TCollisionTypeMap;

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

	//Get collision tileset
	CollisionTileset& GetCollisionTileset() { return m_collisionTileset; }
	const CollisionTileset& GetCollisionTileset() const { return m_collisionTileset; }

	//Get palette
	Palette* GetPalette(PaletteId paletteId) { return &m_palettes[paletteId]; }
	int GetNumPalettes() const { return s_maxPalettes; }

	//Palette slots
	int AddPaletteSlot(Palette& palette);
	Palette* GetPaletteSlot(int slotIndex);
	int GetNumPaletteSlots() const;
	void SetActivePaletteSlot(PaletteId paletteId, int slotIndex);

	//Stamps
	StampId AddStamp(int width, int height);
	void RemoveStamp(StampId stampId);
	Stamp* GetStamp(StampId stampId);
	const Stamp* GetStamp(StampId stampId) const;
	const TStampMap::const_iterator StampsBegin() const;
	const TStampMap::const_iterator StampsEnd() const;
	int GetStampCount() const;

	//Collision types
	CollisionType* AddCollisionType(u8 bit);
	void RemoveCollisionType(u8 bit);
	CollisionType* GetCollisionType(const std::string& name);
	CollisionType* GetCollisionType(u8 bit);
	const TCollisionTypeMap::const_iterator CollisionTypesBegin() const;
	const TCollisionTypeMap::const_iterator CollisionTypesEnd() const;
	int GetCollisionTypeCount() const;

	//Set current colour used for editing
	void SetPaintColour(u8 colourIdx);
	u8 GetPaintColour() const;

	//Set current collision type used for painting
	void SetPaintCollisionType(CollisionType* type);
	const CollisionType* GetPaintCollisionType() const;

	//Set current collision tile used for painting
	void SetPaintCollisionTile(CollisionTileId tile);
	CollisionTileId GetPaintCollisionTile() const;

	//Set current tile used for painting
	void SetPaintTile(TileId tile);
	TileId GetPaintTile() const;

	//Set current tile used for erasing
	void SetEraseTile(TileId tile);
	TileId GetEraseTile() const;

	//Set current stamp used for painting
	void SetPaintStamp(StampId stamp);
	StampId GetPaintStamp() const;

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
	void InvalidateCollisionTiles(bool invalidate) { m_collisionTilesInvalidated = invalidate; }
	void InvalidateCollisionTypes(bool invalidate) { m_collisionTypesInvalidated = invalidate; }
	void InvalidateStamps(bool invalidate) { m_stampsInvalidated = invalidate; }
	bool MapIsInvalidated() const { return m_mapInvalidated; }
	bool TilesAreInvalidated() const { return m_tilesInvalidated; }
	bool CollisionTilesAreInvalidated() const { return m_collisionTilesInvalidated; }
	bool CollisionTypesAreInvalidated() const { return m_collisionTypesInvalidated; }
	bool StampsAreInvalidated() const { return m_stampsInvalidated; }

	//Import bitmap
	bool ImportBitmap(const std::string& filename, u32 importFlags, u32 paletteBits);

	//Export
	bool ExportPalettes(const std::string& filename) const;
	bool ExportTiles(const std::string& filename) const;
	bool ExportMap(const std::string& filename) const;
	bool ExportCollisionTiles(const std::string& filename) const;
	bool ExportCollisionMap(const std::string& filename) const;

	//Serialise
	void Serialise(ion::io::Archive& archive);

	//Last used export filenames
	struct ExportFilenames
	{
		std::string palettes;
		std::string tileset;
		std::string map;
		std::string collisionTiles;
		std::string collisionMap;

		void Serialise(ion::io::Archive& archive)
		{
			//TEMP: Wrap strings in block until they get their own serialise tags (need to update shader file format to support it first)
			if(archive.PushBlock("exportFilenames"))
			{
				archive.Serialise(palettes, "exportFNamePalettes");
				archive.Serialise(tileset, "exportFNameTileset");
				archive.Serialise(map, "exportFNameMap");
				archive.Serialise(collisionTiles, "exportFNameCollisionTiles");
				archive.Serialise(collisionMap, "exportFNameCollisionMap");
				archive.PopBlock();
			}
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

	//Project name
	std::string m_name;

	//Filename
	std::string m_filename;

	//Tileset
	Tileset m_tileset;

	//Collision tileset
	CollisionTileset m_collisionTileset;

	//Map
	Map m_map;

	//Collision map
	CollisionMap m_collisionMap;

	//Active palettes
	std::vector<Palette> m_palettes;

	//Saved palette slots
	std::vector<Palette> m_paletteSlots;

	//Stamps
	TStampMap m_stamps;
	StampId m_nextFreeStampId;

	//Collision types
	TCollisionTypeMap m_collisionTypes;

	//Colour used for painting
	u8 m_paintColour;

	//Collision type used for painting
	CollisionType* m_paintCollisionType;

	//Collision tile used for painting
	CollisionTileId m_paintCollisionTile;

	//Tile used for painting
	TileId m_paintTile;

	//Stamp used for painting
	StampId m_paintStamp;

	//Tile used for erasing
	TileId m_eraseTile;

	//Grid
	int m_gridSize;
	bool m_showGrid;
	bool m_snapToGrid;

	//Stamp outlines
	bool m_showStampOutlines;

	//Map needs redraw
	bool m_mapInvalidated;
	bool m_tilesInvalidated;
	bool m_collisionTilesInvalidated;
	bool m_collisionTypesInvalidated;
	bool m_stampsInvalidated;
};