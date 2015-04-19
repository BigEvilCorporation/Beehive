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

				Format format = eRGB;

				//Check colour format
				if(sdlSurface->format->BytesPerPixel == 4)
				{
					if (sdlSurface->format->Rmask == 0x000000ff)
						format = eRGBA;
					else
						format = eBGRA;
				}
				else if(sdlSurface->format->BytesPerPixel == 3)
				{
					if (sdlSurface->format->Rmask == 0x000000ff)
						format = eRGB;
					else
						format = eBGR;
				}
				
				//Load image to OpenGL texture
				Load(mWidth, mHeight, format, BitsPerPixel(sdlSurface->format->BytesPerPixel * 8), (const char*)sdlSurface->pixels);

				//Free SDL surface
				SDL_FreeSurface(sdlSurface);
			}

			return mGLTextureId != 0;
		}

		bool TextureOpenGL::Load(u32 width, u32 height, Format format, BitsPerPixel bitsPerPixel, const char* data)
		{
			//GL thread safety
			RendererOpenGL::LockGLContext();

			//Destroy existing texture
			if(mGLTextureId)
			{
				Unload();
			}

			//Generate texture
			glGenTextures(1, &mGLTextureId);

			debug::Assert(mGLTextureId != 0, "Could not create OpenGL texture");

			//Bind the texture
			glBindTexture(GL_TEXTURE_2D, mGLTextureId);

			//Generate mipmaps
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

			//Set default filters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			//Copy all pixels in SDL surface to GL texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, width, height, 0, GetOpenGLMode(format, bitsPerPixel), GL_UNSIGNED_BYTE, data);

			//Unbind texture
			glBindTexture(GL_TEXTURE_2D, 0);

			//Set dimensions
			mWidth = width;
			mHeight = height;

			//GL thread safety
			RendererOpenGL::UnlockGLContext();

			return mGLTextureId != 0;
		}

		void TextureOpenGL::Unload()
		{
			if(mGLTextureId)
			{
				glDeleteTextures(1, &mGLTextureId);
				mGLTextureId = 0;
				mWidth = 0;
				mHeight = 0;
			}
		}

		GLuint TextureOpenGL::GetTextureId() const
		{
			return mGLTextureId;
		}

		void TextureOpenGL::SetMinifyFilter(Filter filter)
		{
			RendererOpenGL::LockGLContext();
			glBindTexture(GL_TEXTURE_2D, mGLTextureId);

			int filterMode = 0;

			switch(filter)
			{
			case eFilterNearest:
				filterMode = GL_NEAREST;
				break;
			case eFilterLinear:
				filterMode = GL_LINEAR;
				break;
			case eFilterMipMapLinear:
				filterMode = GL_LINEAR_MIPMAP_LINEAR;
				break;
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);

			glBindTexture(GL_TEXTURE_2D, 0);
			RendererOpenGL::UnlockGLContext();
		}

		void TextureOpenGL::SetMagnifyFilter(Filter filter)
		{
			RendererOpenGL::LockGLContext();
			glBindTexture(GL_TEXTURE_2D, mGLTextureId);

			int filterMode = 0;

			switch(filter)
			{
			case eFilterNearest:
				filterMode = GL_NEAREST;
				break;
			case eFilterLinear:
				filterMode = GL_LINEAR;
				break;
			default:
				ion::debug::Error("Unsupported magnify filter mode");
				break;
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

			glBindTexture(GL_TEXTURE_2D, 0);
			RendererOpenGL::UnlockGLContext();
		}

		void TextureOpenGL::SetWrapping(Wrapping wrapping)
		{
			RendererOpenGL::LockGLContext();
			glBindTexture(GL_TEXTURE_2D, mGLTextureId);

			int wrapMode = 0;

			switch(wrapping)
			{
			case eWrapClamp:
				wrapMode = GL_CLAMP;
				break;
			case eWrapRepeat:
				wrapMode = GL_REPEAT;
				break;
			case eWrapMirror:
				wrapMode = GL_MIRRORED_REPEAT;
				break;
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

			glBindTexture(GL_TEXTURE_2D, 0);
			RendererOpenGL::UnlockGLContext();
		}

		int TextureOpenGL::GetOpenGLMode(Format format, BitsPerPixel bitsPerPixel)
		{
			switch(bitsPerPixel)
			{
			case eBPP4:
				switch(format)
				{
				case eRGB: return GL_RGB4;
				case eRGBA: return GL_RGBA4;
				default: break;
				}
				break;
			case eBPP8:
				switch(format)
				{
				case eRGB: return GL_RGB8;
				case eRGBA: return GL_RGBA8;
				default: break;
				}
				break;
			case eBPP16:
				switch(format)
				{
				case eRGB: return GL_RGB16;
				case eRGBA: return GL_RGBA16;
				default: break;
				}
				break;
			case eBPP24:
				switch(format)
				{
				case eRGB: return GL_RGB;
				case eRGBA: return GL_RGBA;
				default: break;
				}
				break;
			case eBPP32:
				switch(format)
				{
				case eRGB: return GL_RGB32I;
				case eRGBA: return GL_RGBA32I;
				default: break;
				}
				break;

			default:
				ion::debug::Error("Unsupported texture data format");
				break;
			}

			return 0;
		}
	}
}