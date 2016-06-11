///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
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