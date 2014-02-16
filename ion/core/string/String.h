///////////////////////////////////////////////////
// File:		String.h
// Date:		16th February 2014
// Authors:		Matt Phillips
// Description:	String utils
///////////////////////////////////////////////////

#pragma once

#include <string>
#include <locale>
#include <algorithm>

namespace ion
{
	namespace string
	{
		std::string ToLower(const std::string& string)
		{
			std::locale locale;
			std::string stringLower = string;
			std::transform(stringLower.begin(), stringLower.end(), stringLower.begin(), ::tolower);
			return stringLower;
		}
	}
}