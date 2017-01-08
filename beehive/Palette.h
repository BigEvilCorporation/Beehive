///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#pragma once

#include <sstream>
#include <iomanip>
#include <vector>
#include <core/Types.h>
#include <io/Archive.h>

struct Colour
{
	Colour();
	Colour(u8 red, u8 green, u8 blue);

	inline u8 GetRed() const;
	inline u8 GetGreen() const;
	inline u8 GetBlue() const;

	inline void SetRed(u8 r);
	inline void SetGreen(u8 g);
	inline void SetBlue(u8 b);

	inline bool operator == (const Colour& rhs) const;
	inline bool operator != (const Colour& rhs) const { return !(*this == rhs); }

	u16 ToVDPFormat() const;
	void Serialise(ion::io::Archive& archive)
	{
		archive.Serialise(rgb);
	}

	u32 rgb;
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
