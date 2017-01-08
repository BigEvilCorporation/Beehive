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

#include "Stamp.h"
#include "Project.h"

#include <core/memory/Memory.h>
#include <core/cryptography/Hash.h>

Stamp::Stamp()
{
	m_id = InvalidStampId;
	m_width = 0;
	m_height = 0;
	m_nameHash = 0;
}

Stamp::Stamp(StampId stampId, const Stamp& rhs)
{
	*this = rhs;
	m_id = stampId;
}

Stamp::Stamp(StampId stampId, int width, int height)
{
	m_id = stampId;
	m_width = width;
	m_height = height;
	m_nameHash = 0;

	int size = width * height;
	m_tiles.resize(size);

	for(int i = 0; i < size; i++)
	{
		m_tiles[i].m_id = InvalidTileId;
	}
}

StampId Stamp::GetId() const
{
	return m_id;
}

int Stamp::GetWidth() const
{
	return m_width;
}

int Stamp::GetHeight() const
{
	return m_height;
}

void Stamp::SetTile(int x, int y, TileId tile)
{
	int tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "eOut of range");
	m_tiles[tileIdx].m_id = tile;
	m_tiles[tileIdx].m_flags = 0;
}

TileId Stamp::GetTile(int x, int y) const
{
	int tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "eOut of range");
	return m_tiles[tileIdx].m_id;
}

void Stamp::SetTileFlags(int x, int y, u32 flags)
{
	int tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "eOut of range");
	m_tiles[tileIdx].m_flags = flags;
}

u32 Stamp::GetTileFlags(int x, int y) const
{
	int tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "eOut of range");
	return m_tiles[tileIdx].m_flags;
}

void Stamp::SetName(const std::string& name)
{
	m_name = name;
	m_nameHash = ion::Hash(name.c_str());
}

const std::string& Stamp::GetName() const
{
	return m_name;
}

u32 Stamp::GetNameHash() const
{
	return m_nameHash;
}

void Stamp::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_id, "id");
	archive.Serialise(m_width, "width");
	archive.Serialise(m_height, "height");
	archive.Serialise(m_name, "name");
	archive.Serialise(m_nameHash, "nameHash");
	archive.Serialise(m_tiles, "tiles");
}

void Stamp::Export(const Project& project, std::stringstream& stream) const
{
	//Use background tile if there is one, else use first tile
	u32 backgroundTileId = project.GetBackgroundTile();
	if(backgroundTileId == InvalidTileId)
	{
		backgroundTileId = 0;
	}

	//if(project.GetPlatformConfig().platform == ePlatformMegaDrive)
	{
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

				const TileDesc& tileDesc = m_tiles[(y * m_width) + x];
				u8 paletteId = 0;

				//If blank tile, use background tile
				u32 tileId = (tileDesc.m_id == InvalidTileId) ? backgroundTileId : tileDesc.m_id;

				const Tile* tile = project.GetTileset().GetTile(tileId);
				ion::debug::Assert(tile, "Map::Export() - Invalid tile");

				//Generate components
				u16 tileIndex = tileId & 0x7FF;								//Bottom 11 bits = tile ID (index from 0)
				u16 flipH = (tileDesc.m_flags & Map::eFlipX) ? 1 << 11 : 0;		//12th bit = Flip X flag
				u16 flipV = (tileDesc.m_flags & Map::eFlipY) ? 1 << 12 : 0;		//13th bit = Flip Y flag
				u16 palette = (tile->GetPaletteId() & 0x3) << 13;			//14th+15th bits = Palette ID
				u16 plane = (tileDesc.m_flags & Map::eHighPlane) ? 1 << 15 : 0;	//16th bit = High plane flag

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
	//else if(project.GetPlatformConfig().platform == ePlatformSNES)
	{
		//TODO: SNES export goes here
	}
}

void Stamp::Export(const Project& project, ion::io::File& file) const
{
	//Use background tile if there is one, else use first tile
	u32 backgroundTileId = project.GetBackgroundTile();
	if(backgroundTileId == InvalidTileId)
	{
		backgroundTileId = 0;
	}

	//if(project.GetPlatformConfig().platform == ePlatformMegaDrive)
	{
		for(int y = 0; y < m_height; y++)
		{
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

				const TileDesc& tileDesc = m_tiles[(y * m_width) + x];
				u8 paletteId = 0;

				//If blank tile, use background tile
				u32 tileId = (tileDesc.m_id == InvalidTileId) ? backgroundTileId : tileDesc.m_id;

				const Tile* tile = project.GetTileset().GetTile(tileId);
				ion::debug::Assert(tile, "Map::Export() - Invalid tile");

				//Generate components
				u16 tileIndex = tileId & 0x7FF;								//Bottom 11 bits = tile ID (index from 0)
				u16 flipH = (tileDesc.m_flags & Map::eFlipX) ? 1 << 11 : 0;		//12th bit = Flip X flag
				u16 flipV = (tileDesc.m_flags & Map::eFlipY) ? 1 << 12 : 0;		//13th bit = Flip Y flag
				u16 palette = (tile->GetPaletteId() & 0x3) << 13;			//14th+15th bits = Palette ID
				u16 plane = (tileDesc.m_flags & Map::eHighPlane) ? 1 << 15 : 0;	//16th bit = High plane flag

				//Generate word
				u16 word = tileIndex | flipV | flipH | palette | plane;

				//Endian flip
				ion::memory::EndianSwap(word);

				//Write
				file.Write(&word, sizeof(u16));
			}
		}
	}
	//else if(project.GetPlatformConfig().platform == ePlatformSNES)
	{
		//TODO: SNES export goes here
	}
}