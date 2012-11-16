///////////////////////////////////////////////////
// File:		TextBox.h
// Date:		7th November 2011
// Authors:		Matt Phillips
// Description:	UI editable text box
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

		class TextBox : public Widget
		{
		public:
			static Event sOnTextChangedEvent;

			class Params : public Widget::EventParams
			{
			public:
				Params(const std::string& text) : mText(text) {}
				const std::string mText;
			};

			TextBox(Scheme& scheme);
			~TextBox();

		private:
			bool OnTextChanged(const CEGUI::EventArgs& args);
		};
	}
}