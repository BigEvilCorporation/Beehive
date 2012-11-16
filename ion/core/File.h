/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2010

	File.h

	File IO
*/

#pragma once

#include "Types.h"

#include <fstream>
#include <string>

namespace ion
{
	namespace io
	{
		bool FileExists(std::string filename);
		int GetFileSize(std::string filename);

		class File
		{
		public:
			enum OpenMode { OpenRead, OpenWrite, OpenAppend };
			enum SeekMode { Start, Current };

			File();
			File(std::string filename, OpenMode openMode = OpenRead);
			virtual ~File();

			virtual bool Open(std::string filename, OpenMode openMode = OpenRead);
			virtual void Close();

			unsigned int Seek(u64 position, SeekMode origin = Current);
			unsigned int Read(void* data, u64 size);
			unsigned int Write(const void* data, u64 Size);

			void Flush();

			unsigned int GetSize();
			unsigned int GetPosition();

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