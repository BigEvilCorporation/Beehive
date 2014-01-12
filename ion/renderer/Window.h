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
			Window(const std::string& title, u32 width, u32 height, bool fullscreen);
			virtual ~Window() {}

			u32 GetWidth() const;
			u32 GetHeight() const;

			virtual bool Update() = 0;
			virtual void SetFullscreen(bool fullscreen) = 0;
			virtual void SetTitle(const std::string& title) = 0;

		protected:
			u32 mWidth;
			u32 mHeight;
		};
	}
}