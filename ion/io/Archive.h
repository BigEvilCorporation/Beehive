///////////////////////////////////////////////////
// File:		Archive.h
// Date:		22nd November 2012
// Authors:		Matt Phillips
// Description:	Serialisable archive
///////////////////////////////////////////////////

#pragma once

#include "io/stream.h"

//Includes for ion types handled directly by Archive
#include "core/Types.h"

//Includes STL types handled directly by Archive
#include <vector>
#include <list>
#include <map>

namespace ion
{
	namespace io
	{
		class ResourceManager;
		template <class T> class ResourceHandle;

		class Archive
		{
		public:
			enum Direction { In, Out };

			Archive(Stream& stream, Direction direction, ResourceManager* resourceManager = NULL, u32 version = 0);

			//Register pointer type mapping
			template <typename T> void RegisterPointerType();

			//Register pointer type mapping with strictly defined serialise/runtime types
			template <typename SERIALISE_T, typename RUNTIME_T> void RegisterPointerTypeStrict();

			//Default templated serialise
			template <typename T> void Serialise(T& object);

			//Pointer serialise
			template <typename T> void Serialise(T*& object);

			//Resource serialise
			template <typename T> void Serialise(ResourceHandle<T>& object);

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
			template <> void Serialise<bool>(bool& data);

			//Serialise STL string
			template <> void Serialise<std::string>(std::string& string);

			//Serialise STL containers
			template <typename T> void Serialise(std::vector<T>& objects);
			template <typename T> void Serialise(std::list<T>& objects);
			template <typename KEY, typename T> void Serialise(std::map<KEY, T>& objects);

			Direction GetDirection() const;
			u32 GetVersion() const;
			ResourceManager* GetResourceManager() const;

		private:
			//i/o stream
			Stream& mStream;

			//Stream version
			u32 mVersion;

			//Serialise direction
			Direction mDirection;

			//Resource manager
			ResourceManager* mResourceManager;

			//Pointer constructor/serialiser
			struct PointerMappingBase
			{
				virtual void* Construct() { return NULL; }
				virtual void Serialise(Archive& archive, void* pointer) {}

				std::string mSerialiseTypeName;
				std::string mRuntimeTypeName;
			};

			template <typename SERIALISE_T, typename RUNTIME_T> struct PointerMapping : public PointerMappingBase
			{
				PointerMapping()
				{
					mSerialiseTypeName = typeid(SERIALISE_T).name();
					mRuntimeTypeName = typeid(RUNTIME_T).name();
				}

				virtual void* Construct()
				{
					return (void*)new RUNTIME_T;
				}

				virtual void Serialise(Archive& archive, void* pointer)
				{
					archive.Serialise(*(SERIALISE_T*)(pointer));
				}
			};

			//Registered pointer mappings
			std::map<std::string, PointerMappingBase*> mPointerMappingsByRuntimeName;
			std::map<std::string, PointerMappingBase*> mPointerMappingsBySerialiseName;
		};
	}
}

////////////////////////////////////////////////////////////////
// Template definitions
////////////////////////////////////////////////////////////////

#include "io/ResourceHandle.h"
#include "io/ResourceManager.h"

namespace ion
{
	namespace io
	{
		template <typename T> void Archive::RegisterPointerType()
		{
			std::string className = typeid(T).name();

			if(mPointerMappingsBySerialiseName.find(className) == mPointerMappingsBySerialiseName.end())
			{
				PointerMappingBase* pointerMapping = new PointerMapping<T, T>();
				mPointerMappingsByRuntimeName[className] = pointerMapping;
				mPointerMappingsBySerialiseName[className] = pointerMapping;
			}
		}

		template <typename SERIALISE_T, typename RUNTIME_T> void Archive:: RegisterPointerTypeStrict()
		{
			std::string serialiseName = typeid(SERIALISE_T).name();
			std::string runtimeName = typeid(RUNTIME_T).name();

			if(mPointerMappingsByRuntimeName.find(runtimeName) == mPointerMappingsByRuntimeName.end())
			{
				PointerMappingBase* pointerMapping = new PointerMapping<SERIALISE_T, RUNTIME_T>();
				mPointerMappingsByRuntimeName[runtimeName] = pointerMapping;
				mPointerMappingsBySerialiseName[serialiseName] = pointerMapping;
			}
		}

		template <typename T> void Archive::Serialise(T& object)
		{
			object.Serialise(*this);
		}

		template <typename T> void Archive::Serialise(T*& object)
		{
			//Serialise NULL flag
			bool nullPtr = (object == NULL);
			Serialise(nullPtr);

			if(GetDirection() == In)
			{
				if(nullPtr)
				{
					//Pointer was NULL when serialised out
					object = NULL;
				}
				else
				{
					//Serialise class name
					std::string serialiseClassName;
					Serialise(serialiseClassName);

					//Find in pointer mapping
					std::map<std::string, PointerMappingBase*>::iterator it = mPointerMappingsBySerialiseName.find(serialiseClassName);
					ion::debug::Assert(it != mPointerMappingsBySerialiseName.end(), "Archive::Serialise(T*) - Pointer type not registered");

					//Construct
					object = (T*)it->second->Construct();

					//Serialise in
					it->second->Serialise(*this, object);
				}
			}
			else
			{
				if(!nullPtr)
				{
					//Serialising pointer out, determine actual type
					std::string runtimeClassName = typeid(*object).name();

					//Find in pointer mapping
					std::map<std::string, PointerMappingBase*>::iterator it = mPointerMappingsByRuntimeName.find(runtimeClassName);
					ion::debug::Assert(it != mPointerMappingsByRuntimeName.end(), "Archive::Serialise(T*) - Pointer type not registered");

					//Serialise class name
					Serialise(it->second->mSerialiseTypeName);

					//Serialise out
					it->second->Serialise(*this, object);
				}
			}
		}

		template <typename T> void Archive::Serialise(ResourceHandle<T>& handle)
		{
			//Serialise NULL ptr flag
			bool validResource = handle.IsValid();
			Serialise(validResource);

			if(validResource)
			{
				if(GetDirection() == Archive::In)
				{
					debug::Assert(mResourceManager != NULL, "No ResourceManager, cannot serialise in Resources with this archive");

					//Read resource name
					std::string resourceName;
					Serialise(resourceName);

					//Request resource
					handle = mResourceManager->GetResource<T>(resourceName);
				}
				else
				{
					//Write resource name
					std::string resourceName = handle.GetName();
					Serialise(resourceName);
				}
			}
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

		template <> void Archive::Serialise<bool>(bool& data)
		{
			if(GetDirection() == In)
			{
				u8 boolean = 0;
				Serialise(boolean);
				data = (boolean != 0);
			}
			else
			{
				u8 boolean = data ? 1 : 0;
				Serialise(boolean);
			}
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

				//Clear and reserve vector
				objects.clear();
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

				//Clear map
				objects.clear();

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
					Serialise((KEY&)it->first);

					//Serialise object
					Serialise(it->second);
				}
			}
		}
	}
}