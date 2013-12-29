///////////////////////////////////////////////////
// File:		ResourceManager.h
// Date:		22nd December 2013
// Authors:		Matt Phillips
// Description:	Resource management
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include "core/Thread.h"
#include "core/Queue.h"
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

			template <class T> ResourceHandle<T> RequestLoad(const std::string& filename);
			void RequestUnload(Resource& resource);

			void WaitUntilEmpty();

		protected:
			class WorkerThread : public thread::Thread
			{
			public:

				struct Job
				{
					enum JobType { Load, Unload };

					Job() {}
					Job(JobType jobType, Resource& resource)
					{
						mJobType = jobType;
						mResource = &resource;
					}

					JobType mJobType;
					Resource* mResource;
				};

				void PushJob(Job& job);
				void WaitUntilEmpty();

			protected:
				virtual void Entry();

			private:
				static const int sJobQueueSize = 1024;
				Queue<Job, sJobQueueSize> mJobQueue;
				thread::Event mJobPushThreadEvent;
				thread::Event mQueueEmptyThreadEvent;
			};

			std::map<std::string, Resource*> mResourceMap;
			WorkerThread* mWorkerThread;
		};

		template <class T> ResourceHandle<T> ResourceManager::RequestLoad(const std::string& filename)
		{
			std::map<std::string, Resource*>::iterator it = mResourceMap.find(filename);

			if(it == mResourceMap.end())
			{
				std::pair<std::map<std::string, Resource*>::iterator, bool> result = mResourceMap.insert(std::pair<std::string, Resource*>(filename, new ResourceT<T>(*this, filename)));
				it = result.first;
			}

			mWorkerThread->PushJob(WorkerThread::Job(WorkerThread::Job::Load, *it->second));

			return ResourceHandle<T>((ResourceT<T>*)it->second);
		}
	}
}