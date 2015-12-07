///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <io/Archive.h>
#include <sstream>

#include "Palette.h"

typedef u32 TileId;
static const TileId InvalidTileId = 0xFFFFFFFF;

class Tile
{
public:
	static const int tileWidth = 8;
	static const int tileHeight = 8;
	static const int pixelsPerTile = tileWidth*tileHeight;

	Tile();

	void SetIndex(u32 index);
	u32 GetIndex() const;

	void CalculateHash();
	u64 GetHash() const;

	void SetPixelColour(int x, int y, u8 colourIdx);
	u8 GetPixelColour(int x, int y) const;
	void CopyPixels(const Tile& tile);
	void GetPixels(u8 pixels[pixelsPerTile]) const;

	void SetPaletteId(PaletteId palette);
	PaletteId GetPaletteId() const;

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& stream) const;
	void Export(ion::io::File& file) const;

private:
	PaletteId m_palette;
	u32 m_index;
	u64 m_hash;
	std::vector<u8> m_pixels;
};