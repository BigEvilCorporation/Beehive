///////////////////////////////////////////////////
// File:		Texture.h
// Date:		19th December 2011
// Authors:		Matt Phillips
// Description:	Texture map
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"

#include <string>

namespace ion
{
	namespace render
	{
		class Texture
		{
		public:
			static Texture* Create();
			virtual ~Texture();

			virtual bool Load(const std::string& filename);

			bool GetWidth() const;
			bool GetHeight() const;

		protected:
			Texture();

			u32 mWidth;
			u32 mHeight;
		};
	}
}