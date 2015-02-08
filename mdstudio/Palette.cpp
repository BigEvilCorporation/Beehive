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
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
}

Colour::Colour(float red, float green, float blue)
{
	r = red;
	g = green;
	b = blue;
}

u16 Colour::ToVDPFormat() const
{
	return 0;
}

Palette::Palette()
{
	ion::memory::MemSet(m_colours, 0, sizeof(Colour) * coloursPerPalette);
}

void Palette::SetColour(int colourIdx, const Colour& colour)
{
	ion::debug::Assert(colourIdx < coloursPerPalette, "Out of range");
	m_colours[colourIdx] = colour;
}

const Colour& Palette::GetColour(int colourIdx) const
{
	ion::debug::Assert(colourIdx < coloursPerPalette, "Out of range");
	return m_colours[colourIdx];
}

void Palette::Serialise(ion::io::Archive& archive)
{
}

void Palette::Export(std::stringstream& outputText)
{
}