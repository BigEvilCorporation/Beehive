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

	void Resize(int w, int h);

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