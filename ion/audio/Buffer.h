#include <core/Types.h>

namespace ion
{
	namespace audio
	{
		class Buffer
		{
		public:
			Buffer(u32 size);
			~Buffer();

			void Lock();
			void Unlock();

			void Put(const char* data, u32 size, u32 position);
			char* Get(u32 position) const;

			u32 GetSize() const;

		private:
			char* mData;
			u32 mSize;
			u32 mLockCount;
		};
	}
}