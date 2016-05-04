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
	m_paintTerrainTile = InvalidTerrainTileId;
	m_paintTile = InvalidTileId;
	m_eraseTile = InvalidTileId;
	m_backgroundTile = InvalidTileId;
	m_defaultTerrainTile = InvalidTerrainTileId;
	m_paintStamp = InvalidStampId;
	m_mapInvalidated = true;
	m_tilesInvalidated = true;
	m_terrainTilesInvalidated = true;
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
	m_paintTerrainTile = InvalidTerrainTileId;
	m_paintTile = InvalidTileId;
	m_eraseTile = InvalidTileId;
	m_backgroundTile = InvalidTileId;
	m_defaultTerrainTile = InvalidTerrainTileId;
	m_paintStamp = InvalidStampId;
	m_mapInvalidated = true;
	m_tilesInvalidated = true;
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
	archive.Serialise(m_terrainTileset, "terrainTileset");
	archive.Serialise(m_defaultTerrainTile, "defaultTerrainTile");
	archive.Serialise(m_map, "map");
	archive.Serialise(m_collisionMap, "collisionMap");
	archive.Serialise(m_terrainBeziers, "terrainBeziers");
	archive.Serialise(m_stamps, "stamps");
	archive.Serialise(m_spriteSheets, "spriteSheets");
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

void Project::ExportPaletteSlots(const std::string& filename)
{
	ion::io::File file(filename, ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		ion::io::Archive archive(file, ion::io::Archive::Out);
		archive.Serialise(m_paletteSlots, "paletteSlots");
	}
}

void Project::ImportPaletteSlots(const std::string& filename)
{
	ion::io::File file(filename, ion::io::File::OpenRead);
	if(file.IsOpen())
	{
		ion::io::Archive archive(file, ion::io::Archive::In);
		archive.Serialise(m_paletteSlots, "paletteSlots");
	}
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

			//Ignore background tile
			if(tileId != m_backgroundTile)
			{
				usedTiles.insert(tileId);
			}
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

SpriteSheetId Project::CreateSpriteSheet()
{
	SpriteSheetId spriteSheetId = ion::GenerateUUID64();
	m_spriteSheets.insert(std::make_pair(spriteSheetId, SpriteSheet()));
	return spriteSheetId;
}

void Project::DeleteSpriteSheet(SpriteSheetId spriteSheetId)
{
	TSpriteSheetMap::iterator it = m_spriteSheets.find(spriteSheetId);
	if(it != m_spriteSheets.end())
	{
		m_spriteSheets.erase(it);
	}
}

SpriteSheet* Project::GetSpriteSheet(SpriteSheetId spriteSheetId)
{
	SpriteSheet* spriteSheet = NULL;

	TSpriteSheetMap::iterator it = m_spriteSheets.find(spriteSheetId);
	if(it != m_spriteSheets.end())
	{
		spriteSheet = &it->second;
	}

	return spriteSheet;
}

const SpriteSheet* Project::GetSpriteSheet(SpriteSheetId spriteSheetId) const
{
	const SpriteSheet* spriteSheet = NULL;

	TSpriteSheetMap::const_iterator it = m_spriteSheets.find(spriteSheetId);
	if(it != m_spriteSheets.end())
	{
		spriteSheet = &it->second;
	}

	return spriteSheet;
}

const TSpriteSheetMap::const_iterator Project::SpriteSheetsBegin() const
{
	return m_spriteSheets.begin();
}

const TSpriteSheetMap::const_iterator Project::SpriteSheetsEnd() const
{
	return m_spriteSheets.end();
}

int Project::GetSpriteSheetCount() const
{
	return m_spriteSheets.size();
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

void Project::DeleteTerrainTile(TerrainTileId terrainTileId)
{
	TerrainTileId swapTerrainTileId = (TerrainTileId)m_terrainTileset.GetCount() - 1;

	//Find all uses of terrain tile on map, set blank
	for(int x = 0; x < m_collisionMap.GetWidth(); x++)
	{
		for(int y = 0; y < m_collisionMap.GetHeight(); y++)
		{
			TerrainTileId currTerrainTileId = m_collisionMap.GetTerrainTile(x, y);
			if(currTerrainTileId == terrainTileId)
			{
				//Referencing deleted tile, set as invalid
				m_collisionMap.SetTerrainTile(x, y, InvalidTerrainTileId);
			}
			else if(currTerrainTileId == swapTerrainTileId)
			{
				//Referencing swap tile, set new id
				m_collisionMap.SetTerrainTile(x, y, terrainTileId);
			}
		}
	}

	if(m_terrainTileset.GetCount() > 0)
	{
		if(swapTerrainTileId != terrainTileId)
		{
			//Swap terrain tile
			TerrainTile* terrainTile1 = m_terrainTileset.GetTerrainTile(terrainTileId);
			TerrainTile* terrainTile2 = m_terrainTileset.GetTerrainTile(swapTerrainTileId);
			ion::debug::Assert(terrainTile1 && terrainTile2, "Project::DeleteTerrainTile() - Invalid terrain tile");

			TerrainTile tmpTerrainTile;
			tmpTerrainTile = *terrainTile1;
			*terrainTile1 = *terrainTile2;
			*terrainTile2 = tmpTerrainTile;
		}

		//Erase last terrain tile
		m_terrainTileset.PopBackTerrainTile();

		if(swapTerrainTileId != terrainTileId)
		{
			//Id belonging to swapped hash has changed, reinsert into hash map
			m_terrainTileset.HashChanged(terrainTileId);
		}
	}

	//Clear paint terrain tile
	SetPaintTerrainTile(InvalidTerrainTileId);
}

void Project::SwapTerrainTiles(TerrainTileId tileId1, TerrainTileId tileId2)
{

}

void Project::SetDefaultTerrainTile(TerrainTileId tileId)
{
	m_defaultTerrainTile = tileId;
}

int Project::CleanupTerrainTiles(bool prompt)
{
	//Backup default collision tile
	TerrainTile* defaultTile = (m_defaultTerrainTile != InvalidTerrainTileId) ? GetTerrainTileset().GetTerrainTile(m_defaultTerrainTile) : NULL;
	u64 defaultTileHash = defaultTile ? defaultTile->CalculateHash() : 0;

	std::set<TerrainTileId> usedTerrainTiles;

	//Collect all used TerrainTile ids from map
	for(int x = 0; x < m_map.GetWidth(); x++)
	{
		for(int y = 0; y < m_map.GetHeight(); y++)
		{
			TerrainTileId terrainTileId = m_collisionMap.GetTerrainTile(x, y);
			usedTerrainTiles.insert(terrainTileId);
		}
	}

	//Always add default tile
	usedTerrainTiles.insert(m_defaultTerrainTile);

	std::set<TerrainTileId> unusedTerrainTiles;

	for(TerrainTileId i = 0; i < m_terrainTileset.GetCount(); i++)
	{
		if(usedTerrainTiles.find(i) == usedTerrainTiles.end())
		{
			unusedTerrainTiles.insert(i);
		}
	}

	if(unusedTerrainTiles.size() > 0)
	{
		std::stringstream message;
		message << "Found " << unusedTerrainTiles.size() << " unused collision tiles, delete?";

		if(!prompt || wxMessageBox(message.str().c_str(), "Delete unused collision tiles", wxOK | wxCANCEL | wxICON_WARNING) == wxOK)
		{
			//Delete in reverse, popping from back
			for(std::set<TerrainTileId>::const_reverse_iterator it = unusedTerrainTiles.rbegin(), end = unusedTerrainTiles.rend(); it != end; ++it)
			{
				DeleteTerrainTile(*it);
			}
		}
	}

	//Calculate hashes for all TerrainTiles
	struct Duplicate
	{
		TerrainTileId original;
		TerrainTileId duplicate;
	};

	std::map<u64, TerrainTileId> terrainTileMap;
	std::vector<Duplicate> duplicates;

	for(int i = 0; i < m_terrainTileset.GetCount(); i++)
	{
		TerrainTile* terrainTile = m_terrainTileset.GetTerrainTile(i);

		terrainTile->CalculateHash();
		u64 hash = terrainTile->GetHash();
		bool duplicateFound = false;

		if(hash != defaultTileHash)
		{
			std::map<u64, TerrainTileId>::iterator it = terrainTileMap.find(hash);
			if(it != terrainTileMap.end())
			{
				Duplicate duplicate;
				duplicate.original = it->second;
				duplicate.duplicate = i;
				duplicates.push_back(duplicate);
				duplicateFound = true;
			}

			if(!duplicateFound)
			{
				terrainTileMap.insert(std::make_pair(hash, i));
			}
		}
	}

	if(duplicates.size() > 0)
	{
		std::stringstream message;
		message << "Found " << duplicates.size() << " duplicate collision tiles, delete?";

		if(!prompt || wxMessageBox(message.str().c_str(), "Delete duplicate collision tiles", wxOK | wxCANCEL | wxICON_WARNING) == wxOK)
		{
			for(int i = 0; i < duplicates.size(); i++)
			{
				//Find use of duplicate id in map
				for(int x = 0; x < m_collisionMap.GetWidth(); x++)
				{
					for(int y = 0; y < m_collisionMap.GetHeight(); y++)
					{
						if(m_collisionMap.GetTerrainTile(x, y) == duplicates[i].duplicate)
						{
							//Replace duplicate with original
							m_collisionMap.SetTerrainTile(x, y, duplicates[i].original);
						}
					}
				}
			}

			//Delete duplicates (in reverse, popping from back)
			for(int i = duplicates.size() - 1; i >= 0; --i)
			{
				DeleteTerrainTile(duplicates[i].duplicate);
			}
		}
	}

	//Find and set default tile
	if(defaultTileHash)
	{
		m_defaultTerrainTile = m_terrainTileset.FindDuplicate(defaultTileHash);
	}

	return unusedTerrainTiles.size() + duplicates.size();
}

ion::gamekit::BezierPath* Project::AddTerrainBezier()
{
	m_terrainBeziers.push_back(ion::gamekit::BezierPath());
	return &m_terrainBeziers.back();
}

ion::gamekit::BezierPath* Project::GetTerrainBezier(u32 index)
{
	ion::debug::Assert(index < m_terrainBeziers.size(), "Out of range");
	return &m_terrainBeziers[index];
}

void Project::RemoveTerrainBezier(u32 index)
{
	ion::debug::Assert(index < m_terrainBeziers.size(), "Out of range");
	m_terrainBeziers.erase(m_terrainBeziers.begin() + index);
}

int Project::GetNumTerrainBeziers() const
{
	return m_terrainBeziers.size();
}

void Project::GenerateTerrainFromBeziers(int granularity)
{
	//Clear all terrain tiles
	m_terrainTileset.Clear();

	//Clear all terrain map entries
	for(int x = 0; x < m_collisionMap.GetWidth(); x++)
	{
		for(int y = 0; y < m_collisionMap.GetHeight(); y++)
		{
			m_collisionMap.SetTerrainTile(x, y, InvalidTerrainTileId);
		}
	}

	//Create blank tile, use as default
	TerrainTileId blankTileId = m_terrainTileset.AddTerrainTile();
	SetDefaultTerrainTile(blankTileId);

	const int tileSize = 8;
	const int mapHeightPixels = (m_collisionMap.GetHeight() * 8);

	//Follow paths, generate terrain height tiles
	for(int i = 0; i < m_terrainBeziers.size(); i++)
	{
		const int maxPoints = m_terrainBeziers[i].GetNumPoints();
		std::vector<ion::Vector2> points;
		points.reserve(maxPoints);
		m_terrainBeziers[i].GetPositions(points, 0.0f, 1.0f, granularity);

		for(int posIdx = 0; posIdx < points.size(); posIdx++)
		{
			//Get position
			const ion::Vector2 pixelPos(points[posIdx].x, (float)mapHeightPixels - points[posIdx].y);
			const ion::Vector2i tilePos(ion::maths::Floor(pixelPos.x / (float)tileSize), ion::maths::Floor(pixelPos.y / (float)tileSize));

			//Get tile under cursor
			TerrainTileId tileId = m_collisionMap.GetTerrainTile(tilePos.x, tilePos.y);

			if(tileId == InvalidTerrainTileId)
			{
				//Create new collision tile
				tileId = m_terrainTileset.AddTerrainTile();

				//Set on map
				m_collisionMap.SetTerrainTile(tilePos.x, tilePos.y, tileId);
			}

			//Get collision tile
			if(TerrainTile* terrainTile = m_terrainTileset.GetTerrainTile(tileId))
			{
				//Get pixel offset into tile
				int pixelX = pixelPos.x - (tilePos.x * tileSize);
				int pixelY = pixelPos.y - (tilePos.y * tileSize);

				//Set height at X
				const int height = tileSize - pixelY;
				terrainTile->SetHeight(pixelX, height);
			}
		}
	}

	//Remove duplicates
	CleanupTerrainTiles(false);

	//Reset default terrain tile
	SetDefaultTerrainTile(blankTileId);

	InvalidateTerrainTiles(false);
}

void Project::GenerateTerrain(const std::vector<ion::Vector2i>& graphicTiles)
{
	const int tileWidth = 8;
	const int tileHeight = 8;

	std::map<TileId, TerrainTileId> generatedTiles;

	for(int i = 0; i < graphicTiles.size(); i++)
	{
		const ion::Vector2i& position = graphicTiles[i];

		//Ignore the topmost tiles
		if(position.y > 0)
		{
			TileId graphicTileId = GetTileAtPosition(position);

			if(const Tile* graphicTile = m_tileset.GetTile(graphicTileId))
			{
				//Check if already generated
				std::map<TileId, TerrainTileId>::iterator it = generatedTiles.find(graphicTileId);
				if(it != generatedTiles.end())
				{
					m_collisionMap.SetTerrainTile(position.x, position.y, it->second);
				}
				else
				{
					//Generate height map from tile
					std::vector<u8> heightMap;
					GenerateHeightMap(*graphicTile, heightMap);

					//If height map isn't empty (whole row is 0)
					if(std::count(heightMap.begin(), heightMap.end(), 0) != tileWidth)
					{
						bool spaceForTerrain = true;

						//If height map fills top row (whole row of 1)
						if(std::count(heightMap.begin(), heightMap.end(), 1) == tileWidth)
						{
							//Generate ceiling map from tile above
							TileId upperTile = GetTileAtPosition(ion::Vector2i(position.x, position.y - 1));
							if(const Tile* graphicTile = m_tileset.GetTile(upperTile))
							{
								std::vector<u8> ceilingMap;
								GenerateCeilingMap(*graphicTile, ceilingMap);

								//If there's no space (whole row of tileHeight)
								if(std::count(ceilingMap.begin(), ceilingMap.end(), tileHeight) == tileWidth)
								{
									//Don't bother
									spaceForTerrain = false;
								}
							}
						}

						if(spaceForTerrain)
						{
							//Add new collision tile
							TerrainTileId terrainTileId = m_terrainTileset.AddTerrainTile();
							TerrainTile* terrainTile = m_terrainTileset.GetTerrainTile(terrainTileId);

							//Create collision tile from height map
							for(int x = 0; x < tileWidth; x++)
							{
								if(heightMap[x] > 0)
								{
									terrainTile->SetHeight(x, heightMap[x] - 1);
								}
							}

							//TODO: Find duplicate

							//Set on map
							m_collisionMap.SetTerrainTile(position.x, position.y, terrainTileId);

							//Add to cache
							generatedTiles.insert(std::make_pair(graphicTileId, terrainTileId));
						}
					}
				}
			}
		}
	}
}

void Project::GenerateHeightMap(const Tile& graphicTile, std::vector<u8>& heightMap) const
{
	const int tileWidth = 8;
	const int tileHeight = 8;

	heightMap.resize(tileWidth);
	std::fill(heightMap.begin(), heightMap.end(), 0);

	for(int x = 0; x < tileWidth; x++)
	{
		bool foundHeight = false;

		for(int y = 0; y < tileHeight && !foundHeight; y++)
		{
			//If pixel not background colour
			if(graphicTile.GetPixelColour(x, y) != 0)
			{
				//Use height
				heightMap[x] = y + 1;
				foundHeight = true;
			}
		}
	}
}

void Project::GenerateCeilingMap(const Tile& graphicTile, std::vector<u8>& ceilingMap) const
{
	const int tileWidth = 8;
	const int tileHeight = 8;

	ceilingMap.resize(tileWidth);
	std::fill(ceilingMap.begin(), ceilingMap.end(), 0);

	for(int x = 0; x < tileWidth; x++)
	{
		bool foundHeight = false;

		
		for(int y = tileHeight - 1; y >= 0 && !foundHeight; y--)
		{
			//If pixel not background colour
			if(graphicTile.GetPixelColour(x, y) != 0)
			{
				//Use height
				ceilingMap[x] = y + 1;
				foundHeight = true;
			}
		}
	}
}

TileId Project::GetTileAtPosition(const ion::Vector2i& position)
{
	TileId tileId = InvalidTileId;

	//Find stamp under cursor first
	ion::Vector2i stampPos;
	u32 stampFlags = 0;
	StampId stampId = m_map.FindStamp(position.x, position.y, stampPos, stampFlags);
	if(stampId)
	{
		//Get from stamp
		if(Stamp* stamp = GetStamp(stampId))
		{
			ion::Vector2i offset = position - stampPos;
			tileId = stamp->GetTile(offset.x, offset.y);
		}
	}
	else
	{
		//Pick tile
		tileId = m_map.GetTile(position.x, position.y);
	}

	return tileId;
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

	if(typeId != InvalidGameObjectTypeId)
	{
		TGameObjectTypeMap::iterator it = m_gameObjectTypes.find(typeId);
		if(it != m_gameObjectTypes.end())
			gameObjectType = &it->second;
	}
	
	return gameObjectType;
}

const GameObjectType* Project::GetGameObjectType(GameObjectTypeId typeId) const
{
	const GameObjectType* gameObjectType = NULL;

	if(typeId != InvalidGameObjectTypeId)
	{
		TGameObjectTypeMap::const_iterator it = m_gameObjectTypes.find(typeId);
		if(it != m_gameObjectTypes.end())
			gameObjectType = &it->second;
	}

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

void Project::SetPaintTerrainTile(TerrainTileId tile)
{
	m_paintTerrainTile = tile;
}

TerrainTileId Project::GetPaintTerrainTile() const
{
	return m_paintTerrainTile;
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
			m_collisionMap.Resize(max(m_collisionMap.GetWidth(), tilesWidth), max(m_collisionMap.GetHeight(), tilesHeight));
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

bool Project::ExportTiles(const std::string& filename, bool binary) const
{
	u32 binarySize = 0;

	if(binary)
	{
		std::string binaryFilename = filename.substr(0, filename.find_first_of('.'));
		binaryFilename += ".bin";

		//Export binary data
		ion::io::File binaryFile(binaryFilename, ion::io::File::OpenWrite);
		if(binaryFile.IsOpen())
		{
			m_tileset.Export(binaryFile);
			binarySize = binaryFile.GetSize();
		}
		else
		{
			return false;
		}
	}

	ion::io::File file(filename, ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		WriteFileHeader(stream);

		if(binary)
		{
			//Export size of binary file
			stream << "tiles_" << m_name << "_size_b\tequ 0x" << std::hex << std::setfill('0') << std::uppercase << std::setw(8) << binarySize << std::dec << "\t; Size in bytes" << std::endl;
		}
		else
		{
			//Export label, data and size as inline text
			stream << "tiles_" << m_name << ":" << std::endl;

			m_tileset.Export(stream);

			stream << std::endl;
			stream << "tiles_" << m_name << "_end" << std::endl;
			stream << "tiles_" << m_name << "_size_b\tequ (tiles_" << m_name << "_end-tiles_" << m_name << ")\t; Size in bytes" << std::endl;
		}
		
		stream << "tiles_" << m_name << "_size_w\tequ (tiles_" << m_name << "_size_b/2)\t; Size in words" << std::endl;
		stream << "tiles_" << m_name << "_size_l\tequ (tiles_" << m_name << "_size_b/4)\t; Size in longwords" << std::endl;
		stream << "tiles_" << m_name << "_size_t\tequ (tiles_" << m_name << "_size_b/32)\t; Size in tiles" << std::endl;

		file.Write(stream.str().c_str(), stream.str().size());

		return true;
	}

	return false;
}

bool Project::ExportTerrainTiles(const std::string& filename, bool binary) const
{
	u32 binarySize = 0;

	if(binary)
	{
		std::string binaryFilename = filename.substr(0, filename.find_first_of('.'));
		binaryFilename += ".bin";

		//Export binary data
		ion::io::File binaryFile(binaryFilename, ion::io::File::OpenWrite);
		if(binaryFile.IsOpen())
		{
			m_terrainTileset.Export(binaryFile);
			binarySize = binaryFile.GetSize();
		}
		else
		{
			return false;
		}
	}

	ion::io::File file(filename, ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		WriteFileHeader(stream);

		if(binary)
		{
			//Export size of binary file
			stream << "TerrainTiles_" << m_name << "_size_b\tequ 0x" << std::hex << std::setfill('0') << std::uppercase << std::setw(8) << binarySize << std::dec << "\t; Size in bytes" << std::endl;
		}
		else
		{
			//Export label, data and size as inline text
			stream << "TerrainTiles_" << m_name << ":" << std::endl;

			m_terrainTileset.Export(stream);

			stream << std::endl;
			stream << "TerrainTiles_" << m_name << "_end" << std::endl;
			stream << "TerrainTiles_" << m_name << "_size_b\tequ (TerrainTiles_" << m_name << "_end-TerrainTiles_" << m_name << ")\t; Size in bytes" << std::endl;
		}

		stream << "TerrainTiles_" << m_name << "_size_w\tequ (TerrainTiles_" << m_name << "_size_b/2)\t; Size in words" << std::endl;
		stream << "TerrainTiles_" << m_name << "_size_l\tequ (TerrainTiles_" << m_name << "_size_b/4)\t; Size in longwords" << std::endl;
		stream << "TerrainTiles_" << m_name << "_size_t\tequ (TerrainTiles_" << m_name << "_size_b/32)\t; Size in tiles" << std::endl;

		file.Write(stream.str().c_str(), stream.str().size());

		return true;
	}

	return false;
}

bool Project::ExportMap(const std::string& filename, bool binary) const
{
	u32 binarySize = 0;

	if(binary)
	{
		std::string binaryFilename = filename.substr(0, filename.find_first_of('.'));
		binaryFilename += ".bin";

		//Export binary data
		ion::io::File binaryFile(binaryFilename, ion::io::File::OpenWrite);
		if(binaryFile.IsOpen())
		{
			m_map.Export(*this, binaryFile);
			binarySize = binaryFile.GetSize();
		}
		else
		{
			return false;
		}
	}

	ion::io::File file(filename, ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		WriteFileHeader(stream);
		
		if(binary)
		{
			//Export size of binary file
			stream << "map_" << m_name << "_size_b\tequ 0x" << std::hex << std::setfill('0') << std::uppercase << std::setw(8) << binarySize << std::dec << "\t; Size in bytes" << std::endl;
		}
		else
		{
			//Export label, data and size as inline text
			stream << "map_" << m_name << ":" << std::endl;

			m_map.Export(*this, stream);

			stream << std::endl;
			stream << "map_" << m_name << "_end:" << std::endl;
			stream << "map_" << m_name << "_size_b\tequ (map_" << m_name << "_end-map_" << m_name << ")\t; Size in bytes" << std::endl;
		}

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

bool Project::ExportCollisionMap(const std::string& filename, bool binary) const
{
	u32 binarySize = 0;

	if(binary)
	{
		std::string binaryFilename = filename.substr(0, filename.find_first_of('.'));
		binaryFilename += ".bin";

		//Export binary data
		ion::io::File binaryFile(binaryFilename, ion::io::File::OpenWrite);
		if(binaryFile.IsOpen())
		{
			m_collisionMap.Export(*this, binaryFile);
			binarySize = binaryFile.GetSize();
		}
		else
		{
			return false;
		}
	}

	ion::io::File file(filename, ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		WriteFileHeader(stream);
		
		if(binary)
		{
			//Export size of binary file
			stream << "collisionmap_" << m_name << "_size_b\tequ 0x" << std::hex << std::setfill('0') << std::uppercase << std::setw(8) << binarySize << std::dec << "\t; Size in bytes" << std::endl;
		}
		else
		{
			//Export label, data and size as inline text
			stream << "collisionmap_" << m_name << ":" << std::endl;

			m_collisionMap.Export(*this, stream);

			stream << std::endl;
			stream << "collisionmap_" << m_name << "_end:" << std::endl;
			stream << "collisionmap_" << m_name << "_size_b\tequ (collisionmap_" << m_name << "_end-collisionmap_" << m_name << ")\t; Size in bytes" << std::endl;
		}

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
		stream << "gameobjects_" << m_name << ":" << std::endl;

		const TGameObjectPosMap& gameObjMap = m_map.GetGameObjects();

		//Bake out all game object types, even if there's no game objects to go with them (still need the count and init subroutine)
		for(TGameObjectTypeMap::const_iterator it = m_gameObjectTypes.begin(), end = m_gameObjectTypes.end(); it != end; ++it)
		{
			const GameObjectType& gameObjectType = it->second;
			const TGameObjectPosMap::const_iterator gameObjIt = gameObjMap.find(it->first);
			u32 count = (gameObjIt != gameObjMap.end()) ? gameObjIt->second.size() : 0;
			
			//Export game object count
			stream << "gameobjects_" << m_name << "_" << gameObjectType.GetName() << "_count equ " << count << std::endl;

			//Export game object init sunroutine
			stream << "LoadGameObjects_" << m_name << "_" << gameObjectType.GetName() << ":" << std::endl;

			//Export all game objects of this type
			if(gameObjIt != gameObjMap.end())
			{
				for(int i = 0; i < gameObjIt->second.size(); i++)
				{
					gameObjIt->second[i].m_gameObject.Export(stream, gameObjectType);
					stream << '\t' << "add.l #" << gameObjectType.GetName() << "_Struct_Size, a0" << std::endl;
				}
			}

			//End subroutine
			stream << '\t' << "rts" << std::endl;
			stream << std::endl;
		}

		file.Write(stream.str().c_str(), stream.str().size());

		return true;
	}

	return false;
}