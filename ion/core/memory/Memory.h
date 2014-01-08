///////////////////////////////////////////////////
// File:		MemoryManager.h
// Date:		2nd August 2011
// Authors:		Matt Phillips
// Description:	Memory management
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include <memory.h>

namespace ion
{
	namespace memory
	{
		void MemSet(void* address, int value, u32 size);
		void MemCopy(void* destination, void* source, u32 size);
	}
}