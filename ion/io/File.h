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
		bool FileExists(const std::string& filename);
		u64 GetFileSize(const std::string& filename);

		class File : public io::Stream
		{
			static const int s_bufferSize = (1024 * 32);

		public:
			enum OpenMode { eOpenRead, eOpenWrite, eOpenAppend };
			enum SeekMode { eStart, eCurrent };

			File();
			File(const std::string& filename, OpenMode openMode = eOpenRead);
			virtual ~File();

			virtual bool Open(const std::string& filename, OpenMode openMode = eOpenRead);
			virtual void Close();

			virtual u64 GetSize() const;
			virtual u64 GetPosition() const;
			virtual u64 Read(void* data, u64 size);
			virtual u64 Write(const void* data, u64 Size);
			virtual u64 Seek(u64 position);
			u64 Seek(u64 position, SeekMode origin = eCurrent);

			void Flush();

			bool IsOpen() const;

			OpenMode GetOpenMode() const;

		protected:
			std::string m_filename;
			OpenMode m_openMode;

			u64 m_currentPosition;
			u64 m_size;

			bool m_open;

			std::fstream m_stream;
			u8 m_buffer[s_bufferSize];
		};
	}
} //Namespace