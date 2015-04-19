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
			enum Format
			{
				eRGB,
				eBGR,
				eRGBA,
				eBGRA
			};

			enum BitsPerPixel
			{
				eBPP4 = 4,
				eBPP8 = 8,
				eBPP16 = 16,
				eBPP24 = 24,
				eBPP32 = 32
			};

			enum Filter
			{
				eFilterNearest,
				eFilterLinear,
				eFilterMipMapLinear
			};

			enum Wrapping
			{
				eWrapClamp,
				eWrapRepeat,
				eWrapMirror
			};

			static Texture* Create();
			static Texture* Create(u32 width, u32 height, Format format, BitsPerPixel bitsPerPixel, const u8* data);

			virtual ~Texture();

			u32 GetWidth() const;
			u32 GetHeight() const;

			virtual bool Load(u32 width, u32 height, Format format, BitsPerPixel bitsPerPixel, const u8* data) { return false; }
			void SetImageFilename(const std::string& filename);

			virtual void SetMinifyFilter(Filter filter) = 0;
			virtual void SetMagnifyFilter(Filter filter) = 0;
			virtual void SetWrapping(Wrapping wrapping) = 0;

			//Serialise
			static void RegisterSerialiseType(io::Archive& archive);
			void Serialise(io::Archive& archive);

		protected:
			Texture();
			Texture(u32 width, u32 height, Format format, BitsPerPixel bitsPerPixel, const u8* data);

			virtual bool Load() { return false; }
			virtual void Unload() { }

			u32 mWidth;
			u32 mHeight;

			std::string mImageFilename;
		};
	}
}