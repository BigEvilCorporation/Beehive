///////////////////////////////////////////////////
// File:		Resource.cpp
// Date:		22nd December 2013
// Authors:		Matt Phillips
// Description:	Resource management
///////////////////////////////////////////////////

#include "io/Resource.h"
#include "io/ResourceManager.h"

namespace ion
{
	namespace io
	{
		Resource::Resource(ResourceManager& resourceManager, const std::string& filename)
			: mResourceManager(resourceManager)
		{
			mFilename = filename;
			mIsLoaded = false;
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
			//TODO: Atomic increment
			++mResourceCount;
		}

		void Resource::Release()
		{
			//TODO: atomic decrement
			--mResourceCount;

			if(!mResourceCount)
			{
				mResourceManager.RequestUnload(*this);
			}
		}
	}
}