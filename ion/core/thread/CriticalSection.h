///////////////////////////////////////////////////
// File:		CriticalSection.h
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
		class CriticalSection
		{
		public:
			CriticalSection();
			~CriticalSection();

			bool TryBegin();
			void Begin();
			void End();
			
		private:
			#if defined ION_PLATFORM_WINDOWS
			CRITICAL_SECTION mCriticalSectionHndl;
			#endif
		};
	}
}