#include <audio/Callback.h>
#include <core/containers/Queue.h>

namespace ion
{
	namespace audio
	{
		class Buffer;
		class StreamDesc;

		class Source
		{
			static const int sMaxBufferQueueSize = 8;

		public:
			enum FeedType
			{
				SingleBuffer,
				StreamingFeed
			};

			Source(FeedType feedType);
			virtual ~Source();

			virtual bool OpenStream() = 0;
			virtual void CloseStream() = 0;

			void Lock();
			void Unlock();

			const StreamDesc* GetStreamDesc() const;
			FeedType GetFeedType() const;

			virtual void RequestBuffer(SourceCallback& callback) = 0;

		protected:
			const StreamDesc* mStreamDesc;
			FeedType mFeedType;
			u32 mLockCount;
		};
	}
}