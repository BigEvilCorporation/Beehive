///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <io/Archive.h>
#include <sstream>

typedef u32 CollisionTileId;
static const CollisionTileId InvalidCollisionTileId = 0xFFFFFFFF;

class CollisionTile
{
public:
	static const int tileWidth = 8;
	static const int tileHeight = 8;

	CollisionTile();

	u64 CalculateHash();
	u64 GetHash() const;

	void SetHeight(int x, s8 height);
	void ClearHeight(int x);
	s8 GetHeight(int x) const;

	void GetHeights(s8 heights[tileWidth]) const;
	void CopyHeights(const CollisionTile& tile);

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& stream) const;
	void Export(ion::io::File& file) const;

private:
	u64 m_hash;
	std::vector<s8> m_heightmap;
};