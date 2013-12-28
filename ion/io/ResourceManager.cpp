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

		void ResourceManager::RequestLoad(Resource& resource)
		{
			mWorkerThread->PushJob(WorkerThread::Job(WorkerThread::Job::Load, resource));
		}

		void ResourceManager::RequestUnload(Resource& resource)
		{
			mWorkerThread->PushJob(WorkerThread::Job(WorkerThread::Job::Unload, resource));
		}

		void ResourceManager::WorkerThread::PushJob(Job& job)
		{
			mJobQueue.Push(job);
			mThreadEvent.Signal();
		}

		void ResourceManager::WorkerThread::Entry()
		{
			while(true)
			{
				//Wait for event
				mThreadEvent.Wait();

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
			}
		}
	}
}