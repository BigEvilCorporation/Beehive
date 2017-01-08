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

#include "Palette.h"

#include <core/debug/Debug.h>
#include <core/memory/Memory.h>

Colour::Colour()
{
	rgb = 0;
}

Colour::Colour(u8 red, u8 green, u8 blue)
{
	SetRed(red);
	SetGreen(green);
	SetBlue(blue);
}

u8 Colour::GetRed() const
{
	return (rgb & 0x00FF0000) >> 16;
}

u8 Colour::GetGreen() const
{
	return (rgb & 0x0000FF00) >> 8;
}

u8 Colour::GetBlue() const
{
	return (rgb & 0x000000FF);
}

void Colour::SetRed(u8 r)
{
	rgb &= 0x0000FFFF;
	rgb |= (r << 16);
}

void Colour::SetGreen(u8 g)
{
	rgb &= 0x00FF00FF;
	rgb |= (g << 8);
}

void Colour::SetBlue(u8 b)
{
	rgb &= 0x00FFFF00;
	rgb |= b;
}

bool Colour::operator == (const Colour& rhs) const
{
	return rgb == rhs.rgb;
}

u16 Colour::ToVDPFormat() const
{
	//u8 rNybble = (u8)(((float)GetRed()	/ (float)0xFF) * (float)0xE);
	//u8 gNybble = (u8)(((float)GetGreen() / (float)0xFF) * (float)0xE);
	//u8 bNybble = (u8)(((float)GetBlue() / (float)0xFF) * (float)0xE);

	u8 rNybble = (u8)(GetRed() >> 5) << 1;
	u8 gNybble = (u8)(GetGreen() >> 5) << 1;
	u8 bNybble = (u8)(GetBlue() >> 5) << 1;

	u16 mdValue = bNybble << 8 | gNybble << 4 | rNybble;
	return mdValue;
}

Palette::Palette()
{
	m_colours.resize(coloursPerPalette);
	m_usedColours = 0;
}

void Palette::Clear()
{
	for(int i = 0; i < coloursPerPalette; i++)
	{
		m_colours[i] = Colour();
	}
}

bool Palette::IsColourUsed(int colourIdx) const
{
	return (m_usedColours & (1 << colourIdx)) != 0;
}

u16 Palette::GetUsedColourMask() const
{
	return m_usedColours;
}

void Palette::MarkUsed(int colourIdx)
{
	ion::debug::Assert(colourIdx < coloursPerPalette, "eOut of range");
	m_usedColours |= (1 << colourIdx);
}

int Palette::AddColour(const Colour& colour)
{
	int index = -1;

	for(int i = 0; i < coloursPerPalette && index == -1; i++)
	{
		if(!IsColourUsed(i))
		{
			index = i;
		}
	}

	if(index >= 0)
	{
		SetColour(index, colour);
	}

	return index;
}

void Palette::SetColour(int colourIdx, const Colour& colour)
{
	ion::debug::Assert(colourIdx < coloursPerPalette, "eOut of range");
	MarkUsed(colourIdx);
	m_colours[colourIdx] = colour;
}

const Colour& Palette::GetColour(int colourIdx) const
{
	ion::debug::Assert(IsColourUsed(colourIdx), "Colour not in use");
	return m_colours[colourIdx];
}

bool Palette::GetNearestColourIdx(const Colour& colour, NearestColourAlgo algorithm, int& colourIdx) const
{
	//if(algorithm == eExact)
	{
		for(int i = 0; i < coloursPerPalette; i++)
		{
			if(m_colours[i] == colour && IsColourUsed(i))
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
	archive.Serialise(m_colours, "colours");
	archive.Serialise(m_usedColours, "usedColours");
}

void Palette::Export(std::stringstream& stream) const
{
	stream << std::hex << std::setfill('0') << std::uppercase;

	for(int i = 0; i < coloursPerPalette; i++)
	{
		stream << "\tdc.w\t0x";

		u32 value = m_colours[i].ToVDPFormat();

		stream << std::setw(4) << (u32)m_colours[i].ToVDPFormat() << std::endl;
	}

	stream << std::dec;
}