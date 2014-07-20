#pragma once

#include <core/Types.h>
#include <audio/DataFormat.h>

namespace ion
{
	namespace audio
	{
		class StreamDesc
		{
		public:
			virtual DataFormat GetEncodedFormat() const = 0;
			virtual DataFormat GetDecodedFormat() const = 0;

			virtual u32 GetNumChannels() const = 0;
			virtual u32 GetSampleRate() const = 0;
			virtual u32 GetBitsPerSample() const = 0;
			virtual u32 GetBlockSize() const = 0;

			virtual u32 GetEncodedSizeBytes() const = 0;
			virtual u32 GetDecodedSizeBytes() const = 0;

			virtual u32 GetSizeSamples() const = 0;
		};
	}
}