///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "Palette.h"

#include <core/debug/Debug.h>
#include <core/memory/Memory.h>

Colour::Colour()
{
	r = 0;
	g = 0;
	b = 0;
}

Colour::Colour(u8 red, u8 green, u8 blue)
{
	r = red;
	g = green;
	b = blue;
}

bool Colour::operator == (const Colour& rhs) const
{
	return ToVDPFormat() == rhs.ToVDPFormat();
}

u16 Colour::ToVDPFormat() const
{
	u8 rNybble = (u8)(((float)r / (float)0xFF) * (float)0xE);
	u8 gNybble = (u8)(((float)g / (float)0xFF) * (float)0xE);
	u8 bNybble = (u8)(((float)b / (float)0xFF) * (float)0xE);

	u16 mdValue = bNybble << 8 | gNybble << 4 | rNybble;
	return mdValue;
}

Palette::Palette()
{
	ion::memory::MemSet(m_colours, 0, sizeof(Colour) * coloursPerPalette);
	m_numColours = 0;
}

void Palette::Clear()
{
	for(int i = 0; i < coloursPerPalette; i++)
	{
		m_colours[i] = Colour();
	}
}

int Palette::GetNumColours() const
{
	return m_numColours;
}

int Palette::AddColour(const Colour& colour)
{
	int index = -1;

	if(m_numColours < coloursPerPalette)
	{
		index = m_numColours++;
		SetColour(index, colour);
	}

	return index;
}

void Palette::SetColour(int colourIdx, const Colour& colour)
{
	ion::debug::Assert(colourIdx < m_numColours, "Out of range");
	m_colours[colourIdx] = colour;
}

const Colour& Palette::GetColour(int colourIdx) const
{
	ion::debug::Assert(colourIdx < m_numColours, "Out of range");
	return m_colours[colourIdx];
}

const bool Palette::GetNearestColourIdx(const Colour& colour, NearestColourAlgo algorithm, int& colourIdx) const
{
	if(algorithm == eExact)
	{
		for(int i = 0; i < m_numColours; i++)
		{
			if(m_colours[i] == colour)
			{
				colourIdx = i;
				return true;
			}
		}
	}
	
	return false;
}

void Palette::Serialise(ion::io::Archive& archive)
{
}

void Palette::Export(std::stringstream& stream) const
{
	for(int i = 0; i < coloursPerPalette; i++)
	{
		stream << "\tdc.w\t0x";

		if(i < m_numColours)
		{
			char text[8];
			sprintf(text, "%03X", m_colours[i].ToVDPFormat());
			stream << text;
		}
		else
		{
			stream << "000";
		}

		stream << std::endl;
	}
}