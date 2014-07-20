
#include "EngineXAudio.h"
#include "OutputDeviceXAudio.h"
#include "VoiceXAudio.h"
#include <core/debug/Debug.h>

namespace ion
{
	namespace audio
	{
		Engine* Engine::Create()
		{
			return new EngineXAudio();
		}

		EngineXAudio::EngineXAudio()
		{
			//Initialise COM
			CoInitializeEx(NULL, COINIT_MULTITHREADED);

			//Initialise XAudio2
			if(XAudio2Create(&mXAudio2System, 0, XAUDIO2_DEFAULT_PROCESSOR) != S_OK)
			{
				debug::Error("Could not create XAudio2 system");
			}

			XAUDIO2_DEBUG_CONFIGURATION debugConfig = {0};
			debugConfig.BreakMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
			debugConfig.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS | XAUDIO2_LOG_INFO | XAUDIO2_LOG_DETAIL;
			mXAudio2System->SetDebugConfiguration(&debugConfig);

			std::vector<Device*> devices;
			EnumerateDevices(devices);
		}

		EngineXAudio::~EngineXAudio()
		{
		}

		int EngineXAudio::EnumerateDevices(std::vector<Device*>& devices)
		{
			//Temp: just add default output device
			devices.push_back(new OutputDeviceXAudio(*mXAudio2System));

			return devices.size();
		}

		Voice* EngineXAudio::CreateVoice(Source& source, bool loop)
		{
			return new VoiceXAudio(mXAudio2System, source, loop);
		}

		void EngineXAudio::ReleaseVoice(Voice& voice)
		{
		}
	}
}