///////////////////////////////////////////////////
// File:		Mouse.cpp
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Mouse input class
///////////////////////////////////////////////////

#include "Mouse.h"
#include "../core/MemoryManager.h"
#include "../core/Debug.h"

namespace ion
{
	namespace input
	{
		Mouse::Mouse()
		{
			mAbsX = 0;
			mAbsY = 0;
			mWheelAbs = 0;
			mMouseDevice = NULL;
			memory::MemSet(&mMouseState, 0, sizeof(DIMOUSESTATE));

			if(!g_DirectInputInterface)
			{
				HRESULT hResult = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_DirectInputInterface, NULL);
				if(hResult != 0)
				{
					debug::Error("Mouse::Mouse() - Unable to initialise DirectInput interface");
				}
			}

			if(g_DirectInputInterface)
			{
				HRESULT hResult = g_DirectInputInterface->CreateDevice(GUID_SysMouse, &mMouseDevice, NULL);
				if(hResult != 0)
				{
					debug::Error("Mouse::Mouse() - Unable to initialise DirectInput mouse device");
				}
			}

			if(mMouseDevice)
			{
				mMouseDevice->SetDataFormat(&c_dfDIMouse2);
				mMouseDevice->Acquire();
			}
		}

		Mouse::~Mouse()
		{
			if(mMouseDevice)
			{
				mMouseDevice->Unacquire();
				mMouseDevice->Release();
			}
		}

		void Mouse::Update()
		{
			if(mMouseDevice)
			{
				//Get mouse state
				HRESULT hResult = mMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mMouseState);

				if(hResult == DI_OK)
				{
					mAbsX += mMouseState.lX;
					mAbsY += mMouseState.lY;
					mWheelAbs += mMouseState.lZ;
				}
				else
				{
					//Device lost, attempt to reaqcuire
					hResult = mMouseDevice->Acquire();

					if(hResult == DI_OK)
					{
						//Success, read data again (if this fails, abandon it until next Update())
						mMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mMouseState);
					}
				}
			}
		}

		bool Mouse::ButtonDown(Buttons button)
		{
			return (mMouseState.rgbButtons[button] & 0x80) != 0;
		}

		s32 Mouse::GetDeltaX()
		{
			return mMouseState.lX;
		}

		s32 Mouse::GetDeltaY()
		{
			return mMouseState.lY;
		}

		s32 Mouse::GetAbsoluteX()
		{
			return mAbsX;
		}

		s32 Mouse::GetAbsoluteY()
		{
			return mAbsY;
		}

		s32 Mouse::GetWheelDelta()
		{
			return mMouseState.lZ;
		}

		s32 Mouse::GetWheelAbsolute()
		{
			return mWheelAbs;
		}

		void Mouse::ShowCursor(bool enabled)
		{
			::ShowCursor(enabled);
		}

		void Mouse::SetCooperativeWindow(HWND windowHandle, CoopLevel coopLevel)
		{
			int windowsCoopLevel = 0;

			switch(coopLevel)
			{
			case Background:
				windowsCoopLevel = DISCL_BACKGROUND | DISCL_NONEXCLUSIVE;
				break;

			case Foreground:
				windowsCoopLevel = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
				break;

			case Exclusive:
				windowsCoopLevel = DISCL_EXCLUSIVE | DISCL_FOREGROUND;
				break;
			}

			mMouseDevice->Unacquire();
			mMouseDevice->SetCooperativeLevel(windowHandle, windowsCoopLevel);
			mMouseDevice->Acquire();
		}
	}
}