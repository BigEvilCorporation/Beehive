///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Debug.cpp
// Date:		25th July 2011
// Authors:		Matt Phillips
// Description:	Debug output, log
///////////////////////////////////////////////////

#include "Debug.h"

#include <iostream>

namespace ion
{
	namespace debug
	{
		void Log(const char* message)
		{
			std::cout << message << "\n";
		}

		void Flush()
		{
			std::cout << std::flush;
		}

		void Error(const char* message)
		{
			Log(message);
			Flush();
			Break();
		}

		void Assert(bool condition, const char* message)
		{
			if(!condition)
			{
				Error(message);
			}
		}

		void Break()
		{
			__debugbreak();
		}
	}
}