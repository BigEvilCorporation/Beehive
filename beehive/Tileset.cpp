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

#include "Tileset.h"
#include "Map.h"
#include <ion/core/cryptography/Hash.h>

const u32 Tileset::s_orientationFlags[eNumHashOrientations] = { 0, Map::eFlipX, Map::eFlipY, Map::eFlipX | Map::eFlipY };

Tileset::Tileset(const PlatformConfig& platformConfig)
	: m_platformConfig(platformConfig)
{
}

void Tileset::Clear()
{
	m_tiles.clear();
}

TileId Tileset::AddTile()
{
	TileId index = m_tiles.size();
	m_tiles.push_back(Tile(m_platformConfig.tileWidth, m_platformConfig.tileHeight));
	m_tiles[index].SetIndex(index);
	m_tiles[index].CalculateHash();
	AddToHashMap(index);
	return index;
}

void Tileset::PopBackTile()
{
	m_tiles.pop_back();
	RemoveFromHashMap(m_tiles.size() - 1);
}

void Tileset::HashChanged(TileId tileId)
{
	RemoveFromHashMap(tileId);
	m_tiles[tileId].CalculateHash();
	AddToHashMap(tileId);
}

void Tileset::AddToHashMap(TileId tileId)
{
	m_hashMap.insert(std::make_pair(m_tiles[tileId].GetHash(), tileId));
}

void Tileset::RemoveFromHashMap(TileId tileId)
{
	m_hashMap.erase(m_tiles[tileId].GetHash());
}

void Tileset::CalculateHashes(const Tile& tile, u64 hashes[Tileset::eNumHashOrientations]) const
{
	std::vector<u8> pixels;
	tile.GetPixels(pixels);

	std::vector<u8> pixelsFlipped(pixels.size());

	const int tileWidth = m_platformConfig.tileWidth;
	const int tileHeight = m_platformConfig.tileHeight;

	//Normal
	hashes[eNormal] = ion::Hash64(pixels.data(), pixels.size());

	//Flip X
	for(int x = 0; x < tileWidth; x++)
	{
		for(int y = 0; y < tileHeight; y++)
		{
			pixelsFlipped[(y * tileWidth) + (tileWidth - 1 - x)] = pixels[(y * tileWidth) + x];
		}
	}

	hashes[eFlipX] = ion::Hash64(pixelsFlipped.data(), pixelsFlipped.size());

	//Flip Y
	for(int x = 0; x < tileWidth; x++)
	{
		for(int y = 0; y < tileHeight; y++)
		{
			pixelsFlipped[((tileWidth - 1 - y) * tileWidth) + x] = pixels[(y * tileWidth) + x];
		}
	}

	hashes[eFlipY] = ion::Hash64(pixelsFlipped.data(), pixelsFlipped.size());

	//Flip XY
	for(int x = 0; x < tileWidth; x++)
	{
		for(int y = 0; y < tileHeight; y++)
		{
			pixelsFlipped[((tileWidth - 1 - y) * tileWidth) + (tileWidth - 1 - x)] = pixels[(y * tileWidth) + x];
		}
	}

	hashes[eFlipXY] = ion::Hash64(pixelsFlipped.data(), pixelsFlipped.size());
}

void Tileset::RebuildHashMap()
{
	m_hashMap.clear();

	for(int i = 0; i < m_tiles.size(); i++)
	{
		m_tiles[i].CalculateHash();
		AddToHashMap(i);
	}
}

TileId Tileset::FindDuplicate(const Tile& tile, u32& tileFlags) const
{
	//Calculate hashes for each orientation
	u64 hashes[eNumHashOrientations];
	CalculateHashes(tile, hashes);

	//Find duplicate
	for(int i = 0; i < eNumHashOrientations; i++)
	{
		HashMap::const_iterator it = m_hashMap.find(hashes[i]);

		if(it != m_hashMap.end())
		{
			tileFlags = s_orientationFlags[i];
			return it->second;
		}
	}

	return InvalidTileId;
}

Tile* Tileset::GetTile(TileId tileId)
{
	Tile* tile = NULL;

	if(tileId != InvalidTileId && tileId < m_tiles.size())
	{
		tile = &m_tiles[tileId];
	}

	return tile;
}

const Tile* Tileset::GetTile(TileId tileId) const
{
	const Tile* tile = NULL;

	if(tileId != InvalidTileId && tileId < m_tiles.size())
	{
		tile = &m_tiles[tileId];
	}

	return tile;
}

int Tileset::GetCount() const
{
	return m_tiles.size();
}

void Tileset::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_tiles, "tiles");
	archive.Serialise(m_hashMap, "hashMap");
	RebuildHashMap();
}

void Tileset::Export(const PlatformConfig& config, std::stringstream& stream) const
{
	for(int i = 0; i < m_tiles.size(); i++)
	{
		m_tiles[i].Export(config, stream);
		stream << std::endl;
	}
}

void Tileset::Export(const PlatformConfig& config, ion::io::File& file) const
{
	for(int i = 0; i < m_tiles.size(); i++)
	{
		m_tiles[i].Export(config, file);
	}
}