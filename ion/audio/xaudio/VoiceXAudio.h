#include <audio/Voice.h>

#include <XAudio2.h>

namespace ion
{
	namespace audio
	{
		class VoiceXAudio : public Voice, public IXAudio2VoiceCallback
		{
			friend class EngineXAudio;
		public:
			//Transport
			virtual void Play();
			virtual void Stop();
			virtual void Pause();
			virtual void Resume();

			//Properties
			virtual void SetVolume(float volume);

			//Submit buffer
			virtual void SubmitBuffer(Buffer& buffer);

			//IXAudio2VoiceCallback methods
			virtual void __stdcall OnBufferStart(void* bufferContext) {}
			virtual void __stdcall OnBufferEnd(void* bufferContext) {}
			virtual void __stdcall OnLoopEnd(void* bufferContext) {}
			virtual void __stdcall OnStreamEnd();
			virtual void __stdcall OnVoiceError(void* bufferContext, HRESULT error) {}
			virtual void __stdcall OnVoiceProcessingPassStart(UINT32 bytesRequired) {}
			virtual void __stdcall OnVoiceProcessingPassEnd() {}

		protected:
			VoiceXAudio(IXAudio2* xaudio2System, Source& source, bool loop);
			virtual ~VoiceXAudio();

			virtual void Update();

		private:
			IXAudio2SourceVoice* mXAudioVoice;
		};
	}
}