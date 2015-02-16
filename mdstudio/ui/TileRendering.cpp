///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "TileRendering.h"

namespace tilerendering
{
	void PaintTileToDc(int x, int y, const Tile& tile, const Palette& palette, wxDC& dc)
	{
		int startPixelX = x * 8;
		int startPixelY = y * 8;

		wxPen pen;
		pen.SetWidth(1);

		for(int tileX = 0; tileX < 8; tileX++)
		{
			for(int tileY = 0; tileY < 8; tileY++)
			{
				u8 colourIdx = tile.GetPixelColour(tileX, tileY);

				const Colour& colour = palette.GetColour(colourIdx);
				pen.SetColour(wxColour(colour.r, colour.g, colour.b, 1.0f));
				dc.SetPen(pen);
				dc.DrawPoint(startPixelX + tileX, startPixelY + tileY);
			}
		}
	}
}