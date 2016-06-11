///////////////////////////////////////////////////
// File:		Window.h
// Date:		12th January 2014
// Authors:		Matt Phillips
// Description:	Window management
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include "maths/Vector.h"

namespace ion
{
	namespace render
	{
		class Window
		{
		public:
			Window(const std::string& title, u32 clientAreaWidth, u32 clientAreaHeight, bool fullscreen);
			virtual ~Window() {}

			u32 GetWindowWidth() const;
			u32 GetWindowHeight() const;

			u32 GetClientAreaWidth() const;
			u32 GetClientAreaHeight() const;

			float GetAspectRatio() const;

			virtual bool Update() = 0;
			virtual void SetFullscreen(bool fullscreen) = 0;
			virtual void SetTitle(const std::string& title) = 0;

		protected:
			u32 m_windowWidth;
			u32 m_windowHeight;
			u32 mclientAreaWidth;
			u32 m_clientAreaHeight;
			float maspectRatio;
		};
	}
}