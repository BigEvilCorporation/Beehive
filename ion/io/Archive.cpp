///////////////////////////////////////////////////
// File:		Archive.cpp
// Date:		22nd November 2012
// Authors:		Matt Phillips
// Description:	Serialisable archive
///////////////////////////////////////////////////

#include "io/Archive.h"
#include "io/Stream.h"
#include "core/cryptography/Hash.h"

namespace ion
{
	namespace io
	{
		Archive::Archive(Stream& stream, Direction direction, ResourceManager* resourceManager, u32 version)
			: mStream(stream)
		{
			mDirection = direction;
			mVersion = version;
			mResourceManager = resourceManager;
			m_blockNode = NULL;
		}

		void Archive::Serialise(void* data, u64 size)
		{
			if(GetDirection() == In)
			{
				mStream.Read(data, size);
			}
			else
			{
				if(m_blockNode)
					m_blockNode->data.Write(data, size);
				else
					mStream.Write(data, size);
			}
		}

		Archive::Direction Archive::GetDirection() const
		{
			return mDirection;
		}

		u32 Archive::GetVersion() const
		{
			return mVersion;
		}

		ResourceManager* Archive::GetResourceManager() const
		{
			return mResourceManager;
		}

		bool Archive::PushBlock(const Tag& tag)
		{
			//Create block
			Block* block = new Block;

			if(mDirection == In)
			{
				u64 parentBlockStartPos = m_blockNode ? m_blockNode->startPos : 0;
				u64 parentBlockEndPos = m_blockNode ? (parentBlockStartPos + m_blockNode->header.size - 1) : (mStream.GetSize() - 1);

				//Record search start pos
				u64 searchStartPos = mStream.GetPosition();

				//Record block start pos
				block->startPos = searchStartPos;
				block->header.tag = 0;
				block->header.size = 0;

				//Read header at current position
				Serialise(block->header);

				if(block->header.tag != tag)
				{
					//No match, begin search of current block (or whole file if not in a block)
					if(m_blockNode)
						block->startPos = m_blockNode->startPos + sizeof(Block::Header);
					else
						block->startPos = 0;

					//Seek to start of search block
					mStream.Seek(block->startPos);

					//Seek to each child of current block in turn, checking tag
					while(block->header.tag != tag && block->startPos + block->header.size < parentBlockEndPos)
					{
						//Record start pos
						block->startPos = mStream.GetPosition();

						//Serialise block header
						Serialise(block->header);

						if(block->header.tag != tag)
						{
							//Seek to end of block
							mStream.Seek(block->startPos + block->header.size);
						}
					}

					if(block->header.tag != tag)
					{
						//Block not found, return to original starting position
						mStream.Seek(searchStartPos);
					}
				}
			}
			else
			{
				block->startPos = mStream.GetPosition();
				block->header.tag = tag;
			}

			if(block->header.tag == tag)
			{
				//Add to tree
				block->parent = m_blockNode;

				//Set as current
				m_blockNode = block;

				//Success
				return true;
			}
			else
			{
				//Could not find block, seek back to start pos
				delete block;

				return false;
			}
		}

		void Archive::PopBlock()
		{
			debug::Assert(m_blockNode, "Archive::PopBlock() - No block to pop");

			//Pop back to parent
			Block* child = m_blockNode;
			m_blockNode = child->parent;

			if(mDirection == In)
			{
				//Seek to block end
				mStream.Seek(child->startPos + child->header.size);
			}
			else
			{
				//Set block size
				child->header.size = sizeof(Block::Header) + child->data.GetSize();

				//Write current block
				Serialise(child->header);
				Serialise(child->data);
			}

			//Delete block
			delete child;
		}

		void Archive::Block::Header::Serialise(Archive& archive)
		{
			archive.Serialise(tag.m_hash);
			archive.Serialise(size);
		}

		Archive::Tag::Tag()
		{
			m_hash = 0;
		}

		Archive::Tag::Tag(const char* string)
		{
			m_hash = Hash(string);
		}

		Archive::Tag::Tag(u32 hash)
		{
			m_hash = hash;
		}

		Archive::Tag::Tag(const Tag& rhs)
		{
			m_hash = rhs.m_hash;
		}

		Archive::Tag& Archive::Tag::operator = (u32 hash)
		{
			m_hash = hash;
			return *this;
		}

		bool Archive::Tag::operator == (const Tag& rhs) const
		{
			return m_hash == rhs.m_hash;
		}

		bool Archive::Tag::operator != (const Tag& rhs) const
		{
			return m_hash != rhs.m_hash;
		}
	}
}