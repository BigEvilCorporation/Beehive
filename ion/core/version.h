///////////////////////////////////////////////////
// File:		Version.h
// Date:		27th August 2011
// Authors:		Matt Phillips
// Description:	ion::engine version ID
///////////////////////////////////////////////////

#pragma once

namespace ion
{
	struct Version
	{
		int Major;
		int Minor;
		int Build;
	};

	static const Version sVersion = { 0, 0, 2 };
}