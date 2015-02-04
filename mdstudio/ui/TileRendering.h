#pragma once

#include <wx/dc.h>

#include "../Tile.h"

namespace tilerendering
{
	//Paint single tile to dc
	void PaintTileToDc(int x, int y, const Tile& tile, wxDC& dc);
}