#include <core/debug/Debug.h>
#include <core/memory/Memory.h>
#include <core/cryptography/Hash.h>

#include "Tile.h"

Tile::Tile()
{
	defaultPaletteIdx = 0;
	colourHash = 0;
	collisionHash = 0;
	ion::memory::MemSet(&pixels, 0, sizeof(Pixel) * pixelsPerTile);
}

void Tile::SetPixelColour(int x, int y, u8 colourIdx)
{
	int pixelIdx = (y * tileHeight) + x;
	ion::debug::Assert(pixelIdx < pixelsPerTile, "Out of range");
	pixels[pixelIdx].colourIdx = colourIdx;
	CalculateColourHash();
}

u8 Tile::GetPixelColour(int x, int y) const
{
	int pixelIdx = (y * tileHeight) + x;
	ion::debug::Assert(pixelIdx < pixelsPerTile, "Out of range");
	return pixels[pixelIdx].colourIdx;
}

void Tile::AddPixelCollisionBits(int x, int y, u8 collisionBits)
{
	int pixelIdx = (y * tileHeight) + x;
	ion::debug::Assert(pixelIdx < pixelsPerTile, "Out of range");
	pixels[pixelIdx].collisionBits = collisionBits;
	CalculateCollisionHash();
}

u8 Tile::GetPixelCollisionBits(int x, int y) const
{
	int pixelIdx = (y * tileHeight) + x;
	ion::debug::Assert(pixelIdx < pixelsPerTile, "Out of range");
	return pixels[pixelIdx].collisionBits;
}

void Tile::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(defaultPaletteIdx);
	archive.Serialise(colourHash);
	archive.Serialise(collisionHash);
	
	for(int i = 0; i < pixelsPerTile; i++)
	{
		archive.Serialise(pixels[i].colourIdx);
		archive.Serialise(pixels[i].collisionBits);
	}
}

void Tile::ExportColourText(std::stringstream& outputString)
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

void Tile::ExportCollisionText(std::stringstream& outputString)
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
		stream << pixels[i].colourIdx;
	}

	colourHash = ion::Hash(stream.str().c_str());
}

void Tile::CalculateCollisionHash()
{
	std::stringstream stream;
	for(int i = 0; i < pixelsPerTile; i++)
	{
		stream << pixels[i].collisionBits;
	}

	collisionHash = ion::Hash(stream.str().c_str());
}