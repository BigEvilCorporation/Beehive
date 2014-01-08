///////////////////////////////////////////////////
// File:		Resource.h
// Date:		22nd December 2013
// Authors:		Matt Phillips
// Description:	Resource management
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include "core/debug/Debug.h"
#include "io/Archive.h"
#include "io/File.h"

namespace ion
{
	namespace io
	{
		class ResourceManager;

		class Resource
		{
		public:
			virtual ~Resource();

			void Reference();
			void Release();
			u32 GetResourceCount() const;

			virtual bool Load() { return false; }
			virtual void Unload() {};
			virtual bool IsLoaded() { return false; }

		protected:
			Resource(ResourceManager& resourceManager, const std::string& filename);

			ResourceManager& mResourceManager;
			std::string mFilename;

			u32 mResourceCount;
			bool mIsLoaded;

			friend class ResourceManager;
		};

		template <class T> class ResourceT : public Resource
		{
		public:
			ResourceT(ResourceManager& resourceManager, const std::string& filename)
				: Resource(resourceManager, filename)
			{
				mResourceObject = NULL;
			}

			virtual ~ResourceT() {}

			virtual bool Load();
			virtual void Unload();
			virtual bool IsLoaded() { return mIsLoaded; }

			T* Get() const { return mResourceObject; }

		protected:
			T* mResourceObject;
		};

		template <class T> bool ResourceT<T>::Load()
		{
			//Prepend directory
			std::string fullPath = mResourceManager.GetResourceDirectory<T>();
			fullPath += "/";
			fullPath += mFilename;

			//Create and open scene file stream for reading
			File file(fullPath, File::OpenRead);

			if(file.IsOpen())
			{
				//Create archive for serialising in
				Archive archiveIn(file, Archive::In);

				//Register pointer type
				T::RegisterSerialiseType(archiveIn);

				//Serialise pointer
				archiveIn.Serialise(mResourceObject);

				//Close file
				file.Close();

				//Loaded
				mIsLoaded = true;
			}

			return mIsLoaded;
		}

		template <class T> void ResourceT<T>::Unload()
		{
			mIsLoaded = false;
			delete mResourceObject;
			mResourceObject = NULL;
		}

		template <class T> class ResourceHandle
		{
		public:
			ResourceHandle();
			ResourceHandle(ResourceT<T>* resource);
			ResourceHandle(const ResourceHandle<T>& rhs);
			~ResourceHandle();

			bool IsValid() const;
			void Clear();

			ResourceHandle<T>& operator = (const ResourceHandle& rhs);
			operator bool () const;
			T* operator -> () const;
			T* Get() const;

		protected:
			ResourceT<T>* mResource;

			friend class ResourceManager;
		};

		template <class T> ResourceHandle<T>::ResourceHandle()
		{
			mResource = NULL;
		}

		template <class T> ResourceHandle<T>::ResourceHandle(ResourceT<T>* resource)
		{
			mResource = resource;
			mResource->Reference();
		}

		template <class T> ResourceHandle<T>::ResourceHandle(const ResourceHandle<T>& rhs)
		{
			Clear();
			mResource = rhs.mResource;
			mResource->Reference();
		}

		template <class T> ResourceHandle<T>::~ResourceHandle()
		{
			Clear();
		}

		template <class T> bool ResourceHandle<T>::IsValid() const
		{
			return mResource && mResource->IsLoaded();
		}

		template <class T> void ResourceHandle<T>::Clear()
		{
			if(mResource)
			{
				mResource->Release();
				mResource = NULL;
			}
		}

		template <class T> ResourceHandle<T>& ResourceHandle<T>::operator = (const ResourceHandle& rhs)
		{
			Clear();
			mResource = rhs.mResource;
			mResource->Reference();
			return *this;
		}

		template <class T> ResourceHandle<T>::operator bool () const
		{
			return IsValid();
		}

		template <class T> T* ResourceHandle<T>::Get() const
		{
			debug::Assert(IsValid(), "Resource not loaded");
			return mResource->Get();
		}

		template <class T> T* ResourceHandle<T>::operator -> () const
		{
			return Get();
		}
	}
}