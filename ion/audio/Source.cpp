#include <core/debug/Debug.h>

#include "Source.h"

namespace ion
{
	namespace audio
	{
		Source::Source(FeedType feedType)
		{
			mStreamDesc = NULL;
			mFeedType = feedType;
			mLockCount = 0;
		}

		Source::~Source()
		{
		}

		void Source::Lock()
		{
			thread::atomic::Increment(mLockCount);
		}

		void Source::Unlock()
		{
			debug::Assert(mLockCount > 0, "Source::Unlock() - Source not locked");
			thread::atomic::Decrement(mLockCount);
		}

		const StreamDesc* Source::GetStreamDesc() const
		{
			return mStreamDesc;
		}

		Source::FeedType Source::GetFeedType() const
		{
			return mFeedType;
		}
	}
}