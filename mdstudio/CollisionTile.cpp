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
	m_collisionPixels.resize(pixelsPerCollisionTile);
}

void CollisionTile::SetIndex(u32 index)
{
	m_index = index;
}

u32 CollisionTile::GetIndex() const
{
	return m_index;
}

void CollisionTile::CopyPixels(const CollisionTile& tile)
{
	m_collisionPixels = tile.m_collisionPixels;
}

void CollisionTile::GetPixels(u8 pixels[pixelsPerCollisionTile]) const
{
	ion::memory::MemCopy(pixels, &m_collisionPixels[0], pixelsPerCollisionTile);
}

void CollisionTile::AddPixelCollisionBits(int x, int y, u8 collisionBits)
{
	int pixelIdx = (y * tileHeight) + x;
	ion::debug::Assert(pixelIdx < pixelsPerCollisionTile, "Out of range");
	m_collisionPixels[pixelIdx] |= collisionBits;
}

void CollisionTile::ClearPixelCollisionBits(int x, int y, u8 collisionBits)
{
	int pixelIdx = (y * tileHeight) + x;
	ion::debug::Assert(pixelIdx < pixelsPerCollisionTile, "Out of range");
	m_collisionPixels[pixelIdx] &= ~collisionBits;
}

u8 CollisionTile::GetPixelCollisionBits(int x, int y) const
{
	int pixelIdx = (y * tileHeight) + x;
	ion::debug::Assert(pixelIdx < pixelsPerCollisionTile, "Out of range");
	return m_collisionPixels[pixelIdx];
}

void CollisionTile::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_index, "index");
	archive.Serialise(m_hash, "hash");
	archive.Serialise(m_collisionPixels, "collisionPixels");
}

void CollisionTile::Export(std::stringstream& stream) const
{
	stream << std::hex << std::setfill('0') << std::uppercase;

	for(int y = 0; y < tileHeight; y++)
	{
		stream << "\tdc.l\t0x";

		for(int x = 0; x < tileWidth; x++)
		{
			stream << std::setw(1) << (int)GetPixelCollisionBits(x, y);
		}

		stream << std::endl;
	}

	stream << std::dec;
}

void CollisionTile::CalculateHash()
{
	m_hash = ion::Hash64(&m_collisionPixels[0], pixelsPerCollisionTile);
}

u64 CollisionTile::GetHash() const
{
	return m_hash;
}
