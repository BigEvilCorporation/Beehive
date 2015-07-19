///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <io/Archive.h>
#include <sstream>

typedef u32 CollisionTileId;
static const CollisionTileId InvalidCollisionTileId = 0xFFFFFFFF;

class CollisionTile
{
public:
	static const int tileWidth = 8;
	static const int tileHeight = 8;
	static const int pixelsPerCollisionTile = tileWidth*tileHeight;

	CollisionTile();

	void SetIndex(u32 index);
	u32 GetIndex() const;

	void CalculateHash();
	u64 GetHash() const;

	void AddPixelCollisionBits(int x, int y, u8 collisionBits);
	void ClearPixelCollisionBits(int x, int y, u8 collisionBits);
	u8 GetPixelCollisionBits(int x, int y) const;

	void GetPixels(u8 pixels[pixelsPerCollisionTile]) const;
	void CopyPixels(const CollisionTile& tile);

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& stream) const;

private:
	u32 m_index;
	u64 m_hash;
	std::vector<u8> m_collisionPixels;
};