///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Atomic.cpp
// Date:		8th January 2014
// Authors:		Matt Phillips
// Description:	Threading and synchronisation
///////////////////////////////////////////////////

#include "core/thread/Atomic.h"

namespace ion
{
	namespace thread
	{
		namespace atomic
		{
			void Increment(u32& integer)
			{
				#if defined ION_PLATFORM_WINDOWS
				InterlockedIncrement((LONG*)&integer);
				#endif
			}

			void Decrement(u32& integer)
			{
				#if defined ION_PLATFORM_WINDOWS
				InterlockedDecrement((LONG*)&integer);
				#endif
			}
		}
	}
}