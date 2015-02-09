///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include <string>

#include <core/Types.h>
#include <io/File.h>

class BMPReader
{
public:
	BMPReader();
	~BMPReader();

	bool Read(const std::string& filename);

	int GetWidth() const;
	int GetHeight() const;

private:
	int m_width;
	int m_height;
	u8* m_data;

#pragma pack(push, 1)
	struct FileHeader
	{
		u16 fileType;
		u32 fileSize;
		u16 unused1;
		u16 unused2;
		u32 dataOffset;
	};

	struct BMPHeader
	{
		u32 headerSize;
		u32 imageWidth;
		u32 imageHeight;
		u8 numPlanes;
		u16 bitsPerPixel;
		u32 compressionFlags;
		u32 dataSizeBytes;
		u32 resolutionX;
		u32 resolutionY;
		u32 colourTableSize;
		u32 importantColourCount;
	};
#pragma pack(pop)
};