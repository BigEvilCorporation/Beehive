///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <sstream>
#include <io/Archive.h>
#include "Tileset.h"

class Map
{
public:
	static const int defaultWidth = 64;
	static const int defaultHeight = 64;

	Map();

	int GetWidth() const;
	int GetHeight() const;

	void Resize(int w, int h);
	void SetTile(int x, int y, TileId tile);
	TileId GetTile(int x, int y) const;

	Tileset& GetTileset();

	void Serialise(ion::io::Archive& archive) {}
	void Export(std::stringstream& outputText) {}

private:
	int m_width;
	int m_height;

	Tileset m_tileset;
	std::vector<TileId> m_tiles;
};