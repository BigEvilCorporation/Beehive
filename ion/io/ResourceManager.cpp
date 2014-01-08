///////////////////////////////////////////////////
// File:		ResourceManager.cpp
// Date:		22nd December 2013
// Authors:		Matt Phillips
// Description:	Resource management
///////////////////////////////////////////////////

#include "io/ResourceManager.h"
#include "core/thread/Atomic.h"

namespace ion
{
	namespace io
	{
		ResourceManager::ResourceManager()
		{
			mWorkerThread = new WorkerThread();
		}

		ResourceManager::~ResourceManager()
		{
			delete mWorkerThread;
		}

		u32 ResourceManager::GetNumResourcesWaiting() const
		{
			return mWorkerThread->GetNumJobsInQueue();
		}

		void ResourceManager::RequestLoad(Resource& resource)
		{
			mWorkerThread->PushJob(WorkerThread::Job(WorkerThread::Job::Load, resource));
		}

		void ResourceManager::RequestUnload(Resource& resource)
		{
			mWorkerThread->PushJob(WorkerThread::Job(WorkerThread::Job::Unload, resource));
		}

		ResourceManager::WorkerThread::WorkerThread()
			: mJobQueueSemaphore(sJobQueueSize)
		{
			mNumJobsInQueue = 0;
		}

		ResourceManager::WorkerThread::~WorkerThread()
		{
			Job shutdownJob;
			shutdownJob.mJobType = Job::Shutdown;
			PushJob(shutdownJob);
		}

		void ResourceManager::WorkerThread::PushJob(Job& job)
		{
			mJobQueue.Push(job);
			thread::atomic::Increment(mNumJobsInQueue);
			mJobQueueSemaphore.Signal();
		}

		u32 ResourceManager::WorkerThread::GetNumJobsInQueue() const
		{
			return mNumJobsInQueue;
		}

		void ResourceManager::WorkerThread::Entry()
		{
			while(true)
			{
				//Wait for job
				mJobQueueSemaphore.Wait();

				//Pop job from queue
				Job job = mJobQueue.Pop();

				if(job.mJobType == ResourceManager::WorkerThread::Job::Load)
				{
					//Load resource
					job.mResource->Load();
				}
				else if(job.mJobType == ResourceManager::WorkerThread::Job::Unload)
				{
					//Unload resource
					job.mResource->Unload();
				}

				thread::atomic::Decrement(mNumJobsInQueue);
			}
		}
	}
}