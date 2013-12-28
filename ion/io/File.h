/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2010

	File.h

	File IO
*/

#pragma once

#include "core/Types.h"
#include "io/Stream.h"

#include <fstream>
#include <string>

namespace ion
{
	namespace io
	{
		bool FileExists(std::string filename);
		u64 GetFileSize(std::string filename);

		class File : public io::Stream
		{
		public:
			enum OpenMode { OpenRead, OpenWrite, OpenAppend };
			enum SeekMode { Start, Current };

			File();
			File(std::string filename, OpenMode openMode = OpenRead);
			virtual ~File();

			virtual bool Open(std::string filename, OpenMode openMode = OpenRead);
			virtual void Close();

			u64 Seek(u64 position, SeekMode origin = Current);
			u64 Read(void* data, u64 size);
			u64 Write(const void* data, u64 Size);

			void Flush();

			u64 GetSize();
			u64 GetPosition();

			bool IsOpen() const;

			OpenMode GetOpenMode() const;

		protected:
			std::string mFilename;
			OpenMode mOpenMode;

			u64 mSize;
			u64 mCurrentPosition;

			bool mOpen;

			std::fstream mStream;
		};
	}
} //Namespace