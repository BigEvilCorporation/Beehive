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

	Colour::Colour(float red, float green, float blue)
	{
		r = red;
		g = green;
		b = blue;
		a = 1.0f;
	}

	Colour::Colour(float red, float green, float blue, float alpha)
	{
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	ColourRGB::ColourRGB()
	{
		r = g = b = 1.0f;
	}

	ColourRGB::ColourRGB(float red, float green, float blue)
	{
		r = red;
		g = green;
		b = blue;
	}
}