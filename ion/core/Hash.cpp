///////////////////////////////////////////////////
// File:		Hash.cpp
// Date:		15th December 2011
// Authors:		Matt Phillips
// Description:	Various hashign algorithms
///////////////////////////////////////////////////

#include "Hash.h"

namespace ion
{
	u32 Hash(const char* string)
	{
		u32 hash = 0;

		while(*string)
		{
			hash = (*string) + (hash << 6) + (hash << 16) - hash;
			++string;
		}

		return hash;
	}
}