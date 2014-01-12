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
		Window::Window(const std::string& title, u32 width, u32 height, bool fullscreen)
		{
			mWidth = width;
			mHeight = height;
		}

		u32 Window::GetWidth() const
		{
			return mWidth;
		}

		u32 Window::GetHeight() const
		{
			return mHeight;
		}
	}
}
