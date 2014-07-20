#include <audio/OutputDevice.h>

#include <XAudio2.h>

namespace ion
{
	namespace audio
	{
		class OutputDeviceXAudio : public OutputDevice
		{
		public:
			OutputDeviceXAudio(IXAudio2& xaudio2System);
			virtual ~OutputDeviceXAudio();
		private:
			IXAudio2& mXAudio2System;
			IXAudio2MasteringVoice* mXAudio2MasteringVoice;
		};
	}
}