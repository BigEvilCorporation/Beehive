#include "FileReader.h"

namespace ion
{
	namespace audio
	{
		FileReader::FileReader(const char* filename)
		{
			mFilename = filename;
		}

		FileReader::~FileReader()
		{
		}
	}
}