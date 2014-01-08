///////////////////////////////////////////////////
// File:		Event.cpp
// Date:		8th January 2014
// Authors:		Matt Phillips
// Description:	Threading and synchronisation
///////////////////////////////////////////////////

#include "core/thread/Event.h"

namespace ion
{
	namespace thread
	{
		Event::Event()
		{
			#if defined ION_PLATFORM_WINDOWS
			mThreadEventHndl = CreateEvent(NULL, FALSE, FALSE, NULL);
			#endif
		}

		Event::~Event()
		{
			#if defined ION_PLATFORM_WINDOWS
			CloseHandle(mThreadEventHndl);
			#endif
		}

		void Event::Signal()
		{
			#if defined ION_PLATFORM_WINDOWS
			SetEvent(mThreadEventHndl);
			#endif
		}

		void Event::Wait()
		{
			#if defined ION_PLATFORM_WINDOWS
			WaitForSingleObject(mThreadEventHndl, INFINITE);
			#endif
		}
	}
}