///////////////////////////////////////////////////
// File:		TexCoord.h
// Date:		14th August 2011
// Authors:		Matt Phillips
// Description:	Texture coordinates
///////////////////////////////////////////////////

#pragma once

namespace ion
{
	namespace renderer
	{
		class TexCoord
		{
		public:
			TexCoord();
			TexCoord(float X, float Y);

			float x;
			float y;
		};
	}
}