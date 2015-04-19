///////////////////////////////////////////////////
// File:		TextureOpenGL.h
// Date:		13th December 2013
// Authors:		Matt Phillips
// Description:	OpenGL texture implementation
///////////////////////////////////////////////////

#pragma once

#include "renderer/Texture.h"

#ifdef ION_PLATFORM_WINDOWS
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL/SDL.h>
#include <SDLImage/SDL_image.h>

#include <string>

namespace ion
{
	namespace render
	{
		class TextureOpenGL : public Texture
		{
		public:
			TextureOpenGL();
			virtual ~TextureOpenGL();

			virtual bool Load(u32 width, u32 height, Format format, BitsPerPixel bitsPerPixel, const u8* data);
			GLuint GetTextureId() const;

			virtual void SetMinifyFilter(Filter filter);
			virtual void SetMagnifyFilter(Filter filter);
			virtual void SetWrapping(Wrapping wrapping);

		protected:
			virtual bool Load();
			virtual void Unload();
			static int GetOpenGLMode(Format format, BitsPerPixel bitsPerPixel);

			GLuint mGLTextureId;
		};
	}
}