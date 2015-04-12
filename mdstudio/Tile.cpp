///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
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
	m_palette = 0;
	m_colourHash = 0;
	m_collisionHash = 0;
	ion::memory::MemSet(&m_pixels, 0, sizeof(Pixel) * pixelsPerTile);
}

void Tile::SetPixelColour(int x, int y, u8 colourIdx)
{
	int pixelIdx = (y * tileHeight) + x;
	ion::debug::Assert(pixelIdx < pixelsPerTile, "Out of range");
	m_pixels[pixelIdx].colourIdx = colourIdx;
}

u8 Tile::GetPixelColour(int x, int y) const
{
	int pixelIdx = (y * tileHeight) + x;
	ion::debug::Assert(pixelIdx < pixelsPerTile, "Out of range");
	return m_pixels[pixelIdx].colourIdx;
}

void Tile::AddPixelCollisionBits(int x, int y, u8 collisionBits)
{
	int pixelIdx = (y * tileHeight) + x;
	ion::debug::Assert(pixelIdx < pixelsPerTile, "Out of range");
	m_pixels[pixelIdx].collisionBits = collisionBits;
}

u8 Tile::GetPixelCollisionBits(int x, int y) const
{
	int pixelIdx = (y * tileHeight) + x;
	ion::debug::Assert(pixelIdx < pixelsPerTile, "Out of range");
	return m_pixels[pixelIdx].collisionBits;
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
	archive.Serialise(m_palette);
	archive.Serialise(m_colourHash);
	archive.Serialise(m_collisionHash);
	
	for(int i = 0; i < pixelsPerTile; i++)
	{
		archive.Serialise(m_pixels[i].colourIdx);
		archive.Serialise(m_pixels[i].collisionBits);
	}
}

void Tile::ExportColour(std::stringstream& stream) const
{
	stream << std::hex << std::setfill('0') << std::uppercase;

	for(int y = 0; y < tileHeight; y++)
	{
		stream << "\tdc.b\t0x";

		for(int x = 0; x < tileWidth; x++)
		{
			stream << std::setw(1) << (int)GetPixelColour(x, y);
		}

		stream << std::endl;
	}

	stream << std::dec;
}

void Tile::ExportCollision(std::stringstream& stream) const
{
	stream << std::hex << std::setfill('0') << std::uppercase;

	for(int y = 0; y < tileHeight; y++)
	{
		stream << "\tdc.l\t0x";

		for(int x = 0; x < tileWidth; x++)
		{
			stream << std::setw(1) << (int)GetPixelCollisionBits(x, y);
		}

		stream << std::endl;
	}

	stream << std::dec;
}

void Tile::CalculateColourHash()
{
	std::string string;
	string.reserve(pixelsPerTile);
	for(int i = 0; i < pixelsPerTile; i++)
	{
		string += ('a' + m_pixels[i].colourIdx);
	}

	m_colourHash = ion::Hash(string.c_str());
}

void Tile::CalculateCollisionHash()
{
	std::string string;
	string.reserve(pixelsPerTile);
	for(int i = 0; i < pixelsPerTile; i++)
	{
		string += ('a' + m_pixels[i].collisionBits);
	}

	m_collisionHash = ion::Hash(string.c_str());
}

u32 Tile::GetColourHash() const
{
	return m_colourHash;
}

u32 Tile::GetCollisionHash() const
{
	return m_collisionHash;
}