///////////////////////////////////////////////////
// File:		StaticText.cpp
// Date:		7th November 2011
// Authors:		Matt Phillips
// Description:	UI static text
///////////////////////////////////////////////////

#include "StaticText.h"

namespace ion
{
	namespace gui
	{
		StaticText::StaticText(std::string text, Scheme& scheme)
			: Widget(scheme, Widget::StaticText)
		{
			mCEWidget->setText(text);
		}

		StaticText::~StaticText()
		{
		}
	}
}