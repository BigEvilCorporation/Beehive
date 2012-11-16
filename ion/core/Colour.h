/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Colour.h

	Colour class
*/

#pragma once

namespace ion
{
	class Colour
	{
	public:

		float r, g, b, a;

		Colour();
		Colour(float R, float G, float B);
		Colour(float R, float G, float B, float A);

		//TODO: Convertions - RGBA, hue/sat, etc...
	};

	class ColourRGB
	{
	public:

		float r, g, b;

		ColourRGB();
		ColourRGB(float R, float G, float B);
	};
}