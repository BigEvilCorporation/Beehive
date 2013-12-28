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

		Texture::~Texture()
		{

		}

		bool Texture::Load(const std::string& filename)
		{
			//TODO: generic texture load using image library
			return false;
		}

		bool Texture::GetWidth() const
		{
			return mWidth;
		}

		bool Texture::GetHeight() const
		{
			return mHeight;
		}
	}
}