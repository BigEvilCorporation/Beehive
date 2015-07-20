///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "CollisionMap.h"
#include "Project.h"
#include <algorithm>

CollisionMap::CollisionMap()
{
	m_width = 0;
	m_height = 0;
	Resize(defaultWidth, defaultHeight);
}

void CollisionMap::Clear()
{
	//Clear tiles
	for(int x = 0; x < m_width; x++)
	{
		for(int y = 0; y < m_height; y++)
		{
			SetCollisionTile(x, y, InvalidCollisionTileId);
		}
	}
}

void CollisionMap::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_width, "width");
	archive.Serialise(m_height, "height");
	archive.Serialise(m_collisionTiles, "collisionTiles");
}

int CollisionMap::GetWidth() const
{
	return m_width;
}

int CollisionMap::GetHeight() const
{
	return m_height;
}

void CollisionMap::Resize(int width, int height)
{
	//Create new tile array
	std::vector<CollisionTileId> collisionTiles;

	//Set new size
	int size = width * height;
	collisionTiles.resize(size);

	//Fill with invalid tile
	std::fill(collisionTiles.begin(), collisionTiles.end(), InvalidCollisionTileId);

	//Copy tiles
	for(int x = 0; x < min(width, m_width); x++)
	{
		for(int y = 0; y < min(height, m_height); y++)
		{
			int tileIdx = (y * width) + x;
			collisionTiles[tileIdx] = GetCollisionTile(x, y);
		}
	}
	
	//Set new
	m_collisionTiles = collisionTiles;
	m_width = width;
	m_height = height;
}

void CollisionMap::SetCollisionTile(int x, int y, CollisionTileId tile)
{
	int tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "Out of range");
	m_collisionTiles[tileIdx] = tile;
}

CollisionTileId CollisionMap::GetCollisionTile(int x, int y) const
{
	int tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "Out of range");
	return m_collisionTiles[tileIdx];
}

void CollisionMap::Export(const Project& project, std::stringstream& stream) const
{
	//Output to stream
	stream << std::hex << std::setfill('0') << std::uppercase;

	for(int y = 0; y < m_height; y++)
	{
		stream << "\tdc.b\t";

		for(int x = 0; x < m_width; x++)
		{
			stream << "0x" << std::setw(2) << (u32)m_collisionTiles[(y * m_width) + x];

			if(x < (m_width - 1))
				stream << ",";
		}

		stream << std::endl;
	}

	stream << std::dec;
}