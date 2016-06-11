///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Hash.cpp
// Date:		15th December 2011
// Authors:		Matt Phillips
// Description:	Various hashing algorithms
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

	u32 Hash(const u8* data, int size)
	{
		u32 hash = 0;

		for(int i = 0; i < size; i++)
		{
			hash = (*data++) + (hash << 6) + (hash << 16) - hash;
		}

		return hash;
	}

	u64 Hash64(const u8* data, int size)
	{
		const u64 mulp = 2654435789;

		u64 hash = 104395301;

		for(int i = 0; i < size; i++)
		{
			hash += (*data++ * mulp) ^ (hash >> 23);
		}

		return hash ^ (hash << 37);
	}
}