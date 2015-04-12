///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include <wx/msgdlg.h>
#include <set>

#include "Project.h"
#include "BMPReader.h"

Project::Project()
{
	m_paintTile = 0;
	m_eraseTile = 0;
	m_mapInvalidated = true;
	m_name = "untitled";
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

bool Project::FindPalette(Colour* pixels, PaletteId& paletteId, PaletteId& closestPalette, int& closestColourCount) const
{
	//Set of found colour idxs
	std::set<int> colourMatches;

	//For each palette
	for(int paletteIdx = 0; paletteIdx < numPalettes; paletteIdx++)
	{
		const Palette& palette = m_palettes[paletteIdx];

		bool match = true;
		colourMatches.clear();

		//For each pixel
		for(int i = 0; i < 8*8; i++)
		{
			int colourIdx = 0;

			//Check if this pixel colour is contained in the palette
			if(palette.GetNearestColourIdx(pixels[i], Palette::eExact, colourIdx))
			{
				colourMatches.insert(colourIdx);
			}
			else
			{
				match = false;
			}
		}

		if(match)
		{
			paletteId = paletteIdx;
			return true;
		}

		if(colourMatches.size() > closestColourCount)
		{
			//Found a closer match
			closestColourCount = colourMatches.size();
			closestPalette = paletteIdx;
		}
	}

	return false;
}

bool Project::ImportPalette(Colour* pixels, Palette& palette)
{
	//Add first colour
	palette.AddColour(pixels[0]);

	//Keep running colour count
	int numColours = 1;

	//Find/add remaining colours
	for(int i = 1; i < 8*8; i++)
	{
		int colourIdx = 0;
		if(!palette.GetNearestColourIdx(pixels[i], Palette::eExact, colourIdx))
		{
			if(numColours == Palette::coloursPerPalette)
			{
				//Hit the colour limit
				return false;
			}

			//Add new colour
			palette.AddColour(pixels[i]);
			numColours++;
		}
	}

	return true;
}

bool Project::MergePalettes(Palette& dest, const Palette& source)
{
	for(int i = 0; i < source.GetNumColours(); i++)
	{
		const Colour& sourceColour = source.GetColour(i);
		int colourIdx = 0;
		if(!dest.GetNearestColourIdx(sourceColour, Palette::eExact, colourIdx))
		{
			if(!dest.AddColour(sourceColour))
			{
				return false;
			}
		}
	}

	return true;
}

bool Project::ImportBitmap(const std::string& filename, u8 importFlags)
{
	if(importFlags & eBMPImportReplaceAll)
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

		const int tileWidth = 8;
		const int tileHeight = 8;

		//For all 8x8 tiles
		for(int tileX = 0; tileX < tilesWidth; tileX++)
		{
			for(int tileY = 0; tileY < tilesHeight; tileY++)
			{
				Colour pixels[tileWidth * tileHeight];

				//Read pixel colours from bitmap
				for(int pixelX = 0; pixelX < tileWidth; pixelX++)
				{
					for(int pixelY = 0; pixelY < tileHeight; pixelY++)
					{
						int soucePixelX = (tileX * tileWidth) + pixelX;
						int soucePixelY = (tileY * tileHeight) + pixelY;
						pixels[(pixelY * tileWidth) + pixelX] = reader.GetPixel(soucePixelX, soucePixelY);
					}
				}

				//Find or create palette
				PaletteId paletteId = 0;
				PaletteId closestPaletteId = 0;
				int closestPaletteColourMatches = 0;
				if(!FindPalette(pixels, paletteId, closestPaletteId, closestPaletteColourMatches))
				{
					//No existing palette found, create new
					if(newPaletteCount == numPalettes)
					{
						wxMessageBox("Exceeded palette count, bailing out", "Error", wxOK);
						return false;
					}

					Palette importedPalette;
					if(!ImportPalette(pixels, importedPalette))
					{
						wxMessageBox("Too many colours in tile, bailing out", "Error", wxOK);
						return false;
					}

					//If closest palette has enough space to merge
					bool merged = false;
					if(closestPaletteColourMatches > 0)
					{
						int spareColours = Palette::coloursPerPalette - m_palettes[closestPaletteId].GetNumColours();
						int requiredNewColours = importedPalette.GetNumColours() - closestPaletteColourMatches;

						if(spareColours >= requiredNewColours)
						{
							//Merge palettes
							if(MergePalettes(m_palettes[closestPaletteId], importedPalette))
							{
								paletteId = closestPaletteId;
								merged = true;
							}
						}
					}
					
					if(!merged)
					{
						//Use imported palette
						paletteId = newPaletteCount++;
						m_palettes[paletteId] = importedPalette;
					}
				}

				//Get palette
				Palette& palette = m_palettes[paletteId];

				Tile tile;
				tile.SetPaletteId(paletteId);

				//Find pixel colours from palette
				for(int pixelX = 0; pixelX < tileWidth; pixelX++)
				{
					for(int pixelY = 0; pixelY < tileHeight; pixelY++)
					{
						for(int i = 0; i < Palette::coloursPerPalette; i++)
						{
							int colourIdx = 0;
							if(!palette.GetNearestColourIdx(pixels[(pixelY * tileWidth) + pixelX], Palette::eExact, colourIdx))
							{
								//Shouldn't reach here - palette should have been validated
								wxMessageBox("Error mapping colour indices", "Error", wxOK);
								return false;
							}

							tile.SetPixelColour(pixelX, pixelY, colourIdx);
						}
					}
				}

				//Hash invalidated, recalculate
				tile.CalculateColourHash();

				//Find duplicate or create new
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

				if(importFlags & eBMPImportDrawToMap)
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

bool Project::ExportPalettes(const std::string& filename) const
{
	ion::io::File file(filename, ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		stream << "palette_" << m_name << ":" << std::endl;

		for(int i = 0; i < numPalettes; i++)
		{
			m_palettes[i].Export(stream);
			stream << std::endl;
		}

		file.Write(stream.str().c_str(), stream.str().size());

		return true;
	}

	return false;
}

bool Project::ExportTiles(const std::string& filename) const
{
	return false;
}

bool Project::ExportCollision(const std::string& filename) const
{
	return false;
}

bool Project::ExportMap(const std::string& filename) const
{
	return false;
}