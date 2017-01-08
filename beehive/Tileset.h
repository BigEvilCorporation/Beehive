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

#include "Tile.h"
#include "PlatformConfig.h"

class Tileset
{
	typedef std::map<u64, TileId> HashMap;

public:
	enum HashOrientation
	{
		eNormal,
		eFlipX,
		eFlipY,
		eFlipXY,

		eNumHashOrientations
	};

	static const u32 s_orientationFlags[eNumHashOrientations];

	Tileset(const PlatformConfig& platformConfig);

	void Clear();

	TileId AddTile();
	void PopBackTile();
	void HashChanged(TileId tileId);
	TileId FindDuplicate(const Tile& tile, u32& tileFlags) const;

	Tile* GetTile(TileId tileId);
	const Tile* GetTile(TileId tileId) const;
	int GetCount() const;

	void CalculateHashes(const Tile& tile, u64 hashes[eNumHashOrientations]) const;
	void RebuildHashMap();

	void Serialise(ion::io::Archive& archive);
	void Export(const PlatformConfig& config, std::stringstream& stream) const;
	void Export(const PlatformConfig& config, ion::io::File& file) const;
	
private:

	void AddToHashMap(TileId tileId);
	void RemoveFromHashMap(TileId tileId);
	
	const PlatformConfig& m_platformConfig;
	std::vector<Tile> m_tiles;
	HashMap m_hashMap;
};