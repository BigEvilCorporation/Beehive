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

#include "Tile.h"
#include "Tileset.h"

typedef u32 StampId;
static const StampId InvalidStampId = 0;

class Project;

class Stamp
{
public:
	Stamp();
	Stamp(StampId stampId, const Stamp& rhs);
	Stamp(StampId stampId, int width, int height);

	StampId GetId() const;
	int GetWidth() const;
	int GetHeight() const;

	void SetTile(int x, int y, TileId tile);
	TileId GetTile(int x, int y) const;

	void SetTileFlags(int x, int y, u32 flags);
	u32 GetTileFlags(int x, int y) const;

	void SetName(const std::string& name);
	const std::string& GetName() const;
	u32 GetNameHash() const;

	void Serialise(ion::io::Archive& archive);
	void Export(const Project& project, std::stringstream& stream) const;
	void Export(const Project& project, ion::io::File& file) const;

	int GetBinarySize() const { return m_width * m_height * sizeof(u16); }

private:
	StampId m_id;
	int m_width;
	int m_height;
	std::string m_name;
	u32 m_nameHash;

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

	std::vector<TileDesc> m_tiles;
};