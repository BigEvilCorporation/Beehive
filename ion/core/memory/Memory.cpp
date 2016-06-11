///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		MemoryManager.cpp
// Date:		2nd August 2011
// Authors:		Matt Phillips
// Description:	Memory management
///////////////////////////////////////////////////

#include "core/memory/Memory.h"

namespace ion
{
	namespace memory
	{
		void MemSet(void* address, int value, u32 size)
		{
			memset(address, value, (size_t)size);
		}

		void MemCopy(void* destination, const void* source, u32 size)
		{
			memcpy(destination, source, size);
		}

		void EndianSwap(u16& value)
		{
			u8* bytes = (u8*)&value;
			u8 temp = bytes[0];
			bytes[0] = bytes[1];
			bytes[1] = temp;
		}

		void EndianSwap(s16& value)
		{
			u8* bytes = (u8*)&value;
			u8 temp = bytes[0];
			bytes[0] = bytes[1];
			bytes[1] = temp;
		}

		void EndianSwap(u32& value)
		{
			u8* bytes = (u8*)&value;
			u8 temp = bytes[0];
			bytes[0] = bytes[3];
			bytes[3] = temp;
			temp = bytes[1];
			bytes[1] = bytes[2];
			bytes[2] = temp;
		}

		void EndianSwap(s32& value)
		{

		}
	}
}