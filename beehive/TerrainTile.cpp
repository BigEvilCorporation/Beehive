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

#include <core/debug/Debug.h>
#include <core/memory/Memory.h>
#include <core/cryptography/Hash.h>

#include <string>
#include <iomanip>

#include "TerrainTile.h"
#include "PlatformConfig.h"

TerrainTile::TerrainTile()
{
	const PlatformConfig& defaultPlatformConfig = PlatformPresets::s_configs[PlatformPresets::ePresetMegaDrive];

	m_hash = 0;
	m_width = defaultPlatformConfig.tileWidth;
	m_height = defaultPlatformConfig.tileHeight;
}

TerrainTile::TerrainTile(u8 width, u8 height)
{
	m_hash = 0;
	m_width = width;
	m_height = height;
	m_heightmap.resize(width * height, 0);
}

void TerrainTile::CopyHeights(const TerrainTile& tile)
{
	m_heightmap = tile.m_heightmap;
}

void TerrainTile::GetHeights(std::vector<s8>& heights) const
{
	heights.resize(m_heightmap.size());
	ion::memory::MemCopy(heights.data(), m_heightmap.data(), m_heightmap.size());
}

void TerrainTile::SetHeight(int x, s8 height)
{
	ion::debug::Assert(x < m_width, "eOut of range");
	ion::debug::Assert(height >= -m_height && height <= m_height, "eOut of range");
	m_heightmap[x] = height;
}

void TerrainTile::ClearHeight(int x)
{
	ion::debug::Assert(x < m_width, "eOut of range");
	m_heightmap[x] = 0;
}

s8 TerrainTile::GetHeight(int x) const
{
	ion::debug::Assert(x < m_width, "eOut of range");
	return m_heightmap[x];
}

void TerrainTile::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_width, "width");
	archive.Serialise(m_height, "height");
	archive.Serialise(m_hash, "hash");
	archive.Serialise(m_heightmap, "heightMap");
}

void TerrainTile::Export(std::stringstream& stream) const
{
	stream << std::hex << std::setfill('0') << std::uppercase;

	stream << "\tdc.b\t";

	//1 byte per height
	for(int x = 0; x < m_width; x++)
	{
		stream << "0x";
		stream << std::setw(1) << (int)GetHeight(x);

		if(x < (m_width-1))
			stream << ", ";
	}

	stream << std::dec;
}

void TerrainTile::Export(ion::io::File& file) const
{
	//1 byte per width
	for(int x = 0; x < m_width; x++)
	{
		s8 byte = GetHeight(x);
		file.Write(&byte, sizeof(s8));
	}
}

u64 TerrainTile::CalculateHash()
{
	m_hash = ion::Hash64((const u8*)m_heightmap.data(), m_width);
	return m_hash;
}

u64 TerrainTile::GetHash() const
{
	return m_hash;
}
