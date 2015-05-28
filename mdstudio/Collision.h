///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <ion/core/Types.h>
#include <string>

#include "Icon.h"

struct CollisionType
{
	std::string name;
	Icon* icon;
	u8 bit;

	void Serialise(ion::io::Archive& archive)
	{
		archive.Serialise(name);
		archive.Serialise(bit);
	}
};