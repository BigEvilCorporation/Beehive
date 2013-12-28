///////////////////////////////////////////////////
// File:		Resource.h
// Date:		22nd December 2013
// Authors:		Matt Phillips
// Description:	Resource management
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
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

			u32 GetResourceCount() const;

		protected:
			Resource(ResourceManager& resourceManager, const std::string& filename);

			virtual bool Load() = 0;
			virtual void Unload() = 0;
			virtual bool IsLoaded() = 0;

			void Reference();
			void Release();

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
				: Resource(resourceManager, filename) {}

			virtual ~ResourceT() {}

		protected:
			virtual bool Load() = 0;
			virtual void Unload() = 0;
			virtual bool IsLoaded() { return mIsLoaded; }

			T* mResourceObject;
			bool mIsLoaded;
		};

		template <class T> bool ResourceT<T>::Load()
		{
			//Create and open scene file stream for reading
			File file(mFilename, File::OpenRead);

			if(file.IsOpen())
			{
				//Create archive for serialising in
				Archive archiveIn(file, Archive::In);

				//Serialise pointer
				archiveIn.Serialise(mResourceObject);

				//Close file
				file.Close();

				//Loaded
				mIsLoaded = true;
			}
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
			ResourceHandle(Resource* resource);
			ResourceHandle(const ResourceHandle<T>& rhs);
			~ResourceHandle();

			bool IsValid() const;
			void Clear();

			ResourceHandle<T>& operator = (const ResourceHandle& rhs);
			operator bool () const;
			T* operator -> () const;

		protected:
			T* mResource;

			friend class ResourceManager;
		};

		template <class T> ResourceHandle<T>::ResourceHandle()
		{
			mResource = NULL;
		}

		template <class T> ResourceHandle<T>::ResourceHandle(Resource* resource)
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
		}

		template <class T> ResourceHandle<T>::operator bool () const
		{
			return IsValid();
		}

		template <class T> T* ResourceHandle<T>::operator -> () const
		{
			return mResource;
		}
	}
}