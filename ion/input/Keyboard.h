///////////////////////////////////////////////////
// File:		Keyboard.h
// Date:		1st August 2011
// Authors:		Matt Phillips
// Description:	Keyboard input class
///////////////////////////////////////////////////

#pragma once

#include <dinput.h>

namespace ion
{
	namespace input
	{
		//TODO: Platform specific header
		static LPDIRECTINPUT8 g_DirectInputInterface = NULL;

		class Keyboard
		{
		public:
			enum CoopLevel { Background, Foreground, Exclusive };

			Keyboard();
			~Keyboard();

			//Is key currently down
			bool KeyDown(int key) const;

			//Was key pressed this frame
			bool KeyPressedThisFrame(int key) const;

			void Update();

			void SetCooperativeWindow(HWND windowHandle, CoopLevel coopLevel);

		private:
			static const int sMaxKeys = 256;
			char mCurrKeyStates[sMaxKeys];
			char mPrevKeyStates[sMaxKeys];

			LPDIRECTINPUTDEVICE8 mKeyboardDevice;
		};
	}
}