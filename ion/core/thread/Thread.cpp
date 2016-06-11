///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Thread.cpp
// Date:		25th July 2011
// Authors:		Matt Phillips
// Description:	Threading and synchronisation
///////////////////////////////////////////////////

#include "core/thread/Thread.h"

namespace ion
{
	namespace thread
	{
		ThreadId GetCurrentThreadId()
		{
			#if defined ION_PLATFORM_WINDOWS
			return (ThreadId)::GetCurrentThreadId();
			#endif
		}

		Thread::Thread(const char* name)
		{
			if(!name)
			{
				name = "ion::thread";
			}

			#if defined ION_PLATFORM_WINDOWS
			m_threadHndl = CreateThread(	NULL,                   //Default security attributes
										0,                      //Default stack size  
										Thread::ThreadFunction, //Thread function
										this,					//User data
										0,                      //Default creation flags 
										&m_threadId);			//Thread id

			//Set thread name by invoking VC exception
			const DWORD MS_VC_EXCEPTION = 0x406D1388;

			struct THREADNAME_INFO
			{
				DWORD dwType;
				LPCSTR szName;
				DWORD dwThreadID;
				DWORD dwFlags;
			};

			THREADNAME_INFO nameInfo;
			nameInfo.dwType = 0x1000;
			nameInfo.szName = name;
			nameInfo.dwThreadID = m_threadId;
			nameInfo.dwFlags = 0;

			__try
			{
				RaiseException(MS_VC_EXCEPTION, 0, sizeof(nameInfo)/sizeof(DWORD), (DWORD*)&nameInfo);
			}
			__except(EXCEPTION_CONTINUE_EXECUTION)
			{
			}
			#endif
		}

		Thread::~Thread()
		{
			#if defined ION_PLATFORM_WINDOWS
			CloseHandle(m_threadHndl);
			#endif
		}

		void Thread::Join()
		{
			#if defined ION_PLATFORM_WINDOWS
			WaitForSingleObject(m_threadHndl, INFINITE);
			#endif
		}

		u32 Thread::GetId() const
		{
			return m_threadId;
		}

		unsigned long WINAPI Thread::ThreadFunction(void* params)
		{
			Thread* thread = (Thread*)params;
			thread->Entry();
			return 0;
		}
	}
}
