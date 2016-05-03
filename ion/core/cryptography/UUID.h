///////////////////////////////////////////////////
// File:		UUID.h
// Date:		2nd May 2016
// Authors:		Matt Phillips
// Description:	Uinique Identifier generation
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"

namespace ion
{
	typedef u64 UUID64;

	UUID64 GenerateUUID64();
}