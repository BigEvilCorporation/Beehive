#include "AudioTest.h"

#include <sstream>

AudioTest::AudioTest()
	: ion::framework::Application("AudioTest")
{

}

AudioTest::~AudioTest()
{

}

bool AudioTest::Initialise()
{
	mAudioEngine = ion::audio::Engine::Create();

	ion::audio::FileReaderWAV wavReader("..\\audio\\test.wav");
	ion::audio::FileSource source(ion::audio::Source::SingleBuffer, wavReader);

	if(source.Load())
	{
		ion::audio::Voice* voice = mAudioEngine->CreateVoice(source, false);
		if(voice)
		{
			voice->Play();

			while(voice->GetState() == ion::audio::Voice::Playing)
			{
			}

			mAudioEngine->ReleaseVoice(*voice);
		}

		source.CloseStream();
	}

	return true;
}

void AudioTest::Shutdown()
{

}

bool AudioTest::Update(float deltaTime)
{
	return false;
}

void AudioTest::Render()
{

}