///////////////////////////////////////////////////
// File:		Root.h
// Date:		26th November 2011
// Authors:		Matt Phillips
// Description:	UI tree root
///////////////////////////////////////////////////

#pragma once

#include "Widget.h"
#include <CEGUI/CEGUI.h>

namespace ion
{
	namespace gui
	{
		class Root : public Widget
		{
		public:
			Root() {}
			virtual ~Root() {}
		};
	}
}