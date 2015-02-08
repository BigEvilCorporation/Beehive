///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "Map.h"

Map::Map()
{
	Resize(defaultWidth, defaultHeight);

	//Create one blank tile for background
	TileId backgroundId = m_tileset.AddTile();

	//Fill background
	for(int i = 0; i < m_tiles.size(); i++)
	{
		m_tiles[i] = backgroundId;
	}
}

int Map::GetWidth() const
{
	return m_width;
}

int Map::GetHeight() const
{
	return m_height;
}

void Map::Resize(int width, int height)
{
	int size = width * height;
	m_tiles.resize(size);
	m_width = width;
	m_height = height;
}

void Map::SetTile(int x, int y, TileId tile)
{
	int tileIdx = (y * m_height) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "Out of range");
	m_tiles[tileIdx] = tile;
}

TileId Map::GetTile(int x, int y) const
{
	int tileIdx = (y * m_height) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "Out of range");
	return m_tiles[tileIdx];
}

Tileset& Map::GetTileset()
{
	return m_tileset;
}