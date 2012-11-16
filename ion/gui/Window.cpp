///////////////////////////////////////////////////
// File:		Window.cpp
// Date:		23nd September 2011
// Authors:		Matt Phillips
// Description:	UI window
///////////////////////////////////////////////////

#include "Window.h"

namespace ion
{
	namespace gui
	{
		Window::Window(std::string text, Scheme& scheme, bool modal)
			: Widget(scheme, Widget::Window)
		{
			mModal = modal;
			mCEWidget->setText(text);
			mCEWidget->setModalState(modal);

			if(modal)
			{
				mCEWidget->moveToFront();
				mCEWidget->setAlwaysOnTop(true);
			}
		}

		Window::~Window()
		{
		}

		void Window::MoveToFront()
		{
			mCEWidget->moveToFront();
		}

		void Window::MoveToBack()
		{
			mCEWidget->moveToBack();
		}

		void Window::Show()
		{
			//Hack - hide first to reset active state after adding as child, or CEGUI won't do anything
			Hide();

			mCEWidget->setModalState(mModal);

			if(mModal)
			{
				mCEWidget->setAlwaysOnTop(true);
			}

			Widget::Show();
		}

		void Window::Hide()
		{
			mCEWidget->setModalState(false);
			mCEWidget->setAlwaysOnTop(false);
			Widget::Hide();
		}
	}
}