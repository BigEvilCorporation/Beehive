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
typedef std::map<TileId, Tile> TTileMap;

class Tileset
{
public:
	Tileset();

	void Clear();

	TileId AddTile();
	void RemoveTile(TileId tileId);
	TileId FindDuplicate(const Tile& tile) const;

	Tile* GetTile(TileId tileId);
	const TTileMap::const_iterator Begin() const;
	const TTileMap::const_iterator End() const;
	int GetCount() const;

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& stream) const;
	
private:
	TileId m_nextFreeId;
	
	TTileMap m_tiles;
};