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

			GLuint GetTextureId() const;

		protected:
			virtual bool Load();

			GLuint mGLTextureId;
		};
	}
}