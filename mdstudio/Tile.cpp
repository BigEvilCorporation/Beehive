///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include <core/debug/Debug.h>
#include <core/memory/Memory.h>
#include <core/cryptography/Hash.h>

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
	CalculateColourHash();
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
	CalculateCollisionHash();
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

void Tile::ExportColour(std::stringstream& outputString)
{
	for(int y = 0; y < tileHeight; y++)
	{
		outputString << "dc.b\t";

		for(int x = 0; x < tileWidth; x++)
		{
			outputString << GetPixelColour(x, y);

			if(x < 8)
				outputString << ',';
			else
				outputString << '\r';
		}
	}
}

void Tile::ExportCollision(std::stringstream& outputString)
{
	for(int y = 0; y < tileHeight; y++)
	{
		outputString << "dc.b\t";

		for(int x = 0; x < tileWidth; x++)
		{
			outputString << GetPixelCollisionBits(x, y);

			if(x < 7)
				outputString << ',';
			else
				outputString << '\r';
		}
	}
}

void Tile::CalculateColourHash()
{
	std::stringstream stream;
	for(int i = 0; i < pixelsPerTile; i++)
	{
		stream << m_pixels[i].colourIdx;
	}

	m_colourHash = ion::Hash(stream.str().c_str());
}

void Tile::CalculateCollisionHash()
{
	std::stringstream stream;
	for(int i = 0; i < pixelsPerTile; i++)
	{
		stream << m_pixels[i].collisionBits;
	}

	m_collisionHash = ion::Hash(stream.str().c_str());
}

u32 Tile::GetColourHash() const
{
	return m_colourHash;
}

u32 Tile::GetCollisionHash() const
{
	return m_collisionHash;
}