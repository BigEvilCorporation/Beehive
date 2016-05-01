///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "Tile.h"
#include "Palette.h"

typedef std::vector<Tile> SpriteFrame;

class Sprite
{
public:
	Sprite();

	bool ImportBitmap(const std::string& filename, int widthCells, int heightCells, int maxCells);

	const SpriteFrame& GetFrame(int index) const;
	int GetNumFrames() const;

	const Palette& GetPalette() const;

private:
	std::vector<SpriteFrame> m_frames;
	Palette m_palette;
};