///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <io/Archive.h>

#include "Tile.h"

class Tileset
{
	typedef std::map<u64, TileId> HashMap;

public:
	Tileset();

	void Clear();

	TileId AddTile();
	void PopBackTile();
	void HashChanged(TileId tileId);
	TileId FindDuplicate(const Tile& tile, u32& tileFlags) const;

	Tile* GetTile(TileId tileId);
	const Tile* GetTile(TileId tileId) const;
	int GetCount() const;

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& stream) const;
	
private:
	enum HashOrientation
	{
		eNormal,
		eFlipX,
		eFlipY,
		eFlipXY,

		eNumHashOrientations
	};

	static const u32 s_orientationFlags[eNumHashOrientations];

	void AddToHashMap(TileId tileId);
	void RemoveFromHashMap(TileId tileId);
	void CalculateHashes(const Tile& tile, u64 hashes[eNumHashOrientations]) const;
	
	std::vector<Tile> m_tiles;
	HashMap m_hashMap;
};