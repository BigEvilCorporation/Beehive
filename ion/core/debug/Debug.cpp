///////////////////////////////////////////////////
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

		void Error(const char* message)
		{
			Log(message);
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