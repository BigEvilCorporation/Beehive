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
	ePlatformSNES,

	ePlatformNum
};

struct PlatformConfig
{
	TargetPlatform platform;
	std::string name;
	u16 screenWidth;
	u16 screenHeight;
	u8 tileWidth;
	u8 tileHeight;
	u16 scrollPlaneWidthTiles;
	u16 scrollPlaneHeightTiles;
	u8 numPlanes;

	void Serialise(ion::io::Archive& archive);
};

namespace PlatformPresets
{
	enum Presets
	{
		ePresetMegaDrive,
		ePresetSNES_8x8,
		ePresetSNES_16x16,

		ePresetNum
	};

	static PlatformConfig s_configs[ePresetNum] =
	{
		{ ePlatformMegaDrive, "SEGA Mega Drive", 320, 240, 8, 8, 64, 32, 2 },					// SEGA Mega Drive = 8x8 tiles, 64x32 scroll planes, Plane A and plane B
		{ ePlatformSNES, "Super Nintendo (8x8 tile mode)", 256, 224, 8, 8, 64, 64, 1 },			// SNES = 8x8 tiles, 64x64 scroll planes, 1 plane
		{ ePlatformSNES, "Super Nintendo (16x16 tile mode)", 256, 224, 16, 16, 64, 64, 1 },		// SNES = 16x16 tiles, 64x64 scroll planes, 1 plane
	};
}
