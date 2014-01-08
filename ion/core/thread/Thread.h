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
			static unsigned long WINAPI ThreadFunction(void* params);

			unsigned long mThreadId;

			#if defined ION_PLATFORM_WINDOWS
			HANDLE mThreadHndl;
			#endif
		};
	}
}