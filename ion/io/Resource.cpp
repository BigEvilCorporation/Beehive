///////////////////////////////////////////////////
// File:		Resource.cpp
// Date:		22nd December 2013
// Authors:		Matt Phillips
// Description:	Resource management
///////////////////////////////////////////////////

#include "io/Resource.h"
#include "io/ResourceManager.h"
#include "core/thread/Atomic.h"

namespace ion
{
	namespace io
	{
		Resource::Resource(ResourceManager& resourceManager, const std::string& filename)
			: mResourceManager(resourceManager)
		{
			mFilename = filename;
			mIsLoaded = false;
			mResourceCount = 0;
		}

		Resource::~Resource()
		{

		}

		u32 Resource::GetResourceCount() const
		{
			return mResourceCount;
		}

		void Resource::Reference()
		{
			if(!mResourceCount)
			{
				mResourceManager.RequestLoad(*this);
			}

			thread::atomic::Increment(mResourceCount);
		}

		void Resource::Release()
		{
			thread::atomic::Decrement(mResourceCount);

			if(!mResourceCount)
			{
				mResourceManager.RequestUnload(*this);
			}
		}
	}
}