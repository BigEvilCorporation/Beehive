#pragma once

#include <map>
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
	
private:
	TileId m_nextFreeId;
	
	TileMap m_tiles;
};