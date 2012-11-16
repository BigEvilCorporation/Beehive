///////////////////////////////////////////////////
// File:		Button.cpp
// Date:		6th November 2011
// Authors:		Matt Phillips
// Description:	UI button
///////////////////////////////////////////////////

#include "Button.h"

namespace ion
{
	namespace gui
	{
		Event Button::sOnPressedEvent;

		Button::Button(std::string text, Scheme& scheme)
			: Widget(scheme, Widget::Button)
		{
			mCEWidget->setText(text);
			mCEWidget->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Button::OnClicked, this));
		}

		Button::~Button()
		{
		}

		bool Button::OnClicked(const CEGUI::EventArgs& args)
		{
			Widget::EventParams params;
			params.mWidget = this;

			sOnPressedEvent.Post(params);

			return true;
		}
	}
}