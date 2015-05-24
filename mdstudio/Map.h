///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <sstream>

#include <io/Archive.h>
#include <maths/Vector.h>

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

	//Set stamp on map
	void SetStamp(int x, int y, const Stamp& stamp, u32 flipFlags);
	void BakeStamp(int x, int y, const Stamp& stamp, u32 flipFlags);
	StampId FindStamp(int x, int y, ion::Vector2i& topLeft, u32& flags) const;

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

	struct StampDesc
	{
		StampDesc() { m_id = 0; m_flags = 0; }
		StampDesc(StampId stampId, u32 flags) { m_id = stampId; m_flags = flags; }

		void Serialise(ion::io::Archive& archive)
		{
			archive.Serialise(m_id);
			archive.Serialise(m_flags);
		}

		StampId m_id;
		u32 m_flags;
	};

	std::vector<TileDesc> m_tiles;
	std::vector< std::tuple<ion::Vector2i, ion::Vector2i, StampDesc> > m_stamps;
};