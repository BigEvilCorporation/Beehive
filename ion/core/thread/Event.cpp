///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
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
			m_threadEventHndl = CreateEvent(NULL, FALSE, FALSE, NULL);
			#endif
		}

		Event::~Event()
		{
			#if defined ION_PLATFORM_WINDOWS
			CloseHandle(m_threadEventHndl);
			#endif
		}

		void Event::Signal()
		{
			#if defined ION_PLATFORM_WINDOWS
			SetEvent(m_threadEventHndl);
			#endif
		}

		void Event::Wait()
		{
			#if defined ION_PLATFORM_WINDOWS
			WaitForSingleObject(m_threadEventHndl, INFINITE);
			#endif
		}
	}
}