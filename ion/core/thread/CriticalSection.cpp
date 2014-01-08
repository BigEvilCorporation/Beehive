///////////////////////////////////////////////////
// File:		CriticalSection.cpp
// Date:		8th January 2014
// Authors:		Matt Phillips
// Description:	Threading and synchronisation
///////////////////////////////////////////////////

#include "core/thread/CriticalSection.h"

namespace ion
{
	namespace thread
	{
		CriticalSection::CriticalSection()
		{
			#if defined ION_PLATFORM_WINDOWS
			InitializeCriticalSection(&mCriticalSectionHndl);
			#endif
		}

		CriticalSection::~CriticalSection()
		{
			#if defined ION_PLATFORM_WINDOWS
			DeleteCriticalSection(&mCriticalSectionHndl);
			#endif
		}

		bool CriticalSection::TryBegin()
		{
			#if defined ION_PLATFORM_WINDOWS
			return TryEnterCriticalSection(&mCriticalSectionHndl) != 0;
			#endif
		}

		void CriticalSection::Begin()
		{
			#if defined ION_PLATFORM_WINDOWS
			EnterCriticalSection(&mCriticalSectionHndl);
			#endif
		}

		void CriticalSection::End()
		{
			#if defined ION_PLATFORM_WINDOWS
			LeaveCriticalSection(&mCriticalSectionHndl);
			#endif
		}
	}
}