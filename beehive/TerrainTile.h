///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#pragma once

#include <io/Archive.h>
#include <sstream>

enum eCollisionTileFlags
{
	eCollisionTileFlagSolid = (1 << 13),
	eCollisionTileFlagHole = (1 << 14),

	eCollisionTileFlagAll = (eCollisionTileFlagSolid | eCollisionTileFlagHole),
	eCollisionTileFlagNone = (eCollisionTileFlagSolid - 1)
};

typedef u32 TerrainTileId;
static const TerrainTileId InvalidTerrainTileId = eCollisionTileFlagNone;

class TerrainTile
{
public:
	TerrainTile();
	TerrainTile(u8 width, u8 height);

	u64 CalculateHash();
	u64 GetHash() const;

	void SetHeight(int x, s8 height);
	void ClearHeight(int x);
	s8 GetHeight(int x) const;

	void GetHeights(std::vector<s8>& heights) const;
	void CopyHeights(const TerrainTile& tile);

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& stream) const;
	void Export(ion::io::File& file) const;

private:
	u64 m_hash;
	u8 m_width;
	u8 m_height;
	std::vector<s8> m_heightmap;
};