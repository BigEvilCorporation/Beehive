///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include <core/debug/Debug.h>
#include <core/memory/Memory.h>
#include <core/cryptography/Hash.h>

#include <string>

#include "Tile.h"

Tile::Tile()
{
	m_index = 0;
	m_palette = 0;
	m_hash = 0;
	m_pixels.resize(pixelsPerTile);
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
	int pixelIdx = (y * tileHeight) + x;
	ion::debug::Assert(pixelIdx < pixelsPerTile, "Out of range");
	m_pixels[pixelIdx] = colourIdx;
}

u8 Tile::GetPixelColour(int x, int y) const
{
	int pixelIdx = (y * tileHeight) + x;
	ion::debug::Assert(pixelIdx < pixelsPerTile, "Out of range");
	return m_pixels[pixelIdx];
}

void Tile::CopyPixels(const Tile& tile)
{
	m_pixels = tile.m_pixels;
}

void Tile::GetPixels(u8 pixels[pixelsPerTile]) const
{
	ion::memory::MemCopy(pixels, &m_pixels[0], pixelsPerTile);
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
	archive.Serialise(m_index, "index");
	archive.Serialise(m_palette, "palette");
	archive.Serialise(m_hash, "hash");
	archive.Serialise(m_pixels, "pixels");
}

void Tile::Export(std::stringstream& stream) const
{
	stream << std::hex << std::setfill('0') << std::uppercase;

	for(int y = 0; y < tileHeight; y++)
	{
		stream << "\tdc.l\t0x";

		for(int x = 0; x < tileWidth; x++)
		{
			stream << std::setw(1) << (int)GetPixelColour(x, y);
		}

		stream << std::endl;
	}

	stream << std::dec;
}

void Tile::Export(ion::io::File& file) const
{
	for(int y = 0; y < tileHeight; y++)
	{
		for(int x = 0; x < tileWidth; x += 2)
		{
			u8 nybble1 = (u8)GetPixelColour(x, y) << 4;
			u8 nybble2 = ((x + 1) < tileWidth) ? (u8)GetPixelColour(x + 1, y) : 0;

			u8 byte = nybble1 | nybble2;
			file.Write(&byte, sizeof(u8));
		}
	}
}

u32 Tile::GetBinarySize() const
{
	//4 bits per pixel
	return (tileWidth * tileHeight) / 2;
}

void Tile::CalculateHash()
{
	m_hash = ion::Hash64(&m_pixels[0], pixelsPerTile);
}

u64 Tile::GetHash() const
{
	return m_hash;
}
