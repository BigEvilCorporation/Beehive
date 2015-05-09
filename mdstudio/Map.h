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
#include "Stamp.h"

class Map
{
public:
	static const int defaultWidth = 64;
	static const int defaultHeight = 64;

	enum TileFlags
	{
		eFlipX = 1<<0,
		eFlipY = 1<<1,
		eHighPlane = 1<<2
	};

	Map();

	void Clear();

	int GetWidth() const;
	int GetHeight() const;

	void Resize(int w, int h);

	//Set tile on map
	void SetTile(int x, int y, TileId tile);
	TileId GetTile(int x, int y) const;

	//Set tile flags
	void SetTileFlags(int x, int y, u32 flags);
	u32 GetTileFlags(int x, int y) const;

	//Draw stamp on map
	void DrawStamp(int x, int y, const Stamp& stamp);

	const Tileset& GetTileset() const;
	Tileset& GetTileset();

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& stream) const;

private:
	int m_width;
	int m_height;

	struct TileDesc
	{
		TileDesc() { m_id = 0; m_flags = 0; }

		void Serialise(ion::io::Archive& archive)
		{
			archive.Serialise(m_id);
			archive.Serialise(m_flags);
		}

		TileId m_id;
		u32 m_flags;
	};

	Tileset m_tileset;
	std::vector<TileDesc> m_tiles;
};