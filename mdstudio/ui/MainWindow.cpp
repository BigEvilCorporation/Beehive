///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/aui/framemanager.h>
#include <wx/msgdlg.h>

#include <string>

#include "MainWindow.h"
#include "Dialogs.h"
#include "PalettesPanel.h"
#include "TilesPanel.h"
#include "MapPanel.h"

wxDEFINE_SCOPED_PTR(Project, ProjectPtr)

MainWindow::MainWindow()
	: MainWindowBase(NULL)
{
	SetStatusText("BEEhive v0.1");

	m_tilesetSizeSq = 1;
	m_cellSizeTexSpaceSq = 1.0f;

	//Setup panel docking manager
	m_auiManager.SetManagedWindow(m_dockArea);
	m_auiManager.SetFlags(	wxAUI_MGR_ALLOW_FLOATING			//Allow floating panels
							| wxAUI_MGR_TRANSPARENT_HINT		//Draw possible dock locations
							| wxAUI_MGR_TRANSPARENT_DRAG		//Transparent panel whilst dragging
							| wxAUI_MGR_LIVE_RESIZE);			//Refresh on each discreet resize

	//Create blank OpenGL panel to create global DC
	wxGLCanvas* m_blankCanvas = new wxGLCanvas(this, wxID_ANY, NULL);

	//Get GL context
	m_context = m_blankCanvas->GetContext();

	//Create renderer (from global DC
	m_renderer = ion::render::Renderer::Create(m_blankCanvas->GetHDC());

	//Create tileset texture
	m_tilesetTexture = ion::render::Texture::Create();

	//Create default project
	SetProject(new Project());
}

MainWindow::~MainWindow()
{
	//Close project and panels
	SetProject(NULL);

	//Delete texture, renderer and OpenGL context
	delete m_tilesetTexture;
	delete m_renderer;
	delete m_context;
}

void MainWindow::SetProject(Project* project)
{
	if(project != m_project.get())
	{
		//Changed project, close all panels
		if(m_tilesPanel)
		{
			m_auiManager.DetachPane(m_tilesPanel);
			delete m_tilesPanel;
		}

		if(m_palettesPanel)
		{
			m_auiManager.DetachPane(m_palettesPanel);
			delete m_palettesPanel;
		}

		if(m_mapPanel)
		{
			m_auiManager.DetachPane(m_mapPanel);
			delete m_mapPanel;
		}

		if(m_toolboxPanel)
		{
			m_auiManager.DetachPane(m_toolboxPanel);
			delete m_toolboxPanel;
		}

		if(m_stampsPanel)
		{
			m_auiManager.DetachPane(m_stampsPanel);
			delete m_stampsPanel;
		}

		if(m_tileEditorPanel)
		{
			m_auiManager.DetachPane(m_tileEditorPanel);
			delete m_tileEditorPanel;
		}

		//Delete previous, set new
		m_project.reset(project);

		if(project)
		{
			//Recreate tileset texture and tile index cache
			RefreshTileset();

			//New project, open default panels
			ShowPanelToolbox();
			ShowPanelPalettes();
			ShowPanelTiles();
			ShowPanelStamps();
			ShowPanelTileEditor();
			ShowPanelMap();

			//Sync settings widgets states
			SyncSettingsWidgets();
		}

		//Refresh whole window
		RefreshAll();
	}
}

void MainWindow::CreateTilesetTexture(const Tileset& tileset)
{
	const int tileWidth = 8;
	const int tileHeight = 8;

	u32 numTiles = tileset.GetCount();
	m_tilesetSizeSq = (u32)ion::maths::Ceil(ion::maths::Sqrt((float)numTiles));
	u32 textureWidth = m_tilesetSizeSq * tileWidth;
	u32 textureHeight = m_tilesetSizeSq * tileHeight;
	u32 bytesPerPixel = 3;
	u32 textureSize = textureWidth * textureHeight * bytesPerPixel;
	m_cellSizeTexSpaceSq = 1.0f / (float)m_tilesetSizeSq;

	u8* data = new u8[textureSize];
	ion::memory::MemSet(data, 0, textureSize);

	u32 tileIndex = 0;

	for(TTileMap::const_iterator it = tileset.Begin(), end = tileset.End(); it != end; ++it, ++tileIndex)
	{
		const Tile& tile = it->second;
		PaletteId paletteId = tile.GetPaletteId();
		Palette* palette = m_project->GetPalette(paletteId);

		u32 x = tileIndex % m_tilesetSizeSq;
		u32 y = tileIndex / m_tilesetSizeSq;

		for(int pixelY = 0; pixelY < 8; pixelY++)
		{
			for(int pixelX = 0; pixelX < 8; pixelX++)
			{
				//Invert Y for OpenGL
				int pixelY_OGL = tileHeight - 1 - pixelY;
				const Colour& colour = palette->GetColour(tile.GetPixelColour(pixelX, pixelY_OGL));

				int destPixelX = (x * tileWidth) + pixelX;
				int destPixelY = (y * tileHeight) + pixelY;
				u32 pixelIdx = (destPixelY * textureWidth) + destPixelX;
				u32 dataOffset = pixelIdx * bytesPerPixel;
				ion::debug::Assert(dataOffset + 2 < textureSize, "Out of bounds");
				data[dataOffset] = colour.r;
				data[dataOffset + 1] = colour.g;
				data[dataOffset + 2] = colour.b;
			}
		}
	}

	m_tilesetTexture->Load(textureWidth, textureHeight, ion::render::Texture::eRGB, ion::render::Texture::eRGB, ion::render::Texture::eBPP24, false, data);
	m_tilesetTexture->SetMinifyFilter(ion::render::Texture::eFilterNearest);
	m_tilesetTexture->SetMagnifyFilter(ion::render::Texture::eFilterNearest);
	m_tilesetTexture->SetWrapping(ion::render::Texture::eWrapClamp);

	delete data;
}

void MainWindow::CacheTileIndices(const Tileset& tileset)
{
	m_tileIndexMap.clear();

	if(m_project.get())
	{
		u32 tileIndex = 0;

		for(TTileMap::const_iterator it = tileset.Begin(), end = tileset.End(); it != end; ++it, ++tileIndex)
		{
			//Map ID to index
			m_tileIndexMap.insert(std::make_pair(it->first, tileIndex));
		}
	}
}

void MainWindow::GetTileTexCoords(TileId tileId, ion::render::TexCoord texCoords[4], u32 flipFlags) const
{
	if(tileId == InvalidTileId)
	{
		//Invalid tile, use top-left pixel
		float onePixelTexSpace = m_cellSizeTexSpaceSq / 8.0f;

		//Top left
		texCoords[0].x = 0.0f;
		texCoords[0].y = 0.0f;
		//Bottom left
		texCoords[1].x = 0.0f;
		texCoords[1].y = onePixelTexSpace;
		//Bottom right
		texCoords[2].x = onePixelTexSpace;
		texCoords[2].y = onePixelTexSpace;
		//Top right
		texCoords[3].x = onePixelTexSpace;
		texCoords[3].y = 0.0f;
	}
	else
	{
		//Map tile ID to index
		u32 tileIndex = GetTilesetTexTileIndex(tileId);

		//Map tile to X/Y on tileset texture
		int tilesetX = (tileIndex % m_tilesetSizeSq);
		int tilesetY = (tileIndex / m_tilesetSizeSq);
		ion::Vector2 textureBottomLeft(m_cellSizeTexSpaceSq * tilesetX, m_cellSizeTexSpaceSq * tilesetY);

		bool flipX = (flipFlags & Map::eFlipX) != 0;
		bool flipY = (flipFlags & Map::eFlipY) != 0;

		float top = flipY ? (textureBottomLeft.y) : (textureBottomLeft.y + m_cellSizeTexSpaceSq);
		float left = flipX ? (textureBottomLeft.x + m_cellSizeTexSpaceSq) : (textureBottomLeft.x);
		float bottom = flipY ? (textureBottomLeft.y + m_cellSizeTexSpaceSq) : (textureBottomLeft.y);
		float right = flipX ? (textureBottomLeft.x) : (textureBottomLeft.x + m_cellSizeTexSpaceSq);

		//Top left
		texCoords[0].x = left;
		texCoords[0].y = top;
		//Bottom left
		texCoords[1].x = left;
		texCoords[1].y = bottom;
		//Bottom right
		texCoords[2].x = right;
		texCoords[2].y = bottom;
		//Top right
		texCoords[3].x = right;
		texCoords[3].y = top;
	}
}

int MainWindow::GetTilesetTexTileIndex(TileId tileId) const
{
	std::map<TileId, u32>::const_iterator it = m_tileIndexMap.find(tileId);
	ion::debug::Assert(it != m_tileIndexMap.end(), "TileId not found in tileset");
	return it->second;
}

void MainWindow::SetTilesetTexPixel(TileId tileId, const ion::Vector2i& pixel, u8 colourIdx)
{
	if(m_project.get() && m_tilesetTexture)
	{
		if(Tile* tile = m_project->GetTileset().GetTile(tileId))
		{
			if(Palette* palette = m_project->GetPalette(tile->GetPaletteId()))
			{
				const int tileWidth = 8;
				const int tileHeight = 8;

				const Colour& colour = palette->GetColour(colourIdx);

				int tileIndex = GetTilesetTexTileIndex(tileId);
				u32 x = tileIndex % m_tilesetSizeSq;
				u32 y = tileIndex / m_tilesetSizeSq;

				//Invert Y for OpenGL
				int y_inv = tileHeight - 1 - pixel.y;

				ion::Vector2i pixelPos((x * tileWidth) + pixel.x, (y * tileHeight) + y_inv);

				m_tilesetTexture->SetPixel(pixelPos, ion::Colour(colour.r, colour.g, colour.b));
			}
		}
	}
}

void MainWindow::ShowPanelPalettes()
{
	if(m_project.get())
	{
		if(!m_palettesPanel)
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(100, 300);
			paneInfo.Left();
			paneInfo.Caption("Palettes");
			paneInfo.CaptionVisible(true);

			m_palettesPanel = new PalettesPanel(this, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_palettesPanel, paneInfo);
			m_palettesPanel->Show();

			m_palettesPanel->SetProject(m_project.get());
			
		}

		if(!m_palettesPanel->IsShown())
		{
			m_palettesPanel->Show();
		}
	}
}

void MainWindow::ShowPanelTiles()
{
	if(m_project.get())
	{
		if(!m_tilesPanel)
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(300, 300);
			paneInfo.Right();
			paneInfo.Caption("Tileset");
			paneInfo.CaptionVisible(true);
			
			m_tilesPanel = new TilesPanel(this, *m_renderer, m_context, m_tilesetTexture, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_tilesPanel, paneInfo);
			m_tilesPanel->Show();

			m_tilesPanel->SetProject(m_project.get());
		}

		if(!m_tilesPanel->IsShown())
		{
			m_tilesPanel->Show();
		}
	}
}

void MainWindow::ShowPanelStamps()
{
	if(m_project.get())
	{
		if(!m_stampsPanel)
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(300, 300);
			paneInfo.Right();
			paneInfo.Caption("Stamps");
			paneInfo.CaptionVisible(true);

			m_stampsPanel = new StampsPanel(this, *m_renderer, m_context, m_tilesetTexture, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_stampsPanel, paneInfo);
			m_stampsPanel->Show();

			m_stampsPanel->SetProject(m_project.get());
		}

		if(!m_stampsPanel->IsShown())
		{
			m_stampsPanel->Show();
		}
	}
}

void MainWindow::ShowPanelMap()
{
	if(m_project.get())
	{
		if(!m_mapPanel)
		{
			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(300, 300);
			paneInfo.Centre();
			paneInfo.Caption("Map");
			paneInfo.CaptionVisible(true);

			m_mapPanel = new MapPanel(this, *m_renderer, m_context, m_tilesetTexture, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_mapPanel, paneInfo);
			m_mapPanel->Show();

			m_mapPanel->SetProject(m_project.get());
		}

		if(!m_mapPanel->IsShown())
		{
			m_mapPanel->Show();
		}
	}
}

void MainWindow::ShowPanelToolbox()
{
	if(!m_toolboxPanel)
	{
		wxAuiPaneInfo paneInfo;
		paneInfo.Dockable(true);
		paneInfo.DockFixed(false);
		paneInfo.BestSize(100, 300);
		paneInfo.Left();
		paneInfo.Caption("Toolbox");
		paneInfo.CaptionVisible(true);

		m_toolboxPanel = new MapToolbox(m_dockArea, NewControlId());
		m_auiManager.AddPane(m_toolboxPanel, paneInfo);

		//Subscribe to toolbox buttons
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_SELECTTILE);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_SELECTSTAMP);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_PAINT);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_STAMP);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_TILEPICKER);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_STAMPPICKER);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_FLIPX);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_FLIPY);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_FILL);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_CLONE);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_CREATESTAMP);
		Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnBtnTool, this, wxID_TOOL_REMOVESTAMP);
	}

	if(!m_toolboxPanel->IsShown())
	{
		m_toolboxPanel->Show();
	}
}

void MainWindow::ShowPanelTileEditor()
{
	if(m_project.get())
	{
		if(!m_tileEditorPanel)
		{
			wxSize clientSize = GetClientSize();

			wxAuiPaneInfo paneInfo;
			paneInfo.Dockable(true);
			paneInfo.DockFixed(false);
			paneInfo.BestSize(300, 300);
			paneInfo.Right();
			paneInfo.Caption("Tile");
			paneInfo.CaptionVisible(true);

			m_tileEditorPanel = new TileEditorPanel(this, *m_renderer, m_context, m_tilesetTexture, m_dockArea, NewControlId());
			m_auiManager.AddPane(m_tileEditorPanel, paneInfo);
			m_tileEditorPanel->Show();
			m_auiManager.Update();

			m_tileEditorPanel->SetProject(m_project.get());
		}

		if(!m_tileEditorPanel->IsShown())
		{
			m_tileEditorPanel->Show();
		}
	}
}

void MainWindow::SetMapTool(MapPanel::Tool tool)
{
	if(m_mapPanel)
	{
		m_mapPanel->SetTool(tool);
	}
}

void MainWindow::SyncSettingsWidgets()
{
	if(m_project.get())
	{
		m_ribbonButtonBarGrid->ToggleButton(wxID_BTN_GRID_SHOW, m_project->GetShowGrid());
		m_ribbonButtonBarGrid->ToggleButton(wxID_BTN_GRID_SNAP, m_project->GetGridSnap());
	}
}

void MainWindow::RefreshAll()
{
	m_auiManager.Update();
	Refresh();

	if(m_project.get())
	{
		m_project->InvalidateMap(true);
		m_project->InvalidateTiles(true);
		m_project->InvalidateStamps(true);
	}

	RedrawAll();

	if(m_project.get())
	{
		m_project->InvalidateMap(false);
		m_project->InvalidateTiles(false);
		m_project->InvalidateStamps(false);
	}
}

void MainWindow::RedrawAll()
{
	if(m_project.get())
		CreateTilesetTexture(m_project->GetTileset());

	if(m_palettesPanel)
		m_palettesPanel->Refresh();

	if(m_tilesPanel)
		m_tilesPanel->Refresh();

	if(m_mapPanel)
		m_mapPanel->Refresh();

	if(m_stampsPanel)
		m_stampsPanel->Refresh();

	if(m_tileEditorPanel)
		m_tileEditorPanel->Refresh();
}

void MainWindow::RefreshTileset()
{
	if(m_project.get())
	{
		//Recreate tileset texture
		CreateTilesetTexture(m_project->GetTileset());

		//Recreate index cache
		CacheTileIndices(m_project->GetTileset());
	}
}

void MainWindow::RefreshPanel(Panel panel)
{
	if(m_project.get())
	{
		m_project->InvalidateMap(true);
		m_project->InvalidateTiles(true);
		m_project->InvalidateStamps(true);

	}
	
	RedrawPanel(panel);

	if(m_project.get())
	{
		m_project->InvalidateMap(false);
		m_project->InvalidateTiles(false);
		m_project->InvalidateStamps(false);
	}
}

void MainWindow::RedrawPanel(Panel panel)
{
	switch(panel)
	{
	case ePanelMap:
		if(m_mapPanel)
			m_mapPanel->Refresh();
		break;
	case ePanelStamps:
		if(m_stampsPanel)
			m_stampsPanel->Refresh();
		break;
	case ePanelTiles:
		if(m_tilesPanel)
			m_tilesPanel->Refresh();
		break;
	case ePanelPalettes:
		if(m_palettesPanel)
			m_palettesPanel->Refresh();
		break;
	case ePanelTileEditor:
		if(m_tileEditorPanel)
			m_tileEditorPanel->Refresh();
		break;
	}
}

void MainWindow::OnBtnProjNew(wxRibbonButtonBarEvent& event)
{
	if(wxMessageBox("Unsaved changes will be lost, are you sure?", "New Project", wxOK | wxCANCEL) == wxOK)
	{
		SetProject(new Project());
	}
}

void MainWindow::OnBtnProjOpen(wxRibbonButtonBarEvent& event)
{
	wxFileDialog dialogue(this, _("Open BEE file"), "", "", "BEE files (*.bee)|*.bee", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(dialogue.ShowModal() == wxID_OK)
	{
		SetStatusText("Opening...");

		Project* project = new Project();

		if(project->Load(dialogue.GetPath().c_str().AsChar()))
		{
			//Set new project, deletes old
			SetProject(project);
			
			SetStatusText("Load complete");
		}
		else
		{
			delete project;
			SetStatusText("Load error");
			wxMessageBox("Error loading project", "Error", wxOK | wxICON_ERROR);
		}
	}
}

void MainWindow::OnBtnProjSave(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		std::string filename = m_project->GetFilename();

		if(!filename.size())
		{
			//No previous filename
			wxFileDialog dialogue(this, _("Open BEE file"), "", "", "BEE files (*.bee)|*.bee", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
			if(dialogue.ShowModal() == wxID_OK)
			{
				filename = dialogue.GetPath().c_str().AsChar();
			}
		}
		
		if(filename.size())
		{
			//Save
			SetStatusText("Saving...");

			if(m_project->Save(filename))
			{
				SetStatusText("Save complete");
			}
			else
			{
				wxMessageBox("Error writing project", "Error", wxOK | wxICON_ERROR);
				SetStatusText("Save error");
			}
		}
	}
}

void MainWindow::OnBtnProjExport(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		wxDirDialog dialogue(this, "Select export directory");
		if(dialogue.ShowModal() == wxID_OK)
		{
			SetStatusText("Exporting...");

			std::string directory = dialogue.GetPath();
			std::string filenamePalettes = directory + "\\" + m_project->GetName() + "_pal.asm";
			std::string filenameTiles = directory + "\\" + m_project->GetName() + "_til.asm";
			std::string filenameMap = directory + "\\" + m_project->GetName() + "_map.asm";
			std::string filenameCollision = directory + "\\" + m_project->GetName() + "_col.asm";

			m_project->ExportPalettes(filenamePalettes);
			m_project->ExportTiles(filenameTiles);
			m_project->ExportMap(filenameMap);
			m_project->ExportCollision(filenameCollision);

			SetStatusText("Export complete");
			wxMessageBox("Export complete", "Error", wxOK | wxICON_INFORMATION);
		}
	}
}

void MainWindow::OnBtnTilesImport( wxRibbonButtonBarEvent& event )
{
	if(m_project.get())
	{
		wxFileDialog dialogue(this, _("Open BMP file"), "", "", "BMP files (*.bmp)|*.bmp", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
		if(dialogue.ShowModal() == wxID_OK)
		{
			SetStatusText("Importing...");

			m_project->ImportBitmap(dialogue.GetPath().c_str().AsChar(), Project::eBMPImportReplaceAll | Project::eBMPImportDrawToMap);

			//Refresh tileset
			RefreshTileset();

			//Refresh whole application
			RefreshAll();

			SetStatusText("Import complete");
		}
	}
}

void MainWindow::OnBtnTilesCreate(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		//Add new tile
		TileId tileId = m_project->GetTileset().AddTile();

		//Rebuild index map
		CacheTileIndices(m_project->GetTileset());

		//Recreate tileset texture
		CreateTilesetTexture(m_project->GetTileset());

		//Set as current paint tile
		m_project->SetPaintTile(tileId);

		//Set paint tool
		SetMapTool(MapPanel::eToolPaintTile);

		//Refresh tiles and tile editor panels
		RefreshPanel(ePanelTiles);
		RefreshPanel(ePanelTileEditor);
	}
}

void MainWindow::OnBtnTilesDelete(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		TileId tileId = m_project->GetPaintTile();
		if(tileId != InvalidTileId)
		{
			Map& map = m_project->GetMap();
			Tileset& tileset = m_project->GetTileset();

			//Erase tile
			tileset.RemoveTile(tileId);

			//Get blank tile
			TileId blankTile = (tileset.GetCount() > 0) ? tileset.Begin()->first : InvalidTileId;

			//Find all uses of tile, set blank
			for(int x = 0; x < map.GetWidth(); x++)
			{
				for(int y = 0; y < map.GetHeight(); y++)
				{
					if(map.GetTile(x, y) == tileId)
					{
						map.SetTile(x, y, blankTile);
						map.SetTileFlags(x, y, 0);
					}
				}
			}
			

			//Set blank paint tile
			m_project->SetPaintTile(InvalidTileId);

			if(tileId == m_project->GetEraseTile())
			{
				//Set blank erase tile
				m_project->SetEraseTile(InvalidTileId);
			}

			//Rebuild index map
			CacheTileIndices(tileset);

			//Recreate tileset texture
			CreateTilesetTexture(m_project->GetTileset());

			//Revert to select tool
			SetMapTool(MapPanel::eToolSelectTiles);

			//Refresh
			RefreshAll();
		}
	}
}

void MainWindow::OnBtnToolsMapEdit( wxRibbonButtonBarEvent& event )
{
	ShowPanelToolbox();
}

void MainWindow::OnBtnToolsTiles( wxRibbonButtonBarEvent& event )
{
	ShowPanelTiles();
}

void MainWindow::OnBtnToolsPalettes( wxRibbonButtonBarEvent& event )
{
	ShowPanelPalettes();
}

void MainWindow::OnBtnMapClear(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		m_project->GetMap().Clear();
		RefreshPanel(ePanelMap);
	}
}

void MainWindow::OnBtnMapResize(wxRibbonButtonBarEvent& event)
{
	if(m_project.get())
	{
		Map& map = m_project->GetMap();

		int originalWidth = map.GetWidth();
		int originalHeight = map.GetHeight();

		DialogMapSize dialog(this);
		dialog.m_spinCtrlWidth->SetValue(originalWidth);
		dialog.m_spinCtrlHeight->SetValue(originalHeight);

		if(dialog.ShowModal() == wxID_OK)
		{
			int width = dialog.m_spinCtrlWidth->GetValue();
			int height = dialog.m_spinCtrlHeight->GetValue();

			if(width > 0 && width <= 10000 && height > 0 && height <= 10000)
			{
				//Resize map
				map.Resize(width, height);

				//Fill extra tiles with background tile
				TileId backgroundTile = InvalidTileId;
				if(m_project->GetTileset().GetCount() > 0)
					backgroundTile = m_project->GetTileset().Begin()->first;

				//Extra X
				for(int y = 0; y < height; y++)
				{
					for(int x = originalWidth; x < width; x++)
					{
						map.SetTile(x, y, backgroundTile);
					}
				}

				//Extra Y
				for(int x = 0; x < width; x++)
				{
					for(int y = originalHeight; y < height; y++)
					{
						map.SetTile(x, y, backgroundTile);
					}
				}

				//Refresh map panel
				RefreshPanel(ePanelMap);
			}
		}
	}
}

void MainWindow::OnBtnGridShow(wxCommandEvent& event)
{
	if(m_project.get())
	{
		m_project->SetShowGrid(!m_project->GetShowGrid());
		RefreshAll();
	}
}

void MainWindow::OnBtnGridSnap(wxCommandEvent& event)
{
	if(m_project.get())
	{
		m_project->SetGridSnap(!m_project->GetGridSnap());
	}
}

void MainWindow::OnBtnTool(wxCommandEvent& event)
{
	if(m_mapPanel)
	{
		switch(event.GetId())
		{
		case wxID_TOOL_SELECTTILE:
			m_mapPanel->SetTool(MapPanel::eToolSelectTiles);
			break;
		case wxID_TOOL_SELECTSTAMP:
			m_mapPanel->SetTool(MapPanel::eToolSelectStamp);
			break;
		case wxID_TOOL_PAINT:
			m_mapPanel->SetTool(MapPanel::eToolPaintTile);
			break;
		case wxID_TOOL_STAMP:
			m_mapPanel->SetTool(MapPanel::eToolPaintStamp);
			break;
		case wxID_TOOL_TILEPICKER:
			m_mapPanel->SetTool(MapPanel::eToolTilePicker);
			break;
		case wxID_TOOL_STAMPPICKER:
			m_mapPanel->SetTool(MapPanel::eToolStampPicker);
			break;
		case wxID_TOOL_FLIPX:
			m_mapPanel->SetTool(MapPanel::eToolFlipX);
			break;
		case wxID_TOOL_FLIPY:
			m_mapPanel->SetTool(MapPanel::eToolFlipY);
			break;
		case wxID_TOOL_FILL:
			m_mapPanel->SetTool(MapPanel::eToolFill);
			break;
		case wxID_TOOL_CLONE:
			m_mapPanel->SetTool(MapPanel::eToolClone);
			break;
		case wxID_TOOL_CREATESTAMP:
			m_mapPanel->SetTool(MapPanel::eToolCreateStamp);
			break;
		case wxID_TOOL_REMOVESTAMP:
			m_mapPanel->SetTool(MapPanel::eToolRemoveStamp);
			break;
		}
	}
}