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

class Project;

struct StampMapEntry
{
	StampMapEntry() { m_id = 0; m_flags = 0; }
	StampMapEntry(StampId stampId, u32 flags, const ion::Vector2i& position, const ion::Vector2i& size)
	{
		m_id = stampId;
		m_flags = flags;
		m_position = position;
		m_size = size;
	}

	void Serialise(ion::io::Archive& archive)
	{
		archive.Serialise(m_id);
		archive.Serialise(m_flags);
		archive.Serialise(m_position);
		archive.Serialise(m_size);
	}

	StampId m_id;
	u32 m_flags;
	ion::Vector2i m_position;
	ion::Vector2i m_size;
};

typedef std::vector<StampMapEntry> TStampPosMap;

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
	void RemoveStamp(int x, int y);

	const TStampPosMap::const_iterator StampsBegin() const;
	const TStampPosMap::const_iterator StampsEnd() const;

	void Serialise(ion::io::Archive& archive);
	void Export(const Project& project, std::stringstream& stream) const;

private:
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

	void BakeStamp(std::vector<TileDesc>& tiles, int x, int y, const Stamp& stamp, u32 flipFlags) const;

	int m_width;
	int m_height;
	std::vector<TileDesc> m_tiles;
	TStampPosMap m_stamps;
};