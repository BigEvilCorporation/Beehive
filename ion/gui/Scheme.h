///////////////////////////////////////////////////
// File:		Scheme.h
// Date:		22nd September 2011
// Authors:		Matt Phillips
// Description:	UI scheme
///////////////////////////////////////////////////

#pragma once

#include <CEGUI/CEGUI.h>

#include <string>

namespace ion
{
	namespace gui
	{
		class Scheme
		{
		public:
			Scheme(std::string schemeName);
			~Scheme();

			const std::string& GetSchemeName();

		private:
			std::string mSchemeName;
		};
	}
}