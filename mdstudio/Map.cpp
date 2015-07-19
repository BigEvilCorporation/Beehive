///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "Map.h"
#include "Project.h"
#include <algorithm>

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

	//Clear stamps
	m_stamps.clear();
}

void Map::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_width, "width");
	archive.Serialise(m_height, "height");
	archive.Serialise(m_tiles, "tiles");
	archive.Serialise(m_stamps, "stamps");
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
	m_stamps.push_back(StampMapEntry(stamp.GetId(), flipFlags, ion::Vector2i(x, y), ion::Vector2i(stamp.GetWidth(), stamp.GetHeight())));
}

void Map::BakeStamp(int x, int y, const Stamp& stamp, u32 flipFlags)
{
	BakeStamp(m_tiles, x, y, stamp, flipFlags);
}

void Map::BakeStamp(std::vector<TileDesc>& tiles, int x, int y, const Stamp& stamp, u32 flipFlags) const
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

				//Copy tile and flags
				int tileIdx = (mapY * m_width) + mapX;
				tiles[tileIdx].m_id = tileId;
				tiles[tileIdx].m_flags = tileFlags;
			}
		}
	}
}

StampId Map::FindStamp(int x, int y, ion::Vector2i& topLeft, u32& flags) const
{
	StampId stampId = InvalidStampId;
	ion::Vector2i size;
	ion::Vector2i bottomRight;

	//Work backwards, find last placed stamp first
	for(TStampPosMap::const_reverse_iterator it = m_stamps.rbegin(), end = m_stamps.rend(); it != end && !stampId; ++it)
	{
		topLeft = it->m_position;
		size = it->m_size;

		bottomRight = topLeft + size;

		if(x >= topLeft.x && y >= topLeft.y
			&& x < bottomRight.x && y < bottomRight.y)
		{
			stampId = it->m_id;
		}
	}

	return stampId;
}

void Map::RemoveStamp(int x, int y)
{
	ion::Vector2i size;
	ion::Vector2i bottomRight;

	for(TStampPosMap::iterator it = m_stamps.begin(), end = m_stamps.end(); it != end; ++it)
	{
		ion::Vector2i topLeft = it->m_position;
		ion::Vector2i size = it->m_size;

		bottomRight = topLeft + size;

		if(x >= topLeft.x && y >= topLeft.y
			&& x < bottomRight.x && y < bottomRight.y)
		{
			std::swap(*it, m_stamps.back());
			m_stamps.pop_back();
			break;
		}
	}
}

const TStampPosMap::const_iterator Map::StampsBegin() const
{
	return m_stamps.begin();
}

const TStampPosMap::const_iterator Map::StampsEnd() const
{
	return m_stamps.end();
}

void Map::Export(const Project& project, std::stringstream& stream) const
{
	//Copy tiles
	std::vector<TileDesc> tiles = m_tiles;

	//Blit stamps
	for(TStampPosMap::const_iterator it = m_stamps.begin(), end = m_stamps.end(); it != end; ++it)
	{
		const Stamp& stamp = *project.GetStamp(it->m_id);
		const ion::Vector2i& position = it->m_position;
		BakeStamp(tiles, position.x, position.y, stamp, 0);
	}

	//Output to stream
	stream << std::hex << std::setfill('0') << std::uppercase;

	for(int y = 0; y < m_height; y++)
	{
		stream << "\tdc.w\t";

		for(int x = 0; x < m_width; x++)
		{
			//16 bit word:
			//-------------------
			//ABBC DEEE EEEE EEEE
			//-------------------
			//A = Low/high plane
			//B = Palette ID
			//C = Horizontal flip
			//D = Vertical flip
			//E = Tile ID

			const TileDesc& tileDesc = tiles[(y * m_width) + x];
			u8 paletteId = 0;

			//If blank tile, use background tile
			u32 tileId = (tileDesc.m_id == InvalidTileId) ? 0 : tileDesc.m_id;

			const Tile* tile = project.GetTileset().GetTile(tileId);
			ion::debug::Assert(tile, "Map::Export() - Invalid tile");

			//Generate components
			u16 tileIndex = tileId & 0x7FF;						//Bottom 11 bits (index from 0)
			u16 flipV = (tileDesc.m_flags & eFlipY) ? 1 << 11 : 0;	//12th bit
			u16 flipH = (tileDesc.m_flags & eFlipX) ? 1 << 12 : 0;	//13th bit
			u16 palette = (tile->GetPaletteId() & 0x3) << 13;		//14th and 15th bits
			u16 plane = 1 << 15;									//16th bit

			//Generate word
			u16 word = tileIndex | flipV | flipH | palette;

			stream << "0x" << std::setw(4) << (u32)word;

			if(x < (m_width - 1))
				stream << ",";
		}

		stream << std::endl;
	}

	stream << std::dec;
}