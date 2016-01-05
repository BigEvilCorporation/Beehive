///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "CollisionMap.h"
#include "Project.h"
#include <algorithm>

#include <ion/core/memory/Memory.h>

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
			SetTerrainTile(x, y, InvalidTerrainTileId);
		}
	}
}

void CollisionMap::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_width, "width");
	archive.Serialise(m_height, "height");
	archive.Serialise(m_collisionTiles, "collisionTiles");

	for(int i = 0; i < m_collisionTiles.size(); i++)
	{
		m_collisionTiles[i] &= s_collisionTileTerrainIdMask;
	}
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
	std::vector<TerrainTileId> terrainTiles;

	//Set new size
	int size = width * height;
	terrainTiles.resize(size);

	//Fill with invalid tile
	std::fill(terrainTiles.begin(), terrainTiles.end(), InvalidTerrainTileId);

	//Copy tiles
	for(int x = 0; x < min(width, m_width); x++)
	{
		for(int y = 0; y < min(height, m_height); y++)
		{
			int tileIdx = (y * width) + x;
			terrainTiles[tileIdx] = GetTerrainTile(x, y);
		}
	}
	
	//Set new
	m_collisionTiles = terrainTiles;
	m_width = width;
	m_height = height;
}

void CollisionMap::SetTerrainTile(int x, int y, TerrainTileId tile)
{
	u32 tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "Out of range");
	u32& entry = m_collisionTiles[tileIdx];
	entry &= ~s_collisionTileTerrainIdMask;
	entry |= tileIdx;
}

TerrainTileId CollisionMap::GetTerrainTile(int x, int y) const
{
	u32 tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "Out of range");
	return m_collisionTiles[tileIdx] & s_collisionTileTerrainIdMask;
}

void CollisionMap::SetCollisionTileFlags(int x, int y, u16 flags)
{
	int tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "Out of range");
	u32& entry = m_collisionTiles[tileIdx];
	entry &= ~s_collisionTileFlagMask;
	entry |= flags;
}

u16 CollisionMap::GetCollisionTileFlags(int x, int y) const
{
	int tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "Out of range");
	return m_collisionTiles[tileIdx] & s_collisionTileFlagMask;
}

void CollisionMap::Export(const Project& project, std::stringstream& stream) const
{
	//Use default tile if there is one, else use first tile
	u32 defaultTileId = project.GetDefaultTerrainTile();
	if(defaultTileId == InvalidTerrainTileId)
	{
		defaultTileId = 0;
	}

	//Output to stream
	stream << std::hex << std::setfill('0') << std::uppercase;

	for(int y = 0; y < m_height; y++)
	{
		stream << "\tdc.w\t";

		for(int x = 0; x < m_width; x++)
		{
			TerrainTileId tileId = GetTerrainTile(x, y);
			u16 tileFlags = GetCollisionTileFlags(x, y);

			//If blank tile, use default tile
			u32 tileIndex = (tileId == InvalidTerrainTileId) ? defaultTileId : tileId;

			u16 word = (u16)tileIndex | tileFlags;
			stream << "0x" << std::setw(4) << (u32)word;

			if(x < (m_width - 1))
				stream << ",";
		}

		stream << std::endl;
	}

	stream << std::dec;
}

void CollisionMap::Export(const Project& project, ion::io::File& file) const
{
	//Use default tile if there is one, else use first tile
	u32 defaultTileId = project.GetDefaultTerrainTile();
	if(defaultTileId == InvalidTerrainTileId)
	{
		defaultTileId = 0;
	}

	for(int y = 0; y < m_height; y++)
	{
		for(int x = 0; x < m_width; x++)
		{
			TerrainTileId tileId = GetTerrainTile(x, y);
			u16 tileFlags = GetCollisionTileFlags(x, y);

			//If blank tile, use default tile
			u32 tileIndex = (tileId == InvalidTerrainTileId) ? defaultTileId : tileId;

			u16 word = (u16)tileIndex | tileFlags;
			ion::memory::EndianSwap(word);
			file.Write(&word, sizeof(u16));
		}
	}
}