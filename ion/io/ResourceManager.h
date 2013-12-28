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

namespace ion
{
	namespace io
	{
		class ResourceManager
		{
		public:
			ResourceManager();
			~ResourceManager();

			void RequestLoad(Resource& resource);
			void RequestUnload(Resource& resource);

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

				WorkerThread();

				void PushJob(Job& job);

			protected:
				virtual void Entry();

			private:
				static const int sJobQueueSize = 1024;
				Queue<Job, sJobQueueSize> mJobQueue;
				thread::Event mThreadEvent;
			};

			WorkerThread* mWorkerThread;
		};
	}
}