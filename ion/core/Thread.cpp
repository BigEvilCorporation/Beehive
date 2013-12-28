///////////////////////////////////////////////////
// File:		Thread.cpp
// Date:		25th July 2011
// Authors:		Matt Phillips
// Description:	Threading and synchronisation
///////////////////////////////////////////////////

#include "core/Thread.h"

namespace ion
{
	namespace thread
	{
		Thread::Thread(const char* name)
		{
			if(!name)
			{
				name = "ion::thread";
			}

			#if defined _WIN32
			mThreadHndl = CreateThread(	NULL,                   //Default security attributes
										0,                      //Default stack size  
										Thread::ThreadFunction, //Thread function
										this,					//User data
										0,                      //Default creation flags 
										&mThreadId);			//Thread id

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
			nameInfo.dwThreadID = mThreadId;
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
			#if defined _WIN32
			CloseHandle(mThreadHndl);
			#endif
		}

		void Thread::Join()
		{
			#if defined _WIN32
			WaitForSingleObject(mThreadHndl, INFINITE);
			#endif
		}

		u32 Thread::GetId() const
		{
			return mThreadId;
		}

		unsigned long WINAPI Thread::ThreadFunction(void* params)
		{
			Thread* thread = (Thread*)params;
			thread->Entry();
			return 0;
		}

		Event::Event()
		{
			#if defined _WIN32
			mThreadEventHndl = CreateEvent(NULL, TRUE, FALSE, NULL);
			#endif
		}

		Event::~Event()
		{
			#if defined _WIN32
			CloseHandle(mThreadEventHndl);
			#endif
		}

		void Event::Signal()
		{
			#if defined _WIN32
			SetEvent(mThreadEventHndl);
			#endif
		}

		void Event::Wait()
		{
			#if defined _WIN32
			WaitForSingleObject(mThreadEventHndl, INFINITE);
			#endif
		}
	}
}
