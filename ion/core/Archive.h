///////////////////////////////////////////////////
// File:		Archive.h
// Date:		22nd November 2012
// Authors:		Matt Phillips
// Description:	Serialisable archive
///////////////////////////////////////////////////

#pragma once

#include "Serialise.h"
#include "Stream.h"

//Includes for ion types handled directly by Archive
#include "Types.h"
#include "maths/Vector.h"
#include "maths/Matrix.h"
#include "maths/Quaternion.h"

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

			//Serialise a Serialisable-derived type
			void Serialise(Serialisable& object);

			//Raw serialisation (no endian flipping)
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

			//ion types that can't derive from Serialisable, or shouldn't have the Serialise libs as a dependency
			void Serialise(Vector3& vector);
			void Serialise(Matrix4& matrix);
			void Serialise(Quaternion& quaternion);

			//Serialise STL string
			void Serialise(std::string& string);

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