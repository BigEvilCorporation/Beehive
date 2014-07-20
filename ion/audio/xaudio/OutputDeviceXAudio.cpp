#include "OutputDeviceXAudio.h"
#include <core/debug/Debug.h>

namespace ion
{
	namespace audio
	{
		OutputDeviceXAudio::OutputDeviceXAudio(IXAudio2& xaudio2System)
			: mXAudio2System(xaudio2System)
		{
			mXAudio2MasteringVoice = NULL;

			HRESULT result = mXAudio2System.CreateMasteringVoice(&mXAudio2MasteringVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, NULL, NULL, AudioCategory_GameEffects);
			if(result != S_OK)
			{
				debug::Error("Could not create XAudio2 mastering voice");
			}
		}

		OutputDeviceXAudio::~OutputDeviceXAudio()
		{
			
		}
	}
}