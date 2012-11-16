/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Colour.cpp

	Colour class
*/

#include "Colour.h"

namespace ion
{
	Colour::Colour()
	{
		r = g = b = a = 1.0f;
	}

	Colour::Colour(float R, float G, float B)
	{
		r = R;
		g = G;
		b = B;
		a = 1.0f;
	}

	Colour::Colour(float R, float G, float B, float A)
	{
		r = R;
		g = G;
		b = B;
		a = A;
	}

	ColourRGB::ColourRGB()
	{
		r = g = b = 1.0f;
	}

	ColourRGB::ColourRGB(float R, float G, float B)
	{
		r = R;
		g = G;
		b = B;
	}
}