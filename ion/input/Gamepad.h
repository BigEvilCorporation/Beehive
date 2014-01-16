///////////////////////////////////////////////////
// File:		Gamepad.h
// Date:		2nd August 2011
// Authors:		Matt Phillips
// Description:	Gamepad input handler
///////////////////////////////////////////////////

#pragma once

#include "core/memory/Memory.h"
#include "maths/Vector.h"

//TODO: Windows headers in platform header
#include <windows.h>
#include <XInput.h>

namespace ion
{
	namespace input
	{
		class Gamepad
		{
		public:
			enum Buttons
			{
				DPAD_UP				= 1 << 1,
				DPAD_DOWN			= 1 << 2,
				DPAD_LEFT			= 1 << 3,
				DPAD_RIGHT			= 1 << 4,

				BUTTON_A			= 1 << 5,
				BUTTON_B			= 1 << 6,
				BUTTON_X			= 1 << 7,
				BUTTON_Y			= 1 << 8,

				START				= 1 << 9,
				SELECT				= 1 << 10,

				LEFT_SHOULDER		= 1 << 11,
				RIGHT_SHOULDER		= 1 << 12,
				
				LEFT_STICK_CLICK	= 1 << 13,
				RIGHT_STICK_CLICK	= 1 << 14
			};

			Gamepad();
			~Gamepad();

			//Update input/connection state
			void Update();
			
			//Test if connected
			bool IsConnected();

			ion::Vector2 GetLeftStick();
			ion::Vector2 GetRightStick();

			bool ButtonDown(Buttons button);

			void SetDeadZone(float deadZone);
			float GetDeadZone() const;

			void SetOuterZone(float outerZone);
			float GetOuterZone() const;

		private:
			static int FindAvailableController();
			static int ToXInputButton(Buttons button);

			int mControllerIndex;
			bool mConnected;
			XINPUT_STATE mInputState;
			float mDeadZone;
			float mOuterZone;

			static const int sInvalidIndex = -1;
			static const int sMaxControllers = 4;
			static bool sRegisteredControllers[sMaxControllers];
		};
	}
}