///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <wx/dc.h>

#include "../Tile.h"
#include "../Palette.h"

namespace tilerendering
{
	//Paint single tile to dc
	void PaintTileToDc(int x, int y, const Tile& tile, const Palette& palette, wxDC& dc);
}