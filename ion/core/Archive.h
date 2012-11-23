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

			Archive(Stream& stream, Direction direction);

			//Raw serialisation of basic types (endian flipped if < 4 bytes)
			template <typename T> void Serialise(T& object);

			//Serialise a Serialisable-derived type
			void Serialise(Serialisable& object);

			//Raw serialisation of an array
			template <typename T> void Serialise(std::vector<T>& objects);

			//Serialise a string
			void Serialise(std::string& string);

			Direction GetDirection() const;

		private:
			Stream& mStream;
			Direction mDirection;
		};

		//Template functions belong in header
		template <typename T> void Archive::Serialise(T& object)
		{
			if(GetDirection() == In)
			{
				mStream.Read(&object, sizeof(T));
			}
			else
			{
				mStream.Write(&object, sizeof(T));
			}
		}

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