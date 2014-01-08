///////////////////////////////////////////////////
// File:		Keyboard.cpp
// Date:		1st August 2011
// Authors:		Matt Phillips
// Description:	SDL keyboard wrapper
///////////////////////////////////////////////////

#include "Keyboard.h"
#include "core/memory/Memory.h"
#include "core/debug/Debug.h"

namespace ion
{
	namespace input
	{
		Keyboard::Keyboard()
		{
			mKeyboardDevice = NULL;

			if(!g_DirectInputInterface)
			{
				HRESULT hResult = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_DirectInputInterface, NULL);
				if(hResult != 0)
				{
					debug::Error("Keyboard::Keyboard() - Unable to initialise DirectInput interface");
				}
			}

			if(g_DirectInputInterface)
			{
				HRESULT hResult = g_DirectInputInterface->CreateDevice(GUID_SysKeyboard, &mKeyboardDevice, NULL);
				if(hResult != 0)
				{
					debug::Error("Keyboard::Keyboard() - Unable to initialise DirectInput keyboard device");
				}
			}

			if(mKeyboardDevice)
			{
				mKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
				mKeyboardDevice->Acquire();
			}

			for(int i = 0; i < sMaxKeys; i++)
			{
				mCurrKeyStates[i] = false;
				mPrevKeyStates[i] = false;
			}
		}

		Keyboard::~Keyboard()
		{
			if(mKeyboardDevice)
			{
				mKeyboardDevice->Unacquire();
				mKeyboardDevice->Release();
			}
		}
		
		bool Keyboard::KeyDown(int key) const
		{
			debug::Assert(key >=0 && key < sMaxKeys, "Keyboard::KeyDown() - Key out of range");
			return (mCurrKeyStates[key] & 0x80) != 0;
		}
		
		bool Keyboard::KeyPressedThisFrame(int key) const
		{
			debug::Assert(key >=0 && key < sMaxKeys, "Keyboard::KeyPressedThisFrame() - Key out of range");
			return ((mCurrKeyStates[key] & 0x80) != 0) && ((mPrevKeyStates[key] & 0x80) == 0);
		}

		void Keyboard::Update()
		{
			//Store prev key states
			memory::MemCopy(mPrevKeyStates, mCurrKeyStates, sizeof(mCurrKeyStates));

			if(mKeyboardDevice)
			{
				//Read key states
				HRESULT hResult = mKeyboardDevice->GetDeviceState(sizeof(mCurrKeyStates), mCurrKeyStates);

				if(hResult != DI_OK)
				{
					//Device lost, attempt to reacquire it
					hResult = mKeyboardDevice->Acquire();

					if(hResult == DI_OK)
					{
						//Success, read key states again (if this fails, abandon it until next Update())
						mKeyboardDevice->GetDeviceState(sizeof(mCurrKeyStates), mCurrKeyStates);
					}
				}
			}
		}

		void Keyboard::SetCooperativeWindow(CoopLevel coopLevel)
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

			mKeyboardDevice->SetCooperativeLevel(GetActiveWindow(), windowsCoopLevel);
		}
	}
}