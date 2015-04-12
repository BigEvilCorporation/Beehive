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
	Colour(u8 red, u8 green, u8 blue);
	bool operator == (const Colour& rhs) const;
	bool operator != (const Colour& rhs) const { return !(*this == rhs); }

	u16 ToVDPFormat() const;

	u8 r, g, b;
};

typedef u8 PaletteId;

class Palette
{
public:
	static const int coloursPerPalette = 16;

	enum NearestColourAlgo
	{
		eExact = 0
	};

	Palette();

	void Clear();

	int AddColour(const Colour& colour);
	void SetColour(int colourIdx, const Colour& colour);
	const Colour& GetColour(int colourIdx) const;
	const bool GetNearestColourIdx(const Colour& colour, NearestColourAlgo algorithm, int& colourIdx) const;

	int GetNumColours() const;

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& outputText);

private:
	Colour m_colours[coloursPerPalette];
	int m_numColours;
};