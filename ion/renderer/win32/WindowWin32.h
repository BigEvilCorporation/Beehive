///////////////////////////////////////////////////
// File:		WindowWin32.h
// Date:		12th January 2014
// Authors:		Matt Phillips
// Description:	Win32 window
///////////////////////////////////////////////////

#pragma once

#include "renderer/Window.h"
#include <Windows.h>

namespace ion
{
	namespace render
	{
		class WindowWin32 : public Window
		{
		public:
			WindowWin32(const std::string& title, u32 clientAreaWidth, u32 clientAreaHeight, bool fullscreen);
			virtual ~WindowWin32();

			virtual bool Update();
			virtual void Resize(u32 width, u32 height);
			virtual void SetFullscreen(bool fullscreen);
			virtual void SetTitle(const std::string& title);

			HDC GetDrawContext() const { return mDrawContext; }
			HWND GetWindowHandle() const { return mWindowHandle; }

		protected:
			static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

			HINSTANCE mhInstance;
			HWND mWindowHandle;
			HDC mDrawContext;
		};
	}
}