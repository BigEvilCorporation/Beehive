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

			ResourceT(ResourceManager& resourceManager, const std::string& filename, T* resourceObject)
				: Resource(resourceManager, filename)
			{
				mResourceObject = resourceObject;
				mIsLoaded = true;
			}

			virtual ~ResourceT() {}

			virtual bool Load();
			virtual void Unload();
			virtual bool IsLoaded() { return mIsLoaded; }

			T* Get() const { return mResourceObject; }

		protected:
			T* mResourceObject;
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
			std::string fullPath = mResourceManager.GetResourceDirectory<T>();
			fullPath += "/";
			fullPath += mFilename;

			//Create and open scene file stream for reading
			File file(fullPath, File::OpenRead);

			if(file.IsOpen())
			{
				//Create archive for serialising in
				Archive archiveIn(file, Archive::In, &mResourceManager);

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
	}
}