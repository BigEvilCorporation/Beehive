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
				//Seek to data pos
				if(file.Seek(fileHeader.dataOffset, ion::io::File::Start))
				{
					//Alloc buffer
					m_data = new u8[bmpHeader.dataSizeBytes];

					//Read data
					if(file.Read(m_data, bmpHeader.dataSizeBytes))
					{
						//Done, set width/height
						m_width = bmpHeader.imageWidth;
						m_height = bmpHeader.imageHeight;

						return true;
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
