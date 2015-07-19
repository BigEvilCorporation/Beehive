///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include <io/Archive.h>
#include <wx/msgdlg.h>
#include <set>

#include "Project.h"
#include "BMPReader.h"

Project::Project()
{
	m_paintColour = 0;
	m_paintCollisionType = NULL;
	m_paintCollisionTile = InvalidCollisionTileId;
	m_paintTile = InvalidTileId;
	m_eraseTile = InvalidTileId;
	m_paintStamp = InvalidStampId;
	m_mapInvalidated = true;
	m_tilesInvalidated = true;
	m_collisionTilesInvalidated = true;
	m_collisionTypesInvalidated = true;
	m_stampsInvalidated = true;
	m_name = "untitled";
	m_gridSize = 1;
	m_showGrid = true;
	m_snapToGrid = false;
	m_showStampOutlines = true;
	m_palettes.resize(s_maxPalettes);
	m_nextFreeStampId = 1;
}

void Project::Clear()
{
	m_paintColour = 0;
	m_paintCollisionType = NULL;
	m_paintCollisionTile = InvalidCollisionTileId;
	m_paintTile = InvalidTileId;
	m_eraseTile = InvalidTileId;
	m_paintStamp = InvalidStampId;
	m_mapInvalidated = true;
	m_tilesInvalidated = true;
	m_collisionTypesInvalidated = true;
	m_stampsInvalidated = true;
	m_name = "untitled";
	m_palettes.resize(s_maxPalettes);

	for(int i = 0; i < s_maxPalettes; i++)
	{
		m_palettes[i].Clear();
	}

	m_map.Clear();
	m_tileset.Clear();
	m_stamps.clear();
	m_nextFreeStampId = 1;
}

bool Project::Load(const std::string& filename)
{
	ion::io::File file(filename, ion::io::File::OpenRead);
	if(file.IsOpen())
	{
		ion::io::Archive archive(file, ion::io::Archive::In);
		Serialise(archive);

		InvalidateMap(true);
		InvalidateTiles(true);
		InvalidateStamps(true);

		m_filename = filename;

		return true;
	}

	return false;
}

bool Project::Save(const std::string& filename)
{
	ion::io::File file(filename, ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		ion::io::Archive archive(file, ion::io::Archive::Out);
		Serialise(archive);
		m_filename = filename;
		return true;
	}

	return false;
}

void Project::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_name, "name");
	archive.Serialise(m_palettes, "palettes");
	archive.Serialise(m_tileset, "tileset");
	archive.Serialise(m_collisionTileset, "collisionTileset");
	archive.Serialise(m_map, "map");
	archive.Serialise(m_stamps, "stamps");
	archive.Serialise(m_collisionTypes, "collisionTypes");
	archive.Serialise(m_nextFreeStampId, "nextFreeStampId");
	archive.Serialise(m_exportFilenames, "exportFilenames");
}

StampId Project::AddStamp(int width, int height)
{
	StampId id = m_nextFreeStampId++;
	m_stamps.insert(std::make_pair(id, Stamp(id, width, height)));
	return id;
}

void Project::RemoveStamp(StampId stampId)
{
	TStampMap::iterator it = m_stamps.find(stampId);
	if(it != m_stamps.end())
	{
		m_stamps.erase(it);
	}
}

Stamp* Project::GetStamp(StampId stampId)
{
	Stamp* stamp = NULL;

	TStampMap::iterator it = m_stamps.find(stampId);
	if(it != m_stamps.end())
	{
		stamp = &it->second;
	}

	return stamp;
}

const Stamp* Project::GetStamp(StampId stampId) const
{
	const Stamp* stamp = NULL;

	TStampMap::const_iterator it = m_stamps.find(stampId);
	if(it != m_stamps.end())
	{
		stamp = &it->second;
	}

	return stamp;
}

const TStampMap::const_iterator Project::StampsBegin() const
{
	return m_stamps.begin();
}

const TStampMap::const_iterator Project::StampsEnd() const
{
	return m_stamps.end();
}

int Project::GetStampCount() const
{
	return m_stamps.size();
}

CollisionType* Project::AddCollisionType(u8 bit)
{
	CollisionType* collisionType = &m_collisionTypes.insert(std::make_pair(bit, CollisionType())).first->second;
	collisionType->bit = bit;
	return collisionType;
}

void Project::RemoveCollisionType(u8 bit)
{
	m_collisionTypes.erase(bit);
}

CollisionType* Project::GetCollisionType(const std::string& name)
{
	for(TCollisionTypeMap::iterator it = m_collisionTypes.begin(), end = m_collisionTypes.end(); it != end; ++it)
	{
		if(name == it->second.name)
		{
			return &it->second;
		}
	}

	return NULL;
}

CollisionType* Project::GetCollisionType(u8 bit)
{
	CollisionType* collisionType = NULL;

	TCollisionTypeMap::iterator it = m_collisionTypes.find(bit);
	if(it != m_collisionTypes.end())
	{
		collisionType = &it->second;
	}

	return collisionType;
}

const TCollisionTypeMap::const_iterator Project::CollisionTypesBegin() const
{
	return m_collisionTypes.begin();
}

const TCollisionTypeMap::const_iterator Project::CollisionTypesEnd() const
{
	return m_collisionTypes.end();
}

int Project::GetCollisionTypeCount() const
{
	return m_collisionTypes.size();
}

void Project::SetPaintColour(u8 colourIdx)
{
	m_paintColour = colourIdx;
}

u8 Project::GetPaintColour() const
{
	return m_paintColour;
}

void Project::SetPaintCollisionType(CollisionType* type)
{
	m_paintCollisionType = type;
}

const CollisionType* Project::GetPaintCollisionType() const
{
	return m_paintCollisionType;
}

void Project::SetPaintCollisionTile(CollisionTileId tile)
{
	m_paintCollisionTile = tile;
}

CollisionTileId Project::GetPaintCollisionTile() const
{
	return m_paintCollisionTile;
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

void Project::SetPaintStamp(StampId stamp)
{
	m_paintStamp = stamp;
}

StampId Project::GetPaintStamp() const
{
	return m_paintStamp;
}

bool Project::FindPalette(Colour* pixels, u32 useablePalettes, PaletteId& paletteId, PaletteId& closestPalette, int& closestColourCount) const
{
	//Set of found colour idxs
	std::set<int> colourMatches;

	//For each palette
	for(int paletteIdx = 0; paletteIdx < s_maxPalettes; paletteIdx++)
	{
		if(useablePalettes & (1 << paletteIdx))
		{
			const Palette& palette = m_palettes[paletteIdx];

			bool match = true;
			colourMatches.clear();

			//For each pixel
			for(int i = 0; i < 8 * 8; i++)
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
	for(int i = 0; i < Palette::coloursPerPalette; i++)
	{
		if(source.IsColourUsed(i))
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
	}

	return true;
}

bool Project::ImportBitmap(const std::string& filename, u32 importFlags, u32 paletteBits)
{
	if(importFlags & eBMPImportClearPalettes)
	{
		for(int i = 0; i < s_maxPalettes; i++)
		{
			if(paletteBits & (1 << i))
			{
				m_palettes[i].Clear();
			}
		}
	}

	if(importFlags & eBMPImportClearTiles)
	{
		m_tileset.Clear();
	}

	if(importFlags & eBMPImportClearMap)
	{
		m_map.Clear();
	}

	//Read BMP
	BMPReader reader;
	if(reader.Read(filename))
	{
		if(reader.GetWidth() % 8 != 0 || reader.GetHeight() % 8 != 0)
		{
			if(wxMessageBox("Bitmap width/height is not multiple of 8, image will be truncated", "Warning", wxOK | wxCANCEL | wxICON_WARNING) == wxCANCEL)
			{
				return false;
			}
		}

		//Get width/height in tiles
		int tilesWidth = reader.GetWidth() / 8;
		int tilesHeight = reader.GetHeight() / 8;

		if(importFlags & eBMPImportDrawToMap)
		{
			//Grow map if necessary
			m_map.Resize(max(m_map.GetWidth(), tilesWidth), max(m_map.GetHeight(), tilesHeight));
		}

		Stamp* stamp = NULL;

		if(importFlags & eBMPImportToStamp)
		{
			//Create new stamp
			StampId stampId = AddStamp(tilesWidth, tilesHeight);
			stamp = GetStamp(stampId);
		}

		const int tileWidth = 8;
		const int tileHeight = 8;

		//For all 8x8 tiles, in row-at-a-time order
		for(int tileY = 0; tileY < tilesHeight; tileY++)
		{
			for(int tileX = 0; tileX < tilesWidth; tileX++)
			{
				Colour pixels[tileWidth * tileHeight];

				//Read pixel colours from bitmap
				for(int pixelX = 0; pixelX < tileWidth; pixelX++)
				{
					for(int pixelY = 0; pixelY < tileHeight; pixelY++)
					{
						int sourcePixelX = (tileX * tileWidth) + pixelX;
						int sourcePixelY = (tileY * tileHeight) + pixelY;
						pixels[(pixelY * tileWidth) + pixelX] = reader.GetPixel(sourcePixelX, sourcePixelY);
					}
				}

				//Find or create palette
				PaletteId paletteId = 0;
				PaletteId closestPaletteId = 0;
				int closestPaletteColourMatches = 0;
				if(!FindPalette(pixels, paletteBits, paletteId, closestPaletteId, closestPaletteColourMatches))
				{
					//Import palette
					Palette importedPalette;
					if(!ImportPalette(pixels, importedPalette))
					{
						wxMessageBox("Too many colours in tile, bailing out", "Error", wxOK | wxICON_ERROR);
						return false;
					}

					//If closest palette has enough space to merge
					bool merged = false;
					if(closestPaletteColourMatches > 0)
					{
						int closestPaletteUsedColours = 0;
						int importedPaletteUsedColours = 0;

						for(int i = 0; i < Palette::coloursPerPalette; i++)
						{
							if(m_palettes[closestPaletteId].IsColourUsed(i))
							{
								closestPaletteUsedColours++;
							}
						}

						for(int i = 0; i < Palette::coloursPerPalette; i++)
						{
							if(importedPalette.IsColourUsed(i))
							{
								importedPaletteUsedColours++;
							}
						}

						int spareColours = Palette::coloursPerPalette - closestPaletteUsedColours;
						int requiredNewColours = importedPaletteUsedColours - closestPaletteColourMatches;

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
						//Find free useable palette
						bool foundFreePalette = false;
						for(int i = 0; i < s_maxPalettes && !foundFreePalette; i++)
						{
							if((paletteBits & (1 << i)) && m_palettes[i].GetUsedColourMask() == 0)
							{
								paletteId = i;
								foundFreePalette = true;
							}
						}

						if(!foundFreePalette)
						{
							wxMessageBox("Exceeded palette count, bailing out", "Error", wxOK | wxICON_ERROR);
							return false;
						}

						//Use imported palette
						m_palettes[paletteId] = importedPalette;
					}
				}

				//Get palette
				Palette& palette = m_palettes[paletteId];

				Tile tile;

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
								wxMessageBox("Error mapping colour indices", "Error", wxOK | wxICON_ERROR);
								return false;
							}

							tile.SetPixelColour(pixelX, pixelY, colourIdx);
						}
					}
				}

				//Hash invalidated
				tile.CalculateHash();

				//Find duplicate or create new
				TileId tileId = 0;
				u32 tileFlags = 0;
				TileId duplicateId = m_tileset.FindDuplicate(tile, tileFlags);
				if(duplicateId != InvalidTileId)
				{
					//Tile already exists
					tileId = duplicateId;
				}
				else
				{
					//Create new tile and copy
					tileId = m_tileset.AddTile();
					Tile* newTile = m_tileset.GetTile(tileId);
					newTile->CopyPixels(tile);
					newTile->SetPaletteId(paletteId);

					//Re-add to hash map
					m_tileset.HashChanged(tileId);
				}

				if(importFlags & eBMPImportDrawToMap)
				{
					//Set in map
					m_map.SetTile(tileX, tileY, tileId);
					m_map.SetTileFlags(tileX, tileY, tileFlags);
				}

				if(importFlags & eBMPImportToStamp)
				{
					//Set in stamp
					stamp->SetTile(tileX, tileY, tileId);
					stamp->SetTileFlags(tileX, tileY, tileFlags);
				}
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

		for(int i = 0; i < s_maxPalettes; i++)
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
	ion::io::File file(filename, ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		stream << "tiles_" << m_name << ":" << std::endl;

		m_tileset.Export(stream);

		stream << std::endl;

		stream << "tiles_" << m_name << "_end" << std::endl;
		stream << "tiles_" << m_name << "_size_b\tequ (tiles_" << m_name << "_end-tiles_" << m_name << ")\t; Size in bytes" << std::endl;
		stream << "tiles_" << m_name << "_size_w\tequ (tiles_" << m_name << "_size_b/2)\t; Size in words" << std::endl;
		stream << "tiles_" << m_name << "_size_l\tequ (tiles_" << m_name << "_size_b/4)\t; Size in longwords" << std::endl;
		stream << "tiles_" << m_name << "_size_t\tequ (tiles_" << m_name << "_size_b/32)\t; Size in tiles" << std::endl;

		file.Write(stream.str().c_str(), stream.str().size());

		return true;
	}

	return false;
}

bool Project::ExportCollision(const std::string& filename) const
{
	ion::io::File file(filename, ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		stream << "collision_" << m_name << ":" << std::endl;

		m_collisionTileset.Export(stream);

		stream << std::endl;

		stream << "collision_" << m_name << "_end" << std::endl;
		stream << "collision_" << m_name << "_size_b\tequ (collision_" << m_name << "_end-collision_" << m_name << ")\t; Size in bytes" << std::endl;
		stream << "collision_" << m_name << "_size_w\tequ (collision_" << m_name << "_size_b/2)\t; Size in words" << std::endl;
		stream << "collision_" << m_name << "_size_l\tequ (collision_" << m_name << "_size_b/4)\t; Size in longwords" << std::endl;
		stream << "collision_" << m_name << "_size_t\tequ (collision_" << m_name << "_size_b/32)\t; Size in tiles" << std::endl;

		file.Write(stream.str().c_str(), stream.str().size());

		return true;
	}

	return false;
}

bool Project::ExportMap(const std::string& filename) const
{
	ion::io::File file(filename, ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		stream << "map_" << m_name << ":" << std::endl;

		m_map.Export(*this, stream);

		stream << "map_" << m_name << "_end:" << std::endl;
		stream << "map_" << m_name << "_size_b\tequ (map_" << m_name << "_end-tiles_" << m_name << ")\t; Size in bytes" << std::endl;
		stream << "map_" << m_name << "_size_w\tequ (map_" << m_name << "_size_b/2)\t; Size in words" << std::endl;
		stream << "map_" << m_name << "_size_l\tequ (map_" << m_name << "_size_b/4)\t; Size in longwords" << std::endl;

		stream << std::hex << std::setfill('0') << std::uppercase;
		stream << "map_" << m_name << "_width\tequ " << "0x" << std::setw(2) << m_map.GetWidth() << std::endl;
		stream << "map_" << m_name << "_height\tequ " << "0x" << std::setw(2) <<  m_map.GetHeight() << std::endl;
		stream << std::dec;

		file.Write(stream.str().c_str(), stream.str().size());

		return true;
	}

	return false;
}