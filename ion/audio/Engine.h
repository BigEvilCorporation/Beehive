
#include <vector>

namespace ion
{
	namespace audio
	{
		class Device;
		class Source;
		class Voice;

		class Engine
		{
		public:
			static Engine* Create();

			virtual int EnumerateDevices(std::vector<Device*>& devices) = 0;

			virtual Voice* CreateVoice(Source& source, bool loop) = 0;
			virtual void ReleaseVoice(Voice& voice) = 0;

		protected:
			Engine() {}
			virtual ~Engine() {}
		};
	}
}