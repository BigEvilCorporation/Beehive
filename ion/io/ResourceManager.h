///////////////////////////////////////////////////
// File:		ResourceManager.h
// Date:		22nd December 2013
// Authors:		Matt Phillips
// Description:	Resource management
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include "core/thread/Thread.h"
#include "core/thread/Semaphore.h"
#include "core/containers/Queue.h"
#include "io/Resource.h"

#include <map>

namespace ion
{
	namespace io
	{
		class ResourceManager
		{
		public:
			ResourceManager();
			~ResourceManager();

			//Set/get resource directory
			template <class T> void SetResourceDirectory(const std::string& directory);
			template <class T> std::string GetResourceDirectory() const;

			//Get resource handle
			template <class T> ResourceHandle<T> GetResource(const std::string& filename);

			//Get number of resources in thread queue
			u32 GetNumResourcesWaiting() const;

		protected:

			//Add load/unload requests to thread queue
			void RequestLoad(Resource& resource);
			void RequestUnload(Resource& resource);

			class WorkerThread : public thread::Thread
			{
			public:

				struct Job
				{
					enum JobType { Load, Unload, Shutdown };

					Job() {}
					Job(JobType jobType, Resource& resource)
					{
						mJobType = jobType;
						mResource = &resource;
					}

					JobType mJobType;
					Resource* mResource;
				};

				WorkerThread();
				~WorkerThread();
				void PushJob(Job& job);
				u32 GetNumJobsInQueue() const;

			protected:
				virtual void Entry();

			private:
				static const int sJobQueueSize = 1024;
				Queue<Job, sJobQueueSize> mJobQueue;
				thread::Semaphore mJobQueueSemaphore;
				u32 mNumJobsInQueue;
			};


			//Directories
			std::map<std::string, std::string> mDirectoryMap;

			//Resources
			std::map<std::string, Resource*> mResourceMap;

			//Worker thread
			WorkerThread* mWorkerThread;

			friend class Resource;
		};

		template <class T> void ResourceManager::SetResourceDirectory(const std::string& directory)
		{
			std::string typeName = typeid(T).name();
			std::map<std::string, std::string>::iterator it = mDirectoryMap.find(typeName);

			if(it == mDirectoryMap.end())
			{
				std::pair<std::map<std::string, std::string>::iterator, bool> result = mDirectoryMap.insert(std::pair<std::string, std::string>(typeName, directory));
				it = result.first;
			}

			it->second = directory;
		}

		template <class T> std::string ResourceManager::GetResourceDirectory() const
		{
			std::string directory = "";

			std::string typeName = typeid(T).name();
			std::map<std::string, std::string>::const_iterator it = mDirectoryMap.find(typeName);

			if(it != mDirectoryMap.end())
			{
				directory = it->second;
			}

			return directory;
		}

		template <class T> ResourceHandle<T> ResourceManager::GetResource(const std::string& filename)
		{
			std::map<std::string, Resource*>::iterator it = mResourceMap.find(filename);

			if(it == mResourceMap.end())
			{
				std::pair<std::map<std::string, Resource*>::iterator, bool> result = mResourceMap.insert(std::pair<std::string, Resource*>(filename, new ResourceT<T>(*this, filename)));
				it = result.first;
			}

			return ResourceHandle<T>((ResourceT<T>*)it->second);
		}
	}
}