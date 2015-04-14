///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <io/Archive.h>
#include <sstream>

#include "Palette.h"

struct Pixel
{
	u8 colourIdx;
	u8 collisionBits;

	void Serialise(ion::io::Archive& archive)
	{
		archive.Serialise(colourIdx);
		archive.Serialise(collisionBits);
	}
};

class Tile
{
public:
	static const int tileWidth = 8;
	static const int tileHeight = 8;
	static const int pixelsPerTile = tileWidth*tileHeight;

	Tile();

	void SetPixelColour(int x, int y, u8 colourIdx);
	u8 GetPixelColour(int x, int y) const;

	void AddPixelCollisionBits(int x, int y, u8 collisionBits);
	u8 GetPixelCollisionBits(int x, int y) const;

	void SetPaletteId(PaletteId palette);
	PaletteId GetPaletteId() const;

	void Serialise(ion::io::Archive& archive);
	void ExportColour(std::stringstream& stream) const;
	void ExportCollision(std::stringstream& stream) const;

	void CalculateColourHash();
	void CalculateCollisionHash();
	u32 GetColourHash() const;
	u32 GetCollisionHash() const;

private:
	PaletteId m_palette;
	u32 m_colourHash;
	u32 m_collisionHash;
	std::vector<Pixel> m_pixels;
};