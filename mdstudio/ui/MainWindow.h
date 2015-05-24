///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <wx/aui/aui.h>

#include <ion/renderer/Renderer.h>
#include <ion/renderer/Texture.h>

#include "UIBase.h"

#include "PalettesPanel.h"
#include "TilesPanel.h"
#include "StampsPanel.h"
#include "MapPanel.h"
#include "TileEditorPanel.h"

#include "../Project.h"

wxDECLARE_SCOPED_PTR(Project, ProjectPtr)

class MainWindow : public MainWindowBase
{
public:
	enum Panel
	{
		ePanelMap,
		ePanelStamps,
		ePanelTiles,
		ePanelPalettes,
		ePanelTileEditor
	};

	MainWindow();
	virtual ~MainWindow();

	//Open panels
	void ShowPanelPalettes();
	void ShowPanelTiles();
	void ShowPanelStamps();
	void ShowPanelMap();
	void ShowPanelTileEditor();

	//Open toolbox
	void ShowPanelToolbox();

	//Set current map editing tool
	void SetMapTool(MapPanel::Tool tool);

	//Get tileset UV coords for tile
	void GetTileTexCoords(TileId tileId, ion::render::TexCoord texCoords[4], u32 flipFlags) const;

	//Edit tileset texture pixel
	void SetTilesetTexPixel(TileId tileId, const ion::Vector2i& pixel, u8 colourIdx);

	//Sync project settings and their respective UI widget states
	void SyncSettingsWidgets();

	//Refresh/redraw panels
	void RefreshPanel(Panel panel);
	void RedrawPanel(Panel panel);
	void RefreshAll();
	void RedrawAll();

	//Refresh tileset
	void RefreshTileset();

protected:
	virtual void OnBtnProjNew(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjOpen(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjSave(wxRibbonButtonBarEvent& event);
	virtual void OnBtnProjExport(wxRibbonButtonBarEvent& event);
	virtual void OnBtnTilesImport(wxRibbonButtonBarEvent& event);
	virtual void OnBtnTilesCreate(wxRibbonButtonBarEvent& event);
	virtual void OnBtnTilesDelete(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsMapEdit(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsTiles(wxRibbonButtonBarEvent& event);
	virtual void OnBtnToolsStamps(wxRibbonButtonBarEvent& event) { event.Skip(); }
	virtual void OnBtnToolsPalettes(wxRibbonButtonBarEvent& event);
	virtual void OnBtnMapClear(wxRibbonButtonBarEvent& event);
	virtual void OnBtnMapResize(wxRibbonButtonBarEvent& event);
	virtual void OnBtnGridShow(wxCommandEvent& event);
	virtual void OnBtnGridSnap(wxCommandEvent& event);

	void OnBtnTool(wxCommandEvent& event);

private:
	//Set current project (opens default panels)
	void SetProject(Project* project);

	//Create and redraw tileset texture
	void CreateTilesetTexture(const Tileset& tileset);

	//Create TileID to index cache
	void CacheTileIndices(const Tileset& tileset);

	//Get tile index into tileset
	int GetTilesetTexTileIndex(TileId tileId) const;

	wxAuiManager m_auiManager;

	wxWeakRef<PalettesPanel> m_palettesPanel;
	wxWeakRef<TilesPanel> m_tilesPanel;
	wxWeakRef<StampsPanel> m_stampsPanel;
	wxWeakRef<MapPanel> m_mapPanel;
	wxWeakRef<TileEditorPanel> m_tileEditorPanel;
	wxWeakRef<MapToolbox> m_toolboxPanel;

	ProjectPtr m_project;

	//Renderer
	ion::render::Renderer* m_renderer;

	//OpenGL context
	wxGLContext* m_context;

	//Blank canvas for creating gobal DC
	wxWeakRef<wxGLCanvas> m_blankCanvas;

	//Map tile IDs to indices
	std::map<TileId, u32> m_tileIndexMap;

	//Tileset texture
	ion::render::Texture* m_tilesetTexture;

	//Tileset size sq
	u32 m_tilesetSizeSq;

	//Tileset texture cell size sq
	float m_cellSizeTexSpaceSq;
};
