///////////////////////////////////////////////////
// File:		Thread.h
// Date:		25th July 2011
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
		typedef u32 ThreadId;

		ThreadId GetCurrentThreadId();

		class Thread
		{
		public:
			Thread(const char* name = NULL);
			virtual ~Thread();

			void Join();
			u32 GetId() const;

		protected:
			virtual void Entry() = 0;

		private:
			#if defined ION_PLATFORM_WINDOWS
			static unsigned long WINAPI ThreadFunction(void* params);
			#endif

			unsigned long mThreadId;

			#if defined ION_PLATFORM_WINDOWS
			HANDLE mThreadHndl;
			#endif
		};
	}
}