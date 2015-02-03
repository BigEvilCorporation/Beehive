#pragma once

#include <io/Archive.h>
#include <sstream>

struct Pixel
{
	u8 colourIdx;
	u8 collisionBits;
};

struct Tile
{
	static const int tileWidth = 8;
	static const int tileHeight = 8;
	static const int pixelsPerTile = tileWidth*tileHeight;

	Tile();

	void SetPixelColour(int x, int y, u8 colourIdx);
	u8 GetPixelColour(int x, int y) const;

	void AddPixelCollisionBits(int x, int y, u8 collisionBits);
	u8 GetPixelCollisionBits(int x, int y) const;

	void Serialise(ion::io::Archive& archive);
	void ExportColourText(std::stringstream& outputString);
	void ExportCollisionText(std::stringstream& outputString);

	void CalculateColourHash();
	void CalculateCollisionHash();

	u8 defaultPaletteIdx;
	u32 colourHash;
	u32 collisionHash;
	Pixel pixels[pixelsPerTile];
};