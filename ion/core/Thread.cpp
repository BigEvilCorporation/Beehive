///////////////////////////////////////////////////
// File:		Thread.cpp
// Date:		25th July 2011
// Authors:		Matt Phillips
// Description:	Threading and synchronisation
///////////////////////////////////////////////////

#include "Thread.h"

#include <Windows.h>

namespace ion
{
	namespace thread
	{
		void Sleep(u32 milliseconds)
		{
			::Sleep((DWORD)milliseconds);
		}
	}
}