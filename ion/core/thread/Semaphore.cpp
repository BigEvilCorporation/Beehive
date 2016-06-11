///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Semaphore.cpp
// Date:		8th January 2014
// Authors:		Matt Phillips
// Description:	Threading and synchronisation
///////////////////////////////////////////////////

#include "core/thread/Semaphore.h"

namespace ion
{
	namespace thread
	{
		Semaphore::Semaphore(int maxSignalCount)
		{
			#if defined ION_PLATFORM_WINDOWS
			m_semaphoreHndl = CreateSemaphore(NULL, 0, maxSignalCount, NULL);
			#endif
		}

		Semaphore::~Semaphore()
		{
			#if defined ION_PLATFORM_WINDOWS
			CloseHandle(m_semaphoreHndl);
			#endif
		}

		void Semaphore::Signal()
		{
			#if defined ION_PLATFORM_WINDOWS
			ReleaseSemaphore(m_semaphoreHndl, 1, NULL);
			#endif
		}

		void Semaphore::Wait()
		{
			#if defined ION_PLATFORM_WINDOWS
			WaitForSingleObject(m_semaphoreHndl, INFINITE);
			#endif
		}
	}
}