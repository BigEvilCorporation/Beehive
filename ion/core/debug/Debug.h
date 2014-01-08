///////////////////////////////////////////////////
// File:		Debug.h
// Date:		25th July 2011
// Authors:		Matt Phillips
// Description:	Debug output, log
///////////////////////////////////////////////////

#pragma once

namespace ion
{
	namespace debug
	{
		void Log(const char* message);
		void Error(const char* message);
		void Assert(bool condition, const char* message);
		void Break();
	}
}