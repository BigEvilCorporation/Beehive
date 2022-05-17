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

#include "MapToolStampManipulator.h"
#include "MapPanel.h"

#include <ion/core/utils/STL.h>

MapToolStampManipulator::MapToolStampManipulator(Project& project, MapPanel& mapPanel, TUndoStack& undoStack)
	: Tool(StaticType(), project, mapPanel, undoStack)
	, m_mapSelector(project, ion::Vector2i(project.GetPlatformConfig().stampWidth * project.GetPlatformConfig().tileWidth, project.GetPlatformConfig().stampHeight * project.GetPlatformConfig().tileHeight), true, true, true)
{

}

std::string MapToolStampManipulator::GetName()
{
	return "Stamp Tool";
}

std::string MapToolStampManipulator::GetDescription()
{
	return "Move, copy, and delete stamps or stamp regions";
}

std::string MapToolStampManipulator::GetStatusText()
{
	return "CTRL: multiple/box select, SHIFT: copy selection";
}

void MapToolStampManipulator::OnKeyboard(wxKeyEvent& event)
{
	m_mapSelector.OnKeyboard(event);
}

void MapToolStampManipulator::OnMousePixelEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int tileX, int tileY)
{
	m_mapSelector.OnMousePixelEvent(mousePos, mouseDelta, tileDelta, buttonBits, tileX, tileY);

	if (m_mapSelector.NeedsRedraw())
	{
		Redraw();
	}

	const Map& map = m_project.GetEditingMap();
	const ion::Vector2i tileSize(m_project.GetPlatformConfig().tileWidth, m_project.GetPlatformConfig().tileHeight);

	//Find stamp under cursor
	ion::Vector2i cursorPosTiles = mousePos / tileSize;
	ion::Vector2i cursorPosStamps = m_mapSelector.GetCursorPos();
	m_cursor.stampId = map.FindStamp(cursorPosTiles.x, cursorPosTiles.y, m_cursor.stampPos, m_cursor.stampFlags, m_cursor.stampMapEntryIdx);
	m_cursor.stamp = m_project.GetStamp(m_cursor.stampId);

	if (m_cursor.stamp && buttonBits == 0)
		SetTooltip(m_cursor, cursorPosTiles);
	else
		m_mapPanel.UnsetToolTip();

	if (buttonBits & eMouseRight)
	{
		EnumerateSelection();

		//Nothing selected? Take stamp under cursor
		if (m_selected.size() == 0 && m_cursor.stamp)
			m_selected.push_back(m_cursor);

		//Right-click menu
		wxMenu contextMenu;
		wxMenuItem* itemDelete = contextMenu.Append((int)ContextMenu::Delete, wxString("Delete stamp(s)"));
		wxMenuItem* itemCollision = contextMenu.Append((int)ContextMenu::EditCollision, wxString("Edit stamp collision"));

		if (m_selected.size() == 0)
		{
			itemDelete->Enable(false);
			itemCollision->Enable(false);
		}

		contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MapToolStampManipulator::OnContextMenuClick, NULL, this);
		m_mapPanel.PopupMenu(&contextMenu);
	}
}

void MapToolStampManipulator::OnRender(ion::render::Renderer& renderer, RenderResources& renderResources, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	m_mapSelector.OnRender(renderer, renderResources, cameraInverseMtx, projectionMtx, z, zOffset);
	Tool::OnRender(renderer, renderResources, cameraInverseMtx, projectionMtx, z, zOffset);
}

void MapToolStampManipulator::EnumerateSelection()
{
	const Map& map = m_project.GetEditingMap();
	const ion::Vector2i stampSize(m_project.GetPlatformConfig().stampWidth, m_project.GetPlatformConfig().stampHeight);

	m_selected.clear();

	for (auto region : m_mapSelector.GetSelectedRegions())
	{
		Selection selection;

		for (int x = region.topLeft.x; x <= region.bottomRight.x; x++)
		{
			for (int y = region.topLeft.y; y <= region.bottomRight.y; y++)
			{
				selection.stampId = map.FindStamp(x * stampSize.x, y * stampSize.y, selection.stampPos, selection.stampFlags, selection.stampMapEntryIdx);
				selection.stamp = m_project.GetStamp(selection.stampId);

				if (selection.stamp)
				{
					if (!ion::utils::stl::Find(m_selected, selection))
					{
						m_selected.push_back(selection);
					}
				}
			}
		}
	}
}

void MapToolStampManipulator::DeleteSelection()
{
	Map& map = m_project.GetEditingMap();

	for(auto stamp : m_selected)
	{
		//Remove stamp
		map.RemoveStamp(stamp.stampId, stamp.stampPos.x, stamp.stampPos.y);

		//Repaint area underneath stamp
		for (int tileX = stamp.stampPos.x; tileX < stamp.stampPos.x + stamp.stamp->GetWidth(); tileX++)
		{
			for (int tileY = stamp.stampPos.y; tileY < stamp.stampPos.y + stamp.stamp->GetHeight(); tileY++)
			{
				//Invert Y for OpenGL
				int y_inv = m_project.GetEditingMap().GetHeight() - 1 - tileY;
				m_mapPanel.PaintTile(map.GetTile(tileX, tileY), tileX, y_inv, map.GetTileFlags(tileX, tileY));
			}
		}
	}

	m_selected.clear();
	Redraw();
}

void MapToolStampManipulator::OnContextMenuClick(wxCommandEvent& event)
{
	Map& map = m_project.GetEditingMap();

	if (event.GetId() == (int)ContextMenu::Delete)
	{
		DeleteSelection();
	}
	else if (event.GetId() == (int)ContextMenu::EditCollision)
	{
		//Show collision editor dialog
		if (m_selected.size())
		{
			for (auto stamp : m_selected)
			{
				m_mapPanel.EditStampCollisionDlg(*stamp.stamp);
			}
		}
		else if (m_cursor.stamp)
		{
			m_mapPanel.EditStampCollisionDlg(*m_cursor.stamp);
		}
	}
}

void MapToolStampManipulator::SetTooltip(const Selection& stamp, const ion::Vector2i& cursorPosTiles)
{
	const ion::Vector2i tileSize(m_project.GetPlatformConfig().tileWidth, m_project.GetPlatformConfig().tileHeight);
	int stampX = stamp.stampPos.x % stamp.stamp->GetWidth();
	int stampY = stamp.stampPos.y % stamp.stamp->GetHeight();
	TileId tileId = stamp.stamp->GetTile(stampX, stampY);

	TerrainTileId terrainTileId = stamp.stamp->GetTerrainTile(stampX, stampY);
	u16 collisionFlags = stamp.stamp->GetCollisionTileFlags(stampX, stampY);
	u8 terrainAngleByte = 0;
	ion::Vector2 terrainNormal;
	if (const TerrainTile* terrainTile = m_project.GetTerrainTileset().GetTerrainTile(terrainTileId))
	{
		terrainAngleByte = terrainTile->GetAngleByte();
		terrainNormal = terrainTile->GetNormal();
	}

	std::stringstream tipStr;
	tipStr << "Cursor: 0x" << SSTREAM_HEX4(cursorPosTiles.x) << ", 0x" << SSTREAM_HEX4(cursorPosTiles.y) << " (" << cursorPosTiles.x << ", " << cursorPosTiles.y << ")" << std::endl;
	tipStr << "Stamp: 0x" << SSTREAM_HEX4(stamp.stampId) << " (" << stamp.stampId << ")" << std::endl;
	tipStr << "Tile: 0x" << SSTREAM_HEX4(tileId) << " (" << tileId << ")" << std::endl;
	tipStr << "Terrain: 0x" << SSTREAM_HEX4(terrainTileId) << " (" << terrainTileId << ")" << " collisionFlags: 0x" << SSTREAM_HEX2(collisionFlags) << " angleByte: 0x" << SSTREAM_HEX2(terrainAngleByte) << " normal: " << terrainNormal.x << "," << terrainNormal.y << std::endl;
	tipStr << "Stamp pos: 0x" << SSTREAM_HEX4(stamp.stampPos.x) << ", 0x" << SSTREAM_HEX4(stamp.stampPos.y) << " (" << stamp.stampPos.x << ", " << stamp.stampPos.y << ")" << std::endl;
	tipStr << "Size: " << stamp.stamp->GetWidth() << ", " << stamp.stamp->GetHeight() << std::endl;
	tipStr << "Addr: 0x" << SSTREAM_HEX8(stamp.stampId * tileSize.x * tileSize.y * 4) << std::endl;
	m_mapPanel.SetToolTip(tipStr.str().c_str());
}
