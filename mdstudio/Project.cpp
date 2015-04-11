///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include <wx/msgdlg.h>

#include "Project.h"
#include "BMPReader.h"

Project::Project()
{
	m_paintTile = 0;
	m_eraseTile = 0;
	m_mapInvalidated = true;
}

void Project::Clear()
{
	for(int i = 0; i < numPalettes; i++)
	{
		m_palettes[i].Clear();
	}

	m_map.Clear();
}

void Project::SetPaintTile(TileId tile)
{
	m_paintTile = tile;
}

TileId Project::GetPaintTile() const
{
	return m_paintTile;
}

void Project::SetEraseTile(TileId tile)
{
	m_eraseTile = tile;
}

TileId Project::GetEraseTile() const
{
	return m_eraseTile;
}

bool Project::FindPalette(Colour* colours, PaletteId& paletteId) const
{
	//For each palette
	for(int paletteIdx = 0; paletteIdx < numPalettes; paletteIdx++)
	{
		const Palette& palette = m_palettes[paletteIdx];

		bool match = true;

		//For each pixel
		for(int i = 0; i < 8*8 && match; i++)
		{
			int colourIdx = 0;

			//Check if this pixel colour is contained in the palette
			if(!palette.GetNearestColourIdx(colours[i], Palette::eExact, colourIdx))
			{
				match = false;
			}
		}

		if(match)
		{
			paletteId = paletteIdx;
			return true;
		}
	}

	return false;
}

bool Project::ImportPalette(Colour* colours, PaletteId paletteId)
{
	//Get palette
	Palette& palette = m_palettes[paletteId];

	//Add first colour
	palette.SetColour(0, colours[0]);

	//Keep running colour count
	int numColours = 1;

	//Find/add remaining colours
	for(int i = 1; i < 8*8; i++)
	{
		int colourIdx = 0;
		if(!palette.GetNearestColourIdx(colours[i], Palette::eExact, colourIdx))
		{
			if(numColours == Palette::coloursPerPalette)
			{
				//Hit the colour limit
				return false;
			}

			//Add new colour
			palette.SetColour(numColours, colours[i]);
			numColours++;
		}
	}

	return true;
}

bool Project::ImportBitmap(const std::string& filename, u8 importFlags)
{
	if(importFlags & BMPImportReplaceAll)
	{
		//Clear map, tiles and palettes
		Clear();
	}

	//Read BMP
	BMPReader reader;
	if(reader.Read(filename))
	{
		if(reader.GetWidth() % 8 != 0 || reader.GetHeight() % 8 != 0)
		{
			if(wxMessageBox("Bitmap width/height is not multiple of 8, image will be truncated", "Warning", wxOK | wxCANCEL) == wxCANCEL)
			{
				return false;
			}
		}

		//Get width/height in tiles
		int tilesWidth = reader.GetWidth() / 8;
		int tilesHeight = reader.GetHeight() / 8;

		//Resize map
		m_map.Resize(tilesWidth, tilesHeight);

		int newPaletteCount = 0;

		//For all 8x8 tiles
		for(int tileX = 0; tileX < tilesWidth; tileX++)
		{
			for(int tileY = 0; tileY < tilesHeight; tileY++)
			{
				Colour colours[8*8];

				//Read pixel colours from bitmap
				for(int pixelX = 0; pixelX < 8; pixelX++)
				{
					for(int pixelY = 0; pixelY < 8; pixelY++)
					{
						int soucePixelX = (tileX * 8) + pixelX;
						int soucePixelY = (tileY * 8) + pixelY;
						colours[(pixelY * 8) + pixelX] = reader.GetPixel(soucePixelX, soucePixelY);
					}
				}

				//Find or create palette
				PaletteId paletteId = 0;
				if(!FindPalette(colours, paletteId))
				{
					//No existing palette found, create new
					if(newPaletteCount == numPalettes)
					{
						wxMessageBox("Exceeded palette count, bailing out", "Error", wxOK);
						return false;
					}

					if(!ImportPalette(colours, newPaletteCount))
					{
						wxMessageBox("Too many colours in tile, bailing out", "Error", wxOK);
						return false;
					}

					paletteId = newPaletteCount++;
				}

				//Get palette
				Palette& palette = m_palettes[paletteId];

				Tile tile;
				tile.SetPaletteId(paletteId);

				//Find pixel colours from palette
				for(int pixelX = 0; pixelX < 8; pixelX++)
				{
					for(int pixelY = 0; pixelY < 8; pixelY++)
					{
						for(int i = 0; i < Palette::coloursPerPalette; i++)
						{
							int colourIdx = 0;
							if(!palette.GetNearestColourIdx(colours[(pixelY * 8) + pixelX], Palette::eExact, colourIdx))
							{
								//Shouldn't reach here - palette should have been validated
								wxMessageBox("Error mapping colour indices", "Error", wxOK);
								return false;
							}

							tile.SetPixelColour(pixelX, pixelY, colourIdx);
						}
					}
				}

				
				TileId tileId = 0;

				TileId duplicateId = m_map.GetTileset().FindDuplicate(tile);
				if(duplicateId)
				{
					//Tile already exists
					tileId = duplicateId;
				}
				else
				{
					//Create new tile and copy
					tileId = m_map.GetTileset().AddTile();
					Tile* newTile = m_map.GetTileset().GetTile(tileId);
					*newTile = tile;
				}

				if(importFlags & BMPImportDrawToMap)
				{
					//Set in map
					m_map.SetTile(tileX, tileY, tileId);
				}

				//Invalidate map
				InvalidateMap(true);
			}
		}
	}
	else
	{
		wxMessageBox("Error reading bitmap", "Error", wxOK);
	}

	return true;
}