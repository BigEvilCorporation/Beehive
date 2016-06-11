///////////////////////////////////////////////////
// File:		Event.h
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
		class Event
		{
		public:
			Event();
			~Event();

			void Signal();
			void Wait();

		private:
			#if defined ION_PLATFORM_WINDOWS
			HANDLE m_threadEventHndl;
			#endif
		};
	}
}