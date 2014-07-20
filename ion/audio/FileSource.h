#include <audio/Buffer.h>
#include <audio/FileReader.h>
#include <audio/Source.h>

namespace ion
{
	namespace audio
	{
		class FileSource : public Source
		{
		public:
			FileSource(FeedType feedType, FileReader& reader);
			virtual ~FileSource();

			//Load whole file
			bool Load();

			//Open/close for streaming
			virtual bool OpenStream();
			virtual void CloseStream();

			//Get next buffer
			virtual void RequestBuffer(SourceCallback& callback);

		protected:
			FileReader& mFileReader;
			Buffer* mBuffer;
		};
	}
}