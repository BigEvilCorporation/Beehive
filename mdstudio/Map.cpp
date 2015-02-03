#include "Map.h"

Map::Map()
{
	Resize(defaultWidth, defaultHeight);
}

void Map::Resize(int w, int h)
{
	int size = w * h;
	tiles.resize(size);
	width = w;
	height = h;
}

void Map::SetTile(int x, int y, const Tile& tile)
{
	int tileIdx = (y * height) + x;
	ion::debug::Assert(tileIdx < (width * height), "Out of range");
	tiles[tileIdx] = tile;
}

const Tile& Map::GetTile(int x, int y) const
{
	int tileIdx = (y * height) + x;
	ion::debug::Assert(tileIdx < (width * height), "Out of range");
	return tiles[tileIdx];
}