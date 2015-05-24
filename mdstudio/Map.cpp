///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "Map.h"

Map::Map()
{
	m_width = 0;
	m_height = 0;
	Resize(defaultWidth, defaultHeight);
}

void Map::Clear()
{
	//Clear tiles
	for(int x = 0; x < m_width; x++)
	{
		for(int y = 0; y < m_height; y++)
		{
			SetTile(x, y, InvalidTileId);
		}
	}
}

void Map::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_width);
	archive.Serialise(m_height);
	archive.Serialise(m_tiles);
	archive.Serialise(m_stamps);
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
	//Create new tile array
	std::vector<TileDesc> tiles;

	//Set new size
	int size = width * height;
	tiles.resize(size);

	//Copy tiles
	for(int x = 0; x < min(width, m_width); x++)
	{
		for(int y = 0; y < min(height, m_height); y++)
		{
			int tileIdx = (y * width) + x;
			tiles[tileIdx].m_id = GetTile(x, y);
			tiles[tileIdx].m_flags = GetTileFlags(x, y);
		}
	}
	
	//Set new
	m_tiles = tiles;
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

void Map::SetStamp(int x, int y, const Stamp& stamp, u32 flipFlags)
{
	//Add to stamp map
	m_stamps.push_back(std::make_tuple(ion::Vector2i(x, y), ion::Vector2i(stamp.GetWidth(), stamp.GetHeight()), StampDesc(stamp.GetId(), flipFlags)));

	//Bake it into the tiles
	BakeStamp(x, y, stamp, flipFlags);
}

void Map::BakeStamp(int x, int y, const Stamp& stamp, u32 flipFlags)
{
	int width = stamp.GetWidth();
	int height = stamp.GetHeight();

	for(int stampX = 0; stampX < width; stampX++)
	{
		for(int stampY = 0; stampY < height; stampY++)
		{
			int sourceX = (flipFlags & eFlipX) ? (width - 1 - stampX) : stampX;
			int sourceY = (flipFlags & eFlipY) ? (height - 1 - stampY) : stampY;

			TileId tileId = stamp.GetTile(sourceX, sourceY);
			if(tileId != InvalidTileId)
			{
				u32 tileFlags = stamp.GetTileFlags(stampX, stampY);
				tileFlags ^= flipFlags;

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

StampId Map::FindStamp(int x, int y, ion::Vector2i& topLeft, u32& flags) const
{
	StampId stampId = InvalidStampId;
	ion::Vector2i size;
	ion::Vector2i bottomRight;

	for(int i = 0; i < m_stamps.size() && !stampId; i++)
	{
		topLeft = std::get<0>(m_stamps[i]);
		size = std::get<1>(m_stamps[i]);

		bottomRight = topLeft + size;

		if(x >= topLeft.x && y >= topLeft.y
			&& x < bottomRight.x && y < bottomRight.y)
		{
			stampId = std::get<2>(m_stamps[i]).m_id;
		}
	}

	return stampId;
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