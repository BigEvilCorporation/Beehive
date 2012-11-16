///////////////////////////////////////////////////
// File:		Widget.h
// Date:		22nd September 2011
// Authors:		Matt Phillips
// Description:	UI widget base class
///////////////////////////////////////////////////

#pragma once

#include "../core/maths/Vector.h"

#include "EventListener.h"

#include <CEGUI/CEGUI.h>

#include <string>

namespace ion
{
	namespace gui
	{
		//Forward declaration
		class Scheme;

		class Widget
		{
			friend class Root;

		public:

			//Default event params
			class EventParams : public Event::Params
			{
			public:
				Widget* mWidget;
			};

			enum Types { Window, Button, StaticText, TextBox, ScrollBar, Slider, ListBox, NumWidgetTypes };
			static const char* sTypeNames[NumWidgetTypes];

			Widget(Scheme& scheme, Types type);
			virtual ~Widget();

			void AddChild(Widget& widget);

			void SetText(std::string text);
			void SetPosition(const Vector2& position);
			void SetSize(const Vector2& size);

			virtual void Show();
			virtual void Hide();

			CEGUI::Window* GetCEWidget();

		protected:

			//Only root nodes can create a widget with no scheme
			Widget();

			CEGUI::Window* mCEWidget;

			static int sNumWidgets;
		};
	}
}