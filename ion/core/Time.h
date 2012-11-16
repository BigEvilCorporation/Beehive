///////////////////////////////////////////////////
// File:		Time.h
// Date:		25th July 2011
// Authors:		Matt Phillips
// Description:	System time and ticks
///////////////////////////////////////////////////

#pragma once

#include "Types.h"

namespace ion
{
	namespace time
	{
		u64 GetSystemTicks();
		double TicksToSeconds(u64 ticks);
	}
}