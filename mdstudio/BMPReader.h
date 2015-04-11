///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
//
// Reader indexed BMP files
///////////////////////////////////////////////////////

#include <string>

#include <core/Types.h>
#include <io/File.h>

#include "Palette.h"

class BMPReader
{
public:
	enum BitFormat
	{
		eUnspecified = 0,
		eMonochrome1Bit = 1,
		eIndexed16Colour = 4,
		eIndexed256Colour = 8
	};

	BMPReader();
	~BMPReader();

	bool Read(const std::string& filename);

	int GetWidth() const;
	int GetHeight() const;

	Colour GetPixel(int x, int y) const;

private:

#pragma pack(push, 2)
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
		u16 numPlanes;
		u16 bitFormat;
		u32 compressionFlags;
		u32 dataSizeBytes;
		u32 resolutionX;
		u32 resolutionY;
		u32 numUsedColours;
		u32 importantColourCount;
	};
#pragma pack(pop)

	struct RGBQuad
	{
		u8 b;
		u8 g;
		u8 r;
		u8 reserved;
	};

	int m_width;
	int m_height;
	u8 m_bitsPerPixel;

	//Palette
	RGBQuad* m_palette;
	u32 m_paletteSize;

	//Pixel data
	u8* m_data;
	u32 m_dataSize;
};