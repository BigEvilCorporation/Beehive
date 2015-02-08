///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <map>
#include <sstream>
#include <io/Archive.h>

#include "Tile.h"

typedef unsigned int TileId;
typedef std::map<TileId, Tile> TileMap;

class Tileset
{
public:
	Tileset();

	TileId AddTile();
	void RemoveTile(TileId tileId);
	TileId FindDuplicate(const Tile& tile) const;

	Tile* GetTile(TileId tileId);
	const TileMap::const_iterator Begin() const;
	const TileMap::const_iterator End() const;
	int GetCount() const;

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& outputText);
	
private:
	TileId m_nextFreeId;
	
	TileMap m_tiles;
};