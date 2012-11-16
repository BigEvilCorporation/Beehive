/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2010

	File.cpp

	File IO
*/

#include "File.h"

namespace ion
{
	namespace io
	{
		bool FileExists(std::string filename)
		{
			return false;
		}

		int GetFileSize(std::string filename)
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

		File::File(std::string filename, File::OpenMode openMode)
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

		bool File::Open(std::string filename, File::OpenMode openMode)
		{
			std::ios::openmode mode = std::ios::binary;

			if(openMode == OpenRead)
				mode |= std::ios::in;
			else if(openMode == OpenWrite)
				mode |= std::ios::out;

			mStream.open(filename.c_str(), mode);
			mOpen = mStream.is_open();
			mOpenMode = openMode;

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

		unsigned int File::Seek(u64 position, SeekMode origin)
		{
			if(mOpen && position != mCurrentPosition)
			{
				std::ios_base::seek_dir Dir = std::ios_base::cur;

				if(origin == Start)
					Dir = std::ios_base::beg;
			
				mStream.seekg(position, Dir);

				return (u64)mStream.tellg();
			}
		
			return 0;
		}

		unsigned int File::Read(void* data, u64 size)
		{
			if(mOpen)
			{
				mStream.read((char*)data, size);
				mCurrentPosition = (int)mStream.tellg();
				return (unsigned int)mStream.gcount();
			}

			return 0;
		}

		unsigned int File::Write(const void* Data, u64 Size)
		{
			if(mOpen)
			{
				mStream.write((const char*)Data, Size);
				mCurrentPosition = (int)mStream.tellp();
				return mCurrentPosition;
			}

			return 0;
		}

		void File::Flush()
		{
			mStream.flush();
		}

		unsigned int File::GetSize()
		{
			//If filesize has been cached
			if(mSize)
				return mSize;

			if(!mOpen)
				return 0;

			//Get size (seek to end, tell, seek back)
			int CurrPos = (int)mStream.tellg();

			mStream.seekg(0, std::ios::end);
			mSize = (int)mStream.tellg();
		
			mStream.seekg(CurrPos, std::ios::beg);

			return mSize;
		}

		unsigned int File::GetPosition()
		{
			if(mOpen)
				return (int)mStream.tellg();
			else
				return 0;
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