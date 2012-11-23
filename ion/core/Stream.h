///////////////////////////////////////////////////
// File:		Stream.h
// Date:		22nd November 2012
// Authors:		Matt Phillips
// Description:	Read/write stream for serialisation
///////////////////////////////////////////////////

#pragma once

#include "Types.h"

namespace ion
{
	namespace serialise
	{
		class Stream
		{
		public:
			virtual u64 Read(void* data, u64 size) = 0;
			virtual u64 Write(const void* data, u64 size) = 0;
		};
	}
}