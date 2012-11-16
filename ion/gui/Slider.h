///////////////////////////////////////////////////
// File:		Slider.h
// Date:		8th November 2011
// Authors:		Matt Phillips
// Description:	UI horizontal slider
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

		class Slider : public Widget
		{
		public:
			static Event sOnValueChangedEvent;

			enum Orientation { Horizontal, Vertical };

			class Params : public Widget::EventParams
			{
			public:
				Params(float value) : mValue(value) {}
				const float mValue;
			};

			Slider(Scheme& scheme, float max, float default, float step);
			~Slider();

			void SetOrientation(Orientation orientation);
			void SetRange(float max, float step);
			void SetValue(float value);
			float GetValue();

		private:
			bool OnValueChanged(const CEGUI::EventArgs& args);
		};
	}
}