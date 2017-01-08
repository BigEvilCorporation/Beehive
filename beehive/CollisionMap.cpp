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

#include "CollisionMap.h"
#include "Project.h"
#include <algorithm>

#include <ion/core/memory/Memory.h>

CollisionMap::CollisionMap()
{
	const PlatformConfig& defaultPlatformConfig = PlatformPresets::s_configs[PlatformPresets::ePresetMegaDrive];
	m_width = 0;
	m_height = 0;
	Resize(defaultPlatformConfig.scrollPlaneWidthTiles, defaultPlatformConfig.scrollPlaneHeightTiles, false, false);
}

CollisionMap::CollisionMap(const PlatformConfig& platformConfig)
{
	m_width = 0;
	m_height = 0;
	Resize(platformConfig.scrollPlaneWidthTiles, platformConfig.scrollPlaneHeightTiles, false, false);
}

CollisionMap::CollisionMap(const PlatformConfig& platformConfig, int width, int height)
{
	m_width = 0;
	m_height = 0;
	Resize(width, height, false, false);
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
	archive.Serialise(m_terrainBeziers, "terrainBeziers");
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

void CollisionMap::Resize(int width, int height, bool shiftRight, bool shiftDown)
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
			int destTileIdx = (y * width) + x;
			if(shiftRight && width > m_width)
				destTileIdx += (width - m_width);
			if(shiftDown && height > m_height)
				destTileIdx += (height - m_height) * width;

			terrainTiles[destTileIdx] = GetTerrainTile(x, y);
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
	ion::debug::Assert(tileIdx < (m_width * m_height), "eOut of range");
	u32& entry = m_collisionTiles[tileIdx];
	entry &= ~s_collisionTileTerrainIdMask;
	entry |= tile;
}

TerrainTileId CollisionMap::GetTerrainTile(int x, int y) const
{
	u32 tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "eOut of range");
	return m_collisionTiles[tileIdx] & s_collisionTileTerrainIdMask;
}

void CollisionMap::SetCollisionTileFlags(int x, int y, u16 flags)
{
	int tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "eOut of range");
	u32& entry = m_collisionTiles[tileIdx];
	entry &= ~s_collisionTileFlagMask;
	entry |= flags;
}

u16 CollisionMap::GetCollisionTileFlags(int x, int y) const
{
	int tileIdx = (y * m_width) + x;
	ion::debug::Assert(tileIdx < (m_width * m_height), "eOut of range");
	return m_collisionTiles[tileIdx] & s_collisionTileFlagMask;
}

ion::gamekit::BezierPath* CollisionMap::AddTerrainBezier()
{
	m_terrainBeziers.push_back(ion::gamekit::BezierPath());
	return &m_terrainBeziers.back();
}

void CollisionMap::AddTerrainBezier(const ion::gamekit::BezierPath& bezier)
{
	m_terrainBeziers.push_back(bezier);
}

ion::gamekit::BezierPath* CollisionMap::GetTerrainBezier(u32 index)
{
	ion::debug::Assert(index < m_terrainBeziers.size(), "eOut of range");
	return &m_terrainBeziers[index];
}

const ion::gamekit::BezierPath* CollisionMap::GetTerrainBezier(u32 index) const
{
	ion::debug::Assert(index < m_terrainBeziers.size(), "eOut of range");
	return &m_terrainBeziers[index];
}

void CollisionMap::RemoveTerrainBezier(u32 index)
{
	ion::debug::Assert(index < m_terrainBeziers.size(), "eOut of range");
	m_terrainBeziers.erase(m_terrainBeziers.begin() + index);
}

int CollisionMap::GetNumTerrainBeziers() const
{
	return m_terrainBeziers.size();
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