#pragma once

namespace ion
{
	namespace audio
	{
		class Buffer;

		class SourceCallback
		{
		public:
			virtual void SubmitBuffer(Buffer& buffer) = 0;
		};
	}
}