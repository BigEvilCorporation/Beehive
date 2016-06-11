///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Archive.h
// Date:		22nd November 2012
// Authors:		Matt Phillips
// Description:	Serialisable archive
///////////////////////////////////////////////////

#pragma once

#include "io/stream.h"

//Includes for ion types handled directly by Archive
#include "core/Types.h"
#include "maths/Maths.h"

//Includes STL types handled directly by Archive
#include <vector>
#include <list>
#include <map>
#include <algorithm>

namespace ion
{
	namespace io
	{
		class ResourceManager;
		template <class T> class ResourceHandle;

		class Archive
		{
		public:
			enum Direction { eIn, eOut };

			struct Tag
			{
				Tag();
				Tag(const char* string);
				Tag(u32 hash);
				Tag(const Tag& rhs);
				Tag& operator = (u32 hash);
				bool operator == (const Tag& rhs) const;
				bool operator != (const Tag& rhs) const;

				u32 m_hash;
			};

			Archive(Stream& stream, Direction direction, ResourceManager* resourceManager = NULL, u32 version = 0);

			//Default named block serialise
			template <typename T> void Serialise(T& object, const Tag& tag);

			//Register pointer type mapping
			template <typename T> void RegisterPointerType();

			//Register pointer type mapping with strictly defined serialise/runtime types
			template <typename SERIALISE_T, typename RUNTIME_T> void RegisterPointerTypeStrict();

			Direction GetDirection() const;
			u32 GetVersion() const;
			ResourceManager* GetResourceManager() const;

		//private:

			//Named blocks
			bool PushBlock(const Tag& tag);
			void PopBlock();

			//Default templated serialise
			template <typename T> void Serialise(T& object);

			//Pointer serialise
			template <typename T> void Serialise(T*& object);

			//Resource serialise
			template <typename T> void Serialise(ResourceHandle<T>& object);

			//Stream serialise
			template <> void Serialise(MemoryStream& stream);

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
			template <typename T1, typename T2> void Serialise(std::pair<T1, T2>& pair);
			template <typename T1, typename T2, typename T3> void Serialise(std::tuple<T1, T2, T3>& tuple);
			template <typename T> void Serialise(std::vector<T>& objects);
			template <typename T> void Serialise(std::list<T>& objects);
			template <typename KEY, typename T> void Serialise(std::map<KEY, T>& objects);

			//i/o stream
			Stream& m_stream;

			//Stream version
			u32 m_version;

			//Serialise direction
			Direction m_direction;

			//Resource manager
			ResourceManager* m_resourceManager;

			//Block
			struct Block
			{
				#pragma pack(push, 1)
				struct Header
				{
					Tag tag;
					u64 size;

					void Serialise(Archive& archive);
				};
				#pragma pack(pop)
				
				Header header;
				MemoryStream data;
				u64 startPos;
				Block* parent;
			};

			//Block tree
			Block* m_blockNode;

			//Pointer constructor/serialiser
			struct PointerMappingBase
			{
				virtual void* Construct() { return NULL; }
				virtual void Serialise(Archive& archive, void* pointer) {}

				std::string m_serialiseTypeName;
				std::string m_runtimeTypeName;
			};

			template <typename SERIALISE_T, typename RUNTIME_T> struct PointerMapping : public PointerMappingBase
			{
				PointerMapping()
				{
					m_serialiseTypeName = typeid(SERIALISE_T).name();
					m_runtimeTypeName = typeid(RUNTIME_T).name();
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
			std::map<std::string, PointerMappingBase*> m_pointerMappingsByRuntimeName;
			std::map<std::string, PointerMappingBase*> m_pointerMappingsBySerialiseName;
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

			if(m_pointerMappingsBySerialiseName.find(className) == m_pointerMappingsBySerialiseName.end())
			{
				PointerMappingBase* pointerMapping = new PointerMapping<T, T>();
				m_pointerMappingsByRuntimeName[className] = pointerMapping;
				m_pointerMappingsBySerialiseName[className] = pointerMapping;
			}
		}

		template <typename SERIALISE_T, typename RUNTIME_T> void Archive:: RegisterPointerTypeStrict()
		{
			std::string serialiseName = typeid(SERIALISE_T).name();
			std::string runtimeName = typeid(RUNTIME_T).name();

			if(m_pointerMappingsByRuntimeName.find(runtimeName) == m_pointerMappingsByRuntimeName.end())
			{
				PointerMappingBase* pointerMapping = new PointerMapping<SERIALISE_T, RUNTIME_T>();
				m_pointerMappingsByRuntimeName[runtimeName] = pointerMapping;
				m_pointerMappingsBySerialiseName[serialiseName] = pointerMapping;
			}
		}

		template <typename T> void Archive::Serialise(T& object)
		{
			object.Serialise(*this);
		}

		template <typename T> void Archive::Serialise(T& object, const Tag& tag)
		{
			if(PushBlock(tag))
			{
				Serialise(object);
				PopBlock();
			}
		}

		template <typename T> void Archive::Serialise(T*& object)
		{
			//Serialise NULL flag
			bool nullPtr = (object == NULL);
			Serialise(nullPtr, "nullPtr");

			if(GetDirection() == eIn)
			{
				if(nullPtr)
				{
					//Pointer was NULL when serialised out
					object = NULL;
				}
				else
				{
					//Serialise class name
					std::string serialiseTypeName;
					Serialise(serialiseTypeName, "typeName");

					//Find in pointer mapping
					std::map<std::string, PointerMappingBase*>::iterator it = m_pointerMappingsBySerialiseName.find(serialiseTypeName);
					ion::debug::Assert(it != m_pointerMappingsBySerialiseName.end(), "Archive::Serialise(T*) - Pointer type not registered");

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
					std::map<std::string, PointerMappingBase*>::iterator it = m_pointerMappingsByRuntimeName.find(runtimeClassName);
					ion::debug::Assert(it != m_pointerMappingsByRuntimeName.end(), "Archive::Serialise(T*) - Pointer type not registered");

					//Serialise class name
					Serialise(it->second->m_serialiseTypeName, "typeName");

					//Serialise out
					it->second->Serialise(*this, object);
				}
			}
		}

		template <typename T> void Archive::Serialise(ResourceHandle<T>& handle)
		{
			//Serialise NULL ptr flag
			bool validResource = handle.IsValid();
			Serialise(validResource, "valid");

			if(validResource)
			{
				if(GetDirection() == Archive::eIn)
				{
					debug::Assert(m_resourceManager != NULL, "No ResourceManager, cannot serialise in Resources with this archive");

					//Read resource name
					std::string resourceName;
					Serialise(resourceName, "name");

					//Request resource
					handle = m_resourceManager->GetResource<T>(resourceName);
				}
				else
				{
					//Write resource name
					std::string resourceName = handle.GetName();
					Serialise(resourceName, "name");
				}
			}
		}

		template <> void Archive::Serialise<MemoryStream>(MemoryStream& stream)
		{
			const u64 bufferSize = 1024;
			u8 buffer[bufferSize] = { 0 };

			if(GetDirection() == eIn)
			{
				debug::Error("Archive::Serialise() - Cannot serialise a stream back in");
			}
			else
			{
				u64 size = stream.GetSize();
				stream.Seek(0);

				while(size)
				{
					u64 bytesToWrite = maths::Min(size, bufferSize);
					stream.Read(buffer, bytesToWrite);
					Serialise((void*)buffer, bytesToWrite);
					size -= bytesToWrite;
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
			if(GetDirection() == eIn)
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
			if(GetDirection() == eIn)
			{
				if(PushBlock("__string"))
				{
					//Serialise in num chars
					int numChars = 0;
					Serialise(numChars, "count");

					//Clear and reserve string
					string.clear();
					string.reserve(numChars);

					//Serialise chars
					for(int i = 0; i < numChars; i++)
					{
						char character = 0;
						Serialise(character);
						string += character;
					}

					PopBlock();
				}
			}
			else
			{
				if(PushBlock("__string"))
				{
					//Serialise out num chars
					int numChars = (int)string.size();
					Serialise(numChars, "count");

					//Serialise out chars
					Serialise((void*)string.data(), numChars);

					PopBlock();
				}
			}
		}

		template <typename T1, typename T2> void Archive::Serialise(std::pair<T1, T2>& pair)
		{
			Serialise(pair.first, "first");
			Serialise(pair.second, "second");
		}

		template <typename T1, typename T2, typename T3> void Archive::Serialise(std::tuple<T1, T2, T3>& tuple)
		{
			Serialise(std::get<0>(tuple), "t1");
			Serialise(std::get<1>(tuple), "t2");
			Serialise(std::get<2>(tuple), "t3");
		}

		template <typename T> void Archive::Serialise(std::vector<T>& objects)
		{
			if(GetDirection() == eIn)
			{
				//Serialise in num objects
				int numObjects = 0;
				Serialise(numObjects, "count");

				//Clear and reserve vector
				objects.clear();
				objects.resize(numObjects);

				//Serialise all objects
				for(int i = 0; i < numObjects; i++)
				{
					Serialise(objects[i], "object");
				}
			}
			else
			{
				//Serialise out num objects
				int numObjects = objects.size();
				Serialise(numObjects, "count");

				//Serialise all objects out
				for(int i = 0; i < numObjects; i++)
				{
					Serialise(objects[i], "object");
				}
			}
		}

		template <typename T> void Archive::Serialise(std::list<T>& objects)
		{
			if(GetDirection() == eIn)
			{
				//Serialise in num objects
				int numObjects = 0;
				Serialise(numObjects, "count");

				//Serialise all objects and add to list
				for(int i = 0; i < numObjects; i++)
				{
					T object;
					Serialise(object, "object");
					objects.push_back(object);
				}
			}
			else
			{
				//Serialise out num objects
				int numObjects = objects.size();
				Serialise(numObjects, "count");

				//Serialise all objects out
				for(std::list<T>::iterator it = objects.begin(), end = objects.end(); it != end; ++it)
				{
					Serialise(*it, "object");
				}
			}
		}

		template <typename KEY, typename T> void Archive::Serialise(std::map<KEY, T>& objects)
		{
			if(GetDirection() == eIn)
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
					Serialise(key, "key");

					//Serialise object
					T object;
					Serialise(object, "object");

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
					Serialise((KEY&)it->first, "key");

					//Serialise object
					Serialise(it->second, "object");
				}
			}
		}
	}
}