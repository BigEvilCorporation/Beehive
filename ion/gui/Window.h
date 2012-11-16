///////////////////////////////////////////////////
// File:		Window.h
// Date:		23nd September 2011
// Authors:		Matt Phillips
// Description:	UI window
///////////////////////////////////////////////////

#pragma once

#include "Widget.h"

namespace ion
{
	namespace gui
	{
		class Window : public Widget
		{
		public:
			Window(std::string text, Scheme& scheme, bool modal = false);
			virtual ~Window();

			void MoveToFront();
			void MoveToBack();

			virtual void Show();
			virtual void Hide();

		protected:
			bool mModal;
		};
	}
}