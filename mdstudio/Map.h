#pragma once

#include <vector>

#include "Tile.h"

struct Map
{
	static const int defaultWidth = 64;
	static const int defaultHeight = 64;

	Map();

	void Resize(int w, int h);
	void SetTile(int x, int y, const Tile& tile);
	const Tile& GetTile(int x, int y) const;

	int width;
	int height;

	std::vector<Tile> tiles;
};