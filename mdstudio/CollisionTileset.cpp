///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "CollisionTileset.h"
#include <ion/core/cryptography/Hash.h>

CollisionTileset::CollisionTileset()
{
}

void CollisionTileset::Clear()
{
	m_tiles.clear();
}

CollisionTileId CollisionTileset::AddCollisionTile()
{
	CollisionTileId index = m_tiles.size();
	m_tiles.push_back(CollisionTile());
	m_tiles[index].SetIndex(index);
	m_tiles[index].CalculateHash();
	AddToHashMap(index);
	return index;
}

void CollisionTileset::RemoveCollisionTile(CollisionTileId tileId)
{

}

void CollisionTileset::HashChanged(CollisionTileId tileId)
{
	RemoveFromHashMap(tileId);
	m_tiles[tileId].CalculateHash();
	AddToHashMap(tileId);
}

void CollisionTileset::AddToHashMap(CollisionTileId tileId)
{
	m_hashMap.insert(std::make_pair(m_tiles[tileId].GetHash(), tileId));
}

void CollisionTileset::RemoveFromHashMap(CollisionTileId tileId)
{
	m_hashMap.erase(m_tiles[tileId].GetHash());
}

void CollisionTileset::CalculateHash(const CollisionTile& tile, u64& hash) const
{
	u8 pixels[CollisionTile::pixelsPerCollisionTile];
	tile.GetPixels(pixels);

	hash = ion::Hash64(pixels, CollisionTile::pixelsPerCollisionTile);
}

CollisionTileId CollisionTileset::FindDuplicate(const CollisionTile& tile) const
{
	//Calculate hash
	u64 hash;
	CalculateHash(tile, hash);

	//Find duplicate
	HashMap::const_iterator it = m_hashMap.find(hash);

	if(it != m_hashMap.end())
	{
		return it->second;
	}

	return InvalidCollisionTileId;
}

CollisionTile* CollisionTileset::GetCollisionTile(CollisionTileId tileId)
{
	CollisionTile* tile = NULL;

	if(tileId != InvalidCollisionTileId && tileId < m_tiles.size())
	{
		tile = &m_tiles[tileId];
	}

	return tile;
}

const CollisionTile* CollisionTileset::GetCollisionTile(CollisionTileId tileId) const
{
	const CollisionTile* tile = NULL;

	if(tileId != InvalidCollisionTileId && tileId < m_tiles.size())
	{
		tile = &m_tiles[tileId];
	}

	return tile;
}

int CollisionTileset::GetCount() const
{
	return m_tiles.size();
}

void CollisionTileset::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_tiles, "tiles");
	archive.Serialise(m_hashMap, "hashMap");
}

void CollisionTileset::Export(std::stringstream& stream) const
{
	for(int i = 0; i < m_tiles.size(); i++)
	{
		m_tiles[i].Export(stream);
		stream << std::endl;
	}
}
