///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "PlatformConfig.h"
#include <ion/io/Archive.h>

void PlatformConfig::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(name, "name");
	archive.Serialise((u32&)platform, "platform");
	archive.Serialise(screenWidth, "screenWidth");
	archive.Serialise(screenHeight, "screenHeight");
	archive.Serialise(tileWidth, "tileWidth");
	archive.Serialise(tileHeight, "tileHeight");
	archive.Serialise(scrollPlaneWidthTiles, "scrollPlaneWidthTiles");
	archive.Serialise(scrollPlaneHeightTiles, "scrollPlaneHeightTiles");
	archive.Serialise(numPlanes, "numPlanes");
}