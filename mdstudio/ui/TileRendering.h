///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <wx/bitmap.h>

#include <map>

#include "../Tile.h"
#include "../Palette.h"
#include "../Map.h"

class TileRenderer
{
public:
	//Paint single tile to dc
	static void PaintTileToDc(int x, int y, const Tile& tile, const Palette& palette, u32 tileFlags, wxMemoryDC& dc);

private:
	typedef std::map<u32, wxBitmap> TTileCacheMap;
	static TTileCacheMap s_tileCache;
};