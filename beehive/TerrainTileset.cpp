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

#include "TerrainTileset.h"
#include <ion/core/cryptography/Hash.h>

TerrainTileset::TerrainTileset(const PlatformConfig& platformConfig)
	: m_platformConfig(platformConfig)
{
}

void TerrainTileset::Clear()
{
	m_tiles.clear();
	m_hashMap.clear();
}

TerrainTileId TerrainTileset::AddTerrainTile()
{
	TerrainTileId index = m_tiles.size();

	if(index >= eCollisionTileFlagNone)
	{
		//Out of bits for id
		return InvalidTerrainTileId;
	}
	else
	{
		m_tiles.push_back(TerrainTile(m_platformConfig.tileWidth, m_platformConfig.tileHeight));
		m_tiles[index].CalculateHash();
		AddToHashMap(index);
		return index;
	}
}

void TerrainTileset::PopBackTerrainTile()
{
	RemoveFromHashMap(m_tiles.size() - 1);
	m_tiles.pop_back();
}

void TerrainTileset::HashChanged(TerrainTileId tileId)
{
	RemoveFromHashMap(tileId);
	m_tiles[tileId].CalculateHash();
	AddToHashMap(tileId);
}

void TerrainTileset::AddToHashMap(TerrainTileId tileId)
{
	m_hashMap.insert(std::make_pair(m_tiles[tileId].GetHash(), tileId));
}

void TerrainTileset::RemoveFromHashMap(TerrainTileId tileId)
{
	m_hashMap.erase(m_tiles[tileId].GetHash());
}

void TerrainTileset::RebuildHashMap()
{
	m_hashMap.clear();

	for(int i = 0; i < m_tiles.size(); i++)
	{
		m_tiles[i].CalculateHash();
		AddToHashMap(i);
	}
}

void TerrainTileset::CalculateHash(const TerrainTile& tile, u64& hash) const
{
	std::vector<s8> heights;
	tile.GetHeights(heights);

	hash = ion::Hash64((u8*)heights.data(), heights.size());
}

TerrainTileId TerrainTileset::FindDuplicate(const TerrainTile& tile) const
{
	//Calculate hash
	u64 hash;
	CalculateHash(tile, hash);

	//Find duplicate
	return FindDuplicate(hash);
}

TerrainTileId TerrainTileset::FindDuplicate(u64 hash) const
{
	//Find duplicate
	HashMap::const_iterator it = m_hashMap.find(hash);

	if(it != m_hashMap.end())
	{
		return it->second;
	}

	return InvalidTerrainTileId;
}

TerrainTile* TerrainTileset::GetTerrainTile(TerrainTileId tileId)
{
	TerrainTile* tile = NULL;

	if(tileId != InvalidTerrainTileId && tileId < m_tiles.size())
	{
		tile = &m_tiles[tileId];
	}

	return tile;
}

const TerrainTile* TerrainTileset::GetTerrainTile(TerrainTileId tileId) const
{
	const TerrainTile* tile = NULL;

	if(tileId != InvalidTerrainTileId && tileId < m_tiles.size())
	{
		tile = &m_tiles[tileId];
	}

	return tile;
}

int TerrainTileset::GetCount() const
{
	return m_tiles.size();
}

void TerrainTileset::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_tiles, "tiles");
	archive.Serialise(m_hashMap, "hashMap");
}

void TerrainTileset::Export(std::stringstream& stream) const
{
	for(int i = 0; i < m_tiles.size(); i++)
	{
		m_tiles[i].Export(stream);
		stream << std::endl;
	}
}

void TerrainTileset::Export(ion::io::File& file) const
{
	for(int i = 0; i < m_tiles.size(); i++)
	{
		m_tiles[i].Export(file);
	}
}