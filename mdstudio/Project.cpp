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
	m_backgroundTile = InvalidTileId;
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
	m_nextFreeGameObjectTypeId = 1;
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
	m_collisionMap.Clear();
	m_tileset.Clear();
	m_stamps.clear();
	m_nextFreeStampId = 1;
	m_nextFreeGameObjectTypeId = 1;
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

		std::map<u64, Tile> tiles;
		int count = m_tileset.GetCount();
		for(int i = 0; i < count; i++)
		{
			Tile tile = *m_tileset.GetTile(i);
			tile.CalculateHash();
			tiles.insert(std::make_pair(tile.GetHash(), tile));
		}

		int countAfter = tiles.size();

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
	archive.Serialise(m_paletteSlots, "paletteSlots");
	archive.Serialise(m_tileset, "tileset");
	archive.Serialise(m_backgroundTile, "backgroundTile");
	archive.Serialise(m_collisionTileset, "collisionTileset");
	archive.Serialise(m_map, "map");
	archive.Serialise(m_collisionMap, "collisionMap");
	archive.Serialise(m_stamps, "stamps");
	archive.Serialise(m_collisionTypes, "collisionTypes");
	archive.Serialise(m_gameObjectTypes, "gameObjectTypes");
	archive.Serialise(m_nextFreeStampId, "nextFreeStampId");
	archive.Serialise(m_nextFreeGameObjectTypeId, "nextFreeGameObjectTypeId");
	archive.Serialise(m_exportFilenames, "exportFilenames");
}

int Project::AddPaletteSlot(Palette& palette)
{
	m_paletteSlots.push_back(palette);
	return m_paletteSlots.size() - 1;
}

Palette* Project::GetPaletteSlot(int slotIndex)
{
	ion::debug::Assert(slotIndex < GetNumPaletteSlots(), "Project::GetPaletteSlot() - Invalid slot index");
	return &m_paletteSlots[slotIndex];
}

int Project::GetNumPaletteSlots() const
{
	return m_paletteSlots.size();
}

void Project::SetActivePaletteSlot(PaletteId paletteId, int slotIndex)
{
	ion::debug::Assert(slotIndex < GetNumPaletteSlots(), "Project::GetPaletteSlot() - Invalid slot index");
	ion::debug::Assert(paletteId < GetNumPalettes(), "Project::GetPaletteSlot() - Invalid palettes index");

	Palette* source = GetPaletteSlot(slotIndex);
	Palette* dest = GetPalette(paletteId);
	*dest = *source;
}

void Project::CollapsePaletteSlots()
{
	m_paletteSlots.clear();

	for(int i = 0; i < GetNumPalettes(); i++)
	{
		if(m_palettes[i].GetUsedColourMask() > 0)
		{
			int slotIndex = AddPaletteSlot(m_palettes[i]);
		}
	}
}

void Project::DeleteTile(TileId tileId)
{
	TileId swapTileId = (TileId)m_tileset.GetCount() - 1;

	//Find all uses of tile on map, set blank
	for(int x = 0; x < m_map.GetWidth(); x++)
	{
		for(int y = 0; y < m_map.GetHeight(); y++)
		{
			TileId currTileId = m_map.GetTile(x, y);
			if(currTileId == tileId)
			{
				//Referencing deleted tile, set as invalid
				m_map.SetTile(x, y, InvalidTileId);
			}
			else if(currTileId == swapTileId)
			{
				//Referencing swap tile, set new id
				u32 flags = m_map.GetTileFlags(x, y);
				m_map.SetTile(x, y, tileId);
				m_map.SetTileFlags(x, y, flags);
			}
		}
	}

	//Find all uses of tiles in stamps, set blank
	for(TStampMap::iterator it = m_stamps.begin(), end = m_stamps.end(); it != end; ++it)
	{
		for(int x = 0; x < it->second.GetWidth(); x++)
		{
			for(int y = 0; y < it->second.GetHeight(); y++)
			{
				TileId currTileId = it->second.GetTile(x, y);
				if(currTileId == tileId)
				{
					//Referencing deleted tile, set as invalid
					it->second.SetTile(x, y, InvalidTileId);
				}
				else if(currTileId == swapTileId)
				{
					//Referencing swap tile, set new id
					u32 flags = it->second.GetTileFlags(x, y);
					it->second.SetTile(x, y, tileId);
					it->second.SetTileFlags(x, y, flags);
				}
			}
		}
	}

	if(m_tileset.GetCount() > 0)
	{
		//Swap tiles
		Tile* tile1 = m_tileset.GetTile(tileId);
		Tile* tile2 = m_tileset.GetTile((TileId)m_tileset.GetCount() - 1);
		ion::debug::Assert(tile1 && tile2, "Project::DeleteTile() - Invalid tile");

		Tile tmpTile;
		tmpTile = *tile1;
		*tile1 = *tile2;
		*tile2 = tmpTile;

	}

	//Erase last tile
	m_tileset.PopBackTile();

	//Clear paint tile
	SetPaintTile(InvalidTileId);

	//Clear erase tile
	SetEraseTile(InvalidTileId);
}

void Project::SwapTiles(TileId tileId1, TileId tileId2)
{
	//Swap in tileset
	Tile* tile1 = m_tileset.GetTile(tileId1);
	Tile* tile2 = m_tileset.GetTile(tileId2);
	ion::debug::Assert(tile1 && tile2, "Project::SwapTiles() - Invalid tile");

	Tile tmpTile;
	tmpTile = *tile1;
	*tile1 = *tile2;
	*tile2 = tmpTile;

	//Find all uses of tiles on map, swap ids
	for(int x = 0; x < m_map.GetWidth(); x++)
	{
		for(int y = 0; y < m_map.GetHeight(); y++)
		{
			TileId currTileId = m_map.GetTile(x, y);
			if(currTileId == tileId1)
			{
				m_map.SetTile(x, y, tileId2);
			}
			else if(currTileId == tileId2)
			{
				m_map.SetTile(x, y, tileId1);
			}
		}
	}

	//Find all uses of tiles in stamps, swap ids
	for(TStampMap::iterator it = m_stamps.begin(), end = m_stamps.end(); it != end; ++it)
	{
		for(int x = 0; x < it->second.GetWidth(); x++)
		{
			for(int y = 0; y < it->second.GetHeight(); y++)
			{
				TileId currTileId = it->second.GetTile(x, y);
				if(currTileId == tileId1)
				{
					it->second.SetTile(x, y, tileId2);
				}
				else if(currTileId == tileId2)
				{
					it->second.SetTile(x, y, tileId1);
				}
			}
		}
	}
}

void Project::SetBackgroundTile(TileId tileId)
{
	m_backgroundTile = tileId;
}

int Project::CleanupTiles()
{
	std::set<TileId> usedTiles;

	//Collect all used tile ids from map
	for(int x = 0; x < m_map.GetWidth(); x++)
	{
		for(int y = 0; y < m_map.GetHeight(); y++)
		{
			TileId tileId = m_map.GetTile(x, y);
			usedTiles.insert(tileId);
		}
	}

	//Collect all used tile ids from stamps
	for(TStampMap::const_iterator it = m_stamps.begin(), end = m_stamps.end(); it != end; ++it)
	{
		const Stamp& stamp = it->second;

		for(int x = 0; x < stamp.GetWidth(); x++)
		{
			for(int y = 0; y < stamp.GetHeight(); y++)
			{
				TileId tileId = stamp.GetTile(x, y);
				usedTiles.insert(tileId);
			}
		}
	}

	std::set<TileId> unusedTiles;

	for(TileId i = 0; i < m_tileset.GetCount(); i++)
	{
		if(usedTiles.find(i) == usedTiles.end())
		{
			unusedTiles.insert(i);
		}
	}

	if(unusedTiles.size() > 0)
	{
		std::stringstream message;
		message << "Found " << unusedTiles.size() << " unused tiles, delete?";

		if(wxMessageBox(message.str().c_str(), "Delete unused tiles", wxOK | wxCANCEL | wxICON_WARNING) == wxOK)
		{
			//Delete in reverse, popping from back
			for(std::set<TileId>::const_reverse_iterator it = unusedTiles.rbegin(), end = unusedTiles.rend(); it != end; ++it)
			{
				DeleteTile(*it);
			}
		}
	}

	//Calculate hashes for all tiles
	struct Duplicate
	{
		TileId original;
		TileId duplicate;
		Tileset::HashOrientation duplicateOrientation;
	};

	std::map<u64, TileId> tileMaps[Tileset::eNumHashOrientations];
	std::vector<Duplicate> duplicates;
	u64 hashes[Tileset::eNumHashOrientations];

	for(int i = 0; i < m_tileset.GetCount(); i++)
	{
		const Tile* tile = m_tileset.GetTile(i);
		m_tileset.CalculateHashes(*tile, hashes);
		bool duplicateFound = false;

		for(int j = 0; j < Tileset::eNumHashOrientations && !duplicateFound; j++)
		{
			std::map<u64, TileId>::iterator it = tileMaps[j].find(tile->GetHash());
			if(it != tileMaps[j].end())
			{
				Duplicate duplicate;
				duplicate.original = it->second;
				duplicate.duplicate = i;
				duplicate.duplicateOrientation = (Tileset::HashOrientation)j;
				duplicates.push_back(duplicate);
				duplicateFound = true;
			}
		}

		if(!duplicateFound)
		{
			for(int j = 0; j < Tileset::eNumHashOrientations; j++)
			{
				tileMaps[j].insert(std::make_pair(hashes[j], i));
			}
		}
	}

	if(duplicates.size() > 0)
	{
		std::stringstream message;
		message << "Found " << duplicates.size() << " duplicate tiles, delete?";

		if(wxMessageBox(message.str().c_str(), "Delete duplicate tiles", wxOK | wxCANCEL | wxICON_WARNING) == wxOK)
		{
			for(int i = 0; i < duplicates.size(); i++)
			{
				//Find use of duplicate id in map
				for(int x = 0; x < m_map.GetWidth(); x++)
				{
					for(int y = 0; y < m_map.GetHeight(); y++)
					{
						if(m_map.GetTile(x, y) == duplicates[i].duplicate)
						{
							u32 originalFlags = m_map.GetTileFlags(x, y);

							//Replace duplicate with original
							m_map.SetTile(x, y, duplicates[i].original);

							//Orientate to match duplicate
							m_map.SetTileFlags(x, y, originalFlags ^ Tileset::s_orientationFlags[duplicates[i].duplicateOrientation]);
						}
					}
				}

				//Find uses of duplicate id in stamps
				for(TStampMap::iterator it = m_stamps.begin(), end = m_stamps.end(); it != end; ++it)
				{
					for(int x = 0; x < it->second.GetWidth(); x++)
					{
						for(int y = 0; y < it->second.GetHeight(); y++)
						{
							if(it->second.GetTile(x, y) == duplicates[i].duplicate)
							{
								u32 originalFlags = it->second.GetTileFlags(x, y);

								//Replace duplicate with original
								it->second.SetTile(x, y, duplicates[i].original);

								//Orientate to match duplicate
								it->second.SetTileFlags(x, y, originalFlags ^ Tileset::s_orientationFlags[duplicates[i].duplicateOrientation]);
							}
						}
					}
				}
			}

			//Delete duplicates (in reverse, popping from back)
			for(int i = duplicates.size() - 1; i >= 0; --i)
			{
				DeleteTile(duplicates[i].duplicate);
			}
		}
	}

	return unusedTiles.size() + duplicates.size();
}

StampId Project::AddStamp(int width, int height)
{
	StampId id = m_nextFreeStampId++;
	m_stamps.insert(std::make_pair(id, Stamp(id, width, height)));
	return id;
}

void Project::DeleteStamp(StampId stampId)
{
	//Remove all uses of stamp on map
	std::vector<ion::Vector2i> stampUseCoords;
	for(TStampPosMap::const_iterator it = m_map.StampsBegin(), end = m_map.StampsEnd(); it != end; ++it)
	{
		if(it->m_id == stampId)
		{
			stampUseCoords.push_back(it->m_position);
		}
	}

	for(int i = 0; i < stampUseCoords.size(); i++)
	{
		m_map.RemoveStamp(stampUseCoords[i].x, stampUseCoords[i].y);
	}

	//Delete stamp
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

GameObjectTypeId Project::AddGameObjectType()
{
	GameObjectTypeId typeId = m_nextFreeGameObjectTypeId++;
	m_gameObjectTypes.insert(std::make_pair(typeId, GameObjectType(typeId)));
	return typeId;
}

void Project::RemoveGameObjectType(GameObjectTypeId typeId)
{
	m_gameObjectTypes.erase(typeId);
}

GameObjectType* Project::GetGameObjectType(GameObjectTypeId typeId)
{
	GameObjectType* gameObjectType = NULL;
	TGameObjectTypeMap::iterator it = m_gameObjectTypes.find(typeId);
	if(it != m_gameObjectTypes.end())
		gameObjectType = &it->second;
	return gameObjectType;
}

const GameObjectType* Project::GetGameObjectType(GameObjectTypeId typeId) const
{
	const GameObjectType* gameObjectType = NULL;
	TGameObjectTypeMap::const_iterator it = m_gameObjectTypes.find(typeId);
	if(it != m_gameObjectTypes.end())
		gameObjectType = &it->second;
	return gameObjectType;
}

const TGameObjectTypeMap& Project::GetGameObjectTypes() const
{
	return m_gameObjectTypes;
}

void Project::ExportGameObjectTypes(const std::string& filename)
{
	ion::io::File file(filename, ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		ion::io::Archive archive(file, ion::io::Archive::Out);
		archive.Serialise(m_gameObjectTypes, "gameObjectTypes");
		archive.Serialise(m_nextFreeGameObjectTypeId, "nextFreeGameObjTypeId");
	}
}

void Project::ImportGameObjectTypes(const std::string& filename)
{
	ion::io::File file(filename, ion::io::File::OpenRead);
	if(file.IsOpen())
	{
		ion::io::Archive archive(file, ion::io::Archive::In);
		archive.Serialise(m_gameObjectTypes, "gameObjectTypes");
		archive.Serialise(m_nextFreeGameObjectTypeId, "nextFreeGameObjTypeId");
	}
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

void Project::SetPaintGameObjectType(GameObjectTypeId typeId)
{
	m_paintGameObjectType = typeId;
}

GameObjectTypeId Project::GetPaintGameObjectType() const
{
	return m_paintGameObjectType;
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

	if(importFlags & eBMPImportInsertBGTile)
	{
		m_tileset.AddTile();
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

		if(importFlags & eBMPImportWholePalette)
		{
			//Read whole palette
			int paletteIndex = -1;

			//Get selected palette index
			for(int i = 0; i < s_maxPalettes && paletteIndex == -1; i++)
			{
				if(paletteBits & (1 << i))
				{
					paletteIndex = i;
				}
			}

			//Insert palettes up to this index
			while(paletteIndex >= m_palettes.size())
			{
				m_palettes.push_back(Palette());
			}

			//Clear palette
			m_palettes[paletteIndex] = Palette();

			//Get palette
			Palette& palette = m_palettes[paletteIndex];

			for(int i = 0; i < reader.GetPaletteSize(); i++)
			{
				palette.AddColour(reader.GetPaletteEntry(i));
			}
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

void Project::WriteFileHeader(std::stringstream& stream) const
{
	stream << "; == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==" << std::endl;
	stream << ";   Created with BEEhive - the complete art tool for SEGA Mega Drive" << std::endl;
	stream << "; == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==" << std::endl;
	stream << ";   http://www.bigevilcorporation.co.uk" << std::endl;
	stream << "; == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==" << std::endl;
	stream << ";   BEEhive and SEGA Genesis Framework (c) Matt Phillips 2015" << std::endl;
	stream << "; == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==" << std::endl;
	stream << std::endl;
	stream << std::endl;
}

bool Project::ExportPalettes(const std::string& filename) const
{
	ion::io::File file(filename, ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		WriteFileHeader(stream);
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
		WriteFileHeader(stream);
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

bool Project::ExportCollisionTiles(const std::string& filename) const
{
	ion::io::File file(filename, ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		WriteFileHeader(stream);
		stream << "collisiontiles_" << m_name << ":" << std::endl;

		m_collisionTileset.Export(stream);

		stream << std::endl;

		stream << "collisiontiles_" << m_name << "_end" << std::endl;
		stream << "collisiontiles_" << m_name << "_size_b\tequ (collisiontiles_" << m_name << "_end-collisiontiles_" << m_name << ")\t; Size in bytes" << std::endl;
		stream << "collisiontiles_" << m_name << "_size_w\tequ (collisiontiles_" << m_name << "_size_b/2)\t; Size in words" << std::endl;
		stream << "collisiontiles_" << m_name << "_size_l\tequ (collisiontiles_" << m_name << "_size_b/4)\t; Size in longwords" << std::endl;
		stream << "collisiontiles_" << m_name << "_size_t\tequ (collisiontiles_" << m_name << "_size_b/32)\t; Size in tiles" << std::endl;

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
		WriteFileHeader(stream);
		stream << "map_" << m_name << ":" << std::endl;

		m_map.Export(*this, stream);

		stream << "map_" << m_name << "_end:" << std::endl;
		stream << "map_" << m_name << "_size_b\tequ (map_" << m_name << "_end-map_" << m_name << ")\t; Size in bytes" << std::endl;
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

bool Project::ExportCollisionMap(const std::string& filename) const
{
	ion::io::File file(filename, ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		WriteFileHeader(stream);
		stream << "collisionmap_" << m_name << ":" << std::endl;

		m_collisionMap.Export(*this, stream);

		stream << "collisionmap_" << m_name << "_end:" << std::endl;
		stream << "collisionmap_" << m_name << "_size_b\tequ (collisionmap_" << m_name << "_end-collisionmap_" << m_name << ")\t; Size in bytes" << std::endl;
		stream << "collisionmap_" << m_name << "_size_w\tequ (collisionmap_" << m_name << "_size_b/2)\t; Size in words" << std::endl;
		stream << "collisionmap_" << m_name << "_size_l\tequ (collisionmap_" << m_name << "_size_b/4)\t; Size in longwords" << std::endl;

		stream << std::hex << std::setfill('0') << std::uppercase;
		stream << "collisionmap_" << m_name << "_width\tequ " << "0x" << std::setw(2) << m_collisionMap.GetWidth() << std::endl;
		stream << "collisionmap_" << m_name << "_height\tequ " << "0x" << std::setw(2) << m_collisionMap.GetHeight() << std::endl;
		stream << std::dec;

		file.Write(stream.str().c_str(), stream.str().size());

		return true;
	}

	return false;
}

bool Project::ExportGameObjects(const std::string& filename) const
{
	ion::io::File file(filename, ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		WriteFileHeader(stream);
		stream << "gameobjects:" << std::endl;

		const TGameObjectPosMap& gameObjects = m_map.GetGameObjects();

		for(TGameObjectPosMap::const_iterator itMap = gameObjects.begin(), endMap = gameObjects.end(); itMap != endMap; ++itMap)
		{
			if(const GameObjectType* gameObjectType = GetGameObjectType(itMap->first))
			{
				stream << "gameobjects_" << gameObjectType->GetName() << "_count equ " << itMap->second.size() << std::endl;
				stream << "LoadGameObjects_" << gameObjectType->GetName() << ":" << std::endl;

				for(int i = 0; i < itMap->second.size(); i++)
				{
					itMap->second[i].m_gameObject.Export(stream, *gameObjectType);
					stream << '\t' << "add.l #" << gameObjectType->GetName() << "_Struct_Size, a0" << std::endl;
				}

				stream << '\t' << "rts" << std::endl;
			}

			stream << std::endl;
		}

		file.Write(stream.str().c_str(), stream.str().size());

		return true;
	}

	return false;
}