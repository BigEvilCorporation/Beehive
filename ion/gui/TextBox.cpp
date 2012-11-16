///////////////////////////////////////////////////
// File:		TextBox.cpp
// Date:		7th November 2011
// Authors:		Matt Phillips
// Description:	UI editable text box
///////////////////////////////////////////////////

#include "TextBox.h"

namespace ion
{
	namespace gui
	{
		Event TextBox::sOnTextChangedEvent;

		TextBox::TextBox(Scheme& scheme)
			: Widget(scheme, Widget::TextBox)
		{
			mCEWidget->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&TextBox::OnTextChanged, this));
		}

		TextBox::~TextBox()
		{
		}

		bool TextBox::OnTextChanged(const CEGUI::EventArgs& args)
		{
			Params params(std::string(mCEWidget->getText().c_str()));
			params.mWidget = this;

			sOnTextChangedEvent.Post(params);

			return true;
		}
	}
}