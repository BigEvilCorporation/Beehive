///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "Tileset.h"

Tileset::Tileset()
{
	m_nextFreeId = 1;
}

void Tileset::Clear()
{
	m_tiles.clear();
	m_nextFreeId = 1;
}

TileId Tileset::AddTile()
{
	m_tiles.insert(std::make_pair(m_nextFreeId, Tile()));
	return m_nextFreeId++;
}

void Tileset::RemoveTile(TileId tileId)
{
	TileMap::iterator it = m_tiles.find(tileId);
	if(it != m_tiles.end())
	{
		m_tiles.erase(it);
	}
}

TileId Tileset::FindDuplicate(const Tile& tile) const
{
	for(TileMap::const_iterator it = m_tiles.begin(), end = m_tiles.end(); it != end; ++it)
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

	TileMap::iterator it = m_tiles.find(tileId);
	if(it != m_tiles.end())
	{
		tile = &it->second;
	}

	return tile;
}

const TileMap::const_iterator Tileset::Begin() const
{
	return m_tiles.begin();
}

const TileMap::const_iterator Tileset::End() const
{
	return m_tiles.end();
}

int Tileset::GetCount() const
{
	return m_tiles.size();
}

void Tileset::Serialise(ion::io::Archive& archive)
{
}