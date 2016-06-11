///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
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
			ResourceT<T>* m_resource;

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
			m_resource = NULL;
		}

		template <class T> ResourceHandle<T>::ResourceHandle(ResourceT<T>* resource)
		{
			m_resource = resource;
			m_resource->Reference();
		}

		template <class T> ResourceHandle<T>::ResourceHandle(const ResourceHandle<T>& rhs)
		{
			m_resource = rhs.mResource;

			if(m_resource)
			{
				m_resource->Reference();
			}
		}

		template <class T> ResourceHandle<T>::~ResourceHandle()
		{
			Clear();
		}

		template <class T> bool ResourceHandle<T>::IsValid() const
		{
			return m_resource && m_resource->IsLoaded();
		}

		template <class T> void ResourceHandle<T>::Clear()
		{
			if(m_resource)
			{
				m_resource->Release();
				m_resource = NULL;
			}
		}

		template <class T> ResourceHandle<T>& ResourceHandle<T>::operator = (const ResourceHandle& rhs)
		{
			if(m_resource != rhs.m_resource)
			{
				Clear();
				m_resource = rhs.m_resource;

				if(m_resource)
				{
					m_resource->Reference();
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
			return m_resource->Get();
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
			return m_resource->GetFilename();
		}
	}
}