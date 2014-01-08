///////////////////////////////////////////////////
// File:		Atomic.h
// Date:		8th January 2014
// Authors:		Matt Phillips
// Description:	Threading and synchronisation
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"

#if defined ION_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace ion
{
	namespace thread
	{
		namespace atomic
		{
			void Increment(u32& integer);
			void Decrement(u32& integer);
		}
	}
}