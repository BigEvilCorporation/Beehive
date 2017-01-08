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

#pragma once

#include <io/Archive.h>
#include <sstream>

#include "Palette.h"
#include "PlatformConfig.h"

typedef u32 TileId;
static const TileId InvalidTileId = 0xFFFFFFFF;

class Tile
{
public:
	Tile();
	Tile(u8 width, u8 height);

	void SetIndex(u32 index);
	u32 GetIndex() const;

	void CalculateHash();
	u64 GetHash() const;

	u8 GetWidth() const { return m_width; }
	u8 GetHeight() const { return m_height; }

	void SetPixelColour(int x, int y, u8 colourIdx);
	u8 GetPixelColour(int x, int y) const;
	void CopyPixels(const Tile& tile);
	void GetPixels(std::vector<u8>& pixels) const;

	void SetPaletteId(PaletteId palette);
	PaletteId GetPaletteId() const;

	void Serialise(ion::io::Archive& archive);
	void Export(const PlatformConfig& config, std::stringstream& stream) const;
	void Export(const PlatformConfig& config, ion::io::File& file) const;
	u32 GetBinarySize() const;

private:
	PaletteId m_palette;
	u32 m_index;
	u64 m_hash;
	u8 m_width;
	u8 m_height;
	std::vector<u8> m_pixels;
};