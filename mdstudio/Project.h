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

class Project
{
public:
	Project();

	//Load/save project
	bool Load(const std::string& filename) { return false; }
	bool Save(const std::string& filename) { return false; }

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

	void Serialise(ion::io::Archive& archive) {}

private:
	//Genesis map
	Map m_map;

	//Palettes
	//TODO: Add/remove palettes
	Palette m_palettes[4];

	//Tile used for painting
	TileId m_paintTile;

	//Tile used for erasing
	TileId m_eraseTile;

	//Map needs redraw
	bool m_mapInvalidated;
};