///////////////////////////////////////////////////
// File:		Archive.cpp
// Date:		22nd November 2012
// Authors:		Matt Phillips
// Description:	Serialisable archive
///////////////////////////////////////////////////

#include "io/Archive.h"
#include "io/Stream.h"

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
		}

		void Archive::Serialise(void* data, u64 size)
		{
			if(GetDirection() == In)
			{
				mStream.Read(data, size);
			}
			else
			{
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
	}
}