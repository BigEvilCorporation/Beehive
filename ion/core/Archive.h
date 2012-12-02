///////////////////////////////////////////////////
// File:		Archive.h
// Date:		22nd November 2012
// Authors:		Matt Phillips
// Description:	Serialisable archive
///////////////////////////////////////////////////

#pragma once

#include "Serialise.h"
#include "Stream.h"

#include <vector>

namespace ion
{
	namespace serialise
	{
		class Archive
		{
		public:
			enum Direction { In, Out };

			Archive(Stream& stream, Direction direction, u32 version);

			//Serialise a Serialisable-derived type
			void Serialise(Serialisable& object);

			//Raw serialisation
			void Serialise(void* data, u64 size);

			//Raw serialisation of basic types (with endian flipping)
			void Serialise(u8& data);
			void Serialise(s8& data);
			void Serialise(u16& data);
			void Serialise(s16& data);
			void Serialise(u32& data);
			void Serialise(s32& data);
			void Serialise(u64& data);
			void Serialise(s64& data);
			void Serialise(float& data);

			//Serialise a string
			void Serialise(std::string& string);

			//Serialise an array
			template <typename T> void Serialise(std::vector<T>& objects);

			Direction GetDirection() const;
			u32 GetVersion() const;

		private:
			Stream& mStream;
			Direction mDirection;
			u32 mVersion;
		};

		//Template functions belong in header
		template <typename T> void Archive::Serialise(std::vector<T>& objects)
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
				for(int i = 0; i < numObjects; i++)
				{
					Serialise(objects[i]);
				}
			}
		}
	}
}