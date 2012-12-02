/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2010

	BinaryFile.h

	Binary tree file reading/writing
*/

#pragma once

#include "File.h"
#include "Hash.h"

#include <string>
#include <list>

#define CHUNK_ID( x ) ((((u32)(x) & 0x000000FF) << 24) | (((u32)(x) & 0x0000FF00) << 8) | (((u32)(x) & 0x00FF0000) >> 8) | (((u32)(x) && 0xFF000000) >> 24))

namespace ion
{
	namespace io
	{
		class BinaryFile
		{
		public:
			class Chunk
			{
			public:
				struct ChunkHeader
				{
					int ChunkID;	//Arbitrary chunk ID
					u64 DataSize;	//Size of data section
					u64 TotalSize;	//Total size of chunk incl. children
					u64 NumItems;	//Number of data items in data section, for client-side use
					int NumChildren;//Number of child chunks - a chunk can only contain children OR data
				};

				Chunk();
				~Chunk();

				//Set chunk data section
				void SetData(void* Data, u64 Size, u64 NumItems);

				//Get chunk ID tag
				int GetID();
				void SetID(int ChunkID);

				//Traverse tree and calculate total size, including this chunk and header
				u64 GetChunkSize();

				//Get size of data section
				u64 GetDataSize();

				//Get number of data items in data section
				u64 GetNumItems();

				//Get number of child chunks
				int GetNumChildren();

				//Add a child chunk
				void AddChild(Chunk& chunk);

				//Iterate through chunks
				Chunk Begin();
				Chunk End();
				Chunk Next();

				u64 ReadData(void* Data, u64 Size);

				//bool Find(u16 chunkID, Chunk& chunk);

				bool operator == (Chunk& chunk);
				bool operator != (Chunk& chunk);

			protected:
				ChunkHeader* GetHeader();
		
				bool ReadHeader(File* InFile);
				u64 GetChunkStartPos();
				u64 GetDataStartPos();
				bool Write(File* OutFile);

				Chunk* mParent;
				io::File* mFile;

			protected:
				Chunk(io::File* file, Chunk* parent);

				ChunkHeader mHeader;
				std::list<Chunk> mChildren;
				void* mData;
				u64 mChunkStartPos;
				
				static const int sInvalidChunkId = -1;

				friend class BinaryFile;
			};

			template <class T> class ChunkT : public Chunk
			{
			public:
				ChunkT()
				{
					mData = &mDataT;
					mHeader.DataSize = sizeof(T);
					mHeader.NumItems = 1;
				}

				void SetData(const T& data)
				{
					mDataT = data;
				}

				T& ReadData()
				{
					Chunk::ReadData((void*)&mDataT, sizeof(T));
					return mDataT;
				}

			private:
				T mDataT;
			};

			struct FileHeader
			{
				char FileType[16];
				short FileVersion;
			};

			BinaryFile();
			BinaryFile(std::string filename, File::OpenMode openMode = File::OpenRead);

			virtual ~BinaryFile();

			bool Open(std::string filename, File::OpenMode openMode = File::OpenRead);
			void Close();
			bool IsOpen();

			//Writes the entire tree to file
			bool Write();

			Chunk& GetTrunk();

			const std::string GetFileType();
			const short GetFileVersion();

			void SetFileType(std::string Typename);
			void SetFileVersion(u16 FileVersion);

		protected:
			File mFile;
			FileHeader mHeader;
			Chunk mTrunk;
		};
	}
} //Namespace