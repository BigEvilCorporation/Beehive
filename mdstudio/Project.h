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

typedef std::map<StampId, Stamp> TStampMap;

class Project
{
public:
	enum BMPImportFlags
	{
		eBMPImportReplaceAll	= (1<<0),
		eBMPImportDrawToMap		= (1<<1),
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

	//Get tileset
	Tileset& GetTileset() { return m_tileset; }
	const Tileset& GetTileset() const { return m_tileset; }

	//Get palette
	Palette* GetPalette(PaletteId paletteId) { return &m_palettes[paletteId]; }

	//Stamps
	StampId AddStamp(int width, int height);
	void RemoveStamp(StampId stampId);
	Stamp* GetStamp(StampId stampId);
	const Stamp* GetStamp(StampId stampId) const;
	const TStampMap::const_iterator StampsBegin() const;
	const TStampMap::const_iterator StampsEnd() const;
	int GetStampCount() const;

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

	void InvalidateMap(bool invalidate) { m_mapInvalidated = invalidate; }
	void InvalidateTiles(bool invalidate) { m_tilesInvalidated = invalidate; }
	void InvalidateStamps(bool invalidate) { m_stampsInvalidated = invalidate; }
	bool MapIsInvalidated() const { return m_mapInvalidated; }
	bool TilesAreInvalidated() const { return m_tilesInvalidated; }
	bool StampsAreInvalidated() const { return m_stampsInvalidated; }

	//Import bitmap
	bool ImportBitmap(const std::string& filename, u8 importFlags = (BMPImportFlags)(0));

	//Export
	bool ExportPalettes(const std::string& filename) const;
	bool ExportTiles(const std::string& filename) const;
	bool ExportCollision(const std::string& filename) const;
	bool ExportMap(const std::string& filename) const;

	//Serialise
	void Serialise(ion::io::Archive& archive);

private:
	//Find palette matching 8x8 colour grid
	bool FindPalette(Colour* pixels, PaletteId& paletteId, PaletteId& closestPalette, int& closestColourCount) const;
	bool ImportPalette(Colour* pixels, Palette& palette);
	bool MergePalettes(Palette& dest, const Palette& source);

	//Project name
	std::string m_name;

	//Filename
	std::string m_filename;

	//Tileset
	Tileset m_tileset;

	//Map
	Map m_map;

	//Palettes
	//TODO: Add/remove palettes
	static const int numPalettes = 4;
	std::vector<Palette> m_palettes;

	//Stamps
	TStampMap m_stamps;
	StampId m_nextFreeStampId;

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

	//Map needs redraw
	bool m_mapInvalidated;
	bool m_tilesInvalidated;
	bool m_stampsInvalidated;
};