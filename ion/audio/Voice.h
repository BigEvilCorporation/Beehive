#include <audio/Callback.h>

namespace ion
{
	namespace audio
	{
		class Buffer;
		class Source;

		class Voice : public SourceCallback
		{
			friend class Engine;

		public:
			enum State
			{
				Playing,
				Paused,
				Stopped
			};

			//Transport
			virtual void Play() = 0;
			virtual void Stop() = 0;
			virtual void Pause() = 0;
			virtual void Resume() = 0;

			State GetState() const;

			//Properties
			virtual void SetVolume(float volume) = 0;
			float GetVolume() const;

		protected:
			Voice(Source& source, bool loop);
			virtual ~Voice();

			virtual void Update() = 0;

			Source& mSource;
			State mState;
		};
	}
}