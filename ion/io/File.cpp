/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2010

	File.cpp

	File IO
*/

#include "io/File.h"

#include <algorithm>

namespace ion
{
	namespace io
	{
		bool FileExists(const std::string& filename)
		{
			return false;
		}

		u64 GetFileSize(const std::string& filename)
		{
			return false;
		}

		File::File()
		{
			mOpenMode = OpenRead;
			mCurrentPosition = 0;
			mSize = 0;
			mOpen = false;
		}

		File::File(const std::string& filename, File::OpenMode openMode)
		{
			mOpenMode = openMode;
			mCurrentPosition = 0;
			mSize = 0;
			mOpen = false;

			Open(filename, openMode);
		}

		File::~File()
		{
			if(mOpen)
				Close();
		}

		bool File::Open(const std::string& filename, File::OpenMode openMode)
		{
			std::ios::openmode mode = std::ios::binary;

			if(openMode == OpenRead)
				mode |= std::ios::in;
			else if(openMode == OpenWrite)
				mode |= std::ios::out;

			mStream.open(filename.c_str(), mode);
			mOpen = mStream.is_open();
			mOpenMode = openMode;
			mFilename = filename;

			if(mOpen)
			{
				//Use buffer
				mStream.rdbuf()->pubsetbuf((char*)m_buffer, s_bufferSize);

				//Get size (seek to end, get pos, seek back)
				mStream.seekg(0, std::ios::end);
				mSize = (u64)mStream.tellg();
				mStream.seekg(0, std::ios::beg);
			}

			return mOpen;
		}

		void File::Close()
		{
			if(mOpen)
			{
				mStream.close();
				mOpen = false;
			}
		}

		u64 File::Seek(u64 position)
		{
			return Seek(position, Start);
		}

		u64 File::Seek(u64 position, SeekMode origin)
		{
			if(mOpen && position != mCurrentPosition)
			{
				std::ios_base::seek_dir direction = std::ios_base::cur;

				if(origin == Start)
				{
					direction = std::ios_base::beg;
					position = std::min(position, mSize - 1);
					mCurrentPosition = position;
				}
				else if(origin == Current)
				{
					position = std::min(position, mSize - mCurrentPosition - 1);
					mCurrentPosition += position;
				}
			
				mStream.seekg(position, direction);
			}
		
			return mCurrentPosition;
		}

		u64 File::Read(void* data, u64 size)
		{
			u64 bytesRead = 0;

			if(mOpen)
			{
				size = std::min(size, mCurrentPosition + mSize);
				mStream.read((char*)data, size);
				mCurrentPosition += size;
				bytesRead = size;
			}

			return bytesRead;
		}

		u64 File::Write(const void* Data, u64 Size)
		{
			u64 bytesWritten = 0;

			if(mOpen)
			{
				u64 startPosition = (u64)mStream.tellp();
				mStream.write((const char*)Data, Size);
				mCurrentPosition = (u64)mStream.tellp();
				bytesWritten = mCurrentPosition - startPosition;
				mSize += bytesWritten;
			}

			return bytesWritten;
		}

		void File::Flush()
		{
			mStream.flush();
		}

		u64 File::GetSize() const
		{
			return mSize;
		}

		u64 File::GetPosition() const
		{
			return mCurrentPosition;
		}

		File::OpenMode File::GetOpenMode() const
		{
			return mOpenMode;
		}

		bool File::IsOpen() const
		{
			return mOpen;
		}
	}
} //Namespace