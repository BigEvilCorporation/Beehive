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

#include <vector>
#include <sstream>

#include <io/Archive.h>
#include <maths/Vector.h>

#include "TerrainTile.h"

class Project;

class CollisionMap
{
public:

	static const u16 s_collisionTileFlagMask = eCollisionTileFlagAll;
	static const u16 s_collisionTileTerrainIdMask = ~eCollisionTileFlagAll;

	static const int defaultWidth = 64;
	static const int defaultHeight = 64;

	CollisionMap();

	void Clear();

	int GetWidth() const;
	int GetHeight() const;

	void Resize(int w, int h, bool shiftRight);

	//Set collision tile on CollisionMap
	void SetTerrainTile(int x, int y, TerrainTileId tile);
	TerrainTileId GetTerrainTile(int x, int y) const;

	//Set collision tile flags
	void SetCollisionTileFlags(int x, int y, u16 flags);
	u16 GetCollisionTileFlags(int x, int y) const;

	void Serialise(ion::io::Archive& archive);
	void Export(const Project& project, std::stringstream& stream) const;
	void Export(const Project& project, ion::io::File& file) const;

private:
	typedef u16 TTerrainTileDesc;

	int m_width;
	int m_height;
	std::vector<TerrainTileId> m_collisionTiles;
};