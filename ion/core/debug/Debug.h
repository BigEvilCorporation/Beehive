///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
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
		void Flush();
		void Error(const char* message);
		void Assert(bool condition, const char* message);
		void Break();
	}
}