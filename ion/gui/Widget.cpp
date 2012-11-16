///////////////////////////////////////////////////
// File:		Widget.cpp
// Date:		22nd September 2011
// Authors:		Matt Phillips
// Description:	UI widget base class
///////////////////////////////////////////////////

#include "Widget.h"
#include "Scheme.h"

#include <sstream>

namespace ion
{
	namespace gui
	{
		int Widget::sNumWidgets = 0;
		const char* Widget::sTypeNames[Widget::NumWidgetTypes] = { "FrameWindow", "Button", "StaticText", "Editbox", "ScrollBar", "Slider", "Listbox" };

		Widget::Widget(Scheme& scheme, Types type)
		{
			std::string typeName = scheme.GetSchemeName() + '/' + sTypeNames[type];
			std::stringstream widgetName;
			widgetName << "widgetId_" << sNumWidgets++;
			mCEWidget = CEGUI::WindowManager::getSingleton().createWindow(typeName, widgetName.str());
		}

		Widget::Widget()
		{
			std::stringstream widgetName;
			widgetName << "rootId_" << sNumWidgets++;
			mCEWidget = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", widgetName.str());
		}

		Widget::~Widget()
		{
			CEGUI::WindowManager::getSingleton().destroyWindow(mCEWidget);
		}

		CEGUI::Window* Widget::GetCEWidget()
		{
			return mCEWidget;
		}

		void Widget::AddChild(Widget& widget)
		{
			#if CEGUI_VERSION >= 8
			mCEWidget->addChild(widget.GetCEWidget());
			#else
			mCEWidget->addChildWindow(widget.GetCEWidget());
			#endif
		}

		void Widget::SetText(std::string text)
		{
			mCEWidget->setText(text);
		}

		void Widget::SetPosition(const Vector2& position)
		{
			mCEWidget->setPosition(CEGUI::UVector2(CEGUI::UDim(position.x, 0.0f), CEGUI::UDim(position.y, 0.0f)));
		}

		void Widget::SetSize(const Vector2& size)
		{
			#if CEGUI_VERSION >= 8
			mCEWidget->setSize(CEGUI::USize(CEGUI::UDim(size.x, 0.0f), CEGUI::UDim(size.y, 0.0f)));
			#else
			mCEWidget->setSize(CEGUI::UVector2(CEGUI::UDim(size.x, 0.0f), CEGUI::UDim(size.y, 0.0f)));
			#endif
		}

		void Widget::Show()
		{
			mCEWidget->activate();
			mCEWidget->show();
			mCEWidget->setEnabled(true);
		}

		void Widget::Hide()
		{
			mCEWidget->setEnabled(false);
			mCEWidget->hide();
			mCEWidget->deactivate();
		}
	}
}