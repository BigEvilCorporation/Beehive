///////////////////////////////////////////////////
// File:		WindowWin32.cpp
// Date:		12th January 2014
// Authors:		Matt Phillips
// Description:	Win32 window
///////////////////////////////////////////////////

#pragma once

#include "core/debug/Debug.h"
#include "renderer/win32/WindowWin32.h"

namespace ion
{
	namespace render
	{
		WindowWin32::WindowWin32(const std::string& title, u32 clientAreaWidth, u32 clientAreaHeight, bool fullscreen)
			: Window(title, clientAreaWidth, clientAreaHeight, fullscreen)
		{
			const char* windowClassName = "WindowClass";
			mhInstance = GetModuleHandle(NULL);

			//Register window class
			WNDCLASSEX windowClass = {0};
			windowClass.cbSize        = sizeof(WNDCLASSEX);
			windowClass.style         = 0;
			windowClass.lpfnWndProc   = WindowProcedure;
			windowClass.cbClsExtra    = 0;
			windowClass.cbWndExtra    = 0;
			windowClass.hInstance     = mhInstance;
			windowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
			windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
			windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
			windowClass.lpszMenuName  = NULL;
			windowClass.lpszClassName = windowClassName;
			windowClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

			if(!RegisterClassEx(&windowClass))
			{
				debug::Error("Could not register window class");
			}

			//Get desktop resolution
			int desktopWidth = GetSystemMetrics(SM_CXSCREEN);
			int desktopHeight = GetSystemMetrics(SM_CYSCREEN);

			//Setup window style flags
			DWORD windowStyle = 0;
			DWORD windowStyleEx = 0;

			if(fullscreen)
			{
				windowStyle = WS_POPUP;
				windowStyleEx = WS_EX_APPWINDOW;
			}
			else
			{
				windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
				windowStyleEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			}

			//Setup window rect
			RECT windowRect = {0};
			windowRect.right = (long)clientAreaWidth;
			windowRect.bottom = (long)clientAreaHeight;

			//Adjust rect to account for window border
			AdjustWindowRectEx(&windowRect, windowStyle, FALSE, windowStyleEx);

			m_windowWidth = windowRect.right - windowRect.left;
			m_windowHeight = windowRect.bottom - windowRect.top;

			//Calculate centre position
			int x = (desktopWidth / 2) - (m_windowWidth / 2);
			int y = (desktopHeight / 2) - (m_windowHeight / 2);

			//Create window
			mWindowHandle = CreateWindowEx(windowStyleEx, windowClassName, title.c_str(), windowStyle, x, y, m_windowWidth, m_windowHeight, NULL, NULL, mhInstance, NULL);

			if(!mWindowHandle)
			{
				debug::Error("Could not create window");
			}

			//Get draw context
			mDrawContext = GetDC(mWindowHandle);

			//Show and update
			ShowWindow(mWindowHandle, SW_SHOW);
			UpdateWindow(mWindowHandle);
		}

		WindowWin32::~WindowWin32()
		{
		}

		bool WindowWin32::Update()
		{
			MSG message;
			bool active = true;

			while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE) > 0)
			{
				active = (message.message != WM_QUIT);

				TranslateMessage(&message);
				DispatchMessage(&message);
			}

			return active;
		}

		void WindowWin32::Resize(u32 width, u32 height)
		{
		}

		void WindowWin32::SetFullscreen(bool fullscreen)
		{
		}

		void WindowWin32::SetTitle(const std::string& title)
		{
			SetWindowText(mWindowHandle, title.c_str());
		}

		LRESULT CALLBACK WindowWin32::WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			switch(message)
			{
				case WM_CLOSE:
					DestroyWindow(hwnd);
				break;
				case WM_DESTROY:
					PostQuitMessage(0);
				break;
				default:
					return DefWindowProc(hwnd, message, wParam, lParam);
			}

			return 0;
		}
	}
}