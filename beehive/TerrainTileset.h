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

#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <io/Archive.h>

#include "TerrainTile.h"
#include "PlatformConfig.h"

class TerrainTileset
{
	typedef std::map<u64, TerrainTileId> HashMap;

public:
	TerrainTileset(const PlatformConfig& platformConfig);

	void Clear();

	TerrainTileId AddTerrainTile();
	void PopBackTerrainTile();
	void HashChanged(TerrainTileId tileId);
	TerrainTileId FindDuplicate(const TerrainTile& tile) const;
	TerrainTileId FindDuplicate(u64 hash) const;
	void RebuildHashMap();

	TerrainTile* GetTerrainTile(TerrainTileId tileId);
	const TerrainTile* GetTerrainTile(TerrainTileId tileId) const;
	int GetCount() const;

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& stream) const;
	void Export(ion::io::File& file) const;
	
private:
	void AddToHashMap(TerrainTileId tileId);
	void RemoveFromHashMap(TerrainTileId tileId);
	void CalculateHash(const TerrainTile& tile, u64& hash) const;
	
	const PlatformConfig& m_platformConfig;
	std::vector<TerrainTile> m_tiles;
	HashMap m_hashMap;
};