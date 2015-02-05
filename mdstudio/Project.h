#pragma once

#include <io/Archive.h>
#include <string>

#include "Map.h"

class Project
{
public:
	Project();

	//Load/save project
	bool Load(const std::string& filename) { return false; }
	bool Save(const std::string& filename) { return false; }

	//Get Genesis map
	Map& GetMap() { return m_map; }

	//Set current tile used for painting
	void SetPaintTile(TileId tile);
	TileId GetPaintTile() const;

	//Set current tile used for erasing
	void SetEraseTile(TileId tile);
	TileId GetEraseTile() const;

	void Serialise(ion::io::Archive& archive) {}

private:
	//Genesis map
	Map m_map;

	//Tile used for painting
	TileId m_paintTile;

	//Tile used for erasing
	TileId m_eraseTile;
};