#include <windows.h>
#include <mmreg.h>

#include <core/debug/Debug.h>
#include <audio/Buffer.h>
#include <audio/StreamDesc.h>
#include <audio/Source.h>

#include "VoiceXAudio.h"

namespace ion
{
	namespace audio
	{
		VoiceXAudio::VoiceXAudio(IXAudio2* xaudio2System, Source& source, bool loop)
			: Voice(source, loop)
		{
			mXAudioVoice = NULL;

			//Get stream desc
			const StreamDesc* streamDesc = source.GetStreamDesc();

			//Create WAVEFORMATEX descriptor
			WAVEFORMATEX waveFormatEx = {0};
			waveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
			waveFormatEx.nChannels = streamDesc->GetNumChannels();
			waveFormatEx.nSamplesPerSec = streamDesc->GetSampleRate();
			waveFormatEx.nAvgBytesPerSec = (streamDesc->GetBitsPerSample() / 8) * streamDesc->GetSampleRate() * streamDesc->GetNumChannels();
			waveFormatEx.nBlockAlign = streamDesc->GetBlockSize();
			waveFormatEx.wBitsPerSample = streamDesc->GetBitsPerSample();
			waveFormatEx.cbSize = 0;

			//Create voice
			HRESULT result = xaudio2System->CreateSourceVoice(&mXAudioVoice, &waveFormatEx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, this);
			debug::Assert(result == S_OK && mXAudioVoice, "VoiceXAudio::VoiceXAudio() - Could not create XAudio2 voice");

			//Request first buffer
			source.RequestBuffer(*this);

			//Set default properties
			SetVolume(1.0f);
		}

		VoiceXAudio::~VoiceXAudio()
		{
		}

		void VoiceXAudio::SubmitBuffer(Buffer& buffer)
		{
			XAUDIO2_BUFFER xaudioBuffer = {0};

			xaudioBuffer.pAudioData = (const BYTE*)buffer.Get(0);
			xaudioBuffer.AudioBytes = buffer.GetSize();
			xaudioBuffer.PlayLength = mSource.GetStreamDesc()->GetSizeSamples();
			xaudioBuffer.Flags = XAUDIO2_END_OF_STREAM;

			mXAudioVoice->SubmitSourceBuffer(&xaudioBuffer);
		}

		void VoiceXAudio::Play()
		{
			mXAudioVoice->Start(0);
			mState = Playing;
		}

		void VoiceXAudio::Stop()
		{
		}

		void VoiceXAudio::Pause()
		{
		}

		void VoiceXAudio::Resume()
		{
		}

		void VoiceXAudio::Update()
		{
		}

		void VoiceXAudio::SetVolume(float volume)
		{
		}

		void VoiceXAudio::OnStreamEnd()
		{
			mState = Stopped;
		}
	}
}