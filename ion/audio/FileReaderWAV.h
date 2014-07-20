#include <audio/DataFormat.h>
#include <audio/FileReader.h>

//TODO: Windows only, replace WAVEFORMATEXTENSIBLE
#include <windows.h>
#include <mmreg.h>

namespace ion
{
	namespace audio
	{
		struct RIFFChunk
		{
			u32 chunkId;
			u32 chunkSize;
		};

		class FileHeaderWAV : public FileHeader
		{
		public:
			virtual DataFormat GetEncodedFormat() const { return PCM16; }
			virtual DataFormat GetDecodedFormat() const { return PCM16; }
			virtual u32 GetNumChannels() const { return mMSWAVHeader.Format.nChannels; }
			virtual u32 GetSampleRate() const { return mMSWAVHeader.Format.nSamplesPerSec; }
			virtual u32 GetBitsPerSample() const { return mMSWAVHeader.Format.wBitsPerSample; }
			virtual u32 GetBlockSize() const { return mMSWAVHeader.Format.nBlockAlign; }
			virtual u32 GetEncodedSizeBytes() const { return mSize; }
			virtual u32 GetDecodedSizeBytes() const { return mSize; }
			virtual u32 GetSizeSamples() const { return mSize / (GetBitsPerSample() / 8) / GetNumChannels(); }

			WAVEFORMATEXTENSIBLE mMSWAVHeader;
			u32 mSize;
			u32 mDataOffset;
		};

		class FileReaderWAV : public FileReaderT<FileHeaderWAV>
		{
		public:
			FileReaderWAV(const char* filename);
			virtual ~FileReaderWAV();

			virtual bool Open();
			virtual void Close();

			virtual u32 Read(char* data, u32 bytes);

			virtual void SeekRaw(u32 byte);
			virtual void SeekSample(u32 sample);
			virtual void SeekTime(float time);

		protected:
			virtual bool ReadHeader();
			bool SeekChunk(u32 fourCC, RIFFChunk& chunk);

			static u32 MakeFourCC(const char* string);
			static void EndianFlipHeader(FileHeaderWAV& header);
		};
	}
}