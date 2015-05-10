///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "TileRendering.h"

TileRenderer::TTileCacheMap TileRenderer::s_tileCache;

void TileRenderer::PaintTileToDc(int x, int y, const Tile& tile, const Palette& palette, u32 tileFlags, wxMemoryDC& dc)
{
	TTileCacheMap::iterator it = s_tileCache.find(tile.GetColourHash());
	if(it == s_tileCache.end())
	{
		//First use of tile, draw and cache bitmap
		it = s_tileCache.insert(std::make_pair(tile.GetColourHash(), wxBitmap(8, 8))).first;

		wxMemoryDC cacheDC(it->second);

		wxPen pen;
		pen.SetWidth(1);

		for(int tileX = 0; tileX < 8; tileX++)
		{
			for(int tileY = 0; tileY < 8; tileY++)
			{
				u8 colourIdx = tile.GetPixelColour(tileX, tileY);

				const Colour& colour = palette.GetColour(colourIdx);
				pen.SetColour(wxColour(colour.r, colour.g, colour.b, 1.0f));
				cacheDC.SetPen(pen);
				cacheDC.DrawPoint(tileX, tileY);
			}
		}
	}

	wxRect sourceRect(0, 0, 8, 8);
	wxRect destRect(x * 8, y * 8, 8, 8);
	wxMemoryDC cacheDC(it->second);

	//Blit to destination dc
	if(tileFlags & Map::eFlipX || tileFlags & Map::eFlipY)
	{
		if(tileFlags & Map::eFlipX)
		{
			//sourceRect.x = 8;
			//sourceRect.width = -1;
		}

		if(tileFlags & Map::eFlipY)
		{
			//sourceRect.y = 8;
			//sourceRect.height = -1;
		}

		dc.StretchBlit(destRect.x, destRect.y, destRect.width, destRect.height, &cacheDC, sourceRect.x, sourceRect.y, sourceRect.width, sourceRect.height);
	}
	else
	{
		//Ni flipping, non-scaled blit is faster
		dc.Blit(destRect.x, destRect.y, destRect.width, destRect.height, &cacheDC, sourceRect.x, sourceRect.y);
	}
}
