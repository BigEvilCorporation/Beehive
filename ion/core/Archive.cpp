///////////////////////////////////////////////////
// File:		Archive.cpp
// Date:		22nd November 2012
// Authors:		Matt Phillips
// Description:	Serialisable archive
///////////////////////////////////////////////////

#include "Archive.h"
#include "Stream.h"

namespace ion
{
	namespace serialise
	{
		Archive::Archive(Stream& stream, Direction direction, u32 version)
			: mStream(stream)
		{
			mDirection = direction;
			mVersion = version;
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
	}
}