///////////////////////////////////////////////////
// File:		StaticText.h
// Date:		7th November 2011
// Authors:		Matt Phillips
// Description:	UI static text
///////////////////////////////////////////////////

#pragma once

#include "Widget.h"
#include "EventListener.h"
#include <CEGUI/CEGUI.h>

#include <string>

namespace ion
{
	namespace gui
	{
		//Forward declaration
		class Scheme;

		class StaticText : public Widget
		{
		public:
			StaticText(std::string text, Scheme& scheme);
			~StaticText();
		};
	}
}