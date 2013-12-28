/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Colour.h

	Colour class
*/

#pragma once

#include "io/Archive.h"

namespace ion
{
	class Colour
	{
	public:

		float r, g, b, a;

		Colour();
		Colour(float red, float green, float blue);
		Colour(float red, float green, float blue, float alpha);

		//TODO: Convertions - RGBA, hue/sat, etc...

		void Serialise(io::Archive& archive)
		{
			archive.Serialise(r);
			archive.Serialise(g);
			archive.Serialise(b);
			archive.Serialise(a);
		}
	};

	class ColourRGB
	{
	public:

		float r, g, b;

		ColourRGB();
		ColourRGB(float red, float green, float blue);

		void Serialise(io::Archive& archive)
		{
			archive.Serialise(r);
			archive.Serialise(g);
			archive.Serialise(b);
		}
	};
}