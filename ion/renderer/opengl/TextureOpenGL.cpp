///////////////////////////////////////////////////
// File:		TextureOpenGL.cpp
// Date:		13th December 2013
// Authors:		Matt Phillips
// Description:	OpenGL texture implementation
///////////////////////////////////////////////////

#pragma once

#include "core/debug/Debug.h"
#include "core/memory/Memory.h"
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
			m_glTextureId = 0;
		}

		TextureOpenGL::~TextureOpenGL()
		{

		}

		bool TextureOpenGL::Load()
		{
			//Load image onto a new SDL surface
			SDL_Surface* sdlSurface = IMG_Load(m_imageFilename.c_str());

			if(sdlSurface)
			{
				//Get dimensions
				m_width = sdlSurface->w;
				m_height = sdlSurface->h;

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
				Load(m_width, m_height, format, eRGBA_DXT5, BitsPerPixel(sdlSurface->format->BytesPerPixel * 8), true, (const u8*)sdlSurface->pixels);

				//Free SDL surface
				SDL_FreeSurface(sdlSurface);
			}

			return m_glTextureId != 0;
		}

		bool TextureOpenGL::Load(u32 width, u32 height, Format sourceFormat, Format destFormat, BitsPerPixel bitsPerPixel, bool generateMipmaps, const u8* data)
		{
			//Destroy existing texture
			if(m_glTextureId)
			{
				Unload();
			}

			//Generate texture
			glGenTextures(1, &m_glTextureId);

			debug::Assert(m_glTextureId != 0, "Could not create OpenGL texture");

			//Bind the texture
			glBindTexture(GL_TEXTURE_2D, m_glTextureId);

			//Generate mipmaps
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, generateMipmaps ? GL_TRUE : GL_FALSE);

			//Set default filters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, generateMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

			//Get GL format
			m_glFormat = GetOpenGLMode(destFormat, bitsPerPixel);
			int GLSourceFormat = GetOpenGLMode(sourceFormat, bitsPerPixel);

			//Copy all pixels in SDL surface to GL texture
			glTexImage2D(GL_TEXTURE_2D, 0, m_glFormat, width, height, 0, GLSourceFormat, GL_UNSIGNED_BYTE, data);

			//Unbind texture
			glBindTexture(GL_TEXTURE_2D, 0);

			//Set dimensions
			m_width = width;
			m_height = height;

			return m_glTextureId != 0;
		}

		void TextureOpenGL::Unload()
		{
			if(m_glTextureId)
			{
				glDeleteTextures(1, &m_glTextureId);
				m_glTextureId = 0;
				m_width = 0;
				m_height = 0;
			}
		}

		GLuint TextureOpenGL::GetTextureId() const
		{
			return m_glTextureId;
		}

		void TextureOpenGL::SetPixel(const ion::Vector2i& position, const Colour& colour)
		{
			ion::debug::Assert(m_glFormat == GL_RGB || m_glFormat == GL_RGBA, "TextureOpenGL::SetPixel() - Only supported for 24 bit texture formats");

			u8 data[4];

			data[0] = (int)(colour.r * 255);
			data[1] = (int)(colour.g * 255);
			data[2] = (int)(colour.b * 255);
			data[3] = (int)(colour.a * 255);

			glBindTexture(GL_TEXTURE_2D, m_glTextureId);
			glTexSubImage2D(GL_TEXTURE_2D, 0, position.x, position.y, 1, 1, m_glFormat, GL_UNSIGNED_BYTE, data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void TextureOpenGL::GetPixels(const ion::Vector2i& position, const ion::Vector2i& size, Format format, BitsPerPixel bitsPerPixel, u8* data) const
		{
			const u32 bytesPerPixel = (u32)bitsPerPixel / 8;
			const u32 bufferSize = size.x * size.y * bytesPerPixel;
			u8* buffer = new u8[bufferSize];
			u8* dest = data;
			u32 lineSize = size.x * bytesPerPixel;

			glBindTexture(GL_TEXTURE_2D, m_glTextureId);
			glGetTexImage(GL_TEXTURE_2D, 0, GetOpenGLMode(format, bitsPerPixel), GL_UNSIGNED_BYTE, buffer);
			glBindTexture(GL_TEXTURE_2D, 0);

			for(int y = position.y; y < size.y; y++)
			{
				u8* source = buffer + (((size.x * y) + position.x) * bytesPerPixel);
				
				memory::MemCopy(dest, source, lineSize);
				dest += lineSize;
			}
		}

		void TextureOpenGL::SetMinifyFilter(Filter filter)
		{
			glBindTexture(GL_TEXTURE_2D, m_glTextureId);

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
		}

		void TextureOpenGL::SetMagnifyFilter(Filter filter)
		{
			glBindTexture(GL_TEXTURE_2D, m_glTextureId);

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
		}

		void TextureOpenGL::SetWrapping(Wrapping wrapping)
		{
			glBindTexture(GL_TEXTURE_2D, m_glTextureId);

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
		}

		int TextureOpenGL::GetOpenGLMode(Format format, BitsPerPixel bitsPerPixel)
		{
			switch(bitsPerPixel)
			{
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
				case eRGBA_DXT5: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
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