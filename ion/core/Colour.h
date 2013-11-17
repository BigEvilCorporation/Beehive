/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Colour.h

	Colour class
*/

#pragma once

#include "../Core/Archive.h"

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

		void Serialise(serialise::Archive& archive)
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
		ColourRGB(float R, float G, float B);

		void Serialise(serialise::Archive& archive)
		{
			archive.Serialise(r);
			archive.Serialise(g);
			archive.Serialise(b);
		}
	};
}