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

#include "MapListPanel.h"
#include "MainWindow.h"
#include "Dialogs.h"

#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>

MapListPanel::MapListPanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: MapListPanelBase(parent, id, pos, size, style)
	, m_project(project)
	, m_mainWindow(mainWindow)
{

}

void MapListPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(!m_mainWindow->IsRefreshLocked())
	{
		m_listMaps->ClearAll();
		m_mapIds.clear();

		MapId editingMap = m_project.GetEditingMapId();

		int index = 0;
		for(TMapMap::const_iterator it = m_project.MapsBegin(), end = m_project.MapsEnd(); it != end; ++it, ++index)
		{
			m_mapIds.push_back(it->first);
			m_listMaps->InsertItem(index, wxString(it->second.GetName()));

			if(it->first == editingMap)
			{
				m_listMaps->SetItemState(index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
			}
		}
	}
}

void MapListPanel::OnMapSelected(wxListEvent& event)
{
	MapId mapId = m_mapIds[event.GetIndex()];
	CollisionMapId collisionMapId = m_mapIds[event.GetIndex()];

	if(mapId != m_project.GetEditingMapId() || collisionMapId != m_project.GetEditingCollisionMapId())
	{
		m_project.SetEditingMap(m_mapIds[event.GetIndex()]);
		m_project.SetEditingCollisionMap(m_mapIds[event.GetIndex()]);

		m_project.InvalidateMap(true);
		m_project.InvalidateCamera(true);
		m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
		m_project.InvalidateMap(false);
		m_project.InvalidateCamera(false);
	}
}

void MapListPanel::OnToolAddMap(wxCommandEvent& event)
{
	DialogNewMap dialog(m_mainWindow);

	if(dialog.ShowModal() == wxID_OK)
	{
		MapId newMapId = m_project.CreateMap();
		CollisionMapId collisionMapId = m_project.CreateCollisionMap(newMapId);
		Map& newMap = m_project.GetMap(newMapId);
		CollisionMap& newCollisionMap = m_project.GetCollisionMap(collisionMapId);
		newMap.SetName(dialog.m_textMapName->GetValue().GetData().AsChar());
		newMap.Resize(dialog.m_spinCtrlWidth->GetValue(), dialog.m_spinCtrlHeight->GetValue(), false, false);
		newCollisionMap.Resize(dialog.m_spinCtrlWidth->GetValue(), dialog.m_spinCtrlHeight->GetValue(), false, false);
		m_project.SetEditingMap(newMapId);
		m_project.SetEditingCollisionMap(newMapId);

		m_project.InvalidateMap(true);
		m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
		m_project.InvalidateMap(false);

		Refresh(false, NULL);
	}
}

void MapListPanel::OnToolRemoveMap(wxCommandEvent& event)
{
	if(m_mapIds.size() > 1)
	{
		m_project.DeleteMap(m_project.GetEditingMapId());
		Refresh(false, NULL);
		m_project.SetEditingMap(m_mapIds[0]);
		m_project.SetEditingCollisionMap(m_mapIds[0]);

		m_project.InvalidateMap(true);
		m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
		m_project.InvalidateMap(false);
	}
}

void MapListPanel::OnToolImportMap(wxCommandEvent& event)
{
	wxFileDialog dialog(this, _("Open BEE file"), "", "", "BEE files (*.bee)|*.bee", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(dialog.ShowModal() == wxID_OK)
	{
		Project project(PlatformPresets::s_configs[ePlatformMegaDrive]);

		const Tileset& tileset = project.GetTileset();

		if(project.Load(dialog.GetPath().c_str().AsChar()))
		{
			//Recalculate all hashes
			m_project.GetTileset().RebuildHashMap();
			project.GetTileset().RebuildHashMap();

			//Import all maps
			for(TMapMap::const_iterator it = project.MapsBegin(), end = project.MapsEnd(); it != end; ++it)
			{
				MapId mapId = it->first;
				const Map& map = it->second;

				MapId newMapId = m_project.CreateMap();
				Map& newMap = m_project.GetMap(newMapId);
				newMap.SetName(map.GetName());
				newMap.Resize(map.GetWidth(), map.GetHeight(), false, false);

				//Import collision map
				const CollisionMap& collisionMap = project.GetCollisionMap(mapId);
				CollisionMapId newCollisionMapId = m_project.CreateCollisionMap(newMapId);
				CollisionMap& newCollisionMap = m_project.GetCollisionMap(newCollisionMapId);
				newCollisionMap.Resize(collisionMap.GetWidth(), collisionMap.GetHeight(), false, false);

				//Import all terrain tiles
				for(int x = 0; x < collisionMap.GetWidth(); x++)
				{
					for(int y = 0; y < collisionMap.GetHeight(); y++)
					{
						TerrainTileId terrainTileId = collisionMap.GetTerrainTile(x, y);
						if(TerrainTile* terrainTile = project.GetTerrainTileset().GetTerrainTile(terrainTileId))
						{
							TerrainTileId existingTileId = m_project.GetTerrainTileset().FindDuplicate(*terrainTile);
							if(existingTileId != InvalidTileId)
							{
								newCollisionMap.SetTerrainTile(x, y, existingTileId);
							}
						}

						u32 collisionTileFlags = collisionMap.GetCollisionTileFlags(x, y);
						newCollisionMap.SetCollisionTileFlags(x, y, collisionTileFlags);
					}
				}

				//Import all terrain bezier paths
				for(int i = 0; i < collisionMap.GetNumTerrainBeziers(); i++)
				{
					newCollisionMap.AddTerrainBezier(*collisionMap.GetTerrainBezier(i));
				}

				//Import all tiles
				for(int x = 0; x < map.GetWidth(); x++)
				{
					for(int y = 0; y < map.GetHeight(); y++)
					{
						TileId tileId = map.GetTile(x, y);
						u32 tileFlags = map.GetTileFlags(x, y);
						if(const Tile* tile = tileset.GetTile(tileId))
						{
							TileId existingTileId = m_project.GetTileset().FindDuplicate(*tile, tileFlags);
							if(existingTileId != InvalidTileId)
							{
								newMap.SetTile(x, y, existingTileId);
								newMap.SetTileFlags(x, y, tileFlags);
							}
						}
					}
				}

				//Import all stamps
				for(TStampPosMap::const_iterator it = map.StampsBegin(), end = map.StampsEnd(); it != end; ++it)
				{
					if(const Stamp* stamp = project.GetStamp(it->m_id))
					{
						Stamp tempStamp(InvalidStampId, stamp->GetWidth(), stamp->GetHeight());

						for(int x = 0; x < stamp->GetWidth(); x++)
						{
							for(int y = 0; y < stamp->GetHeight(); y++)
							{
								TileId tileId = stamp->GetTile(x, y);
								u32 tileFlags = stamp->GetTileFlags(x, y);
								if(const Tile* tile = tileset.GetTile(tileId))
								{
									u32 existingFlags = 0;
									TileId existingTileId = m_project.GetTileset().FindDuplicate(*tile, existingFlags);
									if(existingTileId != InvalidTileId)
									{
										tempStamp.SetTile(x, y, existingTileId);
										tempStamp.SetTileFlags(x, y, tileFlags ^ existingFlags);
									}
								}
							}
						}

						StampId existingStampId = m_project.FindDuplicateStamp(&tempStamp);

						if(existingStampId == InvalidStampId)
						{
							//Duplicate temp stamp
							StampId newStampId = m_project.AddStamp(&tempStamp);
							Stamp* newStamp = m_project.GetStamp(newStampId);
							newMap.SetStamp(it->m_position.x, it->m_position.y, *newStamp, it->m_flags);
						}
						else
						{
							//Set existing
							newMap.SetStamp(it->m_position.x, it->m_position.y, *m_project.GetStamp(existingStampId), it->m_flags);
						}
					}
				}

				//Import all game objects
				for(TGameObjectPosMap::const_iterator it = map.GetGameObjects().begin(), end = map.GetGameObjects().end(); it != end; ++it)
				{
					for(int i = 0; i < it->second.size(); i++)
					{
						if(const GameObjectType* gameObjectType = m_project.GetGameObjectType(it->first))
						{
							newMap.PlaceGameObject(it->second[i].m_gameObject.GetPosition().x, it->second[i].m_gameObject.GetPosition().y, it->second[i].m_gameObject, *gameObjectType);
						}
						else
						{
							wxMessageBox("Game object type ID mismatch, game objects will be missing", "Error", wxOK);
						}
					}
				}
			}

			m_project.InvalidateMap(true);
			m_project.InvalidateTerrainTiles(true);
			m_mainWindow->RefreshPanel(MainWindow::ePanelMap);
			m_project.InvalidateTerrainTiles(false);
			m_project.InvalidateMap(false);

			Refresh(false, NULL);
		}
		else
		{
			wxMessageBox("Error importing map", "Error", wxOK | wxICON_ERROR);
		}
	}
}

void MapListPanel::OnToolRenameMap(wxCommandEvent& event)
{
	wxTextEntryDialog dialog(m_mainWindow, "Rename Map");
	if(dialog.ShowModal() == wxID_OK)
	{
		m_project.GetEditingMap().SetName(dialog.GetValue().c_str().AsChar());
		Refresh(false, NULL);
	}
}