///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Version.h
// Date:		27th August 2011
// Authors:		Matt Phillips
// Description:	ion::engine version ID
///////////////////////////////////////////////////

#pragma once

namespace ion
{
	struct Version
	{
		int Major;
		int Minor;
		int Build;
	};

	static const Version sVersion = { 0, 0, 2 };
}