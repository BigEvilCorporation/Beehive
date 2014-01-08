///////////////////////////////////////////////////
// File:		Texture.h
// Date:		19th December 2011
// Authors:		Matt Phillips
// Description:	Texture map
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include "io/Archive.h"

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

			u32 GetWidth() const;
			u32 GetHeight() const;

			void SetImageFilename(const std::string& filename);

			//Serialise
			static void RegisterSerialiseType(io::Archive& archive);
			void Serialise(io::Archive& archive);

		protected:
			Texture();

			virtual bool Load() { return false; }

			u32 mWidth;
			u32 mHeight;

			std::string mImageFilename;
		};
	}
}