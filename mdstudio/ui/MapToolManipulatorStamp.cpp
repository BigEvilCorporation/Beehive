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

#include "MapToolManipulatorStamp.h"
#include "MapPanel.h"

#include <ion/core/utils/STL.h>

MapToolManipulatorStamp::MapToolManipulatorStamp(Project& project, MapPanel& mapPanel, TUndoStack& undoStack)
	: MapToolManipulator(project, mapPanel, undoStack)
	, m_unitSizePx(	project.GetPlatformConfig().stampWidth * project.GetPlatformConfig().tileWidth,
					project.GetPlatformConfig().stampHeight * project.GetPlatformConfig().tileHeight)
{
	AddContextMenuItem(0, "Edit collision", std::bind(&MapToolManipulatorStamp::OnContextMenuEditCollision, this, std::placeholders::_1, std::placeholders::_2));
}

MapToolManipulatorStamp::~MapToolManipulatorStamp()
{

}

std::string MapToolManipulatorStamp::GetName()
{
	return "Stamp Tool";
}

std::string MapToolManipulatorStamp::GetDescription()
{
	return "Move, copy, and delete stamps or stamp regions";
}

std::string MapToolManipulatorStamp::GetStatusText()
{
	return "CTRL: multiple/box select, SHIFT: copy selection";
}

std::string MapToolManipulatorStamp::GetTooltipText(const MapObjIdentifierStamp& object, const ion::Vector2i& cursorPosPx)
{
	const ion::Vector2i tileSize(GetProject().GetPlatformConfig().tileWidth, GetProject().GetPlatformConfig().tileHeight);
	const ion::Vector2i cursorPosTl = cursorPosPx * tileSize;
	int stampX = cursorPosTl.x % object.stamp->GetWidth();
	int stampY = cursorPosTl.y % object.stamp->GetHeight();
	TileId tileId = object.stamp->GetTile(stampX, stampY);

	TerrainTileId terrainTileId = object.stamp->GetTerrainTile(stampX, stampY);
	u16 collisionFlags = object.stamp->GetCollisionTileFlags(stampX, stampY);
	u8 terrainAngleByte = 0;
	ion::Vector2 terrainNormal;
	if (const TerrainTile* terrainTile = GetProject().GetTerrainTileset().GetTerrainTile(terrainTileId))
	{
		terrainAngleByte = terrainTile->GetAngleByte();
		terrainNormal = terrainTile->GetNormal();
	}

	std::stringstream tipStr;

	tipStr << "Cursor: 0x" << SSTREAM_HEX4(cursorPosTl.x) << ", 0x" << SSTREAM_HEX4(cursorPosTl.y) << " (" << cursorPosTl.x << ", " << cursorPosTl.y << ")" << std::endl;
	tipStr << "Stamp: 0x" << SSTREAM_HEX4(object.stampId) << " (" << object.stampId << ")" << std::endl;
	tipStr << "Tile: 0x" << SSTREAM_HEX4(tileId) << " (" << tileId << ")" << std::endl;
	tipStr << "Terrain: 0x" << SSTREAM_HEX4(terrainTileId) << " (" << terrainTileId << ")" << " collisionFlags: 0x" << SSTREAM_HEX2(collisionFlags) << " angleByte: 0x" << SSTREAM_HEX2(terrainAngleByte) << " normal: " << terrainNormal.x << "," << terrainNormal.y << std::endl;
	tipStr << "Stamp pos: 0x" << SSTREAM_HEX4(object.stampPos.x) << ", 0x" << SSTREAM_HEX4(object.stampPos.y) << " (" << object.stampPos.x << ", " << object.stampPos.y << ")" << std::endl;
	tipStr << "Size: " << object.stamp->GetWidth() << ", " << object.stamp->GetHeight() << std::endl;
	tipStr << "Addr: 0x" << SSTREAM_HEX8(object.stampId * tileSize.x * tileSize.y * 4) << std::endl;

	return tipStr.str();
}

ion::Vector2i MapToolManipulatorStamp::GetUnitSizePx()
{
	return m_unitSizePx;
}

int MapToolManipulatorStamp::GetRegionSelectorFlags()
{
	return MapToolRegionSelector::Flags::AllowMultipleSelection
		 | MapToolRegionSelector::Flags::AllowBoxSelection
		 | MapToolRegionSelector::Flags::DrawRegionFill
		 | MapToolRegionSelector::Flags::DrawCursor;
}

void MapToolManipulatorStamp::FindObjects(const Map& map, const std::vector<MapRegion>& regions, std::vector<MapObjIdentifierStamp>& selection)
{
	const ion::Vector2i tileSize(GetProject().GetPlatformConfig().tileWidth, GetProject().GetPlatformConfig().tileHeight);
	const ion::Vector2i stampSize(GetProject().GetPlatformConfig().stampWidth, GetProject().GetPlatformConfig().stampHeight);

	for (auto region : regions)
	{
		MapObjIdentifierStamp stamp;

		for (int x = region.topLeft.x; x <= region.bottomRight.x; x++)
		{
			for (int y = region.topLeft.y; y <= region.bottomRight.y; y++)
			{
				//Stamp map coords in tiles - convert to and from
				stamp.stampId = map.FindStamp(x * stampSize.x, y * stampSize.y, stamp.stampPos, stamp.stampFlags, stamp.stampMapEntryIdx);
				stamp.stampPos /= stampSize;

				//Resolve
				stamp.stamp = GetProject().GetStamp(stamp.stampId);

				if (stamp.stamp)
				{
					if (!ion::utils::stl::Find(selection, stamp))
					{
						selection.push_back(stamp);
					}
				}
			}
		}
	}
}

void MapToolManipulatorStamp::MoveObjects(Map& map, std::vector<MapObjIdentifierStamp>& selection, const ion::Vector2i& delta)
{

}

void MapToolManipulatorStamp::DeleteObjects(Map& map, std::vector<MapObjIdentifierStamp>& selection)
{
	const ion::Vector2i tileSize(GetProject().GetPlatformConfig().tileWidth, GetProject().GetPlatformConfig().tileHeight);
	const ion::Vector2i stampSize(GetProject().GetPlatformConfig().stampWidth, GetProject().GetPlatformConfig().stampHeight);

	for (auto stamp : selection)
	{
		//Stamp map coords in tiles
		ion::Vector2i stampPosTiles = stamp.stampPos * stampSize;

		//Remove stamp
		map.RemoveStamp(stamp.stampId, stampPosTiles.x, stampPosTiles.y);

		//Repaint area underneath stamp
		for (int tileX = stampPosTiles.x; tileX < stampPosTiles.x + stamp.stamp->GetWidth(); tileX++)
		{
			for (int tileY = stampPosTiles.y; tileY < stampPosTiles.y + stamp.stamp->GetHeight(); tileY++)
			{
				//Invert Y for OpenGL
				int y_inv = map.GetHeight() - 1 - tileY;
				GetMapPanel().PaintTile(map.GetTile(tileX, tileY), tileX, y_inv, map.GetTileFlags(tileX, tileY));
			}
		}
	}
}

void MapToolManipulatorStamp::OnContextMenuEditCollision(int id, std::vector<MapObjIdentifierStamp>& objects)
{
	for (auto stamp : objects)
	{
		GetMapPanel().EditStampCollisionDlg(*stamp.stamp);
	}
}