/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2010

	File.cpp

	File IO
*/

#include "io/File.h"

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
			mSize = 0;
			mCurrentPosition = 0;
			mOpen = false;
		}

		File::File(const std::string& filename, File::OpenMode openMode)
		{
			mOpenMode = openMode;
			mSize = 0;
			mCurrentPosition = 0;
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
				//Cache size
				GetSize();
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

		u64 File::Seek(u64 position, SeekMode origin)
		{
			u64 newPosition = mCurrentPosition;

			if(mOpen && position != mCurrentPosition)
			{
				std::ios_base::seek_dir direction = std::ios_base::cur;

				if(origin == Start)
					direction = std::ios_base::beg;
			
				mStream.seekg(position, direction);

				newPosition = (u64)mStream.tellg();
			}
		
			return newPosition;
		}

		u64 File::Read(void* data, u64 size)
		{
			u64 bytesRead = 0;

			if(mOpen)
			{
				mStream.read((char*)data, size);
				mCurrentPosition = (u64)mStream.tellg();
				bytesRead = (u64)mStream.gcount();
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
				bytesWritten = mCurrentPosition = startPosition;
			}

			return bytesWritten;
		}

		void File::Flush()
		{
			mStream.flush();
		}

		u64 File::GetSize()
		{
			//If open, and filesize hasn't yet been cached
			if(mOpen && !mSize)
			{
				//Get size (seek to end, tell, seek back)
				u64 currPos = (u64)mStream.tellg();

				mStream.seekg(0, std::ios::end);
				mSize = (u64)mStream.tellg();
			
				mStream.seekg(currPos, std::ios::beg);
			}

			return mSize;
		}

		u64 File::GetPosition()
		{
			u64 position = 0;

			if(mOpen)
			{
				position = (u64)mStream.tellg();
			}

			return position;
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