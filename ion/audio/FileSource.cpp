#include "FileSource.h"

namespace ion
{
	namespace audio
	{
		FileSource::FileSource(FeedType feedType, FileReader& reader)
			: Source(feedType),
			mFileReader(reader)
		{
		}

		FileSource::~FileSource()
		{
			CloseStream();
		}

		bool FileSource::Load()
		{
			//Open file
			if(mFileReader.Open())
			{
				//Set stream desc
				mStreamDesc = &mFileReader.GetStreamDesc();

				//Alloc buffer
				mBuffer = new Buffer(mStreamDesc->GetEncodedSizeBytes());

				//Lock buffer while stream is open
				mBuffer->Lock();

				//Read data
				mFileReader.Read(mBuffer->Get(0), mStreamDesc->GetEncodedSizeBytes());

				//Close file
				mFileReader.Close();

				return true;
			}

			return false;
		}

		bool FileSource::OpenStream()
		{
			return false;
		}

		void FileSource::CloseStream()
		{
			if(mBuffer)
			{
				mBuffer->Unlock();
				delete mBuffer;
				mBuffer = NULL;
			}
		}

		void FileSource::RequestBuffer(SourceCallback& callback)
		{
			if(mFeedType == SingleBuffer)
			{
				callback.SubmitBuffer(*mBuffer);
			}
		}
	}
}