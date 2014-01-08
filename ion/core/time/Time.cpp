///////////////////////////////////////////////////
// File:		Time.cpp
// Date:		25th July 2011
// Authors:		Matt Phillips
// Description:	System time and ticks
///////////////////////////////////////////////////

#include "Time.h"

#include <Windows.h>

namespace ion
{
	namespace time
	{
		u64 GetSystemTicks()
		{
			LARGE_INTEGER ticks;
			QueryPerformanceCounter(&ticks);
			return (u64)ticks.QuadPart;
		}

		double TicksToSeconds(u64 ticks)
		{
			static double timerFrequency = 0.0;

			if(timerFrequency == 0)
			{
				LARGE_INTEGER freq = {0};
				QueryPerformanceFrequency(&freq);
				timerFrequency = (double)freq.QuadPart;
			}

			return (double)ticks / timerFrequency;
		}
	}
}