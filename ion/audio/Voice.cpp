#include "Voice.h"

namespace ion
{
	namespace audio
	{
		Voice::Voice(Source& source, bool loop)
			: mSource(source)
		{
		}

		Voice::~Voice()
		{
		}

		Voice::State Voice::GetState() const
		{
			return mState;
		}

		float Voice::GetVolume() const
		{
			return 1.0f;
		}
	}
}