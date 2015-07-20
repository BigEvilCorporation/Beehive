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

#include "CollisionTile.h"

class Project;

class CollisionMap
{
public:
	static const int defaultWidth = 64;
	static const int defaultHeight = 64;

	CollisionMap();

	void Clear();

	int GetWidth() const;
	int GetHeight() const;

	void Resize(int w, int h);

	//Set collision tile on CollisionMap
	void SetCollisionTile(int x, int y, CollisionTileId tile);
	CollisionTileId GetCollisionTile(int x, int y) const;

	void Serialise(ion::io::Archive& archive);
	void Export(const Project& project, std::stringstream& stream) const;

private:
	int m_width;
	int m_height;
	std::vector<CollisionTileId> m_collisionTiles;
};