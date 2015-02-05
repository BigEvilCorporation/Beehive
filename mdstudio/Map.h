#pragma once

#include <vector>

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

private:
	int m_width;
	int m_height;

	Tileset m_tileset;
	std::vector<TileId> m_tiles;
};