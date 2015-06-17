///////////////////////////////////////////////////
// File:		MemoryManager.cpp
// Date:		2nd August 2011
// Authors:		Matt Phillips
// Description:	Memory management
///////////////////////////////////////////////////

#include "core/memory/Memory.h"

namespace ion
{
	namespace memory
	{
		void MemSet(void* address, int value, u32 size)
		{
			memset(address, value, (size_t)size);
		}

		void MemCopy(void* destination, const void* source, u32 size)
		{
			memcpy(destination, source, size);
		}
	}
}