///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// Reader for indexed 16 colour, 4bpp BMP files
///////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>

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

	enum FileType
	{
		eTypeBMP = 19778
	};

	enum Resolution
	{
		eResolutionDefault = 2835
	};

	BMPReader();
	~BMPReader();

	bool Read(const std::string& filename);
	bool Write(const std::string& filename);

	int GetWidth() const;
	int GetHeight() const;

	void SetDimensions(int width, int height);

	int GetPaletteSize() const;
	Colour GetPaletteEntry(int index) const;
	void SetPaletteEntry(int index, const Colour& colour);

	u8 GetColourIndex(int x, int y) const;
	void SetColourIndex(int x, int y, u8 index);
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
	std::vector<RGBQuad> m_palette;

	//Pixel data
	std::vector<u8> m_data;
};