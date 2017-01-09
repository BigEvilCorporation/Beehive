///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#include <ion/io/Archive.h>
#include <ion/gamekit/Bezier.h>

#include <set>
#include <vector>
#include <algorithm>

#include "Project.h"
#include "BMPReader.h"

#define HEX1(val) std::hex << std::setfill('0') << std::setw(1) << std::uppercase << (int)##val
#define HEX2(val) std::hex << std::setfill('0') << std::setw(2) << std::uppercase << (int)##val
#define HEX4(val) std::hex << std::setfill('0') << std::setw(4) << std::uppercase << (int)##val
#define HEX8(val) std::hex << std::setfill('0') << std::setw(8) << std::uppercase << (int)##val

Project::Project(PlatformConfig& defaultPatformConfig)
	: m_platformConfig(defaultPatformConfig)
	, m_tileset(m_platformConfig)
	, m_terrainTileset(m_platformConfig)
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
	m_showDisplayFrame = false;
	m_palettes.resize(s_maxPalettes);
	m_nextFreeStampId = 1;
	m_nextFreeGameObjectTypeId = 1;

	//Add default map
	m_editingMapId = CreateMap();
	m_editingCollisionMapId = CreateCollisionMap(m_editingMapId);
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

	m_maps[m_editingMapId].Clear();
	m_collisionMaps[m_editingCollisionMapId].Clear();
	m_tileset.Clear();
	m_stamps.clear();
	m_nextFreeStampId = 1;
	m_nextFreeGameObjectTypeId = 1;
}

bool Project::Load(const std::string& filename)
{
	ion::io::File file(filename, ion::io::File::eOpenRead);
	if(file.IsOpen())
	{
		ion::io::Archive archive(file, ion::io::Archive::eIn);
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
	ion::io::File file(filename, ion::io::File::eOpenWrite);
	if(file.IsOpen())
	{
		ion::io::Archive archive(file, ion::io::Archive::eOut);
		Serialise(archive);
		m_filename = filename;
		return true;
	}

	return false;
}

void Project::Serialise(ion::io::Archive& archive)
{
	if(archive.GetDirection() == ion::io::Archive::eIn)
	{
		m_maps.clear();
		m_collisionMaps.clear();
		m_editingMapId = InvalidMapId;
		m_editingCollisionMapId = InvalidCollisionMapId;
	}

	archive.Serialise(m_platformConfig, "platformConfig");
	archive.Serialise(m_name, "name");
	archive.Serialise(m_palettes, "palettes");
	archive.Serialise(m_paletteSlots, "paletteSlots");
	archive.Serialise(m_tileset, "tileset");
	archive.Serialise(m_maps, "maps");
	archive.Serialise(m_editingMapId, "editingMap");
	archive.Serialise(m_editingCollisionMapId, "editingCollisionMap");
	archive.Serialise(m_backgroundTile, "backgroundTile");
	archive.Serialise(m_terrainTileset, "terrainTileset");
	archive.Serialise(m_defaultTerrainTile, "defaultTerrainTile");
	archive.Serialise(m_collisionMaps, "collisionMaps");
	archive.Serialise(m_stamps, "stamps");
	archive.Serialise(m_actors, "actors");
	archive.Serialise(m_animations, "animations");
	archive.Serialise(m_stampAnimations, "stampAnimations");
	archive.Serialise(m_gameObjectTypes, "gameObjectTypes");
	archive.Serialise(m_nextFreeStampId, "nextFreeStampId");
	archive.Serialise(m_nextFreeGameObjectTypeId, "nextFreeGameObjectTypeId");
	archive.Serialise(m_exportFilenames, "exportFilenames");

	if(archive.GetDirection() == ion::io::Archive::eIn && m_maps.size() == 0)
	{
		//Legacy, single map
		m_editingMapId = CreateMap();
		archive.Serialise(m_maps[m_editingMapId], "map");
	}

	if(archive.GetDirection() == ion::io::Archive::eIn && m_collisionMaps.size() != m_maps.size())
	{
		//Legacy, single collision map
		if(m_collisionMaps.size() == 0)
		{
			m_editingCollisionMapId = CreateCollisionMap(m_editingMapId);
			archive.Serialise(m_collisionMaps[m_editingCollisionMapId], "collisionMap");

			std::vector<ion::gamekit::BezierPath> terrainBeziers;
			archive.Serialise(terrainBeziers, "terrainBeziers");

			for(int i = 0; i < terrainBeziers.size(); i++)
			{
				m_collisionMaps[m_editingCollisionMapId].AddTerrainBezier(terrainBeziers[i]);
			}
		}

		//Create remaining collision maps
		for(TMapMap::const_iterator it = m_maps.begin(), end = m_maps.end(); it != end; ++it)
		{
			if(it->first != m_editingCollisionMapId)
			{
				CollisionMapId collisionMapId = CreateCollisionMap(it->first, it->second.GetWidth(), it->second.GetHeight());
			}
		}
	}
}

MapId Project::CreateMap()
{
	MapId mapId = ion::GenerateUUID64();
	m_maps.insert(std::make_pair(mapId, Map(m_platformConfig)));
	return mapId;
}

MapId Project::CreateMap(MapId mapId)
{
	m_maps.insert(std::make_pair(mapId, Map(m_platformConfig)));
	return mapId;
}

void Project::DeleteMap(MapId mapId)
{
	TMapMap::iterator it = m_maps.find(mapId);
	ion::debug::Assert(it != m_maps.end(), "Project::DeleteMap() - Invalid map ID");
	m_maps.erase(it);
}

Map& Project::GetMap(MapId mapId)
{
	TMapMap::iterator it = m_maps.find(mapId);
	ion::debug::Assert(it != m_maps.end(), "Project::GetMap() - Invalid map ID");
	return it->second;
}

const Map& Project::GetMap(MapId mapId) const
{
	TMapMap::const_iterator it = m_maps.find(mapId);
	ion::debug::Assert(it != m_maps.end(), "Project::GetMap() - Invalid map ID");
	return it->second;
}

Map& Project::GetEditingMap()
{
	return GetMap(m_editingMapId);
}

MapId Project::GetEditingMapId() const
{
	return m_editingMapId;
}

void Project::SetEditingMap(MapId mapId)
{
	m_editingMapId = mapId;
}

const TMapMap::const_iterator Project::MapsBegin() const
{
	return m_maps.begin();
}

const TMapMap::const_iterator Project::MapsEnd() const
{
	return m_maps.end();
}

TMapMap::iterator Project::MapsBegin()
{
	return m_maps.begin();
}

TMapMap::iterator Project::MapsEnd()
{
	return m_maps.end();
}

int Project::GetMapCount() const
{
	return m_maps.size();
}

CollisionMapId Project::CreateCollisionMap(CollisionMapId collisionMapId)
{
	m_collisionMaps.insert(std::make_pair(collisionMapId, CollisionMap(m_platformConfig)));
	return collisionMapId;
}

CollisionMapId Project::CreateCollisionMap(CollisionMapId collisionMapId, int width, int height)
{
	m_collisionMaps.insert(std::make_pair(collisionMapId, CollisionMap(m_platformConfig, width, height)));
	return collisionMapId;
}

void Project::DeleteCollisionMap(CollisionMapId collisionMapId)
{
	TCollisionMapMap::iterator it = m_collisionMaps.find(collisionMapId);
	ion::debug::Assert(it != m_collisionMaps.end(), "Project::DeleteCollisionMap() - Invalid collisionMap ID");
	m_collisionMaps.erase(it);
}

CollisionMap& Project::GetCollisionMap(CollisionMapId collisionMapId)
{
	TCollisionMapMap::iterator it = m_collisionMaps.find(collisionMapId);
	ion::debug::Assert(it != m_collisionMaps.end(), "Project::GetEditingCollisionMap() - Invalid collisionMap ID");
	return it->second;
}

const CollisionMap& Project::GetCollisionMap(CollisionMapId collisionMapId) const
{
	TCollisionMapMap::const_iterator it = m_collisionMaps.find(collisionMapId);
	ion::debug::Assert(it != m_collisionMaps.end(), "Project::GetEditingCollisionMap() - Invalid collisionMap ID");
	return it->second;
}

CollisionMap& Project::GetEditingCollisionMap()
{
	return GetCollisionMap(m_editingCollisionMapId);
}

CollisionMapId Project::GetEditingCollisionMapId() const
{
	return m_editingCollisionMapId;
}

void Project::SetEditingCollisionMap(CollisionMapId collisionMapId)
{
	m_editingCollisionMapId = collisionMapId;
}

const TCollisionMapMap::const_iterator Project::CollisionMapsBegin() const
{
	return m_collisionMaps.begin();
}

const TCollisionMapMap::const_iterator Project::CollisionMapsEnd() const
{
	return m_collisionMaps.end();
}

int Project::GetCollisionMapCount() const
{
	return m_collisionMaps.size();
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

const Palette* Project::GetPaletteSlot(int slotIndex) const
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
	ion::io::File file(filename, ion::io::File::eOpenWrite);
	if(file.IsOpen())
	{
		ion::io::Archive archive(file, ion::io::Archive::eOut);
		archive.Serialise(m_paletteSlots, "paletteSlots");
	}
}

void Project::ImportPaletteSlots(const std::string& filename)
{
	ion::io::File file(filename, ion::io::File::eOpenRead);
	if(file.IsOpen())
	{
		ion::io::Archive archive(file, ion::io::Archive::eIn);
		archive.Serialise(m_paletteSlots, "paletteSlots");
	}
}

void Project::SetBackgroundColour(u8 colourIdx)
{
	SwapPaletteEntries(colourIdx, 0);
}

void Project::SwapPaletteEntries(u8 colourA, u8 colourB)
{
	for(int i = 0; i < m_tileset.GetCount(); i++)
	{
		if(Tile* tile = m_tileset.GetTile(i))
		{
			for(int x = 0; x < m_platformConfig.tileWidth; x++)
			{
				for(int y = 0; y < m_platformConfig.tileHeight; y++)
				{
					u8 originalIdx = tile->GetPixelColour(x, y);
					if(originalIdx == colourB)
					{
						tile->SetPixelColour(x, y, colourA);
					}
					else if(originalIdx == colourA)
					{
						tile->SetPixelColour(x, y, colourB);
					}
				}
			}
		}
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

	//Find all uses of tile on all maps, set blank
	for(TMapMap::iterator it = m_maps.begin(), end = m_maps.end(); it != end; ++it)
	{
		Map& map = it->second;
		int mapWidth = map.GetWidth();
		int mapHeight = map.GetHeight();

		for(int x = 0; x < mapWidth; x++)
		{
			for(int y = 0; y < mapHeight; y++)
			{
				TileId currTileId = map.GetTile(x, y);
				if(currTileId == tileId)
				{
					//Referencing deleted tile, set as invalid
					map.SetTile(x, y, InvalidTileId);
				}
				else if(currTileId == swapTileId)
				{
					//Referencing swap tile, set new id
					u32 flags = map.GetTileFlags(x, y);
					map.SetTile(x, y, tileId);
					map.SetTileFlags(x, y, flags);
				}
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

	//Find all uses of tiles on all maps, swap ids
	for(TMapMap::iterator it = m_maps.begin(), end = m_maps.end(); it != end; ++it)
	{
		Map& map = it->second;
		int mapWidth = map.GetWidth();
		int mapHeight = map.GetHeight();

		for(int x = 0; x < mapWidth; x++)
		{
			for(int y = 0; y < mapHeight; y++)
			{
				TileId currTileId = map.GetTile(x, y);
				if(currTileId == tileId1)
				{
					map.SetTile(x, y, tileId2);
				}
				else if(currTileId == tileId2)
				{
					map.SetTile(x, y, tileId1);
				}
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
	m_tileset.RebuildHashMap();

	std::set<TileId> usedTiles;

	//Collect all used tile ids from all maps
	for(TMapMap::iterator it = m_maps.begin(), end = m_maps.end(); it != end; ++it)
	{
		Map& map = it->second;
		int mapWidth = map.GetWidth();
		int mapHeight = map.GetHeight();

		for(int x = 0; x < mapWidth; x++)
		{
			for(int y = 0; y < mapHeight; y++)
			{
				TileId tileId = map.GetTile(x, y);

				//Ignore background tile
				if(tileId != m_backgroundTile)
				{
					usedTiles.insert(tileId);
				}
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

		//TODO: wx message handler in ion::debug
		//if(wxMessageBox(message.str().c_str(), "Delete unused tiles", wxOK | wxCANCEL | wxICON_WARNING) == wxOK)
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

		//TODO: wx message handler in ion::debug
		//if(wxMessageBox(message.str().c_str(), "Delete duplicate tiles", wxOK | wxCANCEL | wxICON_WARNING) == wxOK)
		{
			for(int i = 0; i < duplicates.size(); i++)
			{
				//Find use of duplicate id in all maps
				for(TMapMap::iterator it = m_maps.begin(), end = m_maps.end(); it != end; ++it)
				{
					Map& map = it->second;
					int mapWidth = map.GetWidth();
					int mapHeight = map.GetHeight();

					for(int x = 0; x < mapWidth; x++)
					{
						for(int y = 0; y < mapHeight; y++)
						{
							if(map.GetTile(x, y) == duplicates[i].duplicate)
							{
								u32 originalFlags = map.GetTileFlags(x, y);

								//Replace duplicate with original
								map.SetTile(x, y, duplicates[i].original);

								//Orientate to match duplicate
								map.SetTileFlags(x, y, originalFlags ^ Tileset::s_orientationFlags[duplicates[i].duplicateOrientation]);
							}
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

	m_tileset.RebuildHashMap();

	return unusedTiles.size() + duplicates.size();
}

ActorId Project::CreateActor()
{
	ActorId actorId = ion::GenerateUUID64();
	m_actors.insert(std::make_pair(actorId, Actor()));
	return actorId;
}

void Project::DeleteActor(ActorId actorId)
{
	TActorMap::iterator it = m_actors.find(actorId);
	if(it != m_actors.end())
	{
		m_actors.erase(it);
	}
}

Actor* Project::GetActor(ActorId actorId)
{
	Actor* actor = NULL;

	TActorMap::iterator it = m_actors.find(actorId);
	if(it != m_actors.end())
	{
		actor = &it->second;
	}

	return actor;
}

const Actor* Project::GetActor(ActorId actorId) const
{
	const Actor* actor = NULL;

	TActorMap::const_iterator it = m_actors.find(actorId);
	if(it != m_actors.end())
	{
		actor = &it->second;
	}

	return actor;
}

const TActorMap::const_iterator Project::ActorsBegin() const
{
	return m_actors.begin();
}

const TActorMap::const_iterator Project::ActorsEnd() const
{
	return m_actors.end();
}

int Project::GetActorCount() const
{
	return m_actors.size();
}

AnimationId Project::CreateAnimation()
{
	AnimationId animationId = ion::GenerateUUID64();
	m_animations.insert(std::make_pair(animationId, Animation()));
	return animationId;
}

void Project::DeleteAnimation(AnimationId animationId)
{
	TAnimationMap::iterator it = m_animations.find(animationId);
	if(it != m_animations.end())
	{
		m_animations.erase(it);
	}
}

Animation* Project::GetAnimation(AnimationId animationId)
{
	Animation* animation = NULL;

	TAnimationMap::iterator it = m_animations.find(animationId);
	if(it != m_animations.end())
	{
		animation = &it->second;
	}

	return animation;
}

const Animation* Project::GetAnimation(AnimationId animationId) const
{
	const Animation* animation = NULL;

	TAnimationMap::const_iterator it = m_animations.find(animationId);
	if(it != m_animations.end())
	{
		animation = &it->second;
	}

	return animation;
}

const TAnimationMap::const_iterator Project::AnimationsBegin() const
{
	return m_animations.begin();
}

const TAnimationMap::const_iterator Project::AnimationsEnd() const
{
	return m_animations.end();
}

int Project::GetAnimationCount() const
{
	return m_animations.size();
}

StampAnimId Project::CreateStampAnimation()
{
	StampAnimId animationId = ion::GenerateUUID64();
	m_stampAnimations.insert(std::make_pair(animationId, StampAnimation()));
	return animationId;
}

void Project::DeleteStampAnimation(StampAnimId animationId)
{
	TStampAnimationMap::iterator it = m_stampAnimations.find(animationId);
	if(it != m_stampAnimations.end())
	{
		m_stampAnimations.erase(it);
	}
}

StampAnimation* Project::GetStampAnimation(StampAnimId animationId)
{
	StampAnimation* animation = NULL;

	TStampAnimationMap::iterator it = m_stampAnimations.find(animationId);
	if(it != m_stampAnimations.end())
	{
		animation = &it->second;
	}

	return animation;
}

const StampAnimation* Project::GetStampAnimation(StampAnimId animationId) const
{
	const StampAnimation* animation = NULL;

	TStampAnimationMap::const_iterator it = m_stampAnimations.find(animationId);
	if(it != m_stampAnimations.end())
	{
		animation = &it->second;
	}

	return animation;
}

const TStampAnimationMap::const_iterator Project::StampAnimationsBegin() const
{
	return m_stampAnimations.begin();
}

const TStampAnimationMap::const_iterator Project::StampAnimationsEnd() const
{
	return m_stampAnimations.end();
}

int Project::GetStampAnimationCount() const
{
	return m_stampAnimations.size();
}

StampId Project::AddStamp(int width, int height)
{
	StampId id = m_nextFreeStampId++;
	m_stamps.insert(std::make_pair(id, Stamp(id, width, height)));
	return id;
}

StampId Project::AddStamp(Stamp* stamp)
{
	StampId id = m_nextFreeStampId++;
	m_stamps.insert(std::make_pair(id, Stamp(id, *stamp)));
	return id;
}

void Project::DeleteStamp(StampId stampId)
{
	//Remove all uses of stamp on all maps
	std::vector<ion::Vector2i> stampUseCoords;

	for(TMapMap::iterator it = m_maps.begin(), end = m_maps.end(); it != end; ++it)
	{
		Map& map = it->second;
		int mapWidth = map.GetWidth();
		int mapHeight = map.GetHeight();

		for(TStampPosMap::const_iterator it = map.StampsBegin(), end = map.StampsEnd(); it != end; ++it)
		{
			if(it->m_id == stampId)
			{
				stampUseCoords.push_back(it->m_position);
			}
		}

		for(int i = 0; i < stampUseCoords.size(); i++)
		{
			map.RemoveStamp(stampUseCoords[i].x, stampUseCoords[i].y);
		}
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

StampId Project::FindDuplicateStamp(Stamp* stamp) const
{
	StampId foundStampId = InvalidStampId;

	for(TStampMap::const_iterator it = m_stamps.begin(), end = m_stamps.end(); it != end && foundStampId == InvalidStampId; ++it)
	{
		if(it->second.GetWidth() == stamp->GetWidth() && it->second.GetHeight() == stamp->GetHeight())
		{
			bool tilesMatch = true;

			for(int x = 0; x < it->second.GetWidth() && tilesMatch; x++)
			{
				for(int y = 0; y < it->second.GetHeight() && tilesMatch; y++)
				{
					const Tile* tileA = GetTileset().GetTile(stamp->GetTile(x, y));
					const Tile* tileB = GetTileset().GetTile(it->second.GetTile(x, y));
					const u64 hashA = tileA ? tileA->GetHash() : 0;
					const u64 hashB = tileB ? tileB->GetHash() : 0;
					const u32 flagsA = stamp->GetTileFlags(x, y);
					const u32 flagsB = it->second.GetTileFlags(x, y);

					if(((tileA != NULL) != (tileB != NULL)) || (hashA != hashB) || (flagsA != flagsB))
					{
						tilesMatch = false;
					}
				}
			}

			if(tilesMatch)
				foundStampId = it->first;
		}
	}

	return foundStampId;
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

void Project::SubstituteStamp(StampId stampToReplace, StampId substitution)
{
	for(TMapMap::iterator mapIt = m_maps.begin(), mapEnd = m_maps.end(); mapIt != mapEnd; ++mapIt)
	{
		for(TStampPosMap::iterator stampIt = mapIt->second.GetStamps().begin(), stampEnd = mapIt->second.GetStamps().end(); stampIt != stampEnd; ++stampIt)
		{
			if(stampIt->m_id == stampToReplace)
			{
				stampIt->m_id = substitution;
			}
		}
	}
}

int Project::CleanupStamps()
{
	std::set<StampId> unusedStamps;

	for(TStampMap::const_iterator it = m_stamps.begin(), end = m_stamps.end(); it != end; ++it)
	{
		unusedStamps.insert(it->first);
	}

	for(TMapMap::const_iterator mapIt = m_maps.begin(), mapEnd = m_maps.end(); mapIt != mapEnd; ++mapIt)
	{
		for(TStampPosMap::const_iterator stampIt = mapIt->second.GetStamps().begin(), stampEnd = mapIt->second.GetStamps().end(); stampIt != stampEnd; ++stampIt)
		{
			unusedStamps.erase(stampIt->m_id);
		}
	}
	
	if(unusedStamps.size() > 0)
	{
		std::stringstream message;
		message << "Found " << unusedStamps.size() << " unused stamps, delete?";

		//TODO: wx message handler in ion::debug
		//if(wxMessageBox(message.str().c_str(), "Delete unused stamps", wxOK | wxCANCEL | wxICON_WARNING) == wxOK)
		{
			for(std::set<StampId>::const_iterator it = unusedStamps.begin(), end = unusedStamps.end(); it != end; ++it)
			{
				DeleteStamp(*it);
			}
		}
	}

	return unusedStamps.size();
}

void Project::DeleteTerrainTile(TerrainTileId terrainTileId)
{
	TerrainTileId swapTerrainTileId = (TerrainTileId)m_terrainTileset.GetCount() - 1;

	//Find all uses of terrain tile on all maps, set blank
	for(TCollisionMapMap::iterator it = m_collisionMaps.begin(), end = m_collisionMaps.end(); it != end; ++it)
	{
		CollisionMap& collisionMap = it->second;
		int mapWidth = collisionMap.GetWidth();
		int mapHeight = collisionMap.GetHeight();

		for(int x = 0; x < collisionMap.GetWidth(); x++)
		{
			for(int y = 0; y < collisionMap.GetHeight(); y++)
			{
				TerrainTileId currTerrainTileId = collisionMap.GetTerrainTile(x, y);
				if(currTerrainTileId == terrainTileId)
				{
					//Referencing deleted tile, set as invalid
					collisionMap.SetTerrainTile(x, y, InvalidTerrainTileId);
				}
				else if(currTerrainTileId == swapTerrainTileId)
				{
					//Referencing swap tile, set new id
					collisionMap.SetTerrainTile(x, y, terrainTileId);
				}
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

	//Collect all used TerrainTile ids from all maps
	std::vector<bool> usedTerrainTiles;
	usedTerrainTiles.resize(m_terrainTileset.GetCount(), false);
	bool usedTileCount = 0;

	//Always add default tile
	if(m_defaultTerrainTile != InvalidTerrainTileId)
	{
		usedTerrainTiles[m_defaultTerrainTile] = true;
	}

	for(TCollisionMapMap::iterator it = m_collisionMaps.begin(), end = m_collisionMaps.end(); it != end; ++it)
	{
		CollisionMap& collisionMap = it->second;
		int mapWidth = collisionMap.GetWidth();
		int mapHeight = collisionMap.GetHeight();

		for(int x = 0; x < mapWidth; x++)
		{
			for(int y = 0; y < mapHeight; y++)
			{
				TerrainTileId terrainTileId = collisionMap.GetTerrainTile(x, y);

				if(terrainTileId != InvalidTerrainTileId)
				{
					usedTerrainTiles[terrainTileId] = true;
					usedTileCount++;
				}
			}
		}
	}

	std::vector<TerrainTileId> unusedTerrainTiles;
	unusedTerrainTiles.reserve(m_terrainTileset.GetCount() - usedTileCount);

	for(TerrainTileId i = 0; i < m_terrainTileset.GetCount(); i++)
	{
		if(!usedTerrainTiles[i])
		{
			unusedTerrainTiles.push_back(i);
		}
	}

	if(unusedTerrainTiles.size() > 0)
	{
		std::stringstream message;
		message << "Found " << unusedTerrainTiles.size() << " unused collision tiles, delete?";

		//Sort
		std::sort(unusedTerrainTiles.begin(), unusedTerrainTiles.end());

		//TODO: wx message handler in ion::debug
		//if(!prompt || wxMessageBox(message.str().c_str(), "Delete unused collision tiles", wxOK | wxCANCEL | wxICON_WARNING) == wxOK)
		{
			//Delete in reverse, popping from back
			for(std::vector<TerrainTileId>::const_reverse_iterator it = unusedTerrainTiles.rbegin(), end = unusedTerrainTiles.rend(); it != end; ++it)
			{
				DeleteTerrainTile(*it);
			}
		}
	}

	//Rebuild hash map
	GetTerrainTileset().RebuildHashMap();

	//Default might have moved
	m_defaultTerrainTile = GetTerrainTileset().FindDuplicate(defaultTileHash);

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

		if(i != m_defaultTerrainTile)
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

		//TODO: wx message handler in ion::debug
		//if(!prompt || wxMessageBox(message.str().c_str(), "Delete duplicate collision tiles", wxOK | wxCANCEL | wxICON_WARNING) == wxOK)
		{
			for(int i = 0; i < duplicates.size(); i++)
			{
				//Find use of duplicate id in all maps
				for(TCollisionMapMap::iterator it = m_collisionMaps.begin(), end = m_collisionMaps.end(); it != end; ++it)
				{
					CollisionMap& collisionMap = it->second;

					for(int x = 0; x < collisionMap.GetWidth(); x++)
					{
						for(int y = 0; y < collisionMap.GetHeight(); y++)
						{
							if(collisionMap.GetTerrainTile(x, y) == duplicates[i].duplicate)
							{
								//Replace duplicate with original
								collisionMap.SetTerrainTile(x, y, duplicates[i].original);
							}
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

	//Rebuild hash map
	GetTerrainTileset().RebuildHashMap();

	//Default might have moved
	m_defaultTerrainTile = GetTerrainTileset().FindDuplicate(defaultTileHash);

	return unusedTerrainTiles.size() + duplicates.size();
}

bool Project::GenerateTerrainFromBeziers(int granularity)
{
	//Clear all terrain tiles
	m_terrainTileset.Clear();

	//Create blank tile, use as default
	TerrainTileId blankTileId = m_terrainTileset.AddTerrainTile();
	SetDefaultTerrainTile(blankTileId);

	//Clear all terrain entries from all maps
	for(TCollisionMapMap::iterator it = m_collisionMaps.begin(), end = m_collisionMaps.end(); it != end; ++it)
	{
		CollisionMap& collisionMap = it->second;
		int mapWidth = collisionMap.GetWidth();
		int mapHeight = collisionMap.GetHeight();

		for(int x = 0; x < mapWidth; x++)
		{
			for(int y = 0; y < mapHeight; y++)
			{
				collisionMap.SetTerrainTile(x, y, InvalidTerrainTileId);
			}
		}
	}

	for(TCollisionMapMap::iterator it = m_collisionMaps.begin(), end = m_collisionMaps.end(); it != end; ++it)
	{
		CollisionMap& collisionMap = it->second;
		int mapWidth = collisionMap.GetWidth();
		int mapHeight = collisionMap.GetHeight();

		const int tileWidth = GetPlatformConfig().tileWidth;
		const int tileHeight = GetPlatformConfig().tileHeight;

		const int mapHeightPixels = (collisionMap.GetHeight() * tileHeight);

		//Follow paths, generate terrain height tiles
		for(int i = 0; i < collisionMap.GetNumTerrainBeziers(); i++)
		{
			const ion::gamekit::BezierPath* bezier = collisionMap.GetTerrainBezier(i);
			const int maxPoints = bezier->GetNumPoints();

			if(maxPoints > 0)
			{
				std::vector<ion::Vector2> points;
				points.reserve(maxPoints);
				bezier->GetPositions(points, 0.0f, 1.0f, granularity);

				for(int posIdx = 0; posIdx < points.size(); posIdx++)
				{
					//Get position
					const ion::Vector2 pixelPos(points[posIdx].x, (float)mapHeightPixels - points[posIdx].y);
					const ion::Vector2i tilePos(ion::maths::Floor(pixelPos.x / (float)tileWidth), ion::maths::Floor(pixelPos.y / (float)tileHeight));

					//Get tile under cursor
					TerrainTileId tileId = collisionMap.GetTerrainTile(tilePos.x, tilePos.y);

					if(tileId == InvalidTerrainTileId)
					{
						//Create new collision tile
						tileId = m_terrainTileset.AddTerrainTile();

						if(tileId == InvalidTerrainTileId)
						{
							//Out of tiles
							return false;
						}

						//Set on map
						collisionMap.SetTerrainTile(tilePos.x, tilePos.y, tileId);
					}

					//Get collision tile
					if(TerrainTile* terrainTile = m_terrainTileset.GetTerrainTile(tileId))
					{
						//Get pixel offset into tile
						int pixelX = pixelPos.x - (tilePos.x * tileWidth);
						int pixelY = pixelPos.y - (tilePos.y * tileHeight);

						//Set height at X
						const int height = tileHeight - pixelY;
						terrainTile->SetHeight(pixelX, height);
					}
				}
			}
		}

		//Rebuild hash map
		GetTerrainTileset().RebuildHashMap();

		//Remove duplicates
		CleanupTerrainTiles(false);
	}

	InvalidateTerrainTiles(false);

	return true;
}

void Project::GenerateTerrain(const std::vector<ion::Vector2i>& graphicTiles)
{
	CollisionMap& collisionMap = m_collisionMaps[m_editingCollisionMapId];
	int mapWidth = collisionMap.GetWidth();
	int mapHeight = collisionMap.GetHeight();

	const int tileWidth = GetPlatformConfig().tileWidth;
	const int tileHeight = GetPlatformConfig().tileHeight;

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
					collisionMap.SetTerrainTile(position.x, position.y, it->second);
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
							collisionMap.SetTerrainTile(position.x, position.y, terrainTileId);

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
	const int tileWidth = GetPlatformConfig().tileWidth;
	const int tileHeight = GetPlatformConfig().tileHeight;

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
	const int tileWidth = GetPlatformConfig().tileWidth;
	const int tileHeight = GetPlatformConfig().tileHeight;

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

	Map& map = m_maps[m_editingMapId];

	//Find stamp under cursor first
	ion::Vector2i stampPos;
	u32 stampFlags = 0;
	u32 mapEntryIndex = 0;
	StampId stampId = map.FindStamp(position.x, position.y, stampPos, stampFlags, mapEntryIndex);
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
		tileId = map.GetTile(position.x, position.y);
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
	ion::io::File file(filename, ion::io::File::eOpenWrite);
	if(file.IsOpen())
	{
		ion::io::Archive archive(file, ion::io::Archive::eOut);
		archive.Serialise(m_gameObjectTypes, "gameObjectTypes");
		archive.Serialise(m_nextFreeGameObjectTypeId, "nextFreeGameObjTypeId");
	}
}

void Project::ImportGameObjectTypes(const std::string& filename)
{
	ion::io::File file(filename, ion::io::File::eOpenRead);
	if(file.IsOpen())
	{
		ion::io::Archive archive(file, ion::io::Archive::eIn);
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

	const int tileWidth = GetPlatformConfig().tileWidth;
	const int tileHeight = GetPlatformConfig().tileHeight;

	//For each palette
	for(int paletteIdx = 0; paletteIdx < s_maxPalettes; paletteIdx++)
	{
		if(useablePalettes & (1 << paletteIdx))
		{
			const Palette& palette = m_palettes[paletteIdx];

			bool match = true;
			colourMatches.clear();

			//For each pixel
			for(int i = 0; i < tileWidth * tileHeight; i++)
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

	const int tileWidth = GetPlatformConfig().tileWidth;
	const int tileHeight = GetPlatformConfig().tileHeight;

	//Find/add remaining colours
	for(int i = 1; i < tileWidth * tileHeight; i++)
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
				if(dest.AddColour(sourceColour) < 0)
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool Project::ImportBitmap(const std::string& filename, u32 importFlags, u32 paletteBits, Stamp* stamp)
{
	//Just in case
	m_tileset.RebuildHashMap();

	Map& map = m_maps[m_editingMapId];
	int mapWidth = map.GetWidth();
	int mapHeight = map.GetHeight();

	CollisionMap& collisionMap = m_collisionMaps[m_editingCollisionMapId];

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
		map.Clear();
	}

	if(importFlags & eBMPImportInsertBGTile)
	{
		m_tileset.AddTile();
	}

	const int tileWidth = GetPlatformConfig().tileWidth;
	const int tileHeight = GetPlatformConfig().tileHeight;

	//Read BMP
	BMPReader reader;
	if(reader.Read(filename))
	{
		if(reader.GetWidth() % tileWidth != 0 || reader.GetHeight() % tileHeight != 0)
		{
			//TODO: wx message handler in ion::debug
			//if(wxMessageBox("Bitmap width/height is not multiple of target platform tile width/height, image will be truncated", "Warning", wxOK | wxCANCEL | wxICON_WARNING) == wxCANCEL)
			{
				return false;
			}
		}

		if(stamp)
		{
			if(((reader.GetWidth() / m_platformConfig.tileWidth) != stamp->GetWidth()) || ((reader.GetHeight() / m_platformConfig.tileHeight) != stamp->GetHeight()))
			{
				//TODO: wx message handler in ion::debug
				//wxMessageBox("Bitmap width/height does not match stamp to be replaced, cannot import", "Warning", wxOK | wxICON_WARNING);
				return false;
			}
		}

		//Get width/height in tiles (aligned up to tile width/height)
		int tilesWidth = (int)ion::maths::Ceil((float)reader.GetWidth() / (float)tileWidth);
		int tilesHeight = (int)ion::maths::Ceil((float)reader.GetHeight() / (float)tileHeight);

		if(importFlags & eBMPImportDrawToMap)
		{
			//Grow map if necessary
			map.Resize(max(mapWidth, tilesWidth), max(mapHeight, tilesHeight), false, false);
			collisionMap.Resize(max(collisionMap.GetWidth(), tilesWidth), max(collisionMap.GetHeight(), tilesHeight), false, false);
		}

		if(importFlags & eBMPImportToStamp)
		{
			//Create new stamp
			StampId stampId = AddStamp(tilesWidth, tilesHeight);
			stamp = GetStamp(stampId);
		}

		int paletteIndex = -1;

		if(importFlags & eBMPImportWholePalette)
		{
			//Read whole palette
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
				BMPReader::Colour bmpColour = reader.GetPaletteEntry(i);
				palette.AddColour(Colour(bmpColour.r, bmpColour.g, bmpColour.b));
			}
		}

		//For all 8x8 tiles, in row-at-a-time order
		for(int tileY = 0; tileY < tilesHeight; tileY++)
		{
			for(int tileX = 0; tileX < tilesWidth; tileX++)
			{
				Tile tile(m_platformConfig.tileWidth, m_platformConfig.tileHeight);
				PaletteId paletteId = (importFlags & eBMPImportWholePalette) ? paletteIndex : 0;

				if(importFlags & eBMPImportWholePalette)
				{
					//Copy colour indices directly
					for(int pixelX = 0; pixelX < tileWidth; pixelX++)
					{
						for(int pixelY = 0; pixelY < tileHeight; pixelY++)
						{
							int sourcePixelX = (tileX * tileWidth) + pixelX;
							int sourcePixelY = (tileY * tileHeight) + pixelY;
							tile.SetPixelColour(pixelX, pixelY, reader.GetColourIndex(sourcePixelX, sourcePixelY));
						}
					}
				}
				else
				{
					//Search for colours
					std::vector<Colour> pixels(tileWidth * tileHeight);

					//Read pixel colours from bitmap
					for(int pixelX = 0; pixelX < tileWidth; pixelX++)
					{
						for(int pixelY = 0; pixelY < tileHeight; pixelY++)
						{
							int sourcePixelX = (tileX * tileWidth) + pixelX;
							int sourcePixelY = (tileY * tileHeight) + pixelY;

							if(sourcePixelX < reader.GetWidth() && sourcePixelY < reader.GetHeight())
							{
								BMPReader::Colour bmpColour = reader.GetPixel(sourcePixelX, sourcePixelY);
								pixels[(pixelY * tileWidth) + pixelX] = Colour(bmpColour.r, bmpColour.g, bmpColour.b);
							}
							else
							{
								BMPReader::Colour bmpColour = reader.GetPaletteEntry(0);
								pixels[(pixelY * tileWidth) + pixelX] = Colour(bmpColour.r, bmpColour.g, bmpColour.b);
							}
						}
					}

					//Find or create palette
					PaletteId closestPaletteId = 0;
					int closestPaletteColourMatches = 0;
					if(!FindPalette(pixels.data(), paletteBits, paletteId, closestPaletteId, closestPaletteColourMatches))
					{
						//Import palette
						Palette importedPalette;
						if(!ImportPalette(pixels.data(), importedPalette))
						{
							//TODO: wx message handler in ion::debug
							//wxMessageBox("Too many colours in tile, bailing out", "Error", wxOK | wxICON_ERROR);
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
								//TODO: wx message handler in ion::debug
								//wxMessageBox("Exceeded palette count, bailing out", "Error", wxOK | wxICON_ERROR);
								return false;
							}

							//Use imported palette
							m_palettes[paletteId] = importedPalette;
						}
					}

					//Get palette
					Palette& palette = m_palettes[paletteId];

					//Find pixel colours from palette
					for(int pixelX = 0; pixelX < tileWidth; pixelX++)
					{
						for(int pixelY = 0; pixelY < tileHeight; pixelY++)
						{
							int colourIdx = 0;
							if(!palette.GetNearestColourIdx(pixels[(pixelY * tileWidth) + pixelX], Palette::eExact, colourIdx))
							{
								//Shouldn't reach here - palette should have been validated
								//TODO: wx message handler in ion::debug
								//wxMessageBox("Error mapping colour indices", "Error", wxOK | wxICON_ERROR);
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
					map.SetTile(tileX, tileY, tileId);
					map.SetTileFlags(tileX, tileY, tileFlags);
				}

				if((importFlags & eBMPImportToStamp) || (importFlags & eBMPImportReplaceStamp))
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
		//TODO: wx message handler in ion::debug
		//wxMessageBox("Error reading bitmap", "Error", wxOK);
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
	ion::io::File file(filename, ion::io::File::eOpenWrite);
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
		ion::io::File binaryFile(binaryFilename, ion::io::File::eOpenWrite);
		if(binaryFile.IsOpen())
		{
			m_tileset.Export(m_platformConfig, binaryFile);
			binarySize = binaryFile.GetSize();
		}
		else
		{
			return false;
		}
	}

	ion::io::File file(filename, ion::io::File::eOpenWrite);
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

			m_tileset.Export(m_platformConfig, stream);

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
		ion::io::File binaryFile(binaryFilename, ion::io::File::eOpenWrite);
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

	ion::io::File file(filename, ion::io::File::eOpenWrite);
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

bool Project::ExportMap(MapId mapId, const std::string& filename, bool binary) const
{
	const Map& map = m_maps.find(mapId)->second;
	int mapWidth = map.GetWidth();
	int mapHeight = map.GetHeight();
	const std::string& mapName = map.GetName();

	u32 binarySize = 0;

	if(binary)
	{
		std::string binaryFilename = filename.substr(0, filename.find_first_of('.'));
		binaryFilename += ".bin";

		//Export binary data
		ion::io::File binaryFile(binaryFilename, ion::io::File::eOpenWrite);
		if(binaryFile.IsOpen())
		{
			map.Export(*this, binaryFile);
			binarySize = binaryFile.GetSize();
		}
		else
		{
			return false;
		}
	}

	ion::io::File file(filename, ion::io::File::eOpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		WriteFileHeader(stream);
		
		if(binary)
		{
			//Export size of binary file
			stream << "map_" << mapName << "_size_b\tequ 0x" << std::hex << std::setfill('0') << std::uppercase << std::setw(8) << binarySize << std::dec << "\t; Size in bytes" << std::endl;
		}
		else
		{
			//Export label, data and size as inline text
			stream << "map_" << mapName << ":" << std::endl;

			map.Export(*this, stream);

			stream << std::endl;
			stream << "map_" << mapName << "_end:" << std::endl;
			stream << "map_" << mapName << "_size_b\tequ (map_" << mapName << "_end-map_" << mapName << ")\t; Size in bytes" << std::endl;
		}

		stream << "map_" << mapName << "_size_w\tequ (map_" << mapName << "_size_b/2)\t; Size in words" << std::endl;
		stream << "map_" << mapName << "_size_l\tequ (map_" << mapName << "_size_b/4)\t; Size in longwords" << std::endl;

		stream << std::hex << std::setfill('0') << std::uppercase;
		stream << "map_" << mapName << "_width\tequ " << "0x" << std::setw(2) << mapWidth << std::endl;
		stream << "map_" << mapName << "_height\tequ " << "0x" << std::setw(2) << mapHeight << std::endl;
		stream << std::dec;

		file.Write(stream.str().c_str(), stream.str().size());

		return true;
	}

	return false;
}

bool Project::ExportStamps(const std::string& filename, bool binary) const
{
	u32 binarySize = 0;

#pragma pack(push, 1)
	struct StampTableEntry
	{
		StampTableEntry() {}
		StampTableEntry(u32 offset, u8 width, u8 height)
		{
			m_offset = offset;
			m_width = width;
			m_height = height;
		}

		u32 m_offset;
		u16 m_width;
		u16 m_height;
	};
#pragma pack(pop)

	std::vector<StampTableEntry> stampTable;

	if(binary)
	{
		std::string binaryFilename = filename.substr(0, filename.find_first_of('.'));
		binaryFilename += ".bin";

		//Export binary data
		ion::io::File binaryFile(binaryFilename, ion::io::File::eOpenWrite);
		if(binaryFile.IsOpen())
		{
			for(TStampMap::const_iterator it = m_stamps.begin(), end = m_stamps.end(); it != end; ++it)
			{
				stampTable.push_back(StampTableEntry(binaryFile.GetSize(), it->second.GetWidth(), it->second.GetHeight()));
				it->second.Export(*this, binaryFile);
			}

			binarySize = binaryFile.GetSize();
		}
		else
		{
			return false;
		}
	}

	ion::io::File file(filename, ion::io::File::eOpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		WriteFileHeader(stream);

		if(binary)
		{
			//Export size of binary file
			stream << "stamps_" << m_name << "_size_b\tequ 0x" << HEX8(binarySize) << "\t; Binary size in bytes" << std::endl;

			stream << std::endl;

			//Export stamp table
			stream << "stamps_" << m_name << "_table:" << std::endl;
			stream << std::endl;

			for(int i = 0; i < stampTable.size(); i++)
			{
				stream << "stamp_" << i << "_offset:\tdc.l 0x" << HEX8((int)stampTable[i].m_offset) << std::endl;
				stream << "stamp_" << i << "_width:\tdc.w 0x" << HEX4((int)stampTable[i].m_width) << std::endl;
				stream << "stamp_" << i << "_height:\tdc.w 0x" << HEX4((int)stampTable[i].m_height) << std::endl;
				stream << std::endl;
			}
		}
		else
		{
			//Export label, data and size as inline text
			stream << "stamps_" << m_name << ":" << std::endl;

			int offset = 0;
			for(TStampMap::const_iterator it = m_stamps.begin(), end = m_stamps.end(); it != end; ++it)
			{
				stampTable.push_back(StampTableEntry(offset, it->second.GetWidth(), it->second.GetHeight()));
				stream << "stamp_" << it->first << ":" << std::endl;
				it->second.Export(*this, stream);
				stream << std::endl;

				offset += it->second.GetBinarySize();
			}

			//Export stamp table
			stream << "stamps_" << m_name << "_table:" << std::endl;
			stream << std::endl;

			for(int i = 0; i < stampTable.size(); i++)
			{
				stream << "stamp_" << i << "_offset:\tdc.l 0x" << HEX8((int)stampTable[i].m_offset) << std::endl;
				stream << "stamp_" << i << "_width:\tdc.w 0x" << HEX4((int)stampTable[i].m_width) << std::endl;
				stream << "stamp_" << i << "_height:\tdc.w 0x" << HEX4((int)stampTable[i].m_height) << std::endl;
				stream << std::endl;
			}

			stream << std::endl;
			stream << "stamps_" << m_name << "_end:" << std::endl;
			stream << "stamps_" << m_name << "_size_b\tequ (stamps_" << m_name << "_end-stamps_" << m_name << ")\t; Size in bytes" << std::endl;
		}

		stream << "stamps_" << m_name << "_size_w\tequ (stamps_" << m_name << "_size_b/2)\t; Size in words" << std::endl;
		stream << "stamps_" << m_name << "_size_l\tequ (stamps_" << m_name << "_size_b/4)\t; Size in longwords" << std::endl;

		file.Write(stream.str().c_str(), stream.str().size());

		return true;
	}

	return false;
}

bool Project::ExportStampMap(MapId mapId, const std::string& filename, bool binary) const
{
	const Map& map = m_maps.find(mapId)->second;
	int mapWidth = map.GetWidth();
	int mapHeight = map.GetHeight();
	const std::string& mapName = map.GetName();

	u32 binarySize = 0;

	if(binary)
	{
		std::string binaryFilename = filename.substr(0, filename.find_first_of('.'));
		binaryFilename += ".bin";

		//Export binary data
		ion::io::File binaryFile(binaryFilename, ion::io::File::eOpenWrite);
		if(binaryFile.IsOpen())
		{
			map.ExportStampMap(*this, binaryFile);
			binarySize = binaryFile.GetSize();
		}
		else
		{
			return false;
		}
	}

	ion::io::File file(filename, ion::io::File::eOpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		WriteFileHeader(stream);

		if(binary)
		{
			//Export size of binary file
			stream << "stampmap_" << mapName << "_size_b\tequ 0x" << HEX8(binarySize) << "\t; Size in bytes" << std::endl;
		}
		else
		{
			//Export label, data and size as inline text
			stream << "stampmap_" << mapName << ":" << std::endl;

			map.ExportStampMap(*this, stream);

			stream << std::endl;
			stream << "stampmap_" << mapName << "_end:" << std::endl;
			stream << "stampmap_" << mapName << "_size_b\tequ (stampmap_" << mapName << "_end-stampmap_" << mapName << ")\t; Size in bytes" << std::endl;
		}

		stream << "stampmap_" << mapName << "_size_w\tequ (stampmap_" << mapName << "_size_b/2)\t; Size in words" << std::endl;
		stream << "stampmap_" << mapName << "_size_l\tequ (stampmap_" << mapName << "_size_b/4)\t; Size in longwords" << std::endl;

		stream << std::hex << std::setfill('0') << std::uppercase;
		stream << "stampmap_" << mapName << "_width\tequ " << "0x" << HEX2(mapWidth) << std::endl;
		stream << "stampmap_" << mapName << "_height\tequ " << "0x" << HEX2(mapHeight) << std::endl;
		stream << std::dec;

		file.Write(stream.str().c_str(), stream.str().size());

		return true;
	}

	return false;
}

bool Project::ExportCollisionMap(MapId mapId, const std::string& filename, bool binary) const
{
	u32 binarySize = 0;

	const Map& map = m_maps.find(mapId)->second;
	const std::string& mapName = map.GetName();
	const CollisionMap& collisionMap = m_collisionMaps.find(mapId)->second;

	if(binary)
	{
		std::string binaryFilename = filename.substr(0, filename.find_first_of('.'));
		binaryFilename += ".bin";

		//Export binary data
		ion::io::File binaryFile(binaryFilename, ion::io::File::eOpenWrite);
		if(binaryFile.IsOpen())
		{
			collisionMap.Export(*this, binaryFile);
			binarySize = binaryFile.GetSize();
		}
		else
		{
			return false;
		}
	}

	ion::io::File file(filename, ion::io::File::eOpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		WriteFileHeader(stream);
		
		if(binary)
		{
			//Export size of binary file
			stream << "collisionmap_" << mapName << "_size_b\tequ 0x" << std::hex << std::setfill('0') << std::uppercase << std::setw(8) << binarySize << std::dec << "\t; Size in bytes" << std::endl;
		}
		else
		{
			//Export label, data and size as inline text
			stream << "collisionmap_" << mapName << ":" << std::endl;

			collisionMap.Export(*this, stream);

			stream << std::endl;
			stream << "collisionmap_" << mapName << "_end:" << std::endl;
			stream << "collisionmap_" << mapName << "_size_b\tequ (collisionmap_" << mapName << "_end-collisionmap_" << mapName << ")\t; Size in bytes" << std::endl;
		}

		stream << "collisionmap_" << mapName << "_size_w\tequ (collisionmap_" << mapName << "_size_b/2)\t; Size in words" << std::endl;
		stream << "collisionmap_" << mapName << "_size_l\tequ (collisionmap_" << mapName << "_size_b/4)\t; Size in longwords" << std::endl;

		stream << std::hex << std::setfill('0') << std::uppercase;
		stream << "collisionmap_" << mapName << "_width\tequ " << "0x" << std::setw(2) << collisionMap.GetWidth() << std::endl;
		stream << "collisionmap_" << mapName << "_height\tequ " << "0x" << std::setw(2) << collisionMap.GetHeight() << std::endl;
		stream << std::dec;

		file.Write(stream.str().c_str(), stream.str().size());

		return true;
	}

	return false;
}

bool Project::ExportGameObjects(MapId mapId, const std::string& filename) const
{
	const Map& map = m_maps.find(mapId)->second;
	const std::string& mapName = map.GetName();

	ion::io::File file(filename, ion::io::File::eOpenWrite);
	if(file.IsOpen())
	{
		std::stringstream stream;
		WriteFileHeader(stream);
		stream << "gameobjects_" << mapName << ":" << std::endl;

		const TGameObjectPosMap& gameObjMap = map.GetGameObjects();

		//Bake out all game object types, even if there's no game objects to go with them (still need the count and init subroutine)
		for(TGameObjectTypeMap::const_iterator it = m_gameObjectTypes.begin(), end = m_gameObjectTypes.end(); it != end; ++it)
		{
			const GameObjectType& gameObjectType = it->second;
			const TGameObjectPosMap::const_iterator gameObjIt = gameObjMap.find(it->first);
			u32 count = (gameObjIt != gameObjMap.end()) ? gameObjIt->second.size() : 0;
			
			//Export game object count
			stream << "gameobjects_" << mapName << "_" << gameObjectType.GetName() << "_count equ 0x" << count << std::endl;

			//Export game object init sunroutine
			stream << "LoadGameObjects_" << mapName << "_" << gameObjectType.GetName() << ":" << std::endl;

			//Export all game objects of this type
			if(gameObjIt != gameObjMap.end())
			{
				for(int i = 0; i < gameObjIt->second.size(); i++)
				{
					if(gameObjIt->second[i].m_gameObject.GetName().size() > 0)
					{
						stream << gameObjIt->second[i].m_gameObject.GetName() << "_idx\tequ 0x" << std::hex << i << std::endl;
					}
					else
					{
						//No name, generate one
						stream << mapName << gameObjectType.GetName() << "_" << std::dec << (u32)gameObjIt->second[i].m_gameObject.GetId() << "_idx\tequ 0x" << std::hex << i << std::endl;
					}

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

bool Project::ExportSpriteSheets(const std::string& directory, bool binary) const
{
	for(TActorMap::const_iterator it = m_actors.begin(), end = m_actors.end(); it != end; ++it)
	{
		if(binary)
		{

		}
		else
		{
			std::stringstream filename;
			filename << directory << "\\" << it->second.GetName() << ".ASM";

			ion::io::File file(filename.str(), ion::io::File::eOpenWrite);
			if(file.IsOpen())
			{
				std::stringstream stream;
				WriteFileHeader(stream);
				it->second.ExportSpriteSheets(m_platformConfig, stream);
				file.Write(stream.str().c_str(), stream.str().size());
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}

bool Project::ExportSpriteAnims(const std::string& directory, bool binary) const
{
	for(TActorMap::const_iterator it = m_actors.begin(), end = m_actors.end(); it != end; ++it)
	{
		if(binary)
		{

		}
		else
		{
			std::stringstream filename;
			filename << directory << "\\" << it->second.GetName() << ".ASM";

			ion::io::File file(filename.str(), ion::io::File::eOpenWrite);
			if(file.IsOpen())
			{
				std::stringstream stream;
				WriteFileHeader(stream);
				it->second.ExportSpriteAnims(m_platformConfig, stream);
				file.Write(stream.str().c_str(), stream.str().size());
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}

bool Project::ExportSpritePalettes(const std::string& directory) const
{
	for(TActorMap::const_iterator it = m_actors.begin(), end = m_actors.end(); it != end; ++it)
	{
		std::stringstream filename;
		filename << directory << "\\" << it->second.GetName() << ".ASM";

		ion::io::File file(filename.str(), ion::io::File::eOpenWrite);
		if(file.IsOpen())
		{
			std::stringstream stream;
			WriteFileHeader(stream);
			it->second.ExportSpritePalettes(m_platformConfig, stream);
			file.Write(stream.str().c_str(), stream.str().size());
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Project::ExportMapBitmaps(const std::string& directory) const
{
	for(TMapMap::const_iterator it = m_maps.begin(), end = m_maps.end(); it != end; ++it)
	{
		std::stringstream filename;
		filename << directory << "\\map_" << it->first << ".bmp";

		const Map& map = it->second;

		BMPReader writer;
		writer.SetDimensions(map.GetWidth() * m_platformConfig.tileWidth, map.GetHeight() * m_platformConfig.tileHeight);

		const Palette* palette = GetPalette(0);

		if(palette)
		{
			for(int i = 0; i < Palette::coloursPerPalette && palette->IsColourUsed(i); i++)
			{
				Colour colour = palette->GetColour(i);
				writer.SetPaletteEntry(i, BMPReader::Colour(colour.GetRed(), colour.GetGreen(), colour.GetBlue()));
			}

			for(int tileX = 0; tileX < map.GetWidth(); tileX++)
			{
				for(int tileY = 0; tileY < map.GetHeight(); tileY++)
				{
					//Find stamp at position first
					ion::Vector2i stampPos;
					TileId tileId = InvalidTileId;
					u32 flags = 0;
					u32 mapEntryIndex = 0;
					StampId stampId = map.FindStamp(tileX, tileY, stampPos, flags, mapEntryIndex);

					if(stampId)
					{
						//Get from stamp
						if(const Stamp* stamp = GetStamp(stampId))
						{
							ion::Vector2i offset = ion::Vector2i(tileX, tileY) - stampPos;

							int sourceX = (flags & Map::eFlipX) ? (stamp->GetWidth() - 1 - offset.x) : offset.x;
							int sourceY = (flags & Map::eFlipY) ? (stamp->GetHeight() - 1 - offset.y) : offset.y;

							tileId = stamp->GetTile(sourceX, sourceY);
							flags ^= stamp->GetTileFlags(sourceX, sourceY);
						}
					}
					else
					{
						//Pick tile
						tileId = map.GetTile(tileX, tileY);
						flags = map.GetTileFlags(tileX, tileY);
					}

					if(tileId == InvalidTileId)
					{
						tileId = m_backgroundTile;

						if(tileId == InvalidTileId)
						{
							tileId = 0;
						}
					}

					const Tile* tile = m_tileset.GetTile(tileId);

					for(int x = 0; x < m_platformConfig.tileWidth; x++)
					{
						for(int y = 0; y < m_platformConfig.tileHeight; y++)
						{
							u8 colourIndex = tile->GetPixelColour(x, y);
							int pixelX = (flags & Map::eFlipX) ? (m_platformConfig.tileWidth - x - 1) : x;
							int pixelY = (flags & Map::eFlipY) ? (m_platformConfig.tileHeight - y - 1) : y;
							writer.SetColourIndex((tileX * m_platformConfig.tileWidth) + pixelX, (tileY * m_platformConfig.tileHeight) + pixelY, colourIndex);
						}
					}
				}
			}
		}

		writer.Write(filename.str());
	}

	return true;
}

bool Project::ExportStampBitmaps(const std::string& directory) const
{
	for(TStampMap::const_iterator it = m_stamps.begin(), end = m_stamps.end(); it != end; ++it)
	{
		std::stringstream filename;
		filename << directory << "\\stamp_" << it->first << ".bmp";

		const Stamp& stamp = it->second;

		BMPReader writer;
		writer.SetDimensions(stamp.GetWidth() * m_platformConfig.tileWidth, stamp.GetHeight() * m_platformConfig.tileHeight);

		const Palette& palette = *GetPalette(m_tileset.GetTile(stamp.GetTile(0, 0))->GetPaletteId());
		for(int i = 0; i < Palette::coloursPerPalette && palette.IsColourUsed(i); i++)
		{
			Colour colour = palette.GetColour(i);
			writer.SetPaletteEntry(i, BMPReader::Colour(colour.GetRed(), colour.GetGreen(), colour.GetBlue()));
		}
		
		for(int tileX = 0; tileX < stamp.GetWidth(); tileX++)
		{
			for(int tileY = 0; tileY < stamp.GetHeight(); tileY++)
			{
				TileId tileId = stamp.GetTile(tileX, tileY);
				u32 tileFlags = stamp.GetTileFlags(tileX, tileY);
				const Tile* tile = m_tileset.GetTile(tileId);

				for(int x = 0; x < m_platformConfig.tileWidth; x++)
				{
					for(int y = 0; y < m_platformConfig.tileHeight; y++)
					{
						u8 colourIndex = tile->GetPixelColour(x, y);
						int pixelX = (tileFlags & Map::eFlipX) ? (m_platformConfig.tileWidth - x - 1) : x;
						int pixelY = (tileFlags & Map::eFlipY) ? (m_platformConfig.tileHeight - y - 1) : y;
						writer.SetColourIndex((tileX * m_platformConfig.tileWidth) + pixelX, (tileY * m_platformConfig.tileHeight) + pixelY, colourIndex);
					}
				}
			}
		}

		writer.Write(filename.str());
	}

	return true;
}
