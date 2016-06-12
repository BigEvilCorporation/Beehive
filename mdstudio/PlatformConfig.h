///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <ion/core/Types.h>
#include <string>

namespace ion
{
	namespace io
	{
		class Archive;
	}
}

enum TargetPlatform
{
	ePlatformMegaDrive,
	ePlatformSNES_8x8,
	ePlatformSNES_16x16,

	ePlatformNum
};

struct PlatformConfig
{
	std::string name;
	u8 tileWidth;
	u8 tileHeight;
	u16 scrollPlaneWidthTiles;
	u16 scrollPlaneHeightTiles;
	u8 numPlanes;

	void Serialise(ion::io::Archive& archive);
};

namespace PlatformConfigs
{
	static PlatformConfig s_configs[ePlatformNum] =
	{
		{ "SEGA Mega Drive", 8, 8, 64, 32, 2 }, // SEGA Mega Drive = 8x8 tiles, 64x32 scroll planes, Plane A and plane B
		{ "Super Nintendo (8x8 tile mode)", 8, 8, 64, 64, 1 }, // SNES = 8x8 tiles, 64x64 scroll planes, 1 plane
		{ "Super Nintendo (16x16 tile mode)", 16, 16, 64, 64, 1 }, // SNES = 16x16 tiles, 64x64 scroll planes, 1 plane
	};
}
