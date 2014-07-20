#include <core/debug/Debug.h>
#include <core/memory/Memory.h>

#include "FileReaderWAV.h"

namespace ion
{
	namespace audio
	{
		FileReaderWAV::FileReaderWAV(const char* filename)
			: FileReaderT(filename)
		{
		}

		FileReaderWAV::~FileReaderWAV()
		{
		}

		bool FileReaderWAV::Open()
		{
			if(mFile.Open(mFilename.c_str(), io::File::OpenRead))
			{
				//Seek to 'RIFF' chunk
				RIFFChunk chunk;
				if(SeekChunk(MakeFourCC("RIFF"), chunk))
				{
					//Read file type
					u32 fileType = 0;
					mFile.Read(&fileType, 4);

					//Read file header
					if(ReadHeader())
					{
						//Seek to data
						SeekRaw(0);

						return true;
					}
				}
				else
				{
					Close();
				}
			}

			return false;
		}

		void FileReaderWAV::Close()
		{
			mFile.Close();
		}

		u32 FileReaderWAV::Read(char* data, u32 bytes)
		{
			u32 numBlocks = bytes / mHeader.GetBlockSize();
			u32 blockAlignedReadSize = mHeader.GetBlockSize() * numBlocks;
			return mFile.Read(data, blockAlignedReadSize);
		}

		void FileReaderWAV::SeekRaw(u32 byte)
		{
			mFile.Seek(mHeader.mDataOffset + byte);
		}

		void FileReaderWAV::SeekSample(u32 sample)
		{
			SeekRaw(sample * (mHeader.GetBitsPerSample() / 8) * mHeader.GetNumChannels());
		}

		void FileReaderWAV::SeekTime(float time)
		{
			SeekSample(time * mHeader.GetSampleRate());
		}

		bool FileReaderWAV::ReadHeader()
		{
			//Seek to 'FMT ' chunk
			RIFFChunk chunk;
			if(SeekChunk(MakeFourCC("fmt "), chunk))
			{
				//Read WAVEFORMATEX
				debug::Assert(chunk.chunkSize <= sizeof(mHeader.mMSWAVHeader), "FileReaderWAV::ReadHeader() - Bad chunk size");
				memory::MemSet(&mHeader.mMSWAVHeader, 0, sizeof(mHeader.mMSWAVHeader));
				mFile.Read(&mHeader.mMSWAVHeader, chunk.chunkSize);

				//Endian flip
				EndianFlipHeader(mHeader);

				//Seek to 'DATA' chunk
				if(SeekChunk(MakeFourCC("data"), chunk))
				{
					//Set size
					mHeader.mSize = chunk.chunkSize;

					//Set offset
					mHeader.mDataOffset = mFile.GetPosition();
				}

				return true;
			}

			return false;
		}

		bool FileReaderWAV::SeekChunk(u32 fourCC, RIFFChunk& chunk)
		{
			bool found = false;
			bool endOfFile = false;
			while(!found)
			{
				mFile.Read(&chunk, sizeof(RIFFChunk));
				if(chunk.chunkId == fourCC)
				{
					found = true;
				}
				else
				{
					endOfFile = (mFile.Seek(chunk.chunkSize) == (mFile.GetSize() - 1));
				}
			}

			return found;
		}

		u32 FileReaderWAV::MakeFourCC(const char* string)
		{
			return (u32)(((string[3])<<24) | ((string[2])<<16) | ((string[1])<<8) | (string[0]));
		}

		void FileReaderWAV::EndianFlipHeader(FileHeaderWAV& header)
		{
			//TODO: Implement
		}
	}
}