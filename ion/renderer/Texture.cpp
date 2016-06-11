///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Texture.cpp
// Date:		19th December 2011
// Authors:		Matt Phillips
// Description:	Texture map
///////////////////////////////////////////////////s

#include "renderer/Texture.h"

namespace ion
{
	namespace render
	{
		Texture::Texture()
		{
			m_width = 0;
			m_height = 0;
		}

		Texture::Texture(u32 width, u32 height, Format sourceFormat, Format destFormat, BitsPerPixel bitsPerPixel, bool generateMipmaps, const u8* data)
		{
			Load(width, height, sourceFormat, destFormat, bitsPerPixel, generateMipmaps, data);
		}

		Texture::~Texture()
		{

		}

		void Texture::SetImageFilename(const std::string& filename)
		{
			m_imageFilename = filename;
		}

		u32 Texture::GetWidth() const
		{
			return m_width;
		}

		u32 Texture::GetHeight() const
		{
			return m_height;
		}

		void Texture::Serialise(io::Archive& archive)
		{
			archive.Serialise(m_imageFilename);

			if(archive.GetDirection() == io::Archive::eIn)
			{
				Load();
			}
		}
	}
}