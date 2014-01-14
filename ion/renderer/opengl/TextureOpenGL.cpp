///////////////////////////////////////////////////
// File:		TextureOpenGL.cpp
// Date:		13th December 2013
// Authors:		Matt Phillips
// Description:	OpenGL texture implementation
///////////////////////////////////////////////////

#pragma once

#include "core/debug/Debug.h"
#include "renderer/OpenGL/TextureOpenGL.h"
#include "renderer/OpenGL/RendererOpenGL.h"

namespace ion
{
	namespace render
	{
		Texture* Texture::Create()
		{
			return new TextureOpenGL();
		}

		void Texture::RegisterSerialiseType(io::Archive& archive)
		{
			archive.RegisterPointerTypeStrict<Texture, TextureOpenGL>();
		}

		TextureOpenGL::TextureOpenGL()
		{
			mGLTextureId = 0;
		}

		TextureOpenGL::~TextureOpenGL()
		{

		}

		bool TextureOpenGL::Load()
		{
			//Load image onto a new SDL surface
			SDL_Surface* sdlSurface = IMG_Load(mImageFilename.c_str());

			if(sdlSurface)
			{
				//Get dimensions
				mWidth = sdlSurface->w;
				mHeight = sdlSurface->h;

				int mode = 0;

				//Check colour format
				if(sdlSurface->format->BytesPerPixel == 4)
				{
					if (sdlSurface->format->Rmask == 0x000000ff)
						mode = GL_RGBA;
					else
						mode = GL_BGRA;
				}
				else if(sdlSurface->format->BytesPerPixel == 3)
				{
					if (sdlSurface->format->Rmask == 0x000000ff)
						mode = GL_RGB;
					else
						mode = GL_BGR;
				}
					
				if(mode != 0)
				{
					//GL thread safety
					RendererOpenGL::LockGLContext();

					//Generate texture
					glGenTextures(1, &mGLTextureId);

					debug::Assert(mGLTextureId != 0, "Could not create OpenGL texture");

					//Bind the texture
					glBindTexture(GL_TEXTURE_2D, mGLTextureId);

					//Texture filters
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
					glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 

					//Copy all pixels in SDL surface to GL texture
					glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, sdlSurface->w, sdlSurface->h, 0, mode, GL_UNSIGNED_BYTE, sdlSurface->pixels);

					//Unbind texture
					glBindTexture(GL_TEXTURE_2D, 0);

					//GL thread safety
					RendererOpenGL::UnlockGLContext();
				}

				//Free SDL surface
				SDL_FreeSurface(sdlSurface);
			}

			return mGLTextureId != 0;
		}

		GLuint TextureOpenGL::GetTextureId() const
		{
			return mGLTextureId;
		}
	}
}