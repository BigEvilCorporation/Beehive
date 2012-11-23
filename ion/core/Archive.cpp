///////////////////////////////////////////////////
// File:		Archive.cpp
// Date:		22nd November 2012
// Authors:		Matt Phillips
// Description:	Serialisable archive
///////////////////////////////////////////////////

#include <Archive.h>
#include <Stream.h>

namespace ion
{
	namespace serialise
	{
		Archive::Archive(Stream& stream, Direction direction)
			: mStream(stream)
		{
			mDirection = direction;
		}

		void Archive::Serialise(Serialisable& object)
		{
			object.Serialise(*this);
		}
		
		void Archive::Serialise(std::string& string)
		{
			if(GetDirection() == In)
			{
				//Serialise in num chars
				int numChars = 0;
				Serialise(numChars);

				//Reserve string
				string.reserve(numChars);

				//Serialise chars
				for(int i = 0; i < numChars; i++)
				{
					char character = 0;
					Serialise(character);
					string += character;
				}
			}
			else
			{
				//Serialise out num chars
				int numChars = string.size();
				Serialise(numChars);

				//Serialise out chars
				mStream.Write(string.data(), numChars);
			}
		}

		Archive::Direction Archive::GetDirection() const
		{
			return mDirection;
		}
	}
}