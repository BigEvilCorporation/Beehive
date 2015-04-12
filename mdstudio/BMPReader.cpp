///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
//
// Reader for indexed 16 colour, 4bpp BMP files
///////////////////////////////////////////////////////

#include "BMPReader.h"

BMPReader::BMPReader()
{
	m_width = 0;
	m_height = 0;
	m_bitsPerPixel = 0;
	m_data = NULL;
	m_palette = NULL;
	m_dataSize = 0;
	m_paletteSize = 0;
}

BMPReader::~BMPReader()
{
	if(m_data)
		delete m_data;
}

bool BMPReader::Read(const std::string& filename)
{
	ion::io::File file(filename);
	if(file.IsOpen())
	{
		//Read file header
		FileHeader fileHeader = {0};
		if(file.Read(&fileHeader, sizeof(FileHeader)))
		{
			//Read BMP header
			BMPHeader bmpHeader = {0};
			if(file.Read(&bmpHeader, sizeof(BMPHeader)))
			{
				//Sanity check used colours
				if(bmpHeader.numUsedColours > 0)
				{
					//Sanity check size
					if(bmpHeader.imageWidth > 0 || bmpHeader.imageHeight > 0)
					{
						//Set width/height/data size
						m_width = bmpHeader.imageWidth;
						m_height = bmpHeader.imageHeight;
						m_dataSize = bmpHeader.dataSizeBytes;

						//Set bits per pixel
						if(bmpHeader.bitFormat == eIndexed16Colour)
						{
							m_bitsPerPixel = 4;
						}

						if(m_bitsPerPixel > 0)
						{
							//Seek to palette pos
							u64 paletteDataPos = sizeof(FileHeader) + bmpHeader.headerSize;
							if(file.Seek(paletteDataPos, ion::io::File::Start) == paletteDataPos)
							{
								//Calc palette size and alloc
								m_paletteSize = (1 << m_bitsPerPixel);
								m_palette = new RGBQuad[m_paletteSize];

								//Read palette
								u64 paletteSizeBytes = m_paletteSize * sizeof(RGBQuad);
								if(file.Read(m_palette, paletteSizeBytes) == paletteSizeBytes)
								{
									//Seek to data pos
									if(file.Seek(fileHeader.dataOffset, ion::io::File::Start) == fileHeader.dataOffset)
									{
										//Alloc pixel data buffer
										m_data = new u8[m_dataSize];

										//Read pixel data
										if(file.Read(m_data, m_dataSize) == m_dataSize)
										{
											//Success
											return true;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if(m_data)
	{
		delete m_data;
		m_data = NULL;
	}

	if(m_palette)
	{
		delete m_palette;
		m_palette = NULL;
	}

	return false;
}

int BMPReader::GetWidth() const
{
	return m_width;
}

int BMPReader::GetHeight() const
{
	return m_height;
}

Colour BMPReader::GetPixel(int x, int y) const
{
	ion::debug::Assert(x < m_width && y < m_height, "Out of range");

	u32 pixelOffset = (((m_height - y - 1) * m_width) + x);
	u32 byteOffset = (pixelOffset & 0xFFFFFFFE) / 2;
	u8 colourIndex = m_data[byteOffset];

	if (pixelOffset & 1)
		colourIndex &= 0x0F;
	else
		colourIndex = colourIndex >> 4;

	ion::debug::Assert(colourIndex < m_paletteSize, "Out of range");

	RGBQuad rgbQuad = m_palette[colourIndex];

	Colour colour;
	colour.r = rgbQuad.r;
	colour.g = rgbQuad.g;
	colour.b = rgbQuad.b;
	return colour;
}
