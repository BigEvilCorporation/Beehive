/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2010

	BinaryFile.cpp

	Binary tree file reading/writing
*/

#include "io/BinaryFile.h"
#include "core/MemoryManager.h"

#include "core/Debug.h"

namespace ion
{
	namespace io
	{
		BinaryFile::Chunk::Chunk()
		{
			mData = 0;
			mChunkStartPos = 0;
			memory::MemSet(&mHeader, 0, sizeof(ChunkHeader));
			mFile = NULL;
			mParent = NULL;
		}

		BinaryFile::Chunk::Chunk(io::File* file, Chunk* parent)
		{
			mData = 0;
			mChunkStartPos = 0;
			memory::MemSet(&mHeader, 0, sizeof(ChunkHeader));
			mFile = file;
			mParent = parent;
		}

		BinaryFile::Chunk::~Chunk()
		{
			mChildren.clear();
		}

		int BinaryFile::Chunk::GetID()
		{
			return mHeader.ChunkID;
		}

		void BinaryFile::Chunk::SetID(int ChunkID)
		{
			mHeader.ChunkID = ChunkID;
		}

		u64 BinaryFile::Chunk::GetChunkSize()
		{
			if(!mFile || mFile->GetOpenMode() == io::File::OpenWrite)
			{
				u64 Size = mHeader.DataSize + sizeof(ChunkHeader);

				//Traverse all children
				for(std::list<Chunk>::iterator it = mChildren.begin(), end = mChildren.end(); it != end; ++it)
				{
					Size += it->GetChunkSize();
				}

				return Size;
			}
			else
			{
				return mHeader.TotalSize;
			}
		}

		u64 BinaryFile::Chunk::GetDataSize()
		{
			return mHeader.DataSize;
		}

		u64 BinaryFile::Chunk::GetNumItems()
		{
			return mHeader.NumItems;
		}

		int BinaryFile::Chunk::GetNumChildren()
		{
			return (int)mChildren.size();
		}

		void BinaryFile::Chunk::AddChild(Chunk& chunk)
		{
			mChildren.push_back(chunk);
			mHeader.TotalSize = GetChunkSize();
		}

		void BinaryFile::Chunk::SetData(void* Data, u64 Size, u64 NumItems)
		{
			mData = Data;
			mHeader.DataSize = Size;
			mHeader.NumItems = NumItems;
		}

		BinaryFile::Chunk::ChunkHeader* BinaryFile::Chunk::GetHeader()
		{
			return &mHeader;
		}

		bool BinaryFile::Chunk::ReadHeader(File* InFile)
		{
			mChunkStartPos = InFile->GetPosition();

			//Read header
			return (InFile->Read(&mHeader, sizeof(ChunkHeader)) == sizeof(ChunkHeader));
		}

		u64 BinaryFile::Chunk::GetChunkStartPos()
		{
			return mChunkStartPos;
		}

		u64 BinaryFile::Chunk::GetDataStartPos()
		{
			return mChunkStartPos + sizeof(ChunkHeader);
		}

		bool BinaryFile::Chunk::Write(File* OutFile)
		{
			//Set number of children
			mHeader.NumChildren = mChildren.size();

			//Set total size
			mHeader.TotalSize = GetChunkSize();

			//Write header and data
			OutFile->Write(&mHeader, sizeof(ChunkHeader));
			OutFile->Write(mData, mHeader.DataSize);

			//Traverse all children
			for(std::list<Chunk>::iterator it = mChildren.begin(), end = mChildren.end(); it != end; ++it)
			{
				if(!it->Write(OutFile))
					return false;
			}

			return true;
		}

		BinaryFile::BinaryFile()
		{
			memory::MemSet(&mHeader, 0, sizeof(FileHeader));
		}

		BinaryFile::BinaryFile(std::string filename, File::OpenMode Mode)
		{
			Open(filename, Mode);
		}

		BinaryFile::~BinaryFile()
		{
			mFile.Close();
		}

		bool BinaryFile::Open(std::string filename, File::OpenMode Mode)
		{
			//Open file
			bool Result = mFile.Open(filename, Mode);

			if(Result)
			{
				//If in read mode, read the header and root chunk
				if(Mode == File::OpenRead)
				{
					//Read header
					if(!mFile.Read(&mHeader, sizeof(FileHeader)))
					{
						mFile.Close();
						return false;
					}
					else
					{
						//Read root chunk
						if(!mTrunk.ReadHeader(&mFile))
						{
							mFile.Close();
							return false;
						}
					}
				}
			}

			mTrunk.mFile = &mFile;

			return Result;
		}

		void BinaryFile::Close()
		{
			mFile.Close();
		}

		bool BinaryFile::IsOpen()
		{
			return mFile.IsOpen();
		}

		bool BinaryFile::Write()
		{
			if(mFile.IsOpen() && mFile.GetOpenMode() == File::OpenWrite)
			{
				//Get total size
				u64 Size = sizeof(FileHeader) + mTrunk.GetChunkSize();

				//Write header
				mFile.Write(&mHeader, sizeof(FileHeader));

				//Traverse trunk to write headers
				mTrunk.Write(&mFile);

				return true;
			}
			else
			{
				return false;
			}
		}

		BinaryFile::Chunk& BinaryFile::GetTrunk()
		{
			return mTrunk;
		}
		/*
		bool BinaryFile::Chunk::Find(u16 chunkID, Chunk& chunk)
		{
			if(mFile.IsOpen() && mFile.GetOpenMode() == File::OpenRead)
			{
				//Store current pos
				unsigned int OriginalPos = mFile.GetPosition();

				bool Found = false;

				while(!Found)
				{
					//Read chunk header
					if(chunk.ReadHeader(&mFile))
					{
						//Chunk read, check ID
						if(chunk.GetID() == chunkID)
						{	
							return true;
						}
						else
						{
							//Check if at end of parent chunk
							if(mFile.GetPosition() >= (GetDataStartPos() - sizeof(ChunkHeader)) + GetTotalSize())
							{
								return false;
							}

							//Seek straight to next chunk
							mFile.Seek(chunk.GetDataSize(), File::Current);
						}
					}
					else
					{
						//Reached end of file
						return false;
					}
				}
			}

			return false;
		}
		*/

		BinaryFile::Chunk BinaryFile::Chunk::Begin()
		{
			if(mFile->GetPosition() != GetDataStartPos())
			{
				ion::debug::Error("BinaryFile::Chunk::Begin() - File position not at start of chunk");
			}

			Chunk chunk(mFile, this);
			chunk.ReadHeader(mFile);
			return chunk;
		}

		BinaryFile::Chunk BinaryFile::Chunk::Next()
		{
			if(!mParent)
			{
				//Only one root chunk
				return End();
			}

			u64 endOfChunk = GetChunkStartPos() + GetChunkSize();
			u64 endOfParent = mParent->GetChunkStartPos() + mParent->GetChunkSize();

			//If end of this chunk == end of parent chunk
			if(endOfChunk == endOfParent)
			{
				//Parent has no children left
				return End();
			}

			//Seek to next chunk
			mFile->Seek(endOfChunk, File::Start);

			//Read header
			Chunk chunk(mFile, mParent);
			chunk.ReadHeader(mFile);
			
			return chunk;
		}

		BinaryFile::Chunk BinaryFile::Chunk::End()
		{
			Chunk chunk;
			chunk.SetID(sInvalidChunkId);
			return chunk;
		}

		bool BinaryFile::Chunk::operator == (BinaryFile::Chunk& chunk)
		{
			return GetID() == chunk.GetID();
		}

		bool BinaryFile::Chunk::operator != (BinaryFile::Chunk& chunk)
		{
			return GetID() != chunk.GetID();
		}

		u64 BinaryFile::Chunk::ReadData(void* Data, u64 Size)
		{
			//Check if file position is inside this chunk's data
			if(mFile->GetPosition() < GetDataStartPos() || mFile->GetPosition() >= GetDataStartPos() + GetDataSize())
			{
				ion::debug::Error("BinaryFile::ReadChunkData() - Current file position is outside this chunk");
			}

			//Check size
			if((mFile->GetPosition() + Size) > (GetDataStartPos() + GetDataSize()))
			{
				ion::debug::Error("BinaryFile::ReadChunkData() - Read past end of chunk data");
			}

			//Read data
			return mFile->Read(Data, Size);
		}

		const std::string BinaryFile::GetFileType()
		{
			return mHeader.FileType;
		}

		const short BinaryFile::GetFileVersion()
		{
			return mHeader.FileVersion;
		}

		void BinaryFile::SetFileType(std::string Typename)
		{
			//Utils::SPrintF(mHeader.FileType, sizeof(mHeader.FileType), Typename.GetChar());
			sprintf(mHeader.FileType, "%s", Typename.c_str());
		}

		void BinaryFile::SetFileVersion(u16 FileVersion)
		{
			mHeader.FileVersion = FileVersion;
		}
	}
} //Namespace