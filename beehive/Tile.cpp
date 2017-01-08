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

#include <core/debug/Debug.h>
#include <core/memory/Memory.h>
#include <core/cryptography/Hash.h>

#include <string>

#include "Tile.h"

Tile::Tile()
{
	const PlatformConfig& defaultPlatformConfig = PlatformPresets::s_configs[PlatformPresets::ePresetMegaDrive];

	m_index = 0;
	m_palette = 0;
	m_hash = 0;
	m_width = defaultPlatformConfig.tileWidth;
	m_height = defaultPlatformConfig.tileHeight;
}

Tile::Tile(u8 width, u8 height)
{
	m_index = 0;
	m_palette = 0;
	m_hash = 0;
	m_width = width;
	m_height = height;
	m_pixels.resize(width * height);
}

void Tile::SetIndex(u32 index)
{
	m_index = index;
}

u32 Tile::GetIndex() const
{
	return m_index;
}

void Tile::SetPixelColour(int x, int y, u8 colourIdx)
{
	int pixelIdx = (y * m_height) + x;
	ion::debug::Assert(pixelIdx < (m_width * m_height), "eOut of range");
	m_pixels[pixelIdx] = colourIdx;
}

u8 Tile::GetPixelColour(int x, int y) const
{
	int pixelIdx = (y * m_height) + x;
	ion::debug::Assert(pixelIdx < (m_width * m_height), "eOut of range");
	return m_pixels[pixelIdx];
}

void Tile::CopyPixels(const Tile& tile)
{
	m_pixels = tile.m_pixels;
}

void Tile::GetPixels(std::vector<u8>& pixels) const
{
	pixels.resize(m_pixels.size());
	ion::memory::MemCopy(pixels.data(), m_pixels.data(), m_pixels.size());
}

void Tile::SetPaletteId(PaletteId palette)
{
	m_palette = palette;
}

PaletteId Tile::GetPaletteId() const
{
	return m_palette;
}

void Tile::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_width, "width");
	archive.Serialise(m_height, "height");
	archive.Serialise(m_index, "index");
	archive.Serialise(m_palette, "palette");
	archive.Serialise(m_hash, "hash");
	archive.Serialise(m_pixels, "pixels");
}

void Tile::Export(const PlatformConfig& config, std::stringstream& stream) const
{
	//if(config.platform == ePlatformMegaDrive)
	{
		stream << std::hex << std::setfill('0') << std::uppercase;

		for(int y = 0; y < m_height; y++)
		{
			stream << "\tdc.l\t0x";

			for(int x = 0; x < m_width; x++)
			{
				stream << std::setw(1) << (int)GetPixelColour(x, y);
			}

			stream << std::endl;
		}

		stream << std::dec;
	}
	//else if(config.platform == ePlatformSNES)
	{
		//TODO: SNES text export goes here
	}
}

void Tile::Export(const PlatformConfig& config, ion::io::File& file) const
{
	//if(config.platform == ePlatformMegaDrive)
	{
		for(int y = 0; y < m_height; y++)
		{
			for(int x = 0; x < m_width; x += 2)
			{
				u8 nybble1 = (u8)GetPixelColour(x, y) << 4;
				u8 nybble2 = ((x + 1) < m_width) ? (u8)GetPixelColour(x + 1, y) : 0;

				u8 byte = nybble1 | nybble2;
				file.Write(&byte, sizeof(u8));
			}
		}
	}
	//else if(config.platform == ePlatformSNES)
	{
		//TODO: SNES binary export goes here
	}
}

u32 Tile::GetBinarySize() const
{
	//4 bits per pixel
	return (m_width * m_height) / 2;
}

void Tile::CalculateHash()
{
	m_hash = ion::Hash64(m_pixels.data(), m_width * m_height);
}

u64 Tile::GetHash() const
{
	return m_hash;
}
