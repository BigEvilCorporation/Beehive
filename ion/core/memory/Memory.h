///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		MemoryManager.h
// Date:		2nd August 2011
// Authors:		Matt Phillips
// Description:	Memory management
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include <memory.h>

namespace ion
{
	namespace memory
	{
		void MemSet(void* address, int value, u32 size);
		void MemCopy(void* destination, const void* source, u32 size);
		void EndianSwap(u16& value);
		void EndianSwap(s16& value);
		void EndianSwap(u32& value);
		void EndianSwap(s32& value);
	}
}