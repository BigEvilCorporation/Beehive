///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "Tileset.h"

Tileset::Tileset()
{
	m_nextFreeId = 0;
}

void Tileset::Clear()
{
	m_tiles.clear();
	m_nextFreeId = 0;
}

TileId Tileset::AddTile()
{
	m_tiles.insert(std::make_pair(m_nextFreeId, Tile()));
	m_tiles.begin()->second.CalculateColourHash();
	m_tiles.begin()->second.CalculateCollisionHash();
	return m_nextFreeId++;
}

void Tileset::RemoveTile(TileId tileId)
{
	TTileMap::iterator it = m_tiles.find(tileId);
	if(it != m_tiles.end())
	{
		m_tiles.erase(it);
	}
}

TileId Tileset::FindDuplicate(const Tile& tile) const
{
	for(TTileMap::const_iterator it = m_tiles.begin(), end = m_tiles.end(); it != end; ++it)
	{
		if(it->second.GetPaletteId() == tile.GetPaletteId()
			&& it->second.GetColourHash() == tile.GetColourHash())
		{
			return it->first;
		}
	}

	return 0;
}

Tile* Tileset::GetTile(TileId tileId)
{
	Tile* tile = NULL;

	TTileMap::iterator it = m_tiles.find(tileId);
	if(it != m_tiles.end())
	{
		tile = &it->second;
	}

	return tile;
}

const Tile* Tileset::GetTile(TileId tileId) const
{
	const Tile* tile = NULL;

	TTileMap::const_iterator it = m_tiles.find(tileId);
	if(it != m_tiles.end())
	{
		tile = &it->second;
	}

	return tile;
}

const TTileMap::const_iterator Tileset::Begin() const
{
	return m_tiles.begin();
}

const TTileMap::const_iterator Tileset::End() const
{
	return m_tiles.end();
}

int Tileset::GetCount() const
{
	return m_tiles.size();
}

void Tileset::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_tiles, "tiles");
	archive.Serialise(m_nextFreeId, "nextFreeId");
}

void Tileset::ExportArt(std::stringstream& stream) const
{
	for(TTileMap::const_iterator it = m_tiles.begin(), end = m_tiles.end(); it != end; ++it)
	{
		it->second.ExportColour(stream);
		stream << std::endl;
	}
}

void Tileset::ExportCollision(std::stringstream& stream) const
{
	for(TTileMap::const_iterator it = m_tiles.begin(), end = m_tiles.end(); it != end; ++it)
	{
		it->second.ExportCollision(stream);
		stream << std::endl;
	}
}