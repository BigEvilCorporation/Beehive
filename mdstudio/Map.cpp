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
		m_tiles[i].m_id = backgroundId;
	}
}

void Map::Clear()
{
	//Clear tileset
	m_tileset.Clear();

	//Clear tiles
	m_tiles.clear();
}

void Map::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_width);
	archive.Serialise(m_height);
	archive.Serialise(m_tileset);
	archive.Serialise(m_tiles);
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
	int tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "Out of range");
	m_tiles[tileIdx].m_id = tile;
	m_tiles[tileIdx].m_flags = 0;
}

TileId Map::GetTile(int x, int y) const
{
	int tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "Out of range");
	return m_tiles[tileIdx].m_id;
}

void Map::SetTileFlags(int x, int y, u32 flags)
{
	int tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "Out of range");
	m_tiles[tileIdx].m_flags = flags;
}

u32 Map::GetTileFlags(int x, int y) const
{
	int tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "Out of range");
	return m_tiles[tileIdx].m_flags;
}

void Map::DrawStamp(int x, int y, const Stamp& stamp)
{
	for(int stampX = 0; stampX < stamp.GetWidth(); stampX++)
	{
		for(int stampY = 0; stampY < stamp.GetHeight(); stampY++)
		{
			TileId tileId = stamp.GetTile(stampX, stampY);
			if(tileId != InvalidTileId)
			{
				u32 tileFlags = stamp.GetTileFlags(stampX, stampY);
				int mapX = stampX + x;
				int mapY = stampY + y;

				//Copy tile
				SetTile(mapX, mapY, tileId);

				//Copy flags
				SetTileFlags(mapX, mapY, tileFlags);
			}
		}
	}
}

const Tileset& Map::GetTileset() const
{
	return m_tileset;
}

Tileset& Map::GetTileset()
{
	return m_tileset;
}

void Map::Export(std::stringstream& stream) const
{
	stream << std::hex << std::setfill('0') << std::uppercase;

	for(int y = 0; y < m_height; y++)
	{
		stream << "\tdc.b\t";

		for(int x = 0; x < m_width; x++)
		{
			//TODO: V/H flip bits
			u8 byte = m_tiles[(y * m_width) + x].m_id;
			stream << "0x" << std::setw(2) << (u32)byte;

			if(x < (m_width - 1))
				stream << ",";
		}

		stream << std::endl;
	}

	stream << std::dec;
}