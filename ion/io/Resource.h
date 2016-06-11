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
		class ResourceManager;

		class Resource
		{
		public:
			virtual ~Resource();

			void Reference();
			void Release();
			u32 GetResourceCount() const;

			const std::string& GetFilename() const;

			virtual bool Load() { return false; }
			virtual void Unload() {};
			virtual bool IsLoaded() { return false; }

		protected:
			Resource(ResourceManager& resourceManager, const std::string& filename);

			ResourceManager& m_resourceManager;
			std::string m_filename;

			u32 m_resourceCount;
			bool m_isLoaded;

			friend class ResourceManager;
		};

		template <class T> class ResourceT : public Resource
		{
		public:
			ResourceT(ResourceManager& resourceManager, const std::string& filename)
				: Resource(resourceManager, filename)
			{
				m_resourceObject = NULL;
			}

			ResourceT(ResourceManager& resourceManager, const std::string& filename, T* resourceObject)
				: Resource(resourceManager, filename)
			{
				m_resourceObject = resourceObject;
				m_isLoaded = true;
			}

			virtual ~ResourceT() {}

			virtual bool Load();
			virtual void Unload();
			virtual bool IsLoaded() { return m_isLoaded; }

			T* Get() const { return m_resourceObject; }

		protected:
			T* m_resourceObject;
		};
	}
}

////////////////////////////////////////////////////////////////
// Template definitions
////////////////////////////////////////////////////////////////

#include "io/Archive.h"
#include "io/ResourceManager.h"
#include "io/File.h"

namespace ion
{
	namespace io
	{
		template <class T> bool ResourceT<T>::Load()
		{
			//Prepend directory
			std::string fullPath = m_resourceManager.GetResourceDirectory<T>();
			fullPath += "/";
			fullPath += m_filename;

			//Create and open scene file stream for reading
			File file(fullPath, File::eOpenRead);

			if(file.IsOpen())
			{
				//Create archive for serialising in
				Archive archiveIn(file, Archive::eIn, &m_resourceManager);

				//Register pointer type
				T::RegisterSerialiseType(archiveIn);

				//Serialise pointer
				archiveIn.Serialise(m_resourceObject);

				//Close file
				file.Close();

				//Loaded
				m_isLoaded = true;
			}

			return m_isLoaded;
		}

		template <class T> void ResourceT<T>::Unload()
		{
			m_isLoaded = false;
			delete m_resourceObject;
			m_resourceObject = NULL;
		}
	}
}