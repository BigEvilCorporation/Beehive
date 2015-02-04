#include "TileRendering.h"

namespace tilerendering
{
	void PaintTileToDc(int x, int y, const Tile& tile, wxDC& dc)
	{
		int startPixelX = x * 8;
		int startPixelY = y * 8;

		for(int tileX = 0; tileX < 8; tileX++)
		{
			for(int tileY = 0; tileY < 8; tileY++)
			{
				u8 colourIdx = tile.GetPixelColour(tileX, tileY);

				//TODO: Convert to MD colour
				if(colourIdx == 0)
					dc.SetPen(*wxGREEN_PEN);
				else
					dc.SetPen(*wxBLUE_PEN);

				dc.DrawPoint(startPixelX + tileX, startPixelY + tileY);
			}
		}
	}
}