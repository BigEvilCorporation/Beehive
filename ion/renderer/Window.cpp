///////////////////////////////////////////////////
// File:		Window.cpp
// Date:		12th January 2014
// Authors:		Matt Phillips
// Description:	Window management
///////////////////////////////////////////////////

#include "renderer/Window.h"

namespace ion
{
	namespace render
	{
		Window::Window(const std::string& title, u32 clientAreaWidth, u32 clientAreaHeight, bool fullscreen)
		{
			mClientAreaWidth = clientAreaWidth;
			mClientAreaHeight = clientAreaHeight;
			mAspectRatio = (float)clientAreaHeight / (float)clientAreaWidth;
		}

		u32 Window::GetWindowWidth() const
		{
			return mWindowWidth;
		}

		u32 Window::GetWindowHeight() const
		{
			return mWindowHeight;
		}

		u32 Window::GetClientAreaWidth() const
		{
			return mClientAreaWidth;
		}

		u32 Window::GetClientAreaHeight() const
		{
			return mClientAreaHeight;
		}

		float Window::GetAspectRatio() const
		{
			return mAspectRatio;
		}
	}
}
