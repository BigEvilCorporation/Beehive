
#include <audio/Engine.h>

#include <XAudio2.h>

namespace ion
{
	namespace audio
	{
		class EngineXAudio : public Engine
		{
			friend class Engine;
		public:
			virtual int EnumerateDevices(std::vector<Device*>& devices);

			virtual Voice* CreateVoice(Source& source, bool loop);
			virtual void ReleaseVoice(Voice& voice);

		protected:
			EngineXAudio();
			virtual ~EngineXAudio();

		private:
			IXAudio2* mXAudio2System;
		};
	}
}