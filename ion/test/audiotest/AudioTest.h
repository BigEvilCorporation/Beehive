#include <core/bootstrap/Application.h>
#include <core/Version.h>
#include <core/time/Time.h>
#include <audio/Engine.h>
#include <audio/FileSource.h>
#include <audio/FileReaderWAV.h>
#include <audio/Voice.h>

class AudioTest : public ion::framework::Application
{
public:
	AudioTest();
	~AudioTest();

	bool Initialise();
	void Shutdown();
	bool Update(float deltaTime);
	void Render();

	ion::audio::Engine* mAudioEngine;
};