///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "BMPReader.h"

BMPReader::BMPReader()
{
	m_width = 0;
	m_height = 0;
	m_data = NULL;
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
				//Sanity check palette size
				if(bmpHeader.colourTableSize > 0)
				{
					//Sanity check size
					if(bmpHeader.imageWidth > 0 || bmpHeader.imageHeight > 0)
					{
						//Seek to palette pos

						//Read palette

						//Seek to data pos
						if(file.Seek(fileHeader.dataOffset, ion::io::File::Start))
						{
							//Alloc buffer
							m_data = new u8[bmpHeader.dataSizeBytes];

							//Read data
							if(file.Read(m_data, bmpHeader.dataSizeBytes))
							{
								//Done, set width/height/BPP//palette size
								m_width = bmpHeader.imageWidth;
								m_height = bmpHeader.imageHeight;
								m_bitsPerPixel = bmpHeader.bitsPerPixel;
								m_paletteSize = bmpHeader.colourTableSize;

								return true;
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

int BMPReader::GetPixelColourIdx(int x, int y) const
{
	ion::debug::Assert(x < m_width && y < m_height, "Out of range");
	int index = (y * m_width) + x;
	
}

Colour BMPReader::GetColour(int colourIdx) const
{
	Colour colour;
	colour.r = m_data[index+2];
	colour.g = m_data[index+1];
	colour.b = m_data[index];
	return colour;
}

int BMPReader::GetColourPaletteSize() const
{

}