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

#pragma once

#include <io/Archive.h>
#include <string>
#include <vector>
#include <set>

#include "Types.h"
#include "PlatformConfig.h"
#include "Map.h"
#include "Palette.h"
#include "SpriteSheet.h"
#include "Actor.h"
#include "Stamp.h"
#include "Tile.h"
#include "Tileset.h"
#include "CollisionMap.h"
#include "TerrainTile.h"
#include "TerrainTileset.h"
#include "GameObject.h"
#include "StampAnimation.h"

typedef std::map<MapId, Map> TMapMap;
typedef std::map<CollisionMapId, CollisionMap> TCollisionMapMap;
typedef std::map<StampId, Stamp> TStampMap;
typedef std::map<ActorId, Actor> TActorMap;
typedef std::map<AnimationId, Animation> TAnimationMap;
typedef std::map<StampAnimId, StampAnimation> TStampAnimationMap;
typedef std::map<GameObjectTypeId, GameObjectType> TGameObjectTypeMap;

class Project
{
	//TODO: Add/remove palettes
	static const int s_maxPalettes = 4;

public:
	enum BMPImportFlags
	{
		eBMPImportClearPalettes	= (1 << 0),
		eBMPImportClearTiles	= (1 << 1),
		eBMPImportClearMap		= (1 << 2),
		eBMPImportDrawToMap		= (1 << 3),
		eBMPImportToStamp		= (1 << 4),
		eBMPImportReplaceStamp	= (1 << 5),
		eBMPImportToSpriteSheet	= (1 << 6),
		eBMPImportWholePalette	= (1 << 7),
		eBMPImportInsertBGTile	= (1 << 8),
	};

	Project(PlatformConfig& defaultPatformConfig);

	//Load/save project
	bool Load(const std::string& filename);
	bool Save(const std::string& filename);

	//Get platform config
	const PlatformConfig& GetPlatformConfig() const { return m_platformConfig; }

	//Get/set name
	const std::string& GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

	//Get filename
	const std::string& GetFilename() { return m_filename; }

	//Clear project (palettes, tiles, map)
	void Clear();

	//Maps
	MapId CreateMap();
	MapId CreateMap(MapId mapId);
	void DeleteMap(MapId mapId);
	Map& GetMap(MapId mapId);
	const Map& GetMap(MapId mapId) const;
	Map& GetEditingMap();
	MapId GetEditingMapId() const;
	void SetEditingMap(MapId mapId);
	const TMapMap::const_iterator MapsBegin() const;
	const TMapMap::const_iterator MapsEnd() const;
	TMapMap::iterator MapsBegin();
	TMapMap::iterator MapsEnd();
	int GetMapCount() const;

	//Collision maps
	CollisionMapId CreateCollisionMap(CollisionMapId collisionMapId);
	CollisionMapId CreateCollisionMap(CollisionMapId collisionMapId, int width, int height);
	void DeleteCollisionMap(CollisionMapId collisionMapId);
	CollisionMap& GetCollisionMap(CollisionMapId collisionMapId);
	const CollisionMap& GetCollisionMap(CollisionMapId collisionMapId) const;
	CollisionMap& GetEditingCollisionMap();
	CollisionMapId GetEditingCollisionMapId() const;
	void SetEditingCollisionMap(CollisionMapId collisionMapId);
	const TCollisionMapMap::const_iterator CollisionMapsBegin() const;
	const TCollisionMapMap::const_iterator CollisionMapsEnd() const;
	int GetCollisionMapCount() const;

	//Get tileset
	Tileset& GetTileset() { return m_tileset; }
	const Tileset& GetTileset() const { return m_tileset; }

	//Get terrain tileset
	TerrainTileset& GetTerrainTileset() { return m_terrainTileset; }
	const TerrainTileset& GetTerrainTileset() const { return m_terrainTileset; }

	//Get palette
	Palette* GetPalette(PaletteId paletteId) { return &m_palettes[paletteId]; }
	const Palette* GetPalette(PaletteId paletteId) const { return &m_palettes[paletteId]; }
	int GetNumPalettes() const { return s_maxPalettes; }

	//Palette slots
	int AddPaletteSlot(Palette& palette);
	Palette* GetPaletteSlot(int slotIndex);
	const Palette* GetPaletteSlot(int slotIndex) const;
	int GetNumPaletteSlots() const;
	void SetActivePaletteSlot(PaletteId paletteId, int slotIndex);
	void ExportPaletteSlots(const std::string& filename);
	void ImportPaletteSlots(const std::string& filename);
	void SetBackgroundColour(u8 colourIdx);
	void SwapPaletteEntries(u8 colourA, u8 colourB);

	//Tiles
	void DeleteTile(TileId tileId);
	void SwapTiles(TileId tileId1, TileId tileId2);
	void SetBackgroundTile(TileId tileId);
	TileId GetBackgroundTile() const { return m_backgroundTile; }
	int CleanupTiles();

	//Actors
	ActorId CreateActor();
	void DeleteActor(ActorId actorId);
	Actor* GetActor(ActorId actorId);
	const Actor* GetActor(ActorId actorId) const;
	const TActorMap::const_iterator ActorsBegin() const;
	const TActorMap::const_iterator ActorsEnd() const;
	int GetActorCount() const;

	//Animations
	AnimationId CreateAnimation();
	void DeleteAnimation(AnimationId animationId);
	Animation* GetAnimation(AnimationId animationId);
	const Animation* GetAnimation(AnimationId animationId) const;
	const TAnimationMap::const_iterator AnimationsBegin() const;
	const TAnimationMap::const_iterator AnimationsEnd() const;
	int GetAnimationCount() const;

	//Stamp animations
	StampAnimId CreateStampAnimation();
	void DeleteStampAnimation(StampAnimId StampAnimId);
	StampAnimation* GetStampAnimation(StampAnimId StampAnimId);
	const StampAnimation* GetStampAnimation(StampAnimId StampAnimId) const;
	const TStampAnimationMap::const_iterator StampAnimationsBegin() const;
	const TStampAnimationMap::const_iterator StampAnimationsEnd() const;
	int GetStampAnimationCount() const;

	//Stamps
	StampId AddStamp(int width, int height);
	StampId AddStamp(Stamp* stamp);
	void DeleteStamp(StampId stampId);
	Stamp* GetStamp(StampId stampId);
	const Stamp* GetStamp(StampId stampId) const;
	StampId FindDuplicateStamp(Stamp* stamp) const;
	const TStampMap::const_iterator StampsBegin() const;
	const TStampMap::const_iterator StampsEnd() const;
	int GetStampCount() const;
	int CleanupStamps();
	void SubstituteStamp(StampId stampToReplace, StampId substitution);

	//Collision tiles
	void DeleteTerrainTile(TerrainTileId tileId);
	void SwapTerrainTiles(TerrainTileId tileId1, TerrainTileId tileId2);
	void SetDefaultTerrainTile(TerrainTileId tileId);
	TerrainTileId GetDefaultTerrainTile() const { return m_defaultTerrainTile; }
	int CleanupTerrainTiles(bool prompt);

	//Terrain generation from bezier paths
	bool GenerateTerrainFromBeziers(int granularity);

	//Terrain generation from graphic tiles
	void GenerateTerrain(const std::vector<ion::Vector2i>& graphicTiles);

	//Game object types
	GameObjectTypeId AddGameObjectType();
	void RemoveGameObjectType(GameObjectTypeId typeId);
	GameObjectType* GetGameObjectType(GameObjectTypeId typeId);
	const GameObjectType* GetGameObjectType(GameObjectTypeId typeId) const;
	const TGameObjectTypeMap& GetGameObjectTypes() const;
	void ExportGameObjectTypes(const std::string& filename);
	void ImportGameObjectTypes(const std::string& filename);

	//Set current colour used for editing
	void SetPaintColour(u8 colourIdx);
	u8 GetPaintColour() const;

	//Set current collision tile used for painting
	void SetPaintTerrainTile(TerrainTileId tile);
	TerrainTileId GetPaintTerrainTile() const;

	//Set current tile used for painting
	void SetPaintTile(TileId tile);
	TileId GetPaintTile() const;

	//Set current tile used for erasing
	void SetEraseTile(TileId tile);
	TileId GetEraseTile() const;

	//Set current stamp used for painting
	void SetPaintStamp(StampId stamp);
	StampId GetPaintStamp() const;

	//Set current game object type for placing
	void SetPaintGameObjectType(GameObjectTypeId typeId);
	GameObjectTypeId GetPaintGameObjectType() const;

	//Grid
	int GetGridSize() const { return m_gridSize; }
	void SetGridSize(int gridSize) { m_gridSize = gridSize; }
	bool GetShowGrid() const { return m_showGrid; }
	void SetShowGrid(bool show) { m_showGrid = show; }
	bool GetGridSnap() const { return m_snapToGrid; }
	void SetGridSnap(bool snap) { m_snapToGrid = snap; }

	//Stamp outlines
	bool GetShowStampOutlines() const { return m_showStampOutlines; }
	void SetShowStampOutlines(bool show) { m_showStampOutlines = show; }

	//Display frame
	bool GetShowDisplayFrame() const { return m_showDisplayFrame; }
	void SetShowDisplayFrame(bool show) { m_showDisplayFrame = show; }

	//Collision
	bool GetShowCollision() const { return m_showCollision; }
	void SetShowCollision(bool show) { m_showCollision = show; }

	void InvalidateMap(bool invalidate) { m_mapInvalidated = invalidate; }
	void InvalidateTiles(bool invalidate) { m_tilesInvalidated = invalidate; }
	void InvalidateTerrainTiles(bool invalidate) { m_terrainTilesInvalidated = invalidate; }
	void InvalidateTerrainBeziers(bool invalidate) { m_terrainBeziersInvalidated = invalidate; }
	void InvalidateStamps(bool invalidate) { m_stampsInvalidated = invalidate; }
	bool MapIsInvalidated() const { return m_mapInvalidated; }
	bool TilesAreInvalidated() const { return m_tilesInvalidated; }
	bool TerrainTilesAreInvalidated() const { return m_terrainTilesInvalidated; }
	bool TerrainBeziersAreInvalidated() const { return m_terrainBeziersInvalidated; }
	bool StampsAreInvalidated() const { return m_stampsInvalidated; }

	//Import bitmap
	bool ImportBitmap(const std::string& filename, u32 importFlags, u32 paletteBits, Stamp* stamp = NULL);

	//Export
	bool ExportPalettes(const std::string& filename) const;
	bool ExportTiles(const std::string& filename, bool binary) const;
	bool ExportTerrainTiles(const std::string& filename, bool binary) const;
	bool ExportSpriteSheets(const std::string& directory, bool binary) const;
	bool ExportSpriteAnims(const std::string& directory, bool binary) const;
	bool ExportSpritePalettes(const std::string& directory) const;
	bool ExportMapBitmaps(const std::string& directory) const;
	bool ExportStampBitmaps(const std::string& directory) const;

	bool ExportMap(MapId mapId, const std::string& filename, bool binary) const;
	bool ExportStamps(const std::string& filename, bool binary) const;
	bool ExportStampMap(MapId mapId, const std::string& filename, bool binary) const;
	bool ExportCollisionMap(MapId mapId, const std::string& filename, bool binary) const;
	bool ExportGameObjects(MapId mapId, const std::string& filename) const;

	//Serialise
	void Serialise(ion::io::Archive& archive);

	//Last used export filenames
	struct ExportFilenames
	{
		std::string palettes;
		std::string tileset;
		std::string stamps;
		std::string terrainTiles;
		std::string spriteSheets;
		std::string spriteAnims;
		std::string spritePalettes;

		bool palettesExportEnabled;
		bool tilesetExportEnabled;
		bool stampsExportEnabled;
		bool terrainTilesExportEnabled;
		bool spriteSheetsExportEnabled;
		bool spriteAnimsExportEnabled;
		bool spritePalettesExportEnabled;

		bool exportBinary;

		void Serialise(ion::io::Archive& archive)
		{
			archive.Serialise(palettesExportEnabled, "palettesExportEnabled");
			archive.Serialise(tilesetExportEnabled, "tilesetExportEnabled");
			archive.Serialise(stampsExportEnabled, "stampsExportEnabled");
			archive.Serialise(terrainTilesExportEnabled, "terrainTilesExportEnabled");
			archive.Serialise(spriteSheetsExportEnabled, "spriteSheetsExportEnabled");
			archive.Serialise(spriteAnimsExportEnabled, "spriteAnimsExportEnabled");
			archive.Serialise(spritePalettesExportEnabled, "spritePalettesExportEnabled");
			archive.Serialise(exportBinary, "exportBinary");

			archive.Serialise(palettes, "exportFNamePalettes");
			archive.Serialise(tileset, "exportFNameTileset");
			archive.Serialise(stamps, "exportFNameStamps");
			archive.Serialise(terrainTiles, "exportFNameTerrainTiles");
			archive.Serialise(spriteSheets, "exportDirSpriteSheets");
			archive.Serialise(spriteAnims, "exportDirSpriteAnims");
			archive.Serialise(spritePalettes, "exportDirSpritePalettes");
		}
	};
	
	ExportFilenames m_exportFilenames;

private:
	//Find palette matching 8x8 colour grid
	bool FindPalette(Colour* pixels, u32 useablePalettes, PaletteId& paletteId, PaletteId& closestPalette, int& closestColourCount) const;
	bool ImportPalette(Colour* pixels, Palette& palette);
	bool MergePalettes(Palette& dest, const Palette& source);

	void WriteFileHeader(std::stringstream& stream) const;

	//Collapse palette slots to palettes in use
	void CollapsePaletteSlots();

	//Generate heighmap from graphic tile
	void GenerateHeightMap(const Tile& graphicTile, std::vector<u8>& heightMap) const;
	void GenerateCeilingMap(const Tile& graphicTile, std::vector<u8>& ceilingMap) const;

	//Get tile at position (including on stamps)
	TileId GetTileAtPosition(const ion::Vector2i& position);

	//Platform config
	PlatformConfig m_platformConfig;

	//Project name
	std::string m_name;

	//Filename
	std::string m_filename;

	//Tileset
	Tileset m_tileset;

	//terrain tileset
	TerrainTileset m_terrainTileset;

	//Map
	TMapMap m_maps;

	//Collision map
	TCollisionMapMap m_collisionMaps;

	//Active palettes
	std::vector<Palette> m_palettes;

	//Saved palette slots
	std::vector<Palette> m_paletteSlots;

	//Stamps
	TStampMap m_stamps;
	StampId m_nextFreeStampId;

	//Actors
	std::map<ActorId, Actor> m_actors;

	//Animations
	TAnimationMap m_animations;

	//Stamp animations
	TStampAnimationMap m_stampAnimations;

	//Game object types
	TGameObjectTypeMap m_gameObjectTypes;
	GameObjectTypeId m_nextFreeGameObjectTypeId;

	//Colour used for painting
	u8 m_paintColour;

	//Collision tile used for painting
	TerrainTileId m_paintTerrainTile;

	//Tile used for painting
	TileId m_paintTile;

	//Stamp used for painting
	StampId m_paintStamp;

	//Tile used for erasing
	TileId m_eraseTile;

	//Game object used for placing
	GameObjectTypeId m_paintGameObjectType;

	//Background tile (replaced InvalidTileId on export)
	TileId m_backgroundTile;

	//Default collision tile (replaces InvalidTerrainTileId on export)
	TerrainTileId m_defaultTerrainTile;

	//Currently editing map id
	MapId m_editingMapId;

	//Currently editing collision map id
	CollisionMapId m_editingCollisionMapId;

	//Grid
	int m_gridSize;
	bool m_showGrid;
	bool m_snapToGrid;

	//Stamp outlines
	bool m_showStampOutlines;

	//Display frame
	bool m_showDisplayFrame;

	//Collision
	bool m_showCollision;

	//Map needs redraw
	bool m_mapInvalidated;
	bool m_tilesInvalidated;
	bool m_terrainTilesInvalidated;
	bool m_terrainBeziersInvalidated;
	bool m_stampsInvalidated;
};