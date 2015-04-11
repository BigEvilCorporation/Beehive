///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <io/Archive.h>
#include <string>

#include "Map.h"
#include "Palette.h"
#include "Tile.h"

class Project
{
public:
	enum BMPImportFlags
	{
		BMPImportReplaceAll	= (1<<0),
		BMPImportDrawToMap	= (1<<1),
	};

	Project();

	//Load/save project
	bool Load(const std::string& filename) { return false; }
	bool Save(const std::string& filename) { return false; }

	//Clear project (palettes, tiles, map)
	void Clear();

	//Get Genesis map
	Map& GetMap() { return m_map; }

	//Get palette
	Palette* GetPalette(PaletteId paletteId) { return &m_palettes[paletteId]; }

	//Set current tile used for painting
	void SetPaintTile(TileId tile);
	TileId GetPaintTile() const;

	//Set current tile used for erasing
	void SetEraseTile(TileId tile);
	TileId GetEraseTile() const;

	void InvalidateMap(bool invalidate) { m_mapInvalidated = invalidate; }
	bool MapIsInvalidated() const { return m_mapInvalidated; }

	//Import bitmap
	bool ImportBitmap(const std::string& filename, u8 importFlags = (BMPImportFlags)(0));

	//Serialise
	void Serialise(ion::io::Archive& archive) {}

private:
	//Find palette matching 8x8 colour grid
	bool FindPalette(Colour* colours, PaletteId& paletteId) const;
	bool ImportPalette(Colour* colours, PaletteId paletteId);

	//Genesis map
	Map m_map;

	//Palettes
	//TODO: Add/remove palettes
	static const int numPalettes = 4;
	Palette m_palettes[numPalettes];

	//Tile used for painting
	TileId m_paintTile;

	//Tile used for erasing
	TileId m_eraseTile;

	//Map needs redraw
	bool m_mapInvalidated;
};