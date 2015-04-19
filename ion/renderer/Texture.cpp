///////////////////////////////////////////////////
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
			mWidth = 0;
			mHeight = 0;
		}

		Texture::Texture(u32 width, u32 height, Format format, BitsPerPixel bitsPerPixel, const u8* data)
		{
			Load(width, height, format, bitsPerPixel, data);
		}

		Texture::~Texture()
		{

		}

		void Texture::SetImageFilename(const std::string& filename)
		{
			mImageFilename = filename;
		}

		u32 Texture::GetWidth() const
		{
			return mWidth;
		}

		u32 Texture::GetHeight() const
		{
			return mHeight;
		}

		void Texture::Serialise(io::Archive& archive)
		{
			archive.Serialise(mImageFilename);

			if(archive.GetDirection() == io::Archive::In)
			{
				Load();
			}
		}
	}
}