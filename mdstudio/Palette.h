///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <sstream>
#include <iomanip>
#include <core/Types.h>
#include <io/Archive.h>

struct Colour
{
	Colour();
	Colour(u8 red, u8 green, u8 blue);
	bool operator == (const Colour& rhs) const;
	bool operator != (const Colour& rhs) const { return !(*this == rhs); }

	u16 ToVDPFormat() const;
	void Serialise(ion::io::Archive& archive)
	{
		archive.Serialise(r);
		archive.Serialise(g);
		archive.Serialise(b);
	}

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
	bool IsColourUsed(int colourIdx) const;
	u16 GetUsedColourMask() const;

	const Colour& GetColour(int colourIdx) const;
	bool GetNearestColourIdx(const Colour& colour, NearestColourAlgo algorithm, int& colourIdx) const;

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& stream) const;

private:
	void MarkUsed(int colourIdx);

	std::vector<Colour> m_colours;
	u16 m_usedColours;
};
