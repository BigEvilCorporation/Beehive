///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "Tileset.h"
#include <ion/core/cryptography/Hash.h>

const u32 Tileset::s_orientationFlags[eNumHashOrientations] = { 0, 1, 2, 4 };

Tileset::Tileset()
{
}

void Tileset::Clear()
{
	m_tiles.clear();
}

TileId Tileset::AddTile()
{
	TileId index = m_tiles.size();
	m_tiles.push_back(Tile());
	m_tiles[index].SetIndex(index);
	m_tiles[index].CalculateHash();
	AddToHashMap(index);
	return index;
}

void Tileset::RemoveTile(TileId tileId)
{

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
	u8 pixels[Tile::pixelsPerTile];
	u8 pixelsFlipped[Tile::pixelsPerTile];
	tile.GetPixels(pixels);

	//Normal
	hashes[eNormal] = ion::Hash64(pixels, Tile::pixelsPerTile);

	//Flip X
	for(int x = 0; x < 8; x++)
	{
		for(int y = 0; y < 8; y++)
		{
			pixelsFlipped[(y * 8) + (7 - x)] = pixels[(y * 8) + x];
		}
	}

	hashes[eFlipX] = ion::Hash64(pixelsFlipped, Tile::pixelsPerTile);

	//Flip Y
	for(int x = 0; x < 8; x++)
	{
		for(int y = 0; y < 8; y++)
		{
			pixelsFlipped[((7 - y) * 8) + x] = pixels[(y * 8) + x];
		}
	}

	hashes[eFlipY] = ion::Hash64(pixelsFlipped, Tile::pixelsPerTile);

	//Flip XY
	for(int x = 0; x < 8; x++)
	{
		for(int y = 0; y < 8; y++)
		{
			pixelsFlipped[((7 - y) * 8) + (7 - x)] = pixels[(y * 8) + x];
		}
	}

	hashes[eFlipXY] = ion::Hash64(pixelsFlipped, Tile::pixelsPerTile);
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
}

void Tileset::Export(std::stringstream& stream) const
{
	for(int i = 0; i < m_tiles.size(); i++)
	{
		m_tiles[i].Export(stream);
		stream << std::endl;
	}
}

void Tileset::ExportCollision(std::stringstream& stream) const
{
	for(int i = 0; i < m_tiles.size(); i++)
	{
		m_tiles[i].ExportCollision(stream);
		stream << std::endl;
	}
}