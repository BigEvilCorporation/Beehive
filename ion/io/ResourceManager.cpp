///////////////////////////////////////////////////
// File:		ResourceManager.cpp
// Date:		22nd December 2013
// Authors:		Matt Phillips
// Description:	Resource management
///////////////////////////////////////////////////

#include "io/ResourceManager.h"

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

		void ResourceManager::RequestUnload(Resource& resource)
		{
			mWorkerThread->PushJob(WorkerThread::Job(WorkerThread::Job::Unload, resource));
		}

		void ResourceManager::WaitUntilEmpty()
		{
			mWorkerThread->WaitUntilEmpty();
		}

		void ResourceManager::WorkerThread::PushJob(Job& job)
		{
			mJobQueue.Push(job);
			mJobPushThreadEvent.Signal();
		}

		void ResourceManager::WorkerThread::WaitUntilEmpty()
		{
			if(!mJobQueue.IsEmpty())
			{
				mQueueEmptyThreadEvent.Wait();
			}
		}

		void ResourceManager::WorkerThread::Entry()
		{
			while(true)
			{
				//Wait for event
				mJobPushThreadEvent.Wait();

				//Pop job from queue
				Job job = mJobQueue.Pop();

				if(job.mJobType == ResourceManager::WorkerThread::Job::Load)
				{
					//Load resource
					job.mResource->Load();
				}
				else
				{
					//Unload resource
					job.mResource->Unload();
				}

				if(mJobQueue.IsEmpty())
				{
					mQueueEmptyThreadEvent.Signal();
				}
			}
		}
	}
}