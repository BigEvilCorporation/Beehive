///////////////////////////////////////////////////
// File:		Button.h
// Date:		6th November 2011
// Authors:		Matt Phillips
// Description:	UI button
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

		class Button : public Widget
		{
		public:
			static Event sOnPressedEvent;

			Button(std::string text, Scheme& scheme);
			~Button();

		private:
			bool OnClicked(const CEGUI::EventArgs& args);
		};
	}
}