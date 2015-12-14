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

#include "CollisionTile.h"

CollisionTile::CollisionTile()
{
	m_index = 0;
	m_hash = 0;
	m_heightmap.resize(tileWidth);
}

void CollisionTile::SetIndex(u32 index)
{
	m_index = index;
}

u32 CollisionTile::GetIndex() const
{
	return m_index;
}

void CollisionTile::CopyHeights(const CollisionTile& tile)
{
	m_heightmap = tile.m_heightmap;
}

void CollisionTile::GetHeights(s8 heights[tileWidth]) const
{
	ion::memory::MemCopy(heights, &m_heightmap[0], tileWidth);
}

void CollisionTile::SetHeight(int x, s8 height)
{
	ion::debug::Assert(x < tileWidth, "Out of range");
	ion::debug::Assert(height >= -tileHeight && height <= tileHeight, "Out of range");
	m_heightmap[x] = height;
}

void CollisionTile::ClearHeight(int x)
{
	ion::debug::Assert(x < tileWidth, "Out of range");
	m_heightmap[x] = 0;
}

s8 CollisionTile::GetHeight(int x) const
{
	ion::debug::Assert(x < tileWidth, "Out of range");
	return m_heightmap[x];
}

void CollisionTile::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_index, "index");
	archive.Serialise(m_hash, "hash");
	archive.Serialise(m_heightmap, "heightMap");
}

void CollisionTile::Export(std::stringstream& stream) const
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

void CollisionTile::Export(ion::io::File& file) const
{
	//1 byte per width
	for(int x = 0; x < tileWidth; x += 2)
	{
		s8 byte = GetHeight(x);;
		file.Write(&byte, sizeof(s8));
	}
}

void CollisionTile::CalculateHash()
{
	m_hash = ion::Hash64((const u8*)&m_heightmap[0], tileWidth);
}

u64 CollisionTile::GetHash() const
{
	return m_hash;
}
