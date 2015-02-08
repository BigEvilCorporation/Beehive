///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <sstream>
#include <core/Types.h>
#include <io/Archive.h>

struct Colour
{
	Colour();
	Colour(float red, float green, float blue);

	u16 ToVDPFormat() const;

	float r, g, b;
};

typedef u8 PaletteId;

class Palette
{
public:
	static const int coloursPerPalette = 16;

	Palette();

	void SetColour(int colourIdx, const Colour& colour);
	const Colour& GetColour(int colourIdx) const;

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& outputText);

private:
	Colour m_colours[coloursPerPalette];
};