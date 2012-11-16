///////////////////////////////////////////////////
// File:		Slider.cpp
// Date:		8th November 2011
// Authors:		Matt Phillips
// Description:	UI horizontal slider
///////////////////////////////////////////////////

#include "Slider.h"

namespace ion
{
	namespace gui
	{
		Event Slider::sOnValueChangedEvent;

		Slider::Slider(Scheme& scheme, float max, float default, float step)
			: Widget(scheme, Widget::Slider)
		{
			SetOrientation(Horizontal);
			SetRange(max, step);
			SetValue(default);
			mCEWidget->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&Slider::OnValueChanged, this));
		}

		Slider::~Slider()
		{
		}

		void Slider::SetOrientation(Orientation orientation)
		{
			//Not yet supported by CEGUI :(
		}

		void Slider::SetRange(float max, float step)
		{
			((CEGUI::Slider*)mCEWidget)->setMaxValue(max);
			((CEGUI::Slider*)mCEWidget)->setClickStep(step);
		}

		void Slider::SetValue(float value)
		{
			((CEGUI::Slider*)mCEWidget)->setCurrentValue(value);
		}

		float Slider::GetValue()
		{
			return ((CEGUI::Slider*)mCEWidget)->getCurrentValue();
		}

		bool Slider::OnValueChanged(const CEGUI::EventArgs& args)
		{
			Params params(((CEGUI::Slider*)mCEWidget)->getCurrentValue());
			params.mWidget = this;

			sOnValueChangedEvent.Post(params);

			return true;
		}
	}
}