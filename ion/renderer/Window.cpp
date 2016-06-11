///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
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
			mclientAreaWidth = clientAreaWidth;
			m_clientAreaHeight = clientAreaHeight;
			maspectRatio = (float)clientAreaHeight / (float)clientAreaWidth;
		}

		u32 Window::GetWindowWidth() const
		{
			return m_windowWidth;
		}

		u32 Window::GetWindowHeight() const
		{
			return m_windowHeight;
		}

		u32 Window::GetClientAreaWidth() const
		{
			return mclientAreaWidth;
		}

		u32 Window::GetClientAreaHeight() const
		{
			return m_clientAreaHeight;
		}

		float Window::GetAspectRatio() const
		{
			return maspectRatio;
		}
	}
}
