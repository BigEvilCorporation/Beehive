///////////////////////////////////////////////////
// File:		UUID.cpp
// Date:		2nd May 2016
// Authors:		Matt Phillips
// Description:	Uinique Identifier generation
///////////////////////////////////////////////////

#include "UUID.h"

#include <objbase.h>

namespace ion
{
	UUID64 GenerateUUID64()
	{
		GUID guid;
		CoCreateGuid(&guid);
		UUID64 uuid = ((u64)guid.Data1 << 32 | (u64)guid.Data2 << 16 | (u64)guid.Data3);
		return uuid;
	}
}