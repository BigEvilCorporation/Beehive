///////////////////////////////////////////////////
// File:		TexCoord.h
// Date:		14th August 2011
// Authors:		Matt Phillips
// Description:	Texture coordinates
///////////////////////////////////////////////////

#include "TexCoord.h"

namespace ion
{
	namespace renderer
	{
		TexCoord::TexCoord()
		{
			x = 0.0f;
			y = 0.0f;
		}

		TexCoord::TexCoord(float X, float Y)
		{
			x = X;
			y = Y;
		}
	}
}