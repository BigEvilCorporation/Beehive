///////////////////////////////////////////////////
// File:		Gamepad.cpp
// Date:		2nd August 2011
// Authors:		Matt Phillips
// Description:	Gamepad input handler
///////////////////////////////////////////////////

#include "Gamepad.h"

namespace ion
{
	namespace input
	{
		bool Gamepad::sRegisteredControllers[sMaxControllers] = { false };

		Gamepad::Gamepad()
		{
			mControllerIndex = FindAvailableController();
			mConnected = false;
			memory::MemSet(&mInputState, 0, sizeof(XINPUT_STATE));
			mDeadZone = 0.05f;
			mOuterZone = 0.9f;
		}

		Gamepad::~Gamepad()
		{
			if(mControllerIndex != sInvalidIndex)
			{
				sRegisteredControllers[mControllerIndex] = false;
			}
		}

		int Gamepad::FindAvailableController()
		{
			//Find a free, connected controller
			int connectedControllerId = sInvalidIndex;
			for(int i = 0; i < sMaxControllers; i++)
			{
				if(!sRegisteredControllers[i])
				{
					XINPUT_STATE state;
					int errorState = XInputGetState(i, &state);
					if(!errorState)
					{
						connectedControllerId = i;
						sRegisteredControllers[i] = true;
						break;
					}
				}
			}

			return connectedControllerId;
		}

		void Gamepad::Update()
		{
			if(mControllerIndex == sInvalidIndex)
			{
				//Not yet registered, search for an available controller ID
				mControllerIndex = FindAvailableController();
			}
			else
			{
				//Registered, update state
				int errorState = XInputGetState(mControllerIndex, &mInputState);

				if(!errorState)
				{
					//No error, pad is connected
					mConnected = true;
				}
				else
				{
					//Error, pad disconnected
					mConnected = false;

					//Make sure values aren't cached
					memory::MemSet(&mInputState, 0, sizeof(XINPUT_STATE));
				}
			}
		}

		bool Gamepad::IsConnected()
		{
			return mConnected;
		}

		ion::Vector2 Gamepad::GetLeftStick()
		{
			float lx = (float)mInputState.Gamepad.sThumbLX;
			float ly = (float)mInputState.Gamepad.sThumbLY;

			if(lx != 0.0f)
				lx /= 32768;

			if(ly != 0.0f)
				ly /= 32768;

			if(lx > -mDeadZone && lx < mDeadZone)
				lx = 0.0f;

			if(ly > -mDeadZone && ly < mDeadZone)
				ly = 0.0f;

			return ion::Vector2(lx, ly);
		}

		ion::Vector2 Gamepad::GetRightStick()
		{
			float rx = (float)mInputState.Gamepad.sThumbRX;
			float ry = (float)mInputState.Gamepad.sThumbRY;

			if(rx != 0.0f)
				rx /= 32768;

			if(ry != 0.0f)
				ry /= 32768;

			if(rx > -mDeadZone && rx < mDeadZone)
				rx = 0.0f;

			if(ry > -mDeadZone && ry < mDeadZone)
				ry = 0.0f;

			return ion::Vector2(rx, ry);
		}

		bool Gamepad::ButtonDown(Buttons button)
		{
			int xInputButton = ToXInputButton(button);
			return (mInputState.Gamepad.wButtons & xInputButton) != 0;
		}

		int Gamepad::ToXInputButton(Buttons button)
		{
			int xInputButton = 0;

			switch(button)
			{
				case DPAD_UP:	xInputButton = XINPUT_GAMEPAD_DPAD_UP; break;
				case DPAD_DOWN: xInputButton = XINPUT_GAMEPAD_DPAD_DOWN; break;
				case DPAD_LEFT: xInputButton = XINPUT_GAMEPAD_DPAD_LEFT; break;
				case DPAD_RIGHT:xInputButton = XINPUT_GAMEPAD_DPAD_RIGHT; break;
				case BUTTON_A:	xInputButton = XINPUT_GAMEPAD_A; break;
				case BUTTON_B:	xInputButton = XINPUT_GAMEPAD_B; break;
				case BUTTON_X:	xInputButton = XINPUT_GAMEPAD_X; break;
				case BUTTON_Y:	xInputButton = XINPUT_GAMEPAD_Y; break;
				case START:		xInputButton = XINPUT_GAMEPAD_START; break;
				case SELECT:	xInputButton = XINPUT_GAMEPAD_BACK; break;
				case LEFT_SHOULDER:		xInputButton = XINPUT_GAMEPAD_LEFT_SHOULDER; break;
				case RIGHT_SHOULDER:	xInputButton = XINPUT_GAMEPAD_RIGHT_SHOULDER; break;
				case LEFT_STICK_CLICK:	xInputButton = XINPUT_GAMEPAD_LEFT_THUMB; break;
				case RIGHT_STICK_CLICK:	xInputButton = XINPUT_GAMEPAD_RIGHT_THUMB; break;
				default: break;
			}

			return xInputButton;
		}

		void Gamepad::SetDeadZone(float deadZone)
		{
			mDeadZone = deadZone;
		}

		float Gamepad::GetDeadZone() const
		{
			return mDeadZone;
		}

		void Gamepad::SetOuterZone(float outerZone)
		{
			mOuterZone = outerZone;
		}

		float Gamepad::GetOuterZone() const
		{
			return mOuterZone;
		}
	}
}