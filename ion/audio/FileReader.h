#include <audio/StreamDesc.h>
#include <io/File.h>
#include <string>
#pragma once

namespace ion
{
	namespace audio
	{
		class FileHeader : public StreamDesc
		{
		};

		class FileReader
		{
		public:
			FileReader(const char* filename);
			virtual ~FileReader();

			virtual bool Open() = 0;
			virtual void Close() = 0;

			virtual u32 Read(char* data, u32 bytes) = 0;

			virtual void SeekRaw(u32 byte) = 0;
			virtual void SeekSample(u32 sample) = 0;
			virtual void SeekTime(float time) = 0;

			virtual const StreamDesc& GetStreamDesc() const = 0;

		protected:
			virtual bool ReadHeader() = 0;

			std::string mFilename;
			io::File mFile;
		};

		template <class HeaderT> class FileReaderT : public FileReader
		{
		public:
			FileReaderT(const char* filename) : FileReader(filename) {}
			virtual const StreamDesc& GetStreamDesc() const { return mHeader; }
		protected:
			HeaderT mHeader;
		};
	}
}