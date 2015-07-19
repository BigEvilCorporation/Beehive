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

#include "CollisionTile.h"

class CollisionTileset
{
	typedef std::map<u64, CollisionTileId> HashMap;

public:
	CollisionTileset();

	void Clear();

	CollisionTileId AddCollisionTile();
	void RemoveCollisionTile(CollisionTileId tileId);
	void HashChanged(CollisionTileId tileId);
	CollisionTileId FindDuplicate(const CollisionTile& tile) const;

	CollisionTile* GetCollisionTile(CollisionTileId tileId);
	const CollisionTile* GetCollisionTile(CollisionTileId tileId) const;
	int GetCount() const;

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& stream) const;
	
private:
	void AddToHashMap(CollisionTileId tileId);
	void RemoveFromHashMap(CollisionTileId tileId);
	void CalculateHash(const CollisionTile& tile, u64& hash) const;
	
	std::vector<CollisionTile> m_tiles;
	HashMap m_hashMap;
};