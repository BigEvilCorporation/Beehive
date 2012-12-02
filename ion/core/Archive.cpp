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
		Archive::Archive(Stream& stream, Direction direction, u32 version)
			: mStream(stream)
		{
			mDirection = direction;
			mVersion = version;
		}

		void Archive::Serialise(Serialisable& object)
		{
			object.Serialise(*this);
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

		void Archive::Serialise(u8& data)
		{
			Serialise((void*)&data, sizeof(u8));
		}

		void Archive::Serialise(s8& data)
		{
			Serialise((void*)&data, sizeof(s8));
		}

		void Archive::Serialise(u16& data)
		{
			Serialise((void*)&data, sizeof(u16));
		}

		void Archive::Serialise(s16& data)
		{
			Serialise((void*)&data, sizeof(s16));
		}

		void Archive::Serialise(u32& data)
		{
			Serialise((void*)&data, sizeof(u32));
		}

		void Archive::Serialise(s32& data)
		{
			Serialise((void*)&data, sizeof(s32));
		}

		void Archive::Serialise(u64& data)
		{
			Serialise((void*)&data, sizeof(u64));
		}

		void Archive::Serialise(s64& data)
		{
			Serialise((void*)&data, sizeof(s64));
		}

		void Archive::Serialise(float& data)
		{
			Serialise((void*)&data, sizeof(float));
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

		u32 Archive::GetVersion() const
		{
			return mVersion;
		}
	}
}