///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include <core/debug/Debug.h>
#include <core/memory/Memory.h>
#include <core/cryptography/Hash.h>

#include <string>
#include <iomanip>

#include "TerrainTile.h"

TerrainTile::TerrainTile()
{
	m_hash = 0;
	m_heightmap.resize(tileWidth);
}

void TerrainTile::CopyHeights(const TerrainTile& tile)
{
	m_heightmap = tile.m_heightmap;
}

void TerrainTile::GetHeights(s8 heights[tileWidth]) const
{
	ion::memory::MemCopy(heights, &m_heightmap[0], tileWidth);
}

void TerrainTile::SetHeight(int x, s8 height)
{
	ion::debug::Assert(x < tileWidth, "Out of range");
	ion::debug::Assert(height >= -tileHeight && height <= tileHeight, "Out of range");
	m_heightmap[x] = height;
}

void TerrainTile::ClearHeight(int x)
{
	ion::debug::Assert(x < tileWidth, "Out of range");
	m_heightmap[x] = 0;
}

s8 TerrainTile::GetHeight(int x) const
{
	ion::debug::Assert(x < tileWidth, "Out of range");
	return m_heightmap[x];
}

void TerrainTile::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_hash, "hash");
	archive.Serialise(m_heightmap, "heightMap");
}

void TerrainTile::Export(std::stringstream& stream) const
{
	stream << std::hex << std::setfill('0') << std::uppercase;

	stream << "\tdc.b\t";

	//1 byte per height
	for(int x = 0; x < tileWidth; x++)
	{
		stream << "0x";
		stream << std::setw(1) << (int)GetHeight(x);

		if(x < (tileWidth-1))
			stream << ", ";
	}

	stream << std::dec;
}

void TerrainTile::Export(ion::io::File& file) const
{
	//1 byte per width
	for(int x = 0; x < tileWidth; x++)
	{
		s8 byte = GetHeight(x);
		file.Write(&byte, sizeof(s8));
	}
}

u64 TerrainTile::CalculateHash()
{
	m_hash = ion::Hash64((const u8*)&m_heightmap[0], tileWidth);
	return m_hash;
}

u64 TerrainTile::GetHash() const
{
	return m_hash;
}
