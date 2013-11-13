///////////////////////////////////////////////////
// File:		Archive.h
// Date:		22nd November 2012
// Authors:		Matt Phillips
// Description:	Serialisable archive
///////////////////////////////////////////////////

#pragma once

#include "Stream.h"

//Includes for ion types handled directly by Archive
#include "Types.h"

//Includes STL types handled directly by Archive
#include <vector>
#include <list>
#include <map>

namespace ion
{
	namespace serialise
	{
		class Archive
		{
		public:
			enum Direction { In, Out };

			Archive(Stream& stream, Direction direction, u32 version);

			//Default templated serialise
			template <typename T> void Serialise(T& object);

			//Raw serialisation (no endian flipping)
			void Serialise(void* data, u64 size);

			//Raw serialisation of basic types (with endian flipping)
			template <> void Serialise<u8>(u8& data);
			template <> void Serialise<s8>(s8& data);
			template <> void Serialise<u16>(u16& data);
			template <> void Serialise<s16>(s16& data);
			template <> void Serialise<u32>(u32& data);
			template <> void Serialise<s32>(s32& data);
			template <> void Serialise<u64>(u64& data);
			template <> void Serialise<s64>(s64& data);
			template <> void Serialise<float>(float& data);

			//Serialise STL string
			template <> void Serialise<std::string>(std::string& string);

			//Serialise STL containers
			template <typename T> void Serialise(std::vector<T>& objects);
			template <typename T> void Serialise(std::list<T>& objects);
			template <typename KEY, typename T> void Serialise(std::map<KEY, T>& objects);

			Direction GetDirection() const;
			u32 GetVersion() const;

		private:
			Stream& mStream;
			Direction mDirection;
			u32 mVersion;
		};

		template <typename T> void Archive::Serialise(T& object)
		{
			object.Serialise(*this);
		}

		template <> void Archive::Serialise<u8>(u8& data)
		{
			Serialise((void*)&data, sizeof(u8));
		}

		template <> void Archive::Serialise<s8>(s8& data)
		{
			Serialise((void*)&data, sizeof(s8));
		}

		template <> void Archive::Serialise<u16>(u16& data)
		{
			Serialise((void*)&data, sizeof(u16));
		}

		template <> void Archive::Serialise<s16>(s16& data)
		{
			Serialise((void*)&data, sizeof(s16));
		}

		template <> void Archive::Serialise<u32>(u32& data)
		{
			Serialise((void*)&data, sizeof(u32));
		}

		template <> void Archive::Serialise<s32>(s32& data)
		{
			Serialise((void*)&data, sizeof(s32));
		}

		template <> void Archive::Serialise<u64>(u64& data)
		{
			Serialise((void*)&data, sizeof(u64));
		}

		template <> void Archive::Serialise<s64>(s64& data)
		{
			Serialise((void*)&data, sizeof(s64));
		}

		template <> void Archive::Serialise<float>(float& data)
		{
			Serialise((void*)&data, sizeof(float));
		}
		
		template <> void Archive::Serialise<std::string>(std::string& string)
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
				int numChars = (int)string.size();
				Serialise(numChars);

				//Serialise out chars
				mStream.Write(string.data(), numChars);
			}
		}

		template <typename T> void Archive::Serialise(std::vector<T>& objects)
		{
			if(GetDirection() == In)
			{
				//Serialise in num objects
				int numObjects = 0;
				Serialise(numObjects);

				//Resize vector to construct elements
				objects.resize(numObjects);

				//Serialise all objects
				for(int i = 0; i < numObjects; i++)
				{
					Serialise(objects[i]);
				}
			}
			else
			{
				//Serialise out num objects
				int numObjects = objects.size();
				Serialise(numObjects);

				//Serialise all objects out
				for(int i = 0; i < numObjects; i++)
				{
					Serialise(objects[i]);
				}
			}
		}

		template <typename T> void Archive::Serialise(std::list<T>& objects)
		{
			if(GetDirection() == In)
			{
				//Serialise in num objects
				int numObjects = 0;
				Serialise(numObjects);

				//Serialise all objects and add to list
				for(int i = 0; i < numObjects; i++)
				{
					T object;
					Serialise(object);
					objects.push_back(object);
				}
			}
			else
			{
				//Serialise out num objects
				int numObjects = objects.size();
				Serialise(numObjects);

				//Serialise all objects out
				for(std::list<T>::iterator it = objects.begin(), end = objects.end(); it != end; ++it)
				{
					Serialise(*it);
				}
			}
		}

		template <typename KEY, typename T> void Archive::Serialise(std::map<KEY, T>& objects)
		{
			if(GetDirection() == In)
			{
				//Serialise in num objects
				int numObjects = 0;
				Serialise(numObjects);

				//Serialise all objects and add to map
				for(int i = 0; i < numObjects; i++)
				{
					//Serialise key
					KEY key;
					Serialise(key);

					//Serialise object
					T object;
					Serialise(object);

					//Add to map
					objects.insert(std::pair<KEY, T>(key, object));
				}
			}
			else
			{
				//Serialise out num objects
				int numObjects = (int)objects.size();
				Serialise(numObjects);

				//Serialise all objects out
				for(std::map<KEY, T>::iterator it = objects.begin(), end = objects.end(); it != end; ++it)
				{
					//Serialise key (lose const correctness, direction is known)
					Serialise((std::string&)it->first);

					//Serialise object
					Serialise(it->second);
				}
			}
		}
	}
}