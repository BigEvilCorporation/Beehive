///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <io/Archive.h>
#include <sstream>

enum eCollisionTileFlags
{
	eCollisionTileFlagSolid = (1 << 13),

	eCollisionTileFlagAll = eCollisionTileFlagSolid
};

typedef u32 TerrainTileId;
static const TerrainTileId InvalidTerrainTileId = (~eCollisionTileFlagAll & 0xFFFF);

class TerrainTile
{
public:
	static const int tileWidth = 8;
	static const int tileHeight = 8;

	TerrainTile();

	u64 CalculateHash();
	u64 GetHash() const;

	void SetHeight(int x, s8 height);
	void ClearHeight(int x);
	s8 GetHeight(int x) const;

	void GetHeights(s8 heights[tileWidth]) const;
	void CopyHeights(const TerrainTile& tile);

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& stream) const;
	void Export(ion::io::File& file) const;

private:
	u64 m_hash;
	std::vector<s8> m_heightmap;
};