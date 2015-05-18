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

	Colour::Colour(u8 red, u8 green, u8 blue)
	{
		r = (float)red / 255.0f;
		g = (float)green / 255.0f;
		b = (float)blue / 255.0f;
		a = 1.0f;
	}

	Colour::Colour(u8 red, u8 green, u8 blue, u8 alpha)
	{
		r = (float)red / 255.0f;
		g = (float)green / 255.0f;
		b = (float)blue / 255.0f;
		a = (float)alpha / 255.0f;
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