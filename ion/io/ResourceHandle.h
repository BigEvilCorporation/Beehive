///////////////////////////////////////////////////
// File:		Resource.h
// Date:		22nd December 2013
// Authors:		Matt Phillips
// Description:	Resource management
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include "core/debug/Debug.h"

#include <string>

namespace ion
{
	namespace io
	{
		template <class T> class ResourceT;

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
			T& operator * () const;
			T* operator -> () const;
			T* Get() const;

			const std::string& GetName() const;

		protected:
			ResourceT<T>* mResource;

			friend class ResourceManager;
		};
	}
}

////////////////////////////////////////////////////////////////
// Template definitions
////////////////////////////////////////////////////////////////

#include "io/Resource.h"

namespace ion
{
	namespace io
	{
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
			mResource = rhs.mResource;

			if(mResource)
			{
				mResource->Reference();
			}
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
			if(mResource != rhs.mResource)
			{
				Clear();
				mResource = rhs.mResource;

				if(mResource)
				{
					mResource->Reference();
				}
			}

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

		template <class T> T& ResourceHandle<T>::operator * () const
		{
			return *Get();
		}

		template <class T> T* ResourceHandle<T>::operator -> () const
		{
			return Get();
		}

		template <class T> const std::string& ResourceHandle<T>::GetName() const
		{
			debug::Assert(IsValid(), "Resource not loaded");
			return mResource->GetFilename();
		}
	}
}