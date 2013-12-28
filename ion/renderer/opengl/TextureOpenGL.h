///////////////////////////////////////////////////
// File:		TextureOpenGL.h
// Date:		13th December 2013
// Authors:		Matt Phillips
// Description:	OpenGL texture implementation
///////////////////////////////////////////////////

#pragma once

#include "renderer/Texture.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

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

			virtual bool Load(const std::string& filename);

			GLuint GetTextureId() const;

		protected:
			GLuint mGLTextureId;
		};
	}
}