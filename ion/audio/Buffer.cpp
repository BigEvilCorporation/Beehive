#include <core/memory/Memory.h>
#include <core/thread/Atomic.h>
#include <core/debug/Debug.h>

#include "Buffer.h"

namespace ion
{
	namespace audio
	{
		Buffer::Buffer(u32 size)
		{
			mLockCount = 0;
			mSize = size;
			mData = new char[size];
			memory::MemSet(mData, 0, size);
		}

		Buffer::~Buffer()
		{
			debug::Assert(mLockCount == 0, "Buffer::~Buffer() - Buffer still locked");
			delete mData;
		}

		void Buffer::Lock()
		{
			thread::atomic::Increment(mLockCount);
		}

		void Buffer::Unlock()
		{
			debug::Assert(mLockCount > 0, "Buffer::Unlock() - Buffer not locked");
			thread::atomic::Decrement(mLockCount);
		}

		void Buffer::Put(const char* data, u32 size, u32 position)
		{
			debug::Assert(mLockCount > 0, "Buffer::Put() - Buffer not locked");
			debug::Assert(size <= (mSize + position), "Buffer::Put() - Not enough space");
			memory::MemCopy(mData + position, (void*)data, size);
		}

		char* Buffer::Get(u32 position) const
		{
			debug::Assert(mLockCount > 0, "Buffer::Get() - Buffer not locked");
			debug::Assert(position < mSize, "Buffer::Get() - Overflow");
			return mData + position;
		}

		u32 Buffer::GetSize() const
		{
			return mSize;
		}
	}
}