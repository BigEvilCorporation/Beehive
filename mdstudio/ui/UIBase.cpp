///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "KeyframePanel.h"
#include "SpriteCanvas.h"

#include "UIBase.h"

#include "../FormBuilderProj/add_16_16.xpm"
#include "../FormBuilderProj/clearmap.xpm"
#include "../FormBuilderProj/config.xpm"
#include "../FormBuilderProj/deletestamp.xpm"
#include "../FormBuilderProj/deletetile.xpm"
#include "../FormBuilderProj/deleteunusedtiles.xpm"
#include "../FormBuilderProj/down_16_16.xpm"
#include "../FormBuilderProj/fastforward_16_16.xpm"
#include "../FormBuilderProj/gameobj.xpm"
#include "../FormBuilderProj/gameobjpanel.xpm"
#include "../FormBuilderProj/genterrain_beziers.xpm"
#include "../FormBuilderProj/grid.xpm"
#include "../FormBuilderProj/gridsnap.xpm"
#include "../FormBuilderProj/importtiles.xpm"
#include "../FormBuilderProj/key_delete_16_16.xpm"
#include "../FormBuilderProj/key_end_16_16.xpm"
#include "../FormBuilderProj/key_insert_16_16.xpm"
#include "../FormBuilderProj/keyall_16_16.xpm"
#include "../FormBuilderProj/loop_16_16.xpm"
#include "../FormBuilderProj/megadrive2.xpm"
#include "../FormBuilderProj/newproj.xpm"
#include "../FormBuilderProj/newstamp.xpm"
#include "../FormBuilderProj/newtile.xpm"
#include "../FormBuilderProj/obj_16_16.xpm"
#include "../FormBuilderProj/open.xpm"
#include "../FormBuilderProj/palettespanel.xpm"
#include "../FormBuilderProj/play_16_16.xpm"
#include "../FormBuilderProj/qmark_16_16.xpm"
#include "../FormBuilderProj/remove_16_16.xpm"
#include "../FormBuilderProj/resizemap.xpm"
#include "../FormBuilderProj/rewind_16_16.xpm"
#include "../FormBuilderProj/save.xpm"
#include "../FormBuilderProj/showoutlines.xpm"
#include "../FormBuilderProj/stampspanel.xpm"
#include "../FormBuilderProj/stepleft_16_16.xpm"
#include "../FormBuilderProj/stepright_16_16.xpm"
#include "../FormBuilderProj/stop_16_16.xpm"
#include "../FormBuilderProj/tilespanel.xpm"
#include "../FormBuilderProj/tool_addterrainbezier.xpm"
#include "../FormBuilderProj/tool_clone.xpm"
#include "../FormBuilderProj/tool_createstamp.xpm"
#include "../FormBuilderProj/tool_deletegameobj.xpm"
#include "../FormBuilderProj/tool_deletestamp.xpm"
#include "../FormBuilderProj/tool_deleteterrainbezier.xpm"
#include "../FormBuilderProj/tool_deleteterraintile.xpm"
#include "../FormBuilderProj/tool_duplicategameobj.xpm"
#include "../FormBuilderProj/tool_editterrainbezier.xpm"
#include "../FormBuilderProj/tool_fill.xpm"
#include "../FormBuilderProj/tool_flipx.xpm"
#include "../FormBuilderProj/tool_flipy.xpm"
#include "../FormBuilderProj/tool_genterrain_beziers.xpm"
#include "../FormBuilderProj/tool_movestamp.xpm"
#include "../FormBuilderProj/tool_paintcolhole.xpm"
#include "../FormBuilderProj/tool_paintcolpixel.xpm"
#include "../FormBuilderProj/tool_paintcolwall.xpm"
#include "../FormBuilderProj/tool_paintstamp.xpm"
#include "../FormBuilderProj/tool_painttile.xpm"
#include "../FormBuilderProj/tool_picker.xpm"
#include "../FormBuilderProj/tool_placegameobj.xpm"
#include "../FormBuilderProj/tool_select.xpm"
#include "../FormBuilderProj/tool_selectgameobj.xpm"
#include "../FormBuilderProj/tool_selectstamp.xpm"
#include "../FormBuilderProj/tool_stamppicker.xpm"
#include "../FormBuilderProj/toolspanel.xpm"
#include "../FormBuilderProj/up_16_16.xpm"

///////////////////////////////////////////////////////////////////////////

MainWindowBase::MainWindowBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_menubar1 = new wxMenuBar( 0 );
	m_menuTools = new wxMenu();
	m_menu1 = new wxMenu();
	wxMenuItem* m_menu1Item = new wxMenuItem( m_menuTools, wxID_ANY, wxT("Tweaks"), wxEmptyString, wxITEM_NORMAL, m_menu1 );
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( m_menu1, wxID_MENU_TOOLS_TWEAKS_GAMEOBJ_CENTRE_ORIGIN, wxString( wxT("Game Object Centre Origins") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem3 );

	m_menuTools->Append( m_menu1Item );

	m_menubar1->Append( m_menuTools, wxT("&Tools") );

	this->SetMenuBar( m_menubar1 );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_ribbonBar = new wxRibbonBar( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxRIBBON_BAR_DEFAULT_STYLE|wxRIBBON_BAR_FLOW_HORIZONTAL|wxRIBBON_BAR_SHOW_PAGE_ICONS|wxRIBBON_BAR_SHOW_PAGE_LABELS|wxRIBBON_BAR_SHOW_PANEL_MINIMISE_BUTTONS|wxBORDER_NONE );
	m_ribbonBar->SetArtProvider(new wxRibbonDefaultArtProvider);
	m_ribbonPageProject = new wxRibbonPage( m_ribbonBar, wxID_ANY, wxT("Project") , wxNullBitmap , 0 );
	m_ribbonBar->SetActivePage( m_ribbonPageProject );
	m_ribbonPanelProject = new wxRibbonPanel( m_ribbonPageProject, wxID_ANY, wxT("Project") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarProject = new wxRibbonButtonBar( m_ribbonPanelProject, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarProject->AddButton( wxID_BTN_PROJ_NEW, wxT("New"), wxBitmap( newproj_xpm ), wxEmptyString);
	m_ribbonButtonBarProject->AddButton( wxID_BTN_PROJ_OPEN, wxT("Open"), wxBitmap( open_xpm ), wxEmptyString);
	m_ribbonButtonBarProject->AddButton( wxID_BTN_PROJ_SAVE, wxT("Save"), wxBitmap( save_xpm ), wxEmptyString);
	m_ribbonButtonBarProject->AddButton( wxID_BTN_PROJ_SETTINGS, wxT("Settings"), wxBitmap( config_xpm ), wxEmptyString);
	m_ribbonButtonBarProject->AddButton( wxID_BTN_PROJ_EXPORT, wxT("Export"), wxBitmap( megadrive2_xpm ), wxEmptyString);
	m_ribbonPageView = new wxRibbonPage( m_ribbonBar, wxID_ANY, wxT("View") , wxNullBitmap , 0 );
	m_ribbonPanelView = new wxRibbonPanel( m_ribbonPageView, wxID_ANY, wxT("Grid") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarGrid = new wxRibbonButtonBar( m_ribbonPanelView, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarGrid->AddButton( wxID_BTN_SAVE_LAYOUT, wxT("Save Layout"), wxBitmap( save_xpm ), wxEmptyString);
	m_ribbonButtonBarGrid->AddToggleButton( wxID_BTN_GRID_SHOW, wxT("Show Grid"), wxBitmap( grid_xpm ), wxEmptyString);
	m_ribbonButtonBarGrid->AddToggleButton( wxID_BTN_GRID_SNAP, wxT("Snap to Grid"), wxBitmap( gridsnap_xpm ), wxEmptyString);
	m_ribbonButtonBarGrid->AddToggleButton( wxID_BTN_SHOW_OUTLINES, wxT("Show Outlines"), wxBitmap( showoutlines_xpm ), wxEmptyString);
	m_ribbonButtonBarGrid->AddToggleButton( wxID_BTN_SHOW_COLLISION, wxT("Show Collision"), wxBitmap( showoutlines_xpm ), wxEmptyString);
	m_ribbonButtonBarGrid->AddToggleButton( wxID_BTN_SHOW_DISPLAYFRAME, wxT("Show Display Frame"), wxBitmap( showoutlines_xpm ), wxEmptyString);
	m_ribbonPageTools = new wxRibbonPage( m_ribbonBar, wxID_ANY, wxT("Tools") , wxNullBitmap , 0 );
	m_ribbonPanelTools = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Tool Panels") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarTools = new wxRibbonButtonBar( m_ribbonPanelTools, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_MAPEDIT, wxT("Toolbox"), wxBitmap( toolspanel_xpm ), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_MAP_LIST, wxT("Maps"), wxBitmap( toolspanel_xpm ), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_TILES, wxT("Tiles"), wxBitmap( tilespanel_xpm ), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_COLLISION_TILES, wxT("Terrain Tiles"), wxBitmap( tilespanel_xpm ), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_STAMPS, wxT("Stamps"), wxBitmap( stampspanel_xpm ), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_PALETTES, wxT("Palettes"), wxBitmap( palettespanel_xpm ), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_GAMEOBJS, wxT("Objects"), wxBitmap( gameobjpanel_xpm ), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_GAMEOBJPARAMS, wxT("Object Params"), wxBitmap( gameobjpanel_xpm ), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_TIMELINE, wxT("Animation Timeline"), wxBitmap( gameobjpanel_xpm ), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_SPRITEANIM, wxT("Sprite Animation"), wxBitmap( gameobjpanel_xpm ), wxEmptyString);
	m_ribbonPanelMap = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Map") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarMap = new wxRibbonButtonBar( m_ribbonPanelMap, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarMap->AddButton( wxID_BTN_MAP_CLEAR, wxT("Clear Map"), wxBitmap( clearmap_xpm ), wxEmptyString);
	m_ribbonButtonBarMap->AddButton( wxID_BTN_MAP_RESIZE, wxT("Resize Map"), wxBitmap( resizemap_xpm ), wxEmptyString);
	m_ribbonButtonBarMap->AddButton( wxID_BTN_MAP_EXPORT_BMP, wxT("Export BMP"), wxBitmap( save_xpm ), wxEmptyString);
	m_ribbonPanelTiles = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Tiles") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarTiles = new wxRibbonButtonBar( m_ribbonPanelTiles, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarTiles->AddButton( wxID_BTN_TILES_IMPORT, wxT("Import"), wxBitmap( importtiles_xpm ), wxEmptyString);
	m_ribbonButtonBarTiles->AddButton( wxID_BTN_TILES_CREATE, wxT("New"), wxBitmap( newtile_xpm ), wxEmptyString);
	m_ribbonButtonBarTiles->AddButton( wxID_BTN_TILES_DELETE, wxT("Delete"), wxBitmap( deletetile_xpm ), wxEmptyString);
	m_ribbonButtonBarTiles->AddButton( wxID_BTN_TILES_CLEANUP, wxT("Cleanup"), wxBitmap( deleteunusedtiles_xpm ), wxEmptyString);
	m_ribbonPanelSprites = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Sprites") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarSprites = new wxRibbonButtonBar( m_ribbonPanelSprites, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarSprites->AddButton( wxID_BTN_SPRITE_EDITOR, wxT("Sprite Editor"), wxBitmap( importtiles_xpm ), wxEmptyString);
	m_ribbonPanelStamps = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Stamps") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarStamps = new wxRibbonButtonBar( m_ribbonPanelStamps, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarStamps->AddButton( wxID_BTN_STAMPS_IMPORT, wxT("Import"), wxBitmap( importtiles_xpm ), wxEmptyString);
	m_ribbonButtonBarStamps->AddButton( wxID_BTN_STAMPS_CREATE, wxT("Create"), wxBitmap( newstamp_xpm ), wxEmptyString);
	m_ribbonButtonBarStamps->AddButton( wxID_BTN_STAMPS_DELETE, wxT("Delete"), wxBitmap( deletestamp_xpm ), wxEmptyString);
	m_ribbonButtonBarStamps->AddButton( wxID_BTN_STAMPS_EXPORT_BMP, wxT("Export As BMPs"), wxBitmap( save_xpm ), wxEmptyString);
	m_ribbonButtonBarStamps->AddButton( wxID_BTN_STAMPS_CLEANUP, wxT("Cleanup"), wxBitmap( deleteunusedtiles_xpm ), wxEmptyString);
	m_ribbonPanelCollision = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Collision") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarCollision = new wxRibbonButtonBar( m_ribbonPanelCollision, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarCollision->AddButton( wxID_BTN_COL_TILES_CREATE, wxT("New"), wxBitmap( newtile_xpm ), wxEmptyString);
	m_ribbonButtonBarCollision->AddButton( wxID_BTN_COL_TILES_DELETE, wxT("Delete"), wxBitmap( deletetile_xpm ), wxEmptyString);
	m_ribbonButtonBarCollision->AddButton( wxID_BTN_COL_TILES_CLEANUP, wxT("Cleanup"), wxBitmap( deleteunusedtiles_xpm ), wxEmptyString);
	m_ribbonButtonBarCollision->AddButton( wxID_BTN_COL_MAP_CLEAR, wxT("Clear Collision Map"), wxBitmap( clearmap_xpm ), wxEmptyString);
	m_ribbonButtonBarCollision->AddButton( wxID_BTN_COL_GEN_BEZIER, wxT("Generate from Beziers"), wxBitmap( genterrain_beziers_xpm ), wxEmptyString);
	m_ribbonPanelGameObj = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Objects") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarGameObj = new wxRibbonButtonBar( m_ribbonPanelGameObj, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarGameObj->AddButton( wxID_BTN_GAME_OBJ_TYPES, wxT("Types"), wxBitmap( gameobj_xpm ), wxEmptyString);
	m_ribbonBar->Realize();

	bSizer1->Add( m_ribbonBar, 0, wxALL|wxEXPAND, 0 );

	m_dockArea = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer1->Add( m_dockArea, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindowBase::OnMenuToolsTweaksGameObjCentreOrigin ), this, m_menuItem3->GetId());
	this->Connect( wxID_BTN_PROJ_NEW, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjNew ) );
	this->Connect( wxID_BTN_PROJ_OPEN, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjOpen ) );
	this->Connect( wxID_BTN_PROJ_SAVE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjSave ) );
	this->Connect( wxID_BTN_PROJ_SETTINGS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjSettings ) );
	this->Connect( wxID_BTN_PROJ_EXPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjExport ) );
	this->Connect( wxID_BTN_SAVE_LAYOUT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnSaveLayout ) );
	this->Connect( wxID_BTN_GRID_SHOW, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnGridShow ) );
	this->Connect( wxID_BTN_GRID_SNAP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnGridSnap ) );
	this->Connect( wxID_BTN_SHOW_OUTLINES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnShowOutlines ) );
	this->Connect( wxID_BTN_SHOW_COLLISION, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnShowCollision ) );
	this->Connect( wxID_BTN_SHOW_DISPLAYFRAME, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnShowDisplayFrame ) );
	this->Connect( wxID_BTN_TOOLS_MAPEDIT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsMapEdit ) );
	this->Connect( wxID_BTN_TOOLS_MAP_LIST, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsMapList ) );
	this->Connect( wxID_BTN_TOOLS_TILES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsTiles ) );
	this->Connect( wxID_BTN_TOOLS_COLLISION_TILES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsCollisionTiles ) );
	this->Connect( wxID_BTN_TOOLS_STAMPS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsStamps ) );
	this->Connect( wxID_BTN_TOOLS_PALETTES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsPalettes ) );
	this->Connect( wxID_BTN_TOOLS_GAMEOBJS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsGameObjs ) );
	this->Connect( wxID_BTN_TOOLS_GAMEOBJPARAMS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsGameObjParams ) );
	this->Connect( wxID_BTN_TOOLS_TIMELINE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsTimeline ) );
	this->Connect( wxID_BTN_TOOLS_SPRITEANIM, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsSpriteAnim ) );
	this->Connect( wxID_BTN_MAP_CLEAR, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnMapClear ) );
	this->Connect( wxID_BTN_MAP_RESIZE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnMapResize ) );
	this->Connect( wxID_BTN_MAP_EXPORT_BMP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnMapExportBMP ) );
	this->Connect( wxID_BTN_TILES_IMPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesImport ) );
	this->Connect( wxID_BTN_TILES_CREATE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesCreate ) );
	this->Connect( wxID_BTN_TILES_DELETE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesDelete ) );
	this->Connect( wxID_BTN_TILES_CLEANUP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesCleanup ) );
	this->Connect( wxID_BTN_SPRITE_EDITOR, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnSpriteEditor ) );
	this->Connect( wxID_BTN_STAMPS_IMPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnStampsImport ) );
	this->Connect( wxID_BTN_STAMPS_EXPORT_BMP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnStampsExportBMPs ) );
	this->Connect( wxID_BTN_STAMPS_CLEANUP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnStampsCleanup ) );
	this->Connect( wxID_BTN_COL_TILES_CREATE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnColTilesCreate ) );
	this->Connect( wxID_BTN_COL_TILES_DELETE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnColTilesDelete ) );
	this->Connect( wxID_BTN_COL_TILES_CLEANUP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnColTilesCleanup ) );
	this->Connect( wxID_BTN_COL_MAP_CLEAR, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnColMapClear ) );
	this->Connect( wxID_BTN_COL_GEN_BEZIER, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnColGenTerrainBezier ) );
	this->Connect( wxID_BTN_GAME_OBJ_TYPES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnGameObjTypes ) );
}

MainWindowBase::~MainWindowBase()
{
	// Disconnect Events
	this->Disconnect( wxID_BTN_PROJ_NEW, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjNew ) );
	this->Disconnect( wxID_BTN_PROJ_OPEN, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjOpen ) );
	this->Disconnect( wxID_BTN_PROJ_SAVE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjSave ) );
	this->Disconnect( wxID_BTN_PROJ_SETTINGS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjSettings ) );
	this->Disconnect( wxID_BTN_PROJ_EXPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjExport ) );
	this->Disconnect( wxID_BTN_SAVE_LAYOUT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnSaveLayout ) );
	this->Disconnect( wxID_BTN_GRID_SHOW, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnGridShow ) );
	this->Disconnect( wxID_BTN_GRID_SNAP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnGridSnap ) );
	this->Disconnect( wxID_BTN_SHOW_OUTLINES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnShowOutlines ) );
	this->Disconnect( wxID_BTN_SHOW_COLLISION, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnShowCollision ) );
	this->Disconnect( wxID_BTN_SHOW_DISPLAYFRAME, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnShowDisplayFrame ) );
	this->Disconnect( wxID_BTN_TOOLS_MAPEDIT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsMapEdit ) );
	this->Disconnect( wxID_BTN_TOOLS_MAP_LIST, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsMapList ) );
	this->Disconnect( wxID_BTN_TOOLS_TILES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsTiles ) );
	this->Disconnect( wxID_BTN_TOOLS_COLLISION_TILES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsCollisionTiles ) );
	this->Disconnect( wxID_BTN_TOOLS_STAMPS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsStamps ) );
	this->Disconnect( wxID_BTN_TOOLS_PALETTES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsPalettes ) );
	this->Disconnect( wxID_BTN_TOOLS_GAMEOBJS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsGameObjs ) );
	this->Disconnect( wxID_BTN_TOOLS_GAMEOBJPARAMS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsGameObjParams ) );
	this->Disconnect( wxID_BTN_TOOLS_TIMELINE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsTimeline ) );
	this->Disconnect( wxID_BTN_TOOLS_SPRITEANIM, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsSpriteAnim ) );
	this->Disconnect( wxID_BTN_MAP_CLEAR, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnMapClear ) );
	this->Disconnect( wxID_BTN_MAP_RESIZE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnMapResize ) );
	this->Disconnect( wxID_BTN_MAP_EXPORT_BMP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnMapExportBMP ) );
	this->Disconnect( wxID_BTN_TILES_IMPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesImport ) );
	this->Disconnect( wxID_BTN_TILES_CREATE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesCreate ) );
	this->Disconnect( wxID_BTN_TILES_DELETE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesDelete ) );
	this->Disconnect( wxID_BTN_TILES_CLEANUP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesCleanup ) );
	this->Disconnect( wxID_BTN_SPRITE_EDITOR, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnSpriteEditor ) );
	this->Disconnect( wxID_BTN_STAMPS_IMPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnStampsImport ) );
	this->Disconnect( wxID_BTN_STAMPS_EXPORT_BMP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnStampsExportBMPs ) );
	this->Disconnect( wxID_BTN_STAMPS_CLEANUP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnStampsCleanup ) );
	this->Disconnect( wxID_BTN_COL_TILES_CREATE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnColTilesCreate ) );
	this->Disconnect( wxID_BTN_COL_TILES_DELETE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnColTilesDelete ) );
	this->Disconnect( wxID_BTN_COL_TILES_CLEANUP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnColTilesCleanup ) );
	this->Disconnect( wxID_BTN_COL_MAP_CLEAR, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnColMapClear ) );
	this->Disconnect( wxID_BTN_COL_GEN_BEZIER, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnColGenTerrainBezier ) );
	this->Disconnect( wxID_BTN_GAME_OBJ_TYPES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnGameObjTypes ) );

}

DialogMapSizeBase::DialogMapSizeBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	m_text1 = new wxStaticText( this, wxID_ANY, wxT("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_text1->Wrap( -1 );
	bSizer3->Add( m_text1, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_spinCtrlWidth = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 64 );
	bSizer3->Add( m_spinCtrlWidth, 0, wxALL, 5 );

	m_text2 = new wxStaticText( this, wxID_ANY, wxT("x"), wxDefaultPosition, wxDefaultSize, 0 );
	m_text2->Wrap( -1 );
	bSizer3->Add( m_text2, 0, wxALIGN_CENTER, 5 );

	m_text3 = new wxStaticText( this, wxID_ANY, wxT("Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_text3->Wrap( -1 );
	bSizer3->Add( m_text3, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_spinCtrlHeight = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 64 );
	bSizer3->Add( m_spinCtrlHeight, 0, wxALL, 5 );


	bSizer2->Add( bSizer3, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer( wxHORIZONTAL );

	wxString m_radioBoxShiftYChoices[] = { wxT("Top"), wxT("Bottom") };
	int m_radioBoxShiftYNChoices = sizeof( m_radioBoxShiftYChoices ) / sizeof( wxString );
	m_radioBoxShiftY = new wxRadioBox( this, wxID_ANY, wxT("Shift contents V"), wxDefaultPosition, wxDefaultSize, m_radioBoxShiftYNChoices, m_radioBoxShiftYChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxShiftY->SetSelection( 0 );
	bSizer35->Add( m_radioBoxShiftY, 0, wxALL, 5 );

	wxString m_radioBoxShiftXChoices[] = { wxT("Left"), wxT("Right") };
	int m_radioBoxShiftXNChoices = sizeof( m_radioBoxShiftXChoices ) / sizeof( wxString );
	m_radioBoxShiftX = new wxRadioBox( this, wxID_ANY, wxT("Shift contents H"), wxDefaultPosition, wxDefaultSize, m_radioBoxShiftXNChoices, m_radioBoxShiftXChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxShiftX->SetSelection( 0 );
	bSizer35->Add( m_radioBoxShiftX, 0, wxALL, 5 );


	bSizer2->Add( bSizer35, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_buttonOk = new wxButton( this, wxID_BTN_CANCEL, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_buttonOk, 0, wxALIGN_RIGHT, 5 );

	m_buttonCancel = new wxButton( this, wxID_BTN_CANCEL, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_buttonCancel, 0, wxALIGN_RIGHT, 5 );


	bSizer2->Add( bSizer4, 1, wxALIGN_RIGHT, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_buttonOk->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogMapSizeBase::OnBtnOk ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogMapSizeBase::OnBtnCancel ), NULL, this );
}

DialogMapSizeBase::~DialogMapSizeBase()
{
	// Disconnect Events
	m_buttonOk->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogMapSizeBase::OnBtnOk ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogMapSizeBase::OnBtnCancel ), NULL, this );

}

DialogUpdateStampBase::DialogUpdateStampBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer41;
	fgSizer41 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer41->AddGrowableCol( 0 );
	fgSizer41->AddGrowableRow( 2 );
	fgSizer41->SetFlexibleDirection( wxBOTH );
	fgSizer41->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_filePicker10 = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("BMP files (*.bmp)|*.bmp"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN );
	fgSizer41->Add( m_filePicker10, 0, wxALL|wxEXPAND, 5 );

	m_chkReplacePalette = new wxCheckBox( this, wxID_ANY, wxT("Replace palette"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer41->Add( m_chkReplacePalette, 0, wxALL, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	m_canvasOld = new StampCanvas( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer3->Add( m_canvasOld, 1, wxEXPAND | wxALL, 5 );

	m_canvasNew = new StampCanvas( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer3->Add( m_canvasNew, 1, wxEXPAND | wxALL, 5 );


	fgSizer41->Add( bSizer3, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer( wxHORIZONTAL );

	wxString m_radioBoxShiftYChoices[] = { wxT("Top"), wxT("Bottom") };
	int m_radioBoxShiftYNChoices = sizeof( m_radioBoxShiftYChoices ) / sizeof( wxString );
	m_radioBoxShiftY = new wxRadioBox( this, wxID_ANY, wxT("Shift contents Y"), wxDefaultPosition, wxDefaultSize, m_radioBoxShiftYNChoices, m_radioBoxShiftYChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxShiftY->SetSelection( 0 );
	bSizer35->Add( m_radioBoxShiftY, 0, wxALL, 5 );

	wxString m_radioBoxShiftXChoices[] = { wxT("Left"), wxT("Right") };
	int m_radioBoxShiftXNChoices = sizeof( m_radioBoxShiftXChoices ) / sizeof( wxString );
	m_radioBoxShiftX = new wxRadioBox( this, wxID_ANY, wxT("Shift contents X"), wxDefaultPosition, wxDefaultSize, m_radioBoxShiftXNChoices, m_radioBoxShiftXChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxShiftX->SetSelection( 0 );
	bSizer35->Add( m_radioBoxShiftX, 0, wxALL, 5 );

	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxVERTICAL );

	m_chkFlipX = new wxCheckBox( this, wxID_ANY, wxT("Flip X"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer61->Add( m_chkFlipX, 0, wxALL, 5 );

	m_chkFlipY = new wxCheckBox( this, wxID_ANY, wxT("Flip Y"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer61->Add( m_chkFlipY, 0, wxALL, 5 );


	bSizer35->Add( bSizer61, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_buttonOk = new wxButton( this, wxID_BTN_CANCEL, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_buttonOk, 0, wxALIGN_RIGHT, 5 );

	m_buttonCancel = new wxButton( this, wxID_BTN_CANCEL, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_buttonCancel, 0, wxALIGN_RIGHT, 5 );


	bSizer35->Add( bSizer4, 1, wxALIGN_BOTTOM|wxALIGN_RIGHT, 5 );


	fgSizer41->Add( bSizer35, 1, wxEXPAND, 5 );


	bSizer2->Add( fgSizer41, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_filePicker10->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( DialogUpdateStampBase::OnFileBrowse ), NULL, this );
	m_buttonOk->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogUpdateStampBase::OnBtnOk ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogUpdateStampBase::OnBtnCancel ), NULL, this );
}

DialogUpdateStampBase::~DialogUpdateStampBase()
{
	// Disconnect Events
	m_filePicker10->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( DialogUpdateStampBase::OnFileBrowse ), NULL, this );
	m_buttonOk->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogUpdateStampBase::OnBtnOk ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogUpdateStampBase::OnBtnCancel ), NULL, this );

}

DialogEditStampCollisionBase::DialogEditStampCollisionBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer41;
	fgSizer41 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer41->AddGrowableCol( 1 );
	fgSizer41->AddGrowableRow( 0 );
	fgSizer41->SetFlexibleDirection( wxBOTH );
	fgSizer41->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_toolbox = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer63;
	fgSizer63 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer63->AddGrowableRow( 1 );
	fgSizer63->SetFlexibleDirection( wxBOTH );
	fgSizer63->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer67;
	fgSizer67 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer67->SetFlexibleDirection( wxBOTH );
	fgSizer67->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_toolAddTerrainBezier = new wxBitmapButton( m_toolbox, wxID_TOOL_COL_ADDTERRAINBEZIER, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolAddTerrainBezier->SetBitmap( wxBitmap( tool_addterrainbezier_xpm ) );
	m_toolAddTerrainBezier->SetToolTip( wxT("Add Terrain Bezier") );

	fgSizer67->Add( m_toolAddTerrainBezier, 0, wxALL, 5 );

	m_toolEditTerrainBezier = new wxBitmapButton( m_toolbox, wxID_TOOL_COL_EDITTERRAINBEZIER, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolEditTerrainBezier->SetBitmap( wxBitmap( tool_editterrainbezier_xpm ) );
	m_toolEditTerrainBezier->SetToolTip( wxT("Edit Terrain Bezier") );

	fgSizer67->Add( m_toolEditTerrainBezier, 0, wxALL, 5 );

	m_toolDeleteTerrainBezier = new wxBitmapButton( m_toolbox, wxID_TOOL_COL_DELETETERRAINBEZIER, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolDeleteTerrainBezier->SetBitmap( wxBitmap( tool_deleteterrainbezier_xpm ) );
	m_toolDeleteTerrainBezier->SetToolTip( wxT("Delete Terrain Bezier") );

	fgSizer67->Add( m_toolDeleteTerrainBezier, 0, wxALL, 5 );

	m_toolPaintCollisionSolid = new wxBitmapButton( m_toolbox, wxID_TOOL_COL_PAINTSOLID, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolPaintCollisionSolid->SetBitmap( wxBitmap( tool_paintcolwall_xpm ) );
	m_toolPaintCollisionSolid->SetToolTip( wxT("Paint Solid Wall/Ceiling") );

	fgSizer67->Add( m_toolPaintCollisionSolid, 0, wxALL, 5 );


	fgSizer63->Add( fgSizer67, 1, wxEXPAND, 5 );


	fgSizer63->Add( 0, 0, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer68;
	fgSizer68 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer68->SetFlexibleDirection( wxBOTH );
	fgSizer68->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_toolGenerateTerrain = new wxBitmapButton( m_toolbox, wxID_TOOL_GENERATETERRAIN, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolGenerateTerrain->SetBitmap( wxBitmap( tool_genterrain_beziers_xpm ) );
	m_toolGenerateTerrain->SetToolTip( wxT("Generate Terrain") );

	fgSizer68->Add( m_toolGenerateTerrain, 0, wxALIGN_BOTTOM|wxALL, 5 );


	fgSizer63->Add( fgSizer68, 1, wxALIGN_BOTTOM|wxEXPAND, 5 );


	m_toolbox->SetSizer( fgSizer63 );
	m_toolbox->Layout();
	fgSizer63->Fit( m_toolbox );
	fgSizer41->Add( m_toolbox, 1, wxEXPAND | wxALL, 5 );

	m_canvas = new StampCanvas( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	fgSizer41->Add( m_canvas, 1, wxEXPAND | wxALL, 5 );


	bSizer2->Add( fgSizer41, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_toolAddTerrainBezier->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogEditStampCollisionBase::OnToolAddBezier ), NULL, this );
	m_toolEditTerrainBezier->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogEditStampCollisionBase::OnToolEditBezier ), NULL, this );
	m_toolDeleteTerrainBezier->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogEditStampCollisionBase::OnToolDeleteBezier ), NULL, this );
	m_toolPaintCollisionSolid->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogEditStampCollisionBase::OnToolPaintSolid ), NULL, this );
	m_toolGenerateTerrain->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogEditStampCollisionBase::OnToolGenerateTerrain ), NULL, this );
}

DialogEditStampCollisionBase::~DialogEditStampCollisionBase()
{
	// Disconnect Events
	m_toolAddTerrainBezier->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogEditStampCollisionBase::OnToolAddBezier ), NULL, this );
	m_toolEditTerrainBezier->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogEditStampCollisionBase::OnToolEditBezier ), NULL, this );
	m_toolDeleteTerrainBezier->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogEditStampCollisionBase::OnToolDeleteBezier ), NULL, this );
	m_toolPaintCollisionSolid->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogEditStampCollisionBase::OnToolPaintSolid ), NULL, this );
	m_toolGenerateTerrain->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogEditStampCollisionBase::OnToolGenerateTerrain ), NULL, this );

}

DialogNewMap::DialogNewMap( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer45;
	bSizer45 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText50 = new wxStaticText( this, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText50->Wrap( -1 );
	bSizer45->Add( m_staticText50, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textMapName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer45->Add( m_textMapName, 1, wxALL|wxEXPAND, 5 );


	bSizer2->Add( bSizer45, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	m_text1 = new wxStaticText( this, wxID_ANY, wxT("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_text1->Wrap( -1 );
	bSizer3->Add( m_text1, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_spinCtrlWidth = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 64 );
	bSizer3->Add( m_spinCtrlWidth, 0, wxALL, 5 );

	m_text2 = new wxStaticText( this, wxID_ANY, wxT("x"), wxDefaultPosition, wxDefaultSize, 0 );
	m_text2->Wrap( -1 );
	bSizer3->Add( m_text2, 0, wxALIGN_CENTER, 5 );

	m_text3 = new wxStaticText( this, wxID_ANY, wxT("Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_text3->Wrap( -1 );
	bSizer3->Add( m_text3, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_spinCtrlHeight = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 64 );
	bSizer3->Add( m_spinCtrlHeight, 0, wxALL, 5 );


	bSizer2->Add( bSizer3, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	m_sdbSizer7 = new wxStdDialogButtonSizer();
	m_sdbSizer7OK = new wxButton( this, wxID_OK );
	m_sdbSizer7->AddButton( m_sdbSizer7OK );
	m_sdbSizer7Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer7->AddButton( m_sdbSizer7Cancel );
	m_sdbSizer7->Realize();

	bSizer2->Add( m_sdbSizer7, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();

	this->Centre( wxBOTH );
}

DialogNewMap::~DialogNewMap()
{
}

DialogNewProjectBase::DialogNewProjectBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer30;
	fgSizer30 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer30->SetFlexibleDirection( wxBOTH );
	fgSizer30->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer27;
	fgSizer27 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer27->AddGrowableCol( 1 );
	fgSizer27->SetFlexibleDirection( wxBOTH );
	fgSizer27->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText49 = new wxStaticText( this, wxID_ANY, wxT("Preset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText49->Wrap( -1 );
	fgSizer27->Add( m_staticText49, 0, wxALL, 5 );

	wxArrayString m_choicePresetChoices;
	m_choicePreset = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePresetChoices, 0 );
	m_choicePreset->SetSelection( 0 );
	fgSizer27->Add( m_choicePreset, 0, wxALL|wxEXPAND, 5 );


	fgSizer30->Add( fgSizer27, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer28;
	fgSizer28 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer28->SetFlexibleDirection( wxBOTH );
	fgSizer28->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_text1 = new wxStaticText( this, wxID_ANY, wxT("Tile width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_text1->Wrap( -1 );
	fgSizer28->Add( m_text1, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_spinCtrlTileWidth = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 64 );
	fgSizer28->Add( m_spinCtrlTileWidth, 0, wxALL, 5 );

	m_text3 = new wxStaticText( this, wxID_ANY, wxT("Tile height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_text3->Wrap( -1 );
	fgSizer28->Add( m_text3, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_spinCtrlTileHeight = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 64 );
	fgSizer28->Add( m_spinCtrlTileHeight, 0, wxALL, 5 );

	m_text11 = new wxStaticText( this, wxID_ANY, wxT("Map width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_text11->Wrap( -1 );
	fgSizer28->Add( m_text11, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_spinCtrlMapWidth = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 64 );
	fgSizer28->Add( m_spinCtrlMapWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_text31 = new wxStaticText( this, wxID_ANY, wxT("Map height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_text31->Wrap( -1 );
	fgSizer28->Add( m_text31, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_spinCtrlMapHeight = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 64 );
	fgSizer28->Add( m_spinCtrlMapHeight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticText53 = new wxStaticText( this, wxID_ANY, wxT("Stamp width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53->Wrap( -1 );
	fgSizer28->Add( m_staticText53, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_spinCtrlStampWidth = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 16 );
	fgSizer28->Add( m_spinCtrlStampWidth, 0, wxALL, 5 );

	m_staticText54 = new wxStaticText( this, wxID_ANY, wxT("Stamp height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText54->Wrap( -1 );
	fgSizer28->Add( m_staticText54, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_spinCtrlStampHeight = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 16 );
	fgSizer28->Add( m_spinCtrlStampHeight, 0, wxALL, 5 );


	fgSizer30->Add( fgSizer28, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_buttonOk = new wxButton( this, wxID_BTN_CANCEL, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_buttonOk, 0, wxALIGN_RIGHT, 5 );

	m_buttonCancel = new wxButton( this, wxID_BTN_CANCEL, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_buttonCancel, 0, wxALIGN_RIGHT, 5 );


	fgSizer30->Add( bSizer4, 1, wxALIGN_RIGHT, 5 );


	bSizer2->Add( fgSizer30, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_choicePreset->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DialogNewProjectBase::OnChoicePreset ), NULL, this );
	m_buttonOk->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogNewProjectBase::OnBtnOk ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogNewProjectBase::OnBtnCancel ), NULL, this );
}

DialogNewProjectBase::~DialogNewProjectBase()
{
	// Disconnect Events
	m_choicePreset->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DialogNewProjectBase::OnChoicePreset ), NULL, this );
	m_buttonOk->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogNewProjectBase::OnBtnOk ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogNewProjectBase::OnBtnCancel ), NULL, this );

}

DialogNewSpriteAnim::DialogNewSpriteAnim( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer22;
	fgSizer22 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer22->AddGrowableCol( 1 );
	fgSizer22->SetFlexibleDirection( wxBOTH );
	fgSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_text1 = new wxStaticText( this, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_text1->Wrap( -1 );
	fgSizer22->Add( m_text1, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textName = new wxTextCtrl( this, wxID_ANY, wxT("Default"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer22->Add( m_textName, 0, wxALL|wxEXPAND, 5 );


	bSizer2->Add( fgSizer22, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxHORIZONTAL );

	m_chkAutoGenerate = new wxCheckBox( this, wxID_ANY, wxT("Auto generate from all frames"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer34->Add( m_chkAutoGenerate, 0, wxALL, 5 );


	bSizer2->Add( bSizer34, 1, wxEXPAND, 5 );

	m_sdbSizer4 = new wxStdDialogButtonSizer();
	m_sdbSizer4OK = new wxButton( this, wxID_OK );
	m_sdbSizer4->AddButton( m_sdbSizer4OK );
	m_sdbSizer4Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer4->AddButton( m_sdbSizer4Cancel );
	m_sdbSizer4->Realize();

	bSizer2->Add( m_sdbSizer4, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();

	this->Centre( wxBOTH );
}

DialogNewSpriteAnim::~DialogNewSpriteAnim()
{
}

DialogNewAnim::DialogNewAnim( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer22;
	fgSizer22 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer22->AddGrowableCol( 1 );
	fgSizer22->SetFlexibleDirection( wxBOTH );
	fgSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_text1 = new wxStaticText( this, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_text1->Wrap( -1 );
	fgSizer22->Add( m_text1, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textName = new wxTextCtrl( this, wxID_ANY, wxT("Default"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer22->Add( m_textName, 0, wxALL|wxEXPAND, 5 );


	bSizer2->Add( fgSizer22, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxHORIZONTAL );


	bSizer2->Add( bSizer34, 1, wxEXPAND, 5 );

	m_sdbSizer4 = new wxStdDialogButtonSizer();
	m_sdbSizer4OK = new wxButton( this, wxID_OK );
	m_sdbSizer4->AddButton( m_sdbSizer4OK );
	m_sdbSizer4Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer4->AddButton( m_sdbSizer4Cancel );
	m_sdbSizer4->Realize();

	bSizer2->Add( m_sdbSizer4, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();

	this->Centre( wxBOTH );
}

DialogNewAnim::~DialogNewAnim()
{
}

DialogNewActor::DialogNewActor( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer22;
	fgSizer22 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer22->AddGrowableCol( 1 );
	fgSizer22->SetFlexibleDirection( wxBOTH );
	fgSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_text1 = new wxStaticText( this, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_text1->Wrap( -1 );
	fgSizer22->Add( m_text1, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textName = new wxTextCtrl( this, wxID_ANY, wxT("Default"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer22->Add( m_textName, 0, wxALL|wxEXPAND, 5 );


	bSizer2->Add( fgSizer22, 1, wxEXPAND, 5 );

	m_sdbSizer4 = new wxStdDialogButtonSizer();
	m_sdbSizer4OK = new wxButton( this, wxID_OK );
	m_sdbSizer4->AddButton( m_sdbSizer4OK );
	m_sdbSizer4Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer4->AddButton( m_sdbSizer4Cancel );
	m_sdbSizer4->Realize();

	bSizer2->Add( m_sdbSizer4, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();

	this->Centre( wxBOTH );
}

DialogNewActor::~DialogNewActor()
{
}

DialogTerrainGenBase::DialogTerrainGenBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	m_text1 = new wxStaticText( this, wxID_ANY, wxT("Granularity:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_text1->Wrap( -1 );
	bSizer3->Add( m_text1, 0, wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrlGranularity = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100000, 1000 );
	bSizer3->Add( m_spinCtrlGranularity, 0, wxALL, 5 );


	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_buttonOk = new wxButton( this, wxID_BTN_CANCEL, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_buttonOk, 0, 0, 5 );

	m_buttonCancel = new wxButton( this, wxID_BTN_CANCEL, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_buttonCancel, 0, 0, 5 );


	bSizer2->Add( bSizer4, 1, 0, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();
	bSizer2->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_buttonOk->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogTerrainGenBase::OnBtnOk ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogTerrainGenBase::OnBtnCancel ), NULL, this );
}

DialogTerrainGenBase::~DialogTerrainGenBase()
{
	// Disconnect Events
	m_buttonOk->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogTerrainGenBase::OnBtnOk ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogTerrainGenBase::OnBtnCancel ), NULL, this );

}

MapToolbox::MapToolbox( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer52;
	bSizer52 = new wxBoxSizer( wxVERTICAL );

	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Tile Tools"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer52->Add( m_staticText11, 0, wxALIGN_CENTER, 5 );

	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_toolSelectStamp = new wxBitmapButton( this, wxID_TOOL_SELECTSTAMP, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolSelectStamp->SetBitmap( wxBitmap( tool_selectstamp_xpm ) );
	m_toolSelectStamp->SetToolTip( wxT("Select Stamp") );

	fgSizer7->Add( m_toolSelectStamp, 0, wxALL, 5 );

	m_toolPaintStamp = new wxBitmapButton( this, wxID_TOOL_STAMP, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolPaintStamp->SetBitmap( wxBitmap( tool_paintstamp_xpm ) );
	m_toolPaintStamp->SetToolTip( wxT("Paint Stamp") );

	fgSizer7->Add( m_toolPaintStamp, 0, wxALL, 5 );

	m_toolMveStamp = new wxBitmapButton( this, wxID_TOOL_MOVESTAMP, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolMveStamp->SetBitmap( wxBitmap( tool_selectstamp_xpm ) );
	m_toolMveStamp->SetToolTip( wxT("Move Stamp") );

	fgSizer7->Add( m_toolMveStamp, 0, wxALL, 5 );

	m_toolStampPicker = new wxBitmapButton( this, wxID_TOOL_STAMPPICKER, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolStampPicker->SetBitmap( wxBitmap( tool_stamppicker_xpm ) );
	m_toolStampPicker->SetToolTip( wxT("Pick Stamp") );

	fgSizer7->Add( m_toolStampPicker, 0, wxALL, 5 );

	m_toolCreateStamp = new wxBitmapButton( this, wxID_TOOL_CREATESTAMP, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolCreateStamp->SetBitmap( wxBitmap( tool_createstamp_xpm ) );
	m_toolCreateStamp->SetToolTip( wxT("Create stamp from selection") );

	fgSizer7->Add( m_toolCreateStamp, 0, wxALL, 5 );

	m_toolRemoveStamp = new wxBitmapButton( this, wxID_TOOL_REMOVESTAMP, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolRemoveStamp->SetBitmap( wxBitmap( tool_deletestamp_xpm ) );
	m_toolRemoveStamp->SetToolTip( wxT("Remove Stamp") );

	fgSizer7->Add( m_toolRemoveStamp, 0, wxALL, 5 );


	bSizer52->Add( fgSizer7, 1, wxEXPAND, 5 );

	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("Collision Tools"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer52->Add( m_staticText13, 0, wxALIGN_CENTER, 5 );

	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_toolPaintCollisionPixel = new wxBitmapButton( this, wxID_TOOL_COL_PAINTTERRAIN, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolPaintCollisionPixel->SetBitmap( wxBitmap( tool_paintcolpixel_xpm ) );
	m_toolPaintCollisionPixel->SetToolTip( wxT("Paint Terrain (direct to map)") );

	fgSizer8->Add( m_toolPaintCollisionPixel, 0, wxALL, 5 );

	m_toolPaintCollisionSolid = new wxBitmapButton( this, wxID_TOOL_COL_PAINTSOLID, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolPaintCollisionSolid->SetBitmap( wxBitmap( tool_paintcolwall_xpm ) );
	m_toolPaintCollisionSolid->SetToolTip( wxT("Paint Solid Wall/Ceiling") );

	fgSizer8->Add( m_toolPaintCollisionSolid, 0, wxALL, 5 );

	m_toolPaintCollisionHole = new wxBitmapButton( this, wxID_TOOL_COL_PAINTHOLE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolPaintCollisionHole->SetBitmap( wxBitmap( tool_paintcolwall_xpm ) );
	m_toolPaintCollisionHole->SetToolTip( wxT("Paint Terrain Hole") );

	fgSizer8->Add( m_toolPaintCollisionHole, 0, wxALL, 5 );

	m_toolDeleteTerrainTile = new wxBitmapButton( this, wxID_TOOL_COL_DELETETERRTILE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolDeleteTerrainTile->SetBitmap( wxBitmap( tool_deleteterraintile_xpm ) );
	m_toolDeleteTerrainTile->SetToolTip( wxT("Delete Terrain Tile") );

	fgSizer8->Add( m_toolDeleteTerrainTile, 0, wxALL, 5 );

	m_toolAddTerrainBezier = new wxBitmapButton( this, wxID_TOOL_COL_ADDTERRAINBEZIER, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolAddTerrainBezier->SetBitmap( wxBitmap( tool_addterrainbezier_xpm ) );
	m_toolAddTerrainBezier->SetToolTip( wxT("Add Terrain Bezier") );

	fgSizer8->Add( m_toolAddTerrainBezier, 0, wxALL, 5 );

	m_toolEditTerrainBezier1 = new wxBitmapButton( this, wxID_TOOL_COL_EDITTERRAINBEZIER, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolEditTerrainBezier1->SetBitmap( wxBitmap( tool_editterrainbezier_xpm ) );
	m_toolEditTerrainBezier1->SetToolTip( wxT("Edit Terrain Bezier") );

	fgSizer8->Add( m_toolEditTerrainBezier1, 0, wxALL, 5 );

	m_toolDeleteTerrainBezier = new wxBitmapButton( this, wxID_TOOL_COL_DELETETERRAINBEZIER, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolDeleteTerrainBezier->SetBitmap( wxBitmap( tool_editterrainbezier_xpm ) );
	m_toolDeleteTerrainBezier->SetToolTip( wxT("Delete Terrain Bezier") );

	fgSizer8->Add( m_toolDeleteTerrainBezier, 0, wxALL, 5 );


	bSizer52->Add( fgSizer8, 1, wxEXPAND, 5 );


	bSizer16->Add( bSizer52, 1, wxEXPAND, 5 );


	bSizer16->Add( 0, 0, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer53;
	bSizer53 = new wxBoxSizer( wxVERTICAL );

	m_staticText12 = new wxStaticText( this, wxID_ANY, wxT("Stamp Tools"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer53->Add( m_staticText12, 0, wxALIGN_CENTER, 5 );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_toolSelectTile = new wxBitmapButton( this, wxID_TOOL_SELECTTILE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolSelectTile->SetBitmap( wxBitmap( tool_select_xpm ) );
	m_toolSelectTile->SetToolTip( wxT("Select Tile(s)") );

	fgSizer1->Add( m_toolSelectTile, 0, wxALL, 5 );

	m_toolPaint = new wxBitmapButton( this, wxID_TOOL_PAINT, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolPaint->SetBitmap( wxBitmap( tool_painttile_xpm ) );
	m_toolPaint->SetToolTip( wxT("Paint Tile") );

	fgSizer1->Add( m_toolPaint, 0, wxALL, 5 );

	m_toolTilePicker = new wxBitmapButton( this, wxID_TOOL_TILEPICKER, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolTilePicker->SetBitmap( wxBitmap( tool_picker_xpm ) );
	m_toolTilePicker->SetToolTip( wxT("Pick Tile") );

	fgSizer1->Add( m_toolTilePicker, 0, wxALL, 5 );

	m_toolFlipX = new wxBitmapButton( this, wxID_TOOL_FLIPX, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolFlipX->SetBitmap( wxBitmap( tool_flipx_xpm ) );
	m_toolFlipX->SetToolTip( wxT("Flip Horizontal") );

	fgSizer1->Add( m_toolFlipX, 0, wxALL, 5 );

	m_toolFlipY = new wxBitmapButton( this, wxID_TOOL_FLIPY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolFlipY->SetBitmap( wxBitmap( tool_flipy_xpm ) );
	m_toolFlipY->SetToolTip( wxT("Flip Vertical") );

	fgSizer1->Add( m_toolFlipY, 0, wxALL, 5 );

	m_toolFill = new wxBitmapButton( this, wxID_TOOL_FILL, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolFill->SetBitmap( wxBitmap( tool_fill_xpm ) );
	m_toolFill->SetToolTip( wxT("Fill Selection") );

	fgSizer1->Add( m_toolFill, 0, wxALL, 5 );

	m_toolClone = new wxBitmapButton( this, wxID_TOOL_CLONE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolClone->SetBitmap( wxBitmap( tool_clone_xpm ) );
	m_toolClone->SetToolTip( wxT("Clone Selection") );

	fgSizer1->Add( m_toolClone, 0, wxALL, 5 );

	m_toolCreateSceneAnim = new wxBitmapButton( this, wxID_TOOL_CREATE_SCENE_ANIM, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolCreateSceneAnim->SetBitmap( wxBitmap( tool_createstamp_xpm ) );
	m_toolCreateSceneAnim->SetToolTip( wxT("Create Scene Anim") );

	fgSizer1->Add( m_toolCreateSceneAnim, 0, wxALL, 5 );

	m_toolCopyToNewMap = new wxBitmapButton( this, wxID_TOOL_COPY_TO_NEW_MAP, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolCopyToNewMap->SetBitmap( wxBitmap( tool_createstamp_xpm ) );
	m_toolCopyToNewMap->SetToolTip( wxT("Copy to new map") );

	fgSizer1->Add( m_toolCopyToNewMap, 0, wxALL, 5 );


	bSizer53->Add( fgSizer1, 1, wxEXPAND, 5 );

	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("Game Object Tools"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer53->Add( m_staticText14, 0, wxALIGN_CENTER, 5 );

	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_toolSelectGameObject = new wxBitmapButton( this, wxID_TOOL_SELECTGAMEOBJ, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolSelectGameObject->SetBitmap( wxBitmap( tool_selectgameobj_xpm ) );
	m_toolSelectGameObject->SetToolTip( wxT("Select Game Object") );

	fgSizer9->Add( m_toolSelectGameObject, 0, wxALL, 5 );

	m_toolPlaceGameObject = new wxBitmapButton( this, wxID_TOOL_PLACEGAMEOBJ, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolPlaceGameObject->SetBitmap( wxBitmap( tool_placegameobj_xpm ) );
	m_toolPlaceGameObject->SetToolTip( wxT("Place Game Object") );

	fgSizer9->Add( m_toolPlaceGameObject, 0, wxALL, 5 );

	m_toolAnimateGameObject = new wxBitmapButton( this, wxID_TOOL_ANIMATEGAMEOBJ, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolAnimateGameObject->SetBitmap( wxBitmap( tool_selectgameobj_xpm ) );
	m_toolAnimateGameObject->SetToolTip( wxT("Animate Game Object") );

	fgSizer9->Add( m_toolAnimateGameObject, 0, wxALL, 5 );

	m_toolRemoveGameObject = new wxBitmapButton( this, wxID_TOOL_REMOVEGAMEOBJ, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolRemoveGameObject->SetBitmap( wxBitmap( tool_deletegameobj_xpm ) );
	m_toolRemoveGameObject->SetToolTip( wxT("Remove Game Object") );

	fgSizer9->Add( m_toolRemoveGameObject, 0, wxALL, 5 );


	bSizer53->Add( fgSizer9, 1, wxEXPAND, 5 );


	bSizer16->Add( bSizer53, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer16 );
	this->Layout();
}

MapToolbox::~MapToolbox()
{
}

MapToolboxGameObjs::MapToolboxGameObjs( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer53;
	bSizer53 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_toolSelectGameObject = new wxBitmapButton( this, wxID_TOOL_SELECTGAMEOBJ, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolSelectGameObject->SetBitmap( wxBitmap( tool_selectgameobj_xpm ) );
	m_toolSelectGameObject->SetToolTip( wxT("Select Game Object") );

	fgSizer9->Add( m_toolSelectGameObject, 0, wxALL, 5 );

	m_toolPlaceGameObject = new wxBitmapButton( this, wxID_TOOL_PLACEGAMEOBJ, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolPlaceGameObject->SetBitmap( wxBitmap( tool_placegameobj_xpm ) );
	m_toolPlaceGameObject->SetToolTip( wxT("Place Game Object") );

	fgSizer9->Add( m_toolPlaceGameObject, 0, wxALL, 5 );

	m_toolDrawGameObject = new wxBitmapButton( this, wxID_TOOL_DRAWGAMEOBJ, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolDrawGameObject->SetBitmap( wxBitmap( tool_placegameobj_xpm ) );
	m_toolDrawGameObject->SetToolTip( wxT("Draw Game Object") );

	fgSizer9->Add( m_toolDrawGameObject, 0, wxALL, 5 );

	m_toolAnimateGameObject = new wxBitmapButton( this, wxID_TOOL_ANIMATEGAMEOBJ, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolAnimateGameObject->SetBitmap( wxBitmap( tool_selectgameobj_xpm ) );
	m_toolAnimateGameObject->SetToolTip( wxT("Animate Game Object") );

	fgSizer9->Add( m_toolAnimateGameObject, 0, wxALL, 5 );

	m_toolRemoveGameObject = new wxBitmapButton( this, wxID_TOOL_REMOVEGAMEOBJ, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolRemoveGameObject->SetBitmap( wxBitmap( tool_deletegameobj_xpm ) );
	m_toolRemoveGameObject->SetToolTip( wxT("Remove Game Object") );

	fgSizer9->Add( m_toolRemoveGameObject, 0, wxALL, 5 );

	m_toolDuplicateGameObject = new wxBitmapButton( this, wxID_TOOL_DUPLICATEGAMEOBJ, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolDuplicateGameObject->SetBitmap( wxBitmap( tool_duplicategameobj_xpm ) );
	m_toolDuplicateGameObject->SetToolTip( wxT("Duplicate Game Object") );

	fgSizer9->Add( m_toolDuplicateGameObject, 0, wxALL, 5 );


	bSizer53->Add( fgSizer9, 1, wxEXPAND, 5 );


	bSizer16->Add( bSizer53, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer16 );
	this->Layout();
}

MapToolboxGameObjs::~MapToolboxGameObjs()
{
}

MapToolboxTerrain::MapToolboxTerrain( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer52;
	bSizer52 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_toolPaintCollisionPixel = new wxBitmapButton( this, wxID_TOOL_COL_PAINTTERRAIN, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolPaintCollisionPixel->SetBitmap( wxBitmap( tool_paintcolpixel_xpm ) );
	m_toolPaintCollisionPixel->SetToolTip( wxT("Paint Terrain (direct to map)") );

	fgSizer8->Add( m_toolPaintCollisionPixel, 0, wxALL, 5 );

	m_toolPaintCollisionSolid = new wxBitmapButton( this, wxID_TOOL_COL_PAINTSOLID, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolPaintCollisionSolid->SetBitmap( wxBitmap( tool_paintcolwall_xpm ) );
	m_toolPaintCollisionSolid->SetToolTip( wxT("Paint Solid Wall/Ceiling") );

	fgSizer8->Add( m_toolPaintCollisionSolid, 0, wxALL, 5 );

	m_toolPaintCollisionHole = new wxBitmapButton( this, wxID_TOOL_COL_PAINTHOLE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolPaintCollisionHole->SetBitmap( wxBitmap( tool_paintcolhole_xpm ) );
	m_toolPaintCollisionHole->SetToolTip( wxT("Paint Terrain Hole") );

	fgSizer8->Add( m_toolPaintCollisionHole, 0, wxALL, 5 );

	m_toolDeleteTerrainTile = new wxBitmapButton( this, wxID_TOOL_COL_DELETETERRTILE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolDeleteTerrainTile->SetBitmap( wxBitmap( tool_deleteterraintile_xpm ) );
	m_toolDeleteTerrainTile->SetToolTip( wxT("Delete Terrain Tile") );

	fgSizer8->Add( m_toolDeleteTerrainTile, 0, wxALL, 5 );

	m_toolAddTerrainBezier = new wxBitmapButton( this, wxID_TOOL_COL_ADDTERRAINBEZIER, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolAddTerrainBezier->SetBitmap( wxBitmap( tool_addterrainbezier_xpm ) );
	m_toolAddTerrainBezier->SetToolTip( wxT("Add Terrain Bezier") );

	fgSizer8->Add( m_toolAddTerrainBezier, 0, wxALL, 5 );

	m_toolEditTerrainBezier1 = new wxBitmapButton( this, wxID_TOOL_COL_EDITTERRAINBEZIER, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolEditTerrainBezier1->SetBitmap( wxBitmap( tool_editterrainbezier_xpm ) );
	m_toolEditTerrainBezier1->SetToolTip( wxT("Edit Terrain Bezier") );

	fgSizer8->Add( m_toolEditTerrainBezier1, 0, wxALL, 5 );

	m_toolDeleteTerrainBezier = new wxBitmapButton( this, wxID_TOOL_COL_DELETETERRAINBEZIER, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolDeleteTerrainBezier->SetBitmap( wxBitmap( tool_deleteterrainbezier_xpm ) );
	m_toolDeleteTerrainBezier->SetToolTip( wxT("Delete Terrain Bezier") );

	fgSizer8->Add( m_toolDeleteTerrainBezier, 0, wxALL, 5 );


	bSizer52->Add( fgSizer8, 1, wxEXPAND, 5 );


	bSizer16->Add( bSizer52, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer16 );
	this->Layout();
}

MapToolboxTerrain::~MapToolboxTerrain()
{
}

MapToolboxTiles::MapToolboxTiles( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer53;
	bSizer53 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_toolSelectTile = new wxBitmapButton( this, wxID_TOOL_SELECTTILE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolSelectTile->SetBitmap( wxBitmap( tool_select_xpm ) );
	m_toolSelectTile->SetToolTip( wxT("Select Tile(s)") );

	fgSizer1->Add( m_toolSelectTile, 0, wxALL, 5 );

	m_toolPaint = new wxBitmapButton( this, wxID_TOOL_PAINT, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolPaint->SetBitmap( wxBitmap( tool_painttile_xpm ) );
	m_toolPaint->SetToolTip( wxT("Paint Tile") );

	fgSizer1->Add( m_toolPaint, 0, wxALL, 5 );

	m_toolTilePicker = new wxBitmapButton( this, wxID_TOOL_TILEPICKER, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolTilePicker->SetBitmap( wxBitmap( tool_picker_xpm ) );
	m_toolTilePicker->SetToolTip( wxT("Pick Tile") );

	fgSizer1->Add( m_toolTilePicker, 0, wxALL, 5 );

	m_toolFlipX = new wxBitmapButton( this, wxID_TOOL_FLIPX, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolFlipX->SetBitmap( wxBitmap( tool_flipx_xpm ) );
	m_toolFlipX->SetToolTip( wxT("Flip Horizontal") );

	fgSizer1->Add( m_toolFlipX, 0, wxALL, 5 );

	m_toolFlipY = new wxBitmapButton( this, wxID_TOOL_FLIPY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolFlipY->SetBitmap( wxBitmap( tool_flipy_xpm ) );
	m_toolFlipY->SetToolTip( wxT("Flip Vertical") );

	fgSizer1->Add( m_toolFlipY, 0, wxALL, 5 );

	m_toolFill = new wxBitmapButton( this, wxID_TOOL_FILL, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolFill->SetBitmap( wxBitmap( tool_fill_xpm ) );
	m_toolFill->SetToolTip( wxT("Fill Selection") );

	fgSizer1->Add( m_toolFill, 0, wxALL, 5 );

	m_toolClone = new wxBitmapButton( this, wxID_TOOL_CLONE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolClone->SetBitmap( wxBitmap( tool_clone_xpm ) );
	m_toolClone->SetToolTip( wxT("Clone Selection") );

	fgSizer1->Add( m_toolClone, 0, wxALL, 5 );

	m_toolCreateSceneAnim = new wxBitmapButton( this, wxID_TOOL_CREATE_SCENE_ANIM, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolCreateSceneAnim->SetBitmap( wxBitmap( tool_createstamp_xpm ) );
	m_toolCreateSceneAnim->SetToolTip( wxT("Create Scene Anim") );

	fgSizer1->Add( m_toolCreateSceneAnim, 0, wxALL, 5 );

	m_toolCopyToNewMap = new wxBitmapButton( this, wxID_TOOL_COPY_TO_NEW_MAP, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolCopyToNewMap->SetBitmap( wxBitmap( tool_createstamp_xpm ) );
	m_toolCopyToNewMap->SetToolTip( wxT("Copy to new map") );

	fgSizer1->Add( m_toolCopyToNewMap, 0, wxALL, 5 );


	bSizer53->Add( fgSizer1, 1, wxEXPAND, 5 );


	bSizer16->Add( bSizer53, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer16 );
	this->Layout();
}

MapToolboxTiles::~MapToolboxTiles()
{
}

MapToolboxStamps::MapToolboxStamps( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer52;
	bSizer52 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_toolSelectStamp = new wxBitmapButton( this, wxID_TOOL_SELECTSTAMP, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolSelectStamp->SetBitmap( wxBitmap( tool_selectstamp_xpm ) );
	m_toolSelectStamp->SetToolTip( wxT("Select Stamp") );

	fgSizer7->Add( m_toolSelectStamp, 0, wxALL, 5 );

	m_toolPaintStamp = new wxBitmapButton( this, wxID_TOOL_STAMP, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolPaintStamp->SetBitmap( wxBitmap( tool_paintstamp_xpm ) );
	m_toolPaintStamp->SetToolTip( wxT("Paint Stamp") );

	fgSizer7->Add( m_toolPaintStamp, 0, wxALL, 5 );

	m_toolMveStamp = new wxBitmapButton( this, wxID_TOOL_MOVESTAMP, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolMveStamp->SetBitmap( wxBitmap( tool_movestamp_xpm ) );
	m_toolMveStamp->SetToolTip( wxT("Move Stamp") );

	fgSizer7->Add( m_toolMveStamp, 0, wxALL, 5 );

	m_toolStampPicker = new wxBitmapButton( this, wxID_TOOL_STAMPPICKER, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolStampPicker->SetBitmap( wxBitmap( tool_stamppicker_xpm ) );
	m_toolStampPicker->SetToolTip( wxT("Pick Stamp") );

	fgSizer7->Add( m_toolStampPicker, 0, wxALL, 5 );

	m_toolCreateStamp = new wxBitmapButton( this, wxID_TOOL_CREATESTAMP, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolCreateStamp->SetBitmap( wxBitmap( tool_createstamp_xpm ) );
	m_toolCreateStamp->SetToolTip( wxT("Create stamp from selection") );

	fgSizer7->Add( m_toolCreateStamp, 0, wxALL, 5 );

	m_toolRemoveStamp = new wxBitmapButton( this, wxID_TOOL_REMOVESTAMP, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_toolRemoveStamp->SetBitmap( wxBitmap( tool_deletestamp_xpm ) );
	m_toolRemoveStamp->SetToolTip( wxT("Remove Stamp") );

	fgSizer7->Add( m_toolRemoveStamp, 0, wxALL, 5 );


	bSizer52->Add( fgSizer7, 1, wxEXPAND, 5 );


	bSizer16->Add( bSizer52, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer16 );
	this->Layout();
}

MapToolboxStamps::~MapToolboxStamps()
{
}

MapListPanelBase::MapListPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );

	m_toolBar1 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	m_toolAddMap = m_toolBar1->AddTool( wxID_ANY, wxT("Add Variable"), wxBitmap( add_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolRemoveMap = m_toolBar1->AddTool( wxID_ANY, wxT("Remove Variable"), wxBitmap( remove_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolRename = m_toolBar1->AddTool( wxID_ANY, wxT("tool"), wxBitmap( qmark_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolImport = m_toolBar1->AddTool( wxID_ANY, wxT("tool"), wxBitmap( qmark_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolBar1->Realize();

	bSizer16->Add( m_toolBar1, 0, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer39;
	fgSizer39 = new wxFlexGridSizer( 1, 1, 0, 0 );
	fgSizer39->AddGrowableCol( 0 );
	fgSizer39->AddGrowableRow( 0 );
	fgSizer39->SetFlexibleDirection( wxBOTH );
	fgSizer39->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_listMaps = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_LIST|wxLC_SINGLE_SEL );
	fgSizer39->Add( m_listMaps, 0, wxALL|wxEXPAND, 5 );


	bSizer16->Add( fgSizer39, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer16 );
	this->Layout();

	// Connect Events
	this->Connect( m_toolAddMap->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapListPanelBase::OnToolAddMap ) );
	this->Connect( m_toolRemoveMap->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapListPanelBase::OnToolRemoveMap ) );
	this->Connect( m_toolRename->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapListPanelBase::OnToolRenameMap ) );
	this->Connect( m_toolImport->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapListPanelBase::OnToolImportMap ) );
	m_listMaps->Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( MapListPanelBase::OnMapRightClick ), NULL, this );
	m_listMaps->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( MapListPanelBase::OnMapSelected ), NULL, this );
}

MapListPanelBase::~MapListPanelBase()
{
	// Disconnect Events
	this->Disconnect( m_toolAddMap->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapListPanelBase::OnToolAddMap ) );
	this->Disconnect( m_toolRemoveMap->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapListPanelBase::OnToolRemoveMap ) );
	this->Disconnect( m_toolRename->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapListPanelBase::OnToolRenameMap ) );
	this->Disconnect( m_toolImport->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapListPanelBase::OnToolImportMap ) );
	m_listMaps->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( MapListPanelBase::OnMapRightClick ), NULL, this );
	m_listMaps->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( MapListPanelBase::OnMapSelected ), NULL, this );

}

GameObjTypesPanelBase::GameObjTypesPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxFlexGridSizer* fgSizer13;
	fgSizer13 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer13->AddGrowableCol( 0 );
	fgSizer13->AddGrowableCol( 1 );
	fgSizer13->AddGrowableRow( 1 );
	fgSizer13->SetFlexibleDirection( wxBOTH );
	fgSizer13->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText77 = new wxStaticText( this, wxID_ANY, wxT("Game Objects:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText77->Wrap( -1 );
	fgSizer13->Add( m_staticText77, 0, wxALL, 5 );

	m_staticText78 = new wxStaticText( this, wxID_ANY, wxT("Archetypes:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText78->Wrap( -1 );
	fgSizer13->Add( m_staticText78, 0, wxALL, 5 );

	m_listGameObjTypes = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	fgSizer13->Add( m_listGameObjTypes, 0, wxALL|wxEXPAND, 5 );

	m_listGameObjArchetypes = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	fgSizer13->Add( m_listGameObjArchetypes, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( fgSizer13 );
	this->Layout();

	// Connect Events
	m_listGameObjTypes->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GameObjTypesPanelBase::OnGameObjectTypeSelected ), NULL, this );
	m_listGameObjArchetypes->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GameObjTypesPanelBase::OnArchetypeSelected ), NULL, this );
}

GameObjTypesPanelBase::~GameObjTypesPanelBase()
{
	// Disconnect Events
	m_listGameObjTypes->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GameObjTypesPanelBase::OnGameObjectTypeSelected ), NULL, this );
	m_listGameObjArchetypes->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GameObjTypesPanelBase::OnArchetypeSelected ), NULL, this );

}

ExportDialogBase::ExportDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 4, 1, 0, 0 );
	fgSizer3->AddGrowableCol( 0 );
	fgSizer3->AddGrowableRow( 1 );
	fgSizer3->AddGrowableRow( 2 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer32;
	fgSizer32 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer32->AddGrowableCol( 1 );
	fgSizer32->SetFlexibleDirection( wxBOTH );
	fgSizer32->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_static1 = new wxStaticText( this, wxID_ANY, wxT("Project name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_static1->Wrap( -1 );
	fgSizer32->Add( m_static1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_txtProjectName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer32->Add( m_txtProjectName, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );

	m_btnText = new wxRadioButton( this, wxID_ANY, wxT("Text"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_btnText->SetValue( true );
	bSizer16->Add( m_btnText, 0, wxALL, 5 );

	m_btnBinary = new wxRadioButton( this, wxID_ANY, wxT("Binary"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_btnBinary, 0, wxALL, 5 );

	m_btnCompressed = new wxRadioButton( this, wxID_ANY, wxT("Compressed"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_btnCompressed, 0, wxALL, 5 );

	m_btnBeeFormat = new wxRadioButton( this, wxID_ANY, wxT(".bee"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_btnBeeFormat, 0, wxALL, 5 );


	fgSizer32->Add( bSizer16, 1, wxEXPAND, 5 );


	fgSizer3->Add( fgSizer32, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer62;
	bSizer62 = new wxBoxSizer( wxVERTICAL );

	m_scrolledWindow1 = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow1->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer41;
	fgSizer41 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer41->AddGrowableCol( 0 );
	fgSizer41->SetFlexibleDirection( wxBOTH );
	fgSizer41->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer33;
	fgSizer33 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer33->AddGrowableCol( 1 );
	fgSizer33->SetFlexibleDirection( wxBOTH );
	fgSizer33->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_chkPalettes = new wxCheckBox( m_scrolledWindow1, wxID_ANY, wxT("Export map palettes:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer33->Add( m_chkPalettes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerPalettes = new wxFilePickerCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer33->Add( m_filePickerPalettes, 0, wxALL|wxEXPAND, 5 );

	m_chkTileset = new wxCheckBox( m_scrolledWindow1, wxID_ANY, wxT("Export tileset:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer33->Add( m_chkTileset, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerTileset = new wxFilePickerCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer33->Add( m_filePickerTileset, 0, wxALL|wxEXPAND, 5 );

	m_chkBlocks = new wxCheckBox( m_scrolledWindow1, wxID_ANY, wxT("Export blocks:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer33->Add( m_chkBlocks, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerBlocks = new wxFilePickerCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer33->Add( m_filePickerBlocks, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	m_chkStamps = new wxCheckBox( m_scrolledWindow1, wxID_ANY, wxT("Export stamps:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer33->Add( m_chkStamps, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerStamps = new wxFilePickerCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer33->Add( m_filePickerStamps, 0, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );

	m_chkStampAnims = new wxCheckBox( m_scrolledWindow1, wxID_ANY, wxT("Export stamp animations:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer33->Add( m_chkStampAnims, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerStampAnims = new wxFilePickerCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer33->Add( m_filePickerStampAnims, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	m_chkTerrainTiles = new wxCheckBox( m_scrolledWindow1, wxID_ANY, wxT("Export terrain tileset:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer33->Add( m_chkTerrainTiles, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerTerrainTiles = new wxFilePickerCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer33->Add( m_filePickerTerrainTiles, 0, wxALL|wxEXPAND, 5 );

	m_chkTerrainAngles = new wxCheckBox( m_scrolledWindow1, wxID_ANY, wxT("Export terrain angles:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer33->Add( m_chkTerrainAngles, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerTerrainAngles = new wxFilePickerCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer33->Add( m_filePickerTerrainAngles, 0, wxALL|wxEXPAND, 5 );

	m_chkGameObjTypes = new wxCheckBox( m_scrolledWindow1, wxID_ANY, wxT("Export game object types:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer33->Add( m_chkGameObjTypes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerGameObjTypes = new wxFilePickerCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer33->Add( m_filePickerGameObjTypes, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	m_chkTerrainBlocks = new wxCheckBox( m_scrolledWindow1, wxID_ANY, wxT("Export terrain blocks:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer33->Add( m_chkTerrainBlocks, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerTerrainBlocks = new wxFilePickerCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer33->Add( m_filePickerTerrainBlocks, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	m_chkSpriteSheets = new wxCheckBox( m_scrolledWindow1, wxID_ANY, wxT("Export sprite sheets:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer33->Add( m_chkSpriteSheets, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_dirPickerSpriteSheets = new wxDirPickerCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxT("Select sprites directory"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer33->Add( m_dirPickerSpriteSheets, 0, wxALL|wxEXPAND, 5 );

	m_chkSpritePalettes = new wxCheckBox( m_scrolledWindow1, wxID_ANY, wxT("Export sprite palettes:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer33->Add( m_chkSpritePalettes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_dirPickerSpritePalettes = new wxDirPickerCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxT("Select sprites directory"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer33->Add( m_dirPickerSpritePalettes, 0, wxALL|wxEXPAND, 5 );

	m_chkSpriteAnims = new wxCheckBox( m_scrolledWindow1, wxID_ANY, wxT("Export sprite animations:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer33->Add( m_chkSpriteAnims, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_dirPickerSpriteAnims = new wxDirPickerCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxT("Select sprites directory"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer33->Add( m_dirPickerSpriteAnims, 0, wxALL|wxEXPAND, 5 );


	fgSizer41->Add( fgSizer33, 1, wxEXPAND, 5 );


	m_scrolledWindow1->SetSizer( fgSizer41 );
	m_scrolledWindow1->Layout();
	fgSizer41->Fit( m_scrolledWindow1 );
	bSizer62->Add( m_scrolledWindow1, 1, wxEXPAND | wxALL, 5 );


	fgSizer3->Add( bSizer62, 1, wxEXPAND, 5 );

	m_notebookMapTabs = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	fgSizer3->Add( m_notebookMapTabs, 1, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxHORIZONTAL );

	m_btnCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer43->Add( m_btnCancel, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_btnExportAll = new wxButton( this, wxID_ANY, wxT("Export All"), wxDefaultPosition, wxDefaultSize, 0 );

	m_btnExportAll->SetDefault();
	bSizer43->Add( m_btnExportAll, 0, wxALIGN_RIGHT|wxALL, 5 );


	fgSizer3->Add( bSizer43, 1, wxALIGN_RIGHT, 5 );


	this->SetSizer( fgSizer3 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExportDialogBase::OnBtnCancel ), NULL, this );
	m_btnExportAll->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExportDialogBase::OnBtnExportAll ), NULL, this );
}

ExportDialogBase::~ExportDialogBase()
{
	// Disconnect Events
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExportDialogBase::OnBtnCancel ), NULL, this );
	m_btnExportAll->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExportDialogBase::OnBtnExportAll ), NULL, this );

}

ProjectSettingsDialogBase::ProjectSettingsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* fgSizer40;
	fgSizer40 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer40->AddGrowableCol( 1 );
	fgSizer40->SetFlexibleDirection( wxBOTH );
	fgSizer40->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText691 = new wxStaticText( this, wxID_ANY, wxT("Project name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText691->Wrap( -1 );
	fgSizer40->Add( m_staticText691, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_textProjectName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_textProjectName->HasFlag( wxTE_MULTILINE ) )
	{
	m_textProjectName->SetMaxLength( 8 );
	}
	#else
	m_textProjectName->SetMaxLength( 8 );
	#endif
	fgSizer40->Add( m_textProjectName, 0, wxALL, 5 );


	fgSizer40->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText51112 = new wxStaticText( this, wxID_ANY, wxT("Engine root dir:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51112->Wrap( -1 );
	fgSizer40->Add( m_staticText51112, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_dirPickerEngine = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer40->Add( m_dirPickerEngine, 0, wxALL|wxEXPAND, 5 );

	m_btnParseProject = new wxButton( this, wxID_ANY, wxT("Scan Project"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer40->Add( m_btnParseProject, 0, wxALL, 5 );

	m_staticText511 = new wxStaticText( this, wxID_ANY, wxT("Project root dir:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText511->Wrap( -1 );
	fgSizer40->Add( m_staticText511, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_dirPickerProject = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer40->Add( m_dirPickerProject, 0, wxALL|wxEXPAND, 5 );


	fgSizer40->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText51 = new wxStaticText( this, wxID_ANY, wxT("Sprites definition file:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	fgSizer40->Add( m_staticText51, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_filePickerSpritesProj = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.bee_sprites"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	fgSizer40->Add( m_filePickerSpritesProj, 0, wxALL|wxEXPAND, 5 );


	fgSizer40->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText53 = new wxStaticText( this, wxID_ANY, wxT("Game Objects definition file:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53->Wrap( -1 );
	fgSizer40->Add( m_staticText53, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_filePickerGameObjTypesFile = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.bee_gameobj"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	fgSizer40->Add( m_filePickerGameObjTypesFile, 0, wxALL|wxEXPAND, 5 );


	fgSizer40->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText531 = new wxStaticText( this, wxID_ANY, wxT("Reference image:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText531->Wrap( -1 );
	fgSizer40->Add( m_staticText531, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_filePickerReference = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.bmp"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	fgSizer40->Add( m_filePickerReference, 0, wxALL|wxEXPAND, 5 );


	fgSizer40->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText68 = new wxStaticText( this, wxID_ANY, wxT("Stamp width (tiles):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText68->Wrap( -1 );
	fgSizer40->Add( m_staticText68, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_spinStampWidth = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1024, 16 );
	fgSizer40->Add( m_spinStampWidth, 0, wxALL, 5 );


	fgSizer40->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText69 = new wxStaticText( this, wxID_ANY, wxT("Stamp height (tiles):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText69->Wrap( -1 );
	fgSizer40->Add( m_staticText69, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_spinStampHeight = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1024, 16 );
	fgSizer40->Add( m_spinStampHeight, 0, wxALL, 5 );


	fgSizer40->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer40->Add( 0, 0, 1, wxEXPAND, 5 );

	m_sdbSizer7 = new wxStdDialogButtonSizer();
	m_sdbSizer7OK = new wxButton( this, wxID_OK );
	m_sdbSizer7->AddButton( m_sdbSizer7OK );
	m_sdbSizer7Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer7->AddButton( m_sdbSizer7Cancel );
	m_sdbSizer7->Realize();

	fgSizer40->Add( m_sdbSizer7, 1, wxEXPAND, 5 );


	this->SetSizer( fgSizer40 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ProjectSettingsDialogBase::OnClose ) );
	m_btnParseProject->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ProjectSettingsDialogBase::OnBtnScanProject ), NULL, this );
}

ProjectSettingsDialogBase::~ProjectSettingsDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ProjectSettingsDialogBase::OnClose ) );
	m_btnParseProject->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ProjectSettingsDialogBase::OnBtnScanProject ), NULL, this );

}

ImportDialogBase::ImportDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Bitmap files:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer11->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filenames = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_filenames, 0, wxALL|wxEXPAND, 5 );

	m_btnSelectFiles = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_btnSelectFiles, 0, wxALL, 5 );


	bSizer10->Add( bSizer11, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer111;
	bSizer111 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText81 = new wxStaticText( this, wxID_ANY, wxT("Stamps directory:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText81->Wrap( -1 );
	bSizer111->Add( m_staticText81, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_dirStamps = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST );
	bSizer111->Add( m_dirStamps, 0, wxALL|wxEXPAND, 5 );


	bSizer10->Add( bSizer111, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	m_chkClearTiles = new wxCheckBox( this, wxID_ANY, wxT("Clear tiles"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_chkClearTiles, 0, wxALL, 5 );

	m_chkClearMap = new wxCheckBox( this, wxID_ANY, wxT("Clear map"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_chkClearMap, 0, wxALL, 5 );

	m_chkClearPalettes = new wxCheckBox( this, wxID_ANY, wxT("Clear palettes"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_chkClearPalettes, 0, wxALL, 5 );

	m_chkImportPalette = new wxCheckBox( this, wxID_ANY, wxT("Import whole palette"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_chkImportPalette, 0, wxALL, 5 );

	m_chkInsertBGTile = new wxCheckBox( this, wxID_ANY, wxT("Insert background tile"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_chkInsertBGTile, 0, wxALL, 5 );

	m_chkPaintToMap = new wxCheckBox( this, wxID_ANY, wxT("Paint to map"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_chkPaintToMap, 0, wxALL, 5 );

	m_chkImportToStamp = new wxCheckBox( this, wxID_ANY, wxT("Import as new stamp(s)"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_chkImportToStamp, 0, wxALL, 5 );

	m_chkReplaceStamps = new wxCheckBox( this, wxID_ANY, wxT("Replace existing stamp(s)"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_chkReplaceStamps, 0, wxALL, 5 );

	m_chkOnlyExisting = new wxCheckBox( this, wxID_ANY, wxT("Don't import unrecognised stamps"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_chkOnlyExisting, 0, wxALL, 5 );

	m_chkNoDuplicateTileCheck = new wxCheckBox( this, wxID_ANY, wxT("Don't discard duplicate tiles"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_chkNoDuplicateTileCheck, 0, wxALL, 5 );


	bSizer12->Add( bSizer13, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );

	m_chkPalette1 = new wxCheckBox( this, wxID_ANY, wxT("Use palette 1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkPalette1->SetValue(true);
	bSizer14->Add( m_chkPalette1, 0, wxALL, 5 );

	m_chkPalette2 = new wxCheckBox( this, wxID_ANY, wxT("Use palette 2"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_chkPalette2, 0, wxALL, 5 );

	m_chkPalette3 = new wxCheckBox( this, wxID_ANY, wxT("Use palette 3"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_chkPalette3, 0, wxALL, 5 );

	m_chkPalette4 = new wxCheckBox( this, wxID_ANY, wxT("Use palette 4"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_chkPalette4, 0, wxALL, 5 );


	bSizer12->Add( bSizer14, 1, wxEXPAND, 5 );


	bSizer10->Add( bSizer12, 1, wxEXPAND, 5 );

	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
	m_sdbSizer2->Realize();

	bSizer10->Add( m_sdbSizer2, 1, 0, 5 );


	this->SetSizer( bSizer10 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_btnSelectFiles->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImportDialogBase::OnBtnBrowse ), NULL, this );
}

ImportDialogBase::~ImportDialogBase()
{
	// Disconnect Events
	m_btnSelectFiles->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImportDialogBase::OnBtnBrowse ), NULL, this );

}

ImportStampsDialogBase::ImportStampsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer54;
	fgSizer54 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer54->AddGrowableCol( 0 );
	fgSizer54->SetFlexibleDirection( wxBOTH );
	fgSizer54->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer52;
	fgSizer52 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer52->AddGrowableCol( 0 );
	fgSizer52->SetFlexibleDirection( wxBOTH );
	fgSizer52->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer57;
	fgSizer57 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer57->AddGrowableCol( 1 );
	fgSizer57->SetFlexibleDirection( wxBOTH );
	fgSizer57->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_radioSingleStamp = new wxRadioButton( this, wxID_ANY, wxT("Import single file:"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_radioSingleStamp->SetValue( true );
	fgSizer57->Add( m_radioSingleStamp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filenames = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer57->Add( m_filenames, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	m_btnSelectFiles = new wxButton( this, wxID_ANY, wxT("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer57->Add( m_btnSelectFiles, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	fgSizer52->Add( fgSizer57, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer59;
	fgSizer59 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer59->AddGrowableCol( 1 );
	fgSizer59->SetFlexibleDirection( wxBOTH );
	fgSizer59->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_radioStampDir = new wxRadioButton( this, wxID_ANY, wxT("Import directory:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer59->Add( m_radioStampDir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_dirStamps = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST );
	fgSizer59->Add( m_dirStamps, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );


	fgSizer52->Add( fgSizer59, 1, wxEXPAND, 5 );


	fgSizer54->Add( fgSizer52, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer77;
	bSizer77 = new wxBoxSizer( wxHORIZONTAL );

	wxFlexGridSizer* fgSizer55;
	fgSizer55 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer55->SetFlexibleDirection( wxBOTH );
	fgSizer55->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_chkClearPalettes = new wxCheckBox( this, wxID_ANY, wxT("Clear palettes"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer55->Add( m_chkClearPalettes, 0, wxALL, 5 );

	m_chkImportPalette = new wxCheckBox( this, wxID_ANY, wxT("Import whole palette"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer55->Add( m_chkImportPalette, 0, wxALL, 5 );

	m_chkReplaceStamps = new wxCheckBox( this, wxID_ANY, wxT("Replace existing stamp(s)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer55->Add( m_chkReplaceStamps, 0, wxALL, 5 );

	wxString m_radioBoxPalChoices[] = { wxT("1"), wxT("2"), wxT("3"), wxT("4") };
	int m_radioBoxPalNChoices = sizeof( m_radioBoxPalChoices ) / sizeof( wxString );
	m_radioBoxPal = new wxRadioBox( this, wxID_ANY, wxT("Palette"), wxDefaultPosition, wxDefaultSize, m_radioBoxPalNChoices, m_radioBoxPalChoices, 1, wxRA_SPECIFY_COLS );
	m_radioBoxPal->SetSelection( 0 );
	fgSizer55->Add( m_radioBoxPal, 0, wxALL|wxEXPAND, 5 );


	bSizer77->Add( fgSizer55, 1, wxEXPAND, 5 );


	fgSizer54->Add( bSizer77, 1, wxEXPAND, 5 );

	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
	m_sdbSizer2->Realize();

	fgSizer54->Add( m_sdbSizer2, 1, wxALIGN_RIGHT, 5 );


	bSizer10->Add( fgSizer54, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer10 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_radioSingleStamp->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ImportStampsDialogBase::OnRadioImportFile ), NULL, this );
	m_btnSelectFiles->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImportStampsDialogBase::OnBtnBrowse ), NULL, this );
	m_radioStampDir->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ImportStampsDialogBase::OnRadioImportDir ), NULL, this );
}

ImportStampsDialogBase::~ImportStampsDialogBase()
{
	// Disconnect Events
	m_radioSingleStamp->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ImportStampsDialogBase::OnRadioImportFile ), NULL, this );
	m_btnSelectFiles->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImportStampsDialogBase::OnBtnBrowse ), NULL, this );
	m_radioStampDir->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ImportStampsDialogBase::OnRadioImportDir ), NULL, this );

}

ImportDialogSpriteSheetBase::ImportDialogSpriteSheetBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 3, 1, 0, 0 );
	fgSizer14->AddGrowableCol( 0 );
	fgSizer14->AddGrowableRow( 1 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Bitmap:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer15->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN );
	fgSizer15->Add( m_filePicker, 1, wxALL|wxEXPAND, 5 );

	m_staticText24 = new wxStaticText( this, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer15->Add( m_staticText24, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textName = new wxTextCtrl( this, wxID_ANY, wxT("SpriteSheet001"), wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_textName->HasFlag( wxTE_MULTILINE ) )
	{
	m_textName->SetMaxLength( 64 );
	}
	#else
	m_textName->SetMaxLength( 64 );
	#endif
	fgSizer15->Add( m_textName, 1, wxALL|wxEXPAND, 5 );


	fgSizer14->Add( fgSizer15, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer13;
	fgSizer13 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer13->AddGrowableCol( 0 );
	fgSizer13->AddGrowableRow( 0 );
	fgSizer13->SetFlexibleDirection( wxBOTH );
	fgSizer13->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_canvas = new SpriteCanvas( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	fgSizer13->Add( m_canvas, 1, wxEXPAND | wxALL, 5 );

	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText241 = new wxStaticText( this, wxID_ANY, wxT("Grid colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText241->Wrap( -1 );
	fgSizer12->Add( m_staticText241, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_gridColourPicker = new wxColourPickerCtrl( this, wxID_ANY, wxColour( 0, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer12->Add( m_gridColourPicker, 0, wxALL, 5 );

	m_staticText21 = new wxStaticText( this, wxID_ANY, wxT("Width (cells):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer12->Add( m_staticText21, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_spinWidthCells = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1024, 4 );
	fgSizer12->Add( m_spinWidthCells, 0, wxALL, 5 );

	m_staticText22 = new wxStaticText( this, wxID_ANY, wxT("Height (cells):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	fgSizer12->Add( m_staticText22, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_spinHeightCells = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1024, 4 );
	fgSizer12->Add( m_spinHeightCells, 0, wxALL, 5 );

	m_staticText23 = new wxStaticText( this, wxID_ANY, wxT("Max cells:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	fgSizer12->Add( m_staticText23, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_spinCellCount = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1024, 1 );
	fgSizer12->Add( m_spinCellCount, 0, wxALL, 5 );


	fgSizer13->Add( fgSizer12, 1, wxEXPAND, 5 );


	fgSizer14->Add( fgSizer13, 1, wxEXPAND, 5 );

	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
	m_sdbSizer2->Realize();

	fgSizer14->Add( m_sdbSizer2, 1, wxALIGN_RIGHT, 5 );


	bSizer10->Add( fgSizer14, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer10 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_filePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( ImportDialogSpriteSheetBase::OnFileOpened ), NULL, this );
	m_gridColourPicker->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ImportDialogSpriteSheetBase::OnGridColourChanged ), NULL, this );
	m_spinWidthCells->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ImportDialogSpriteSheetBase::OnSpinWidthCells ), NULL, this );
	m_spinHeightCells->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ImportDialogSpriteSheetBase::OnSpinHeightCells ), NULL, this );
	m_spinCellCount->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ImportDialogSpriteSheetBase::OnSpinCellCount ), NULL, this );
}

ImportDialogSpriteSheetBase::~ImportDialogSpriteSheetBase()
{
	// Disconnect Events
	m_filePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( ImportDialogSpriteSheetBase::OnFileOpened ), NULL, this );
	m_gridColourPicker->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ImportDialogSpriteSheetBase::OnGridColourChanged ), NULL, this );
	m_spinWidthCells->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ImportDialogSpriteSheetBase::OnSpinWidthCells ), NULL, this );
	m_spinHeightCells->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ImportDialogSpriteSheetBase::OnSpinHeightCells ), NULL, this );
	m_spinCellCount->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ImportDialogSpriteSheetBase::OnSpinCellCount ), NULL, this );

}

SpriteAnimEditorDialogBase::SpriteAnimEditorDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 3, 1, 0, 0 );
	fgSizer14->AddGrowableCol( 0 );
	fgSizer14->AddGrowableRow( 1 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


	fgSizer14->Add( fgSizer15, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer13;
	fgSizer13 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer13->AddGrowableCol( 0 );
	fgSizer13->AddGrowableRow( 0 );
	fgSizer13->SetFlexibleDirection( wxBOTH );
	fgSizer13->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer24;
	fgSizer24 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer24->AddGrowableCol( 1 );
	fgSizer24->AddGrowableRow( 0 );
	fgSizer24->SetFlexibleDirection( wxBOTH );
	fgSizer24->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer25;
	fgSizer25 = new wxFlexGridSizer( 3, 3, 0, 0 );
	fgSizer25->AddGrowableRow( 1 );
	fgSizer25->SetFlexibleDirection( wxBOTH );
	fgSizer25->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText36 = new wxStaticText( this, wxID_ANY, wxT("Actors/Props/Stamps:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText36->Wrap( -1 );
	fgSizer25->Add( m_staticText36, 0, wxALL, 5 );

	m_staticText38 = new wxStaticText( this, wxID_ANY, wxT("Sprite/Stamp Sheets:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38->Wrap( -1 );
	fgSizer25->Add( m_staticText38, 0, wxALL, 5 );

	m_staticText39 = new wxStaticText( this, wxID_ANY, wxT("Animations:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39->Wrap( -1 );
	fgSizer25->Add( m_staticText39, 0, wxALL, 5 );

	m_listActors = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	fgSizer25->Add( m_listActors, 0, wxALL|wxEXPAND, 5 );

	m_listSpriteSheets = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	fgSizer25->Add( m_listSpriteSheets, 0, wxALL|wxEXPAND, 5 );

	m_listAnimations = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	fgSizer25->Add( m_listAnimations, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer36;
	bSizer36 = new wxBoxSizer( wxHORIZONTAL );

	m_btnNewActor = new wxButton( this, wxID_ANY, wxT("New..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer36->Add( m_btnNewActor, 0, wxALL, 5 );

	m_btnDeleteActor = new wxButton( this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer36->Add( m_btnDeleteActor, 0, wxALL, 5 );


	bSizer35->Add( bSizer36, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer60;
	bSizer60 = new wxBoxSizer( wxHORIZONTAL );

	m_btnImport = new wxButton( this, wxID_ANY, wxT("Import..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer60->Add( m_btnImport, 0, wxALL, 5 );

	m_btnExport = new wxButton( this, wxID_ANY, wxT("Export..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer60->Add( m_btnExport, 0, wxALL, 5 );


	bSizer35->Add( bSizer60, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer601;
	bSizer601 = new wxBoxSizer( wxHORIZONTAL );

	m_btnRenameActor = new wxButton( this, wxID_ANY, wxT("Rename..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer601->Add( m_btnRenameActor, 0, wxALL, 5 );


	bSizer35->Add( bSizer601, 1, wxEXPAND, 5 );


	fgSizer25->Add( bSizer35, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer40;
	bSizer40 = new wxBoxSizer( wxHORIZONTAL );

	m_btnImportSpriteSheet = new wxButton( this, wxID_ANY, wxT("New..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer40->Add( m_btnImportSpriteSheet, 0, wxALL, 5 );

	m_btnDeleteSprite = new wxButton( this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer40->Add( m_btnDeleteSprite, 0, wxALL, 5 );


	bSizer26->Add( bSizer40, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxHORIZONTAL );

	m_btnUsePalette = new wxButton( this, wxID_ANY, wxT("Use Palette"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer61->Add( m_btnUsePalette, 0, wxALL, 5 );

	m_btnRenameSheet = new wxButton( this, wxID_ANY, wxT("Rename..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer61->Add( m_btnRenameSheet, 0, wxALL, 5 );


	bSizer26->Add( bSizer61, 1, wxEXPAND, 5 );


	fgSizer25->Add( bSizer26, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer69;
	bSizer69 = new wxBoxSizer( wxHORIZONTAL );

	m_btnNewAnim = new wxButton( this, wxID_ANY, wxT("New..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer69->Add( m_btnNewAnim, 0, wxALL, 5 );

	m_btnDeleteAnim = new wxButton( this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer69->Add( m_btnDeleteAnim, 0, wxALL, 5 );


	bSizer27->Add( bSizer69, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer271;
	bSizer271 = new wxBoxSizer( wxHORIZONTAL );

	m_btnRenameAnim = new wxButton( this, wxID_ANY, wxT("Rename..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer271->Add( m_btnRenameAnim, 0, wxALL, 5 );


	bSizer27->Add( bSizer271, 1, wxEXPAND, 5 );


	fgSizer25->Add( bSizer27, 1, wxEXPAND, 5 );


	fgSizer24->Add( fgSizer25, 1, wxEXPAND, 5 );

	m_canvas = new SpriteCanvas( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	fgSizer24->Add( m_canvas, 1, wxEXPAND | wxALL, 5 );


	fgSizer13->Add( fgSizer24, 1, wxEXPAND, 5 );

	wxFlexGridSizer* m_sizerTimeline;
	m_sizerTimeline = new wxFlexGridSizer( 6, 1, 0, 0 );
	m_sizerTimeline->AddGrowableCol( 0 );
	m_sizerTimeline->AddGrowableRow( 1 );
	m_sizerTimeline->AddGrowableRow( 3 );
	m_sizerTimeline->SetFlexibleDirection( wxBOTH );
	m_sizerTimeline->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText33 = new wxStaticText( this, wxID_ANY, wxT("Sprite Sheet:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33->Wrap( -1 );
	m_sizerTimeline->Add( m_staticText33, 0, wxALL, 5 );

	m_gridSpriteFrames = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL );

	// Grid
	m_gridSpriteFrames->CreateGrid( 1, 1 );
	m_gridSpriteFrames->EnableEditing( false );
	m_gridSpriteFrames->EnableGridLines( true );
	m_gridSpriteFrames->EnableDragGridSize( false );
	m_gridSpriteFrames->SetMargins( 0, 0 );

	// Columns
	m_gridSpriteFrames->SetColSize( 0, 128 );
	m_gridSpriteFrames->EnableDragColMove( false );
	m_gridSpriteFrames->EnableDragColSize( false );
	m_gridSpriteFrames->SetColLabelSize( 0 );
	m_gridSpriteFrames->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_gridSpriteFrames->SetRowSize( 0, 64 );
	m_gridSpriteFrames->EnableDragRowSize( false );
	m_gridSpriteFrames->SetRowLabelSize( 0 );
	m_gridSpriteFrames->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_gridSpriteFrames->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	m_sizerTimeline->Add( m_gridSpriteFrames, 0, wxALL|wxEXPAND, 5 );

	m_staticText32 = new wxStaticText( this, wxID_ANY, wxT("Timeline:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	m_sizerTimeline->Add( m_staticText32, 0, wxALL, 5 );

	m_gridTimeline = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL );

	// Grid
	m_gridTimeline->CreateGrid( 1, 1 );
	m_gridTimeline->EnableEditing( true );
	m_gridTimeline->EnableGridLines( true );
	m_gridTimeline->EnableDragGridSize( false );
	m_gridTimeline->SetMargins( 0, 0 );

	// Columns
	m_gridTimeline->SetColSize( 0, 128 );
	m_gridTimeline->EnableDragColMove( false );
	m_gridTimeline->EnableDragColSize( false );
	m_gridTimeline->SetColLabelSize( 0 );
	m_gridTimeline->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_gridTimeline->SetRowSize( 0, 64 );
	m_gridTimeline->EnableDragRowSize( false );
	m_gridTimeline->SetRowLabelSize( 0 );
	m_gridTimeline->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_gridTimeline->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	m_sizerTimeline->Add( m_gridTimeline, 0, wxALL|wxEXPAND, 5 );

	m_sliderTimeline = new wxSlider( this, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	m_sizerTimeline->Add( m_sliderTimeline, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxHORIZONTAL );

	m_btnPlay = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_btnPlay->SetBitmap( wxBitmap( play_16_16_xpm ) );
	bSizer28->Add( m_btnPlay, 0, wxALL, 5 );

	m_btnStop = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_btnStop->SetBitmap( wxBitmap( stop_16_16_xpm ) );
	bSizer28->Add( m_btnStop, 0, wxALL, 5 );

	m_staticText40 = new wxStaticText( this, wxID_ANY, wxT("Speed:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText40->Wrap( -1 );
	bSizer28->Add( m_staticText40, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_spinCtrlSpeed = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000, 11 );
	bSizer28->Add( m_spinCtrlSpeed, 0, wxALL, 5 );

	m_staticText43 = new wxStaticText( this, wxID_ANY, wxT("Blend mode:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText43->Wrap( -1 );
	bSizer28->Add( m_staticText43, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_radioBlendLerp = new wxRadioButton( this, wxID_ANY, wxT("Linear"), wxDefaultPosition, wxDefaultSize, 0 );
	m_radioBlendLerp->SetValue( true );
	bSizer28->Add( m_radioBlendLerp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_radioBlendSnap = new wxRadioButton( this, wxID_ANY, wxT("Snap"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer28->Add( m_radioBlendSnap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	m_sizerTimeline->Add( bSizer28, 1, wxEXPAND, 5 );


	fgSizer13->Add( m_sizerTimeline, 1, wxEXPAND, 5 );


	fgSizer14->Add( fgSizer13, 1, wxEXPAND, 5 );


	bSizer10->Add( fgSizer14, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer10 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_listActors->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnActorSelected ), NULL, this );
	m_listSpriteSheets->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnSpriteSheetSelected ), NULL, this );
	m_listAnimations->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnAnimSelected ), NULL, this );
	m_btnNewActor->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnActorNew ), NULL, this );
	m_btnDeleteActor->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnActorDelete ), NULL, this );
	m_btnImport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnImport ), NULL, this );
	m_btnExport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnExport ), NULL, this );
	m_btnRenameActor->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnRenameActor ), NULL, this );
	m_btnImportSpriteSheet->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnSpriteSheetImport ), NULL, this );
	m_btnDeleteSprite->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnSpriteSheetDelete ), NULL, this );
	m_btnUsePalette->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnSpriteSheetUsePalette ), NULL, this );
	m_btnRenameSheet->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnRenameSheet ), NULL, this );
	m_btnNewAnim->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnAnimNew ), NULL, this );
	m_btnDeleteAnim->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnAnimDelete ), NULL, this );
	m_btnRenameAnim->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnRenameAnim ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_btnPlay->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnPlay ), NULL, this );
	m_btnStop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnStop ), NULL, this );
	m_spinCtrlSpeed->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( SpriteAnimEditorDialogBase::OnSpinSpeedChange ), NULL, this );
	m_radioBlendLerp->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnRadioBlendLerp ), NULL, this );
	m_radioBlendSnap->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnRadioBlendSnap ), NULL, this );
}

SpriteAnimEditorDialogBase::~SpriteAnimEditorDialogBase()
{
	// Disconnect Events
	m_listActors->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnActorSelected ), NULL, this );
	m_listSpriteSheets->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnSpriteSheetSelected ), NULL, this );
	m_listAnimations->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnAnimSelected ), NULL, this );
	m_btnNewActor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnActorNew ), NULL, this );
	m_btnDeleteActor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnActorDelete ), NULL, this );
	m_btnImport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnImport ), NULL, this );
	m_btnExport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnExport ), NULL, this );
	m_btnRenameActor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnRenameActor ), NULL, this );
	m_btnImportSpriteSheet->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnSpriteSheetImport ), NULL, this );
	m_btnDeleteSprite->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnSpriteSheetDelete ), NULL, this );
	m_btnUsePalette->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnSpriteSheetUsePalette ), NULL, this );
	m_btnRenameSheet->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnRenameSheet ), NULL, this );
	m_btnNewAnim->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnAnimNew ), NULL, this );
	m_btnDeleteAnim->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnAnimDelete ), NULL, this );
	m_btnRenameAnim->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnRenameAnim ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( SpriteAnimEditorDialogBase::OnSliderMove ), NULL, this );
	m_btnPlay->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnPlay ), NULL, this );
	m_btnStop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnStop ), NULL, this );
	m_spinCtrlSpeed->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( SpriteAnimEditorDialogBase::OnSpinSpeedChange ), NULL, this );
	m_radioBlendLerp->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnRadioBlendLerp ), NULL, this );
	m_radioBlendSnap->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnRadioBlendSnap ), NULL, this );

}

GameObjTypeDialogBase::GameObjTypeDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	m_toolBar1 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	m_toolAddGameObjType = m_toolBar1->AddTool( wxID_ANY, wxT("Add Game Object Type"), wxBitmap( add_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolRemoveGameObjType = m_toolBar1->AddTool( wxID_ANY, wxT("Remove Game Object Type"), wxBitmap( remove_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolOrderUp = m_toolBar1->AddTool( wxID_ANY, wxT("Add Variable"), wxBitmap( up_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolOrderDown = m_toolBar1->AddTool( wxID_ANY, wxT("Add Variable"), wxBitmap( down_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolBar1->AddSeparator();

	m_toolAddVariable = m_toolBar1->AddTool( wxID_ANY, wxT("Add Variable"), wxBitmap( add_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolRemoveVariable = m_toolBar1->AddTool( wxID_ANY, wxT("Remove Variable"), wxBitmap( remove_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolBar1->Realize();

	bSizer10->Add( m_toolBar1, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );

	m_listGameObjTypes = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), 0, NULL, 0 );
	bSizer11->Add( m_listGameObjTypes, 0, wxALL|wxEXPAND, 5 );

	m_listVariables = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 400,-1 ), wxLC_REPORT );
	bSizer11->Add( m_listVariables, 0, wxALL|wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 3, 1, 0, 0 );
	fgSizer4->AddGrowableCol( 0 );
	fgSizer4->AddGrowableRow( 0 );
	fgSizer4->AddGrowableRow( 2 );
	fgSizer4->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer5->AddGrowableCol( 1 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("Game Object Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer5->Add( m_staticText10, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_textGameObjName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 300,-1 ), 0 );
	m_textGameObjName->Enable( false );

	fgSizer5->Add( m_textGameObjName, 0, wxALL|wxEXPAND, 5 );

	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("Dimensions:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer5->Add( m_staticText9, 0, wxALIGN_RIGHT|wxALL, 5 );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );

	m_spinWidth = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1024, 16 );
	m_spinWidth->Enable( false );
	m_spinWidth->SetMaxSize( wxSize( 80,-1 ) );

	bSizer12->Add( m_spinWidth, 0, wxALL, 5 );

	m_spinHeight = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1024, 15 );
	m_spinHeight->Enable( false );
	m_spinHeight->SetMaxSize( wxSize( 80,-1 ) );

	bSizer12->Add( m_spinHeight, 0, wxALL, 5 );


	fgSizer5->Add( bSizer12, 1, wxEXPAND, 5 );

	m_staticText49 = new wxStaticText( this, wxID_ANY, wxT("Initialisation order:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText49->Wrap( -1 );
	fgSizer5->Add( m_staticText49, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );


	fgSizer5->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText50 = new wxStaticText( this, wxID_ANY, wxT("Default actor:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText50->Wrap( -1 );
	fgSizer5->Add( m_staticText50, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxArrayString m_choiceSpriteActorChoices;
	m_choiceSpriteActor = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceSpriteActorChoices, 0 );
	m_choiceSpriteActor->SetSelection( 0 );
	fgSizer5->Add( m_choiceSpriteActor, 0, wxALL|wxEXPAND, 5 );

	m_staticText501 = new wxStaticText( this, wxID_ANY, wxT("Default sheet:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText501->Wrap( -1 );
	fgSizer5->Add( m_staticText501, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxArrayString m_choiceSpriteSheetChoices;
	m_choiceSpriteSheet = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceSpriteSheetChoices, 0 );
	m_choiceSpriteSheet->SetSelection( 0 );
	fgSizer5->Add( m_choiceSpriteSheet, 0, wxALL|wxEXPAND, 5 );

	m_staticText5011 = new wxStaticText( this, wxID_ANY, wxT("Default anim:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5011->Wrap( -1 );
	fgSizer5->Add( m_staticText5011, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxArrayString m_choiceSpriteAnimChoices;
	m_choiceSpriteAnim = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceSpriteAnimChoices, 0 );
	m_choiceSpriteAnim->SetSelection( 0 );
	fgSizer5->Add( m_choiceSpriteAnim, 0, wxALL|wxEXPAND, 5 );

	m_btnApplyObjSettings = new wxButton( this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnApplyObjSettings->Enable( false );

	fgSizer5->Add( m_btnApplyObjSettings, 0, wxALL, 5 );

	m_btnLoadSprite = new wxButton( this, wxID_ANY, wxT("Load preview sprite..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnLoadSprite->Enable( false );

	fgSizer5->Add( m_btnLoadSprite, 0, wxALL, 5 );


	fgSizer4->Add( fgSizer5, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer50;
	bSizer50 = new wxBoxSizer( wxVERTICAL );

	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer50->Add( m_staticline2, 0, wxALL|wxEXPAND, 5 );


	fgSizer4->Add( bSizer50, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->AddGrowableRow( 3 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Variable:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer3->Add( m_staticText6, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_textVariableName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 300,-1 ), 0 );
	m_textVariableName->Enable( false );

	fgSizer3->Add( m_textVariableName, 0, wxALL|wxEXPAND, 5 );

	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer3->Add( m_staticText7, 0, wxALIGN_RIGHT|wxALL, 5 );

	wxString m_choiceSizeChoices[] = { wxT("Byte"), wxT("Word"), wxT("Longword") };
	int m_choiceSizeNChoices = sizeof( m_choiceSizeChoices ) / sizeof( wxString );
	m_choiceSize = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceSizeNChoices, m_choiceSizeChoices, 0 );
	m_choiceSize->SetSelection( 0 );
	m_choiceSize->Enable( false );

	fgSizer3->Add( m_choiceSize, 0, wxALL, 5 );

	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Value:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer3->Add( m_staticText8, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_textValue = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textValue->Enable( false );

	fgSizer3->Add( m_textValue, 0, wxALL|wxEXPAND, 5 );


	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );

	m_btnApplyVarSettings = new wxButton( this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );

	m_btnApplyVarSettings->SetDefault();
	m_btnApplyVarSettings->Enable( false );

	fgSizer3->Add( m_btnApplyVarSettings, 0, wxALL, 5 );


	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxHORIZONTAL );

	m_btnImport = new wxButton( this, wxID_ANY, wxT("Import..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer141->Add( m_btnImport, 0, wxALIGN_BOTTOM|wxALL, 5 );

	m_btnExport = new wxButton( this, wxID_ANY, wxT("Export..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer141->Add( m_btnExport, 0, wxALIGN_BOTTOM|wxALL, 5 );


	fgSizer3->Add( bSizer141, 1, wxEXPAND, 5 );


	fgSizer4->Add( fgSizer3, 1, wxEXPAND, 5 );


	bSizer11->Add( fgSizer4, 1, wxEXPAND, 5 );


	bSizer10->Add( bSizer11, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer10 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( m_toolAddGameObjType->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolGameObjAdd ) );
	this->Connect( m_toolRemoveGameObjType->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolGameObjRemove ) );
	this->Connect( m_toolOrderUp->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolOrderUp ) );
	this->Connect( m_toolOrderDown->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolOrderDown ) );
	this->Connect( m_toolAddVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolVariableAdd ) );
	this->Connect( m_toolRemoveVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolVariableRemove ) );
	m_listGameObjTypes->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GameObjTypeDialogBase::OnSelectGameObjType ), NULL, this );
	m_listVariables->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( GameObjTypeDialogBase::OnSelectVariable ), NULL, this );
	m_spinWidth->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( GameObjTypeDialogBase::OnWidthChanged ), NULL, this );
	m_spinHeight->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( GameObjTypeDialogBase::OnHeightChanged ), NULL, this );
	m_choiceSpriteActor->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjTypeDialogBase::OnSelectSpriteActor ), NULL, this );
	m_choiceSpriteSheet->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjTypeDialogBase::OnSelectSpriteSheet ), NULL, this );
	m_choiceSpriteAnim->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjTypeDialogBase::OnSelectSpriteAnim ), NULL, this );
	m_btnApplyObjSettings->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnBtnApplyObjChanges ), NULL, this );
	m_btnLoadSprite->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnBtnLoadSprite ), NULL, this );
	m_choiceSize->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjTypeDialogBase::OnVariableSizeChanged ), NULL, this );
	m_btnApplyVarSettings->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnBtnApplyVarChanges ), NULL, this );
	m_btnImport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnBtnImport ), NULL, this );
	m_btnExport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnBtnExport ), NULL, this );
}

GameObjTypeDialogBase::~GameObjTypeDialogBase()
{
	// Disconnect Events
	this->Disconnect( m_toolAddGameObjType->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolGameObjAdd ) );
	this->Disconnect( m_toolRemoveGameObjType->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolGameObjRemove ) );
	this->Disconnect( m_toolOrderUp->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolOrderUp ) );
	this->Disconnect( m_toolOrderDown->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolOrderDown ) );
	this->Disconnect( m_toolAddVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolVariableAdd ) );
	this->Disconnect( m_toolRemoveVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolVariableRemove ) );
	m_listGameObjTypes->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GameObjTypeDialogBase::OnSelectGameObjType ), NULL, this );
	m_listVariables->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( GameObjTypeDialogBase::OnSelectVariable ), NULL, this );
	m_spinWidth->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( GameObjTypeDialogBase::OnWidthChanged ), NULL, this );
	m_spinHeight->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( GameObjTypeDialogBase::OnHeightChanged ), NULL, this );
	m_choiceSpriteActor->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjTypeDialogBase::OnSelectSpriteActor ), NULL, this );
	m_choiceSpriteSheet->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjTypeDialogBase::OnSelectSpriteSheet ), NULL, this );
	m_choiceSpriteAnim->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjTypeDialogBase::OnSelectSpriteAnim ), NULL, this );
	m_btnApplyObjSettings->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnBtnApplyObjChanges ), NULL, this );
	m_btnLoadSprite->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnBtnLoadSprite ), NULL, this );
	m_choiceSize->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjTypeDialogBase::OnVariableSizeChanged ), NULL, this );
	m_btnApplyVarSettings->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnBtnApplyVarChanges ), NULL, this );
	m_btnImport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnBtnImport ), NULL, this );
	m_btnExport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnBtnExport ), NULL, this );

}

GameObjParamsPanelBase::GameObjParamsPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer31;
	fgSizer31 = new wxFlexGridSizer( 3, 1, 0, 0 );
	fgSizer31->AddGrowableCol( 0 );
	fgSizer31->AddGrowableRow( 2 );
	fgSizer31->SetFlexibleDirection( wxBOTH );
	fgSizer31->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_toolBar1 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	m_toolAddVariable = m_toolBar1->AddTool( wxID_ANY, wxT("Add Variable"), wxBitmap( add_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolRemoveVariable = m_toolBar1->AddTool( wxID_ANY, wxT("Remove Variable"), wxBitmap( remove_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolBar1->Realize();

	fgSizer31->Add( m_toolBar1, 0, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer30;
	fgSizer30 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer30->AddGrowableCol( 1 );
	fgSizer30->SetFlexibleDirection( wxBOTH );
	fgSizer30->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText42 = new wxStaticText( this, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	fgSizer30->Add( m_staticText42, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_textObjectName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_textObjectName, 0, wxALL|wxEXPAND, 5 );

	m_btnApplyObjectName = new wxButton( this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_btnApplyObjectName, 0, wxALL|wxEXPAND, 5 );

	m_staticText63 = new wxStaticText( this, wxID_ANY, wxT("Archetype:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText63->Wrap( -1 );
	fgSizer30->Add( m_staticText63, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxArrayString m_choiceArchetypesChoices;
	m_choiceArchetypes = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceArchetypesChoices, 0 );
	m_choiceArchetypes->SetSelection( 0 );
	fgSizer30->Add( m_choiceArchetypes, 0, wxALL|wxEXPAND, 5 );

	m_btnNewArchetype = new wxButton( this, wxID_ANY, wxT("New Archetype..."), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_btnNewArchetype, 0, wxALL, 5 );

	m_staticText631 = new wxStaticText( this, wxID_ANY, wxT("Sprite Actor:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText631->Wrap( -1 );
	fgSizer30->Add( m_staticText631, 0, wxALL, 5 );

	wxArrayString m_choiceSpriteActorChoices;
	m_choiceSpriteActor = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceSpriteActorChoices, 0 );
	m_choiceSpriteActor->SetSelection( 0 );
	fgSizer30->Add( m_choiceSpriteActor, 0, wxALL|wxEXPAND, 5 );


	fgSizer30->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText641 = new wxStaticText( this, wxID_ANY, wxT("Sprite Sheet:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText641->Wrap( -1 );
	fgSizer30->Add( m_staticText641, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxArrayString m_choiceSpriteSheetChoices;
	m_choiceSpriteSheet = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceSpriteSheetChoices, 0 );
	m_choiceSpriteSheet->SetSelection( 0 );
	fgSizer30->Add( m_choiceSpriteSheet, 0, wxALL|wxEXPAND, 5 );


	fgSizer30->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText64 = new wxStaticText( this, wxID_ANY, wxT("Sprite Anim:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText64->Wrap( -1 );
	fgSizer30->Add( m_staticText64, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxArrayString m_choiceSpriteAnimChoices;
	m_choiceSpriteAnim = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceSpriteAnimChoices, 0 );
	m_choiceSpriteAnim->SetSelection( 0 );
	fgSizer30->Add( m_choiceSpriteAnim, 0, wxALL|wxEXPAND, 5 );


	fgSizer31->Add( fgSizer30, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer51;
	fgSizer51 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer51->AddGrowableCol( 0 );
	fgSizer51->AddGrowableCol( 1 );
	fgSizer51->AddGrowableRow( 0 );
	fgSizer51->SetFlexibleDirection( wxBOTH );
	fgSizer51->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_listVariables = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxLC_REPORT );
	fgSizer51->Add( m_listVariables, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Variable:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer3->Add( m_staticText6, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_textVariableName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	fgSizer3->Add( m_textVariableName, 0, wxALL|wxEXPAND, 5 );

	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer3->Add( m_staticText7, 0, wxALIGN_RIGHT|wxALL, 5 );

	wxString m_choiceSizeChoices[] = { wxT("Byte"), wxT("Word"), wxT("Longword") };
	int m_choiceSizeNChoices = sizeof( m_choiceSizeChoices ) / sizeof( wxString );
	m_choiceSize = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceSizeNChoices, m_choiceSizeChoices, 0 );
	m_choiceSize->SetSelection( 0 );
	fgSizer3->Add( m_choiceSize, 0, wxALL, 5 );

	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Value:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer3->Add( m_staticText8, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_textValue = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_textValue, 0, wxALL|wxEXPAND, 5 );

	m_staticText81 = new wxStaticText( this, wxID_ANY, wxT("Value:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText81->Wrap( -1 );
	fgSizer3->Add( m_staticText81, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxArrayString m_choiceValueChoices;
	m_choiceValue = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceValueChoices, 0 );
	m_choiceValue->SetSelection( 0 );
	fgSizer3->Add( m_choiceValue, 0, wxALL|wxEXPAND, 5 );


	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer51->Add( fgSizer3, 1, wxEXPAND, 5 );


	fgSizer31->Add( fgSizer51, 1, wxEXPAND, 5 );


	bSizer10->Add( fgSizer31, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer10 );
	this->Layout();

	// Connect Events
	this->Connect( m_toolAddVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnToolVariableAdd ) );
	this->Connect( m_toolRemoveVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnToolVariableRemove ) );
	m_btnApplyObjectName->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnButtonApplyObjectName ), NULL, this );
	m_choiceArchetypes->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjParamsPanelBase::OnSelectArchetype ), NULL, this );
	m_btnNewArchetype->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnButtonNewArchetype ), NULL, this );
	m_choiceSpriteActor->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjParamsPanelBase::OnSelectSpriteActor ), NULL, this );
	m_choiceSpriteSheet->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjParamsPanelBase::OnSelectSpriteSheet ), NULL, this );
	m_choiceSpriteAnim->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjParamsPanelBase::OnSelectSpriteAnim ), NULL, this );
	m_listVariables->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( GameObjParamsPanelBase::OnSelectVariable ), NULL, this );
	m_textVariableName->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GameObjParamsPanelBase::OnEnterTextVariable ), NULL, this );
	m_choiceSize->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjParamsPanelBase::OnVariableSizeChanged ), NULL, this );
	m_textValue->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GameObjParamsPanelBase::OnEnterTextValue ), NULL, this );
	m_choiceValue->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjParamsPanelBase::OnSelectValue ), NULL, this );
}

GameObjParamsPanelBase::~GameObjParamsPanelBase()
{
	// Disconnect Events
	this->Disconnect( m_toolAddVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnToolVariableAdd ) );
	this->Disconnect( m_toolRemoveVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnToolVariableRemove ) );
	m_btnApplyObjectName->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnButtonApplyObjectName ), NULL, this );
	m_choiceArchetypes->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjParamsPanelBase::OnSelectArchetype ), NULL, this );
	m_btnNewArchetype->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnButtonNewArchetype ), NULL, this );
	m_choiceSpriteActor->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjParamsPanelBase::OnSelectSpriteActor ), NULL, this );
	m_choiceSpriteSheet->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjParamsPanelBase::OnSelectSpriteSheet ), NULL, this );
	m_choiceSpriteAnim->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjParamsPanelBase::OnSelectSpriteAnim ), NULL, this );
	m_listVariables->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( GameObjParamsPanelBase::OnSelectVariable ), NULL, this );
	m_textVariableName->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GameObjParamsPanelBase::OnEnterTextVariable ), NULL, this );
	m_choiceSize->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjParamsPanelBase::OnVariableSizeChanged ), NULL, this );
	m_textValue->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GameObjParamsPanelBase::OnEnterTextValue ), NULL, this );
	m_choiceValue->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjParamsPanelBase::OnSelectValue ), NULL, this );

}

TimelinePanelBase::TimelinePanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxFlexGridSizer* fgSizer24;
	fgSizer24 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer24->AddGrowableCol( 0 );
	fgSizer24->AddGrowableRow( 1 );
	fgSizer24->SetFlexibleDirection( wxBOTH );
	fgSizer24->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_toolBarAnimation = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	wxArrayString m_choiceAnimsChoices;
	m_choiceAnims = new wxChoice( m_toolBarAnimation, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceAnimsChoices, 0 );
	m_choiceAnims->SetSelection( 0 );
	m_toolBarAnimation->AddControl( m_choiceAnims );
	m_toolAddAnim = m_toolBarAnimation->AddTool( wxID_ANY, wxT("tool"), wxBitmap( add_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("New Animation"), wxEmptyString, NULL );

	m_toolDeleteAnim = m_toolBarAnimation->AddTool( wxID_ANY, wxT("tool"), wxBitmap( remove_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Delete Animation"), wxEmptyString, NULL );

	m_toolBarAnimation->AddSeparator();

	m_toolKeyframeReplace = m_toolBarAnimation->AddTool( wxID_ANY, wxT("Keyframe Actor"), wxBitmap( keyall_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Overwrite current keyframe"), wxEmptyString, NULL );

	m_toolKeyframeInsert = m_toolBarAnimation->AddTool( wxID_ANY, wxT("Keyframe Actor"), wxBitmap( key_insert_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Insert new keyframe at current time"), wxEmptyString, NULL );

	m_toolKeyframeEnd = m_toolBarAnimation->AddTool( wxID_ANY, wxT("Keyframe"), wxBitmap( key_end_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Add new keyframe at end"), wxEmptyString, NULL );

	m_toolKeyframeDelete = m_toolBarAnimation->AddTool( wxID_ANY, wxT("Keyframe"), wxBitmap( key_delete_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Delete keyframe at current time"), wxEmptyString, NULL );

	m_toolToggleLoop = m_toolBarAnimation->AddTool( wxID_ANY, wxT("Loop"), wxBitmap( loop_16_16_xpm ), wxNullBitmap, wxITEM_CHECK, wxT("Looping"), wxEmptyString, NULL );

	m_toolBarAnimation->AddSeparator();

	m_toolPlay = m_toolBarAnimation->AddTool( wxID_ANY, wxT("tool"), wxBitmap( play_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Play"), wxEmptyString, NULL );

	m_toolStop = m_toolBarAnimation->AddTool( wxID_ANY, wxT("tool"), wxBitmap( stop_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Stop"), wxEmptyString, NULL );

	m_toolStepLeft = m_toolBarAnimation->AddTool( wxID_ANY, wxT("Rewind"), wxBitmap( stepleft_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Rewind"), wxEmptyString, NULL );

	m_toolStepRight = m_toolBarAnimation->AddTool( wxID_ANY, wxT("Rewind"), wxBitmap( stepright_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Rewind"), wxEmptyString, NULL );

	m_toolRewind = m_toolBarAnimation->AddTool( wxID_ANY, wxT("Rewind"), wxBitmap( rewind_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Rewind"), wxEmptyString, NULL );

	m_toolFastForward = m_toolBarAnimation->AddTool( wxID_ANY, wxT("Fast Forward"), wxBitmap( fastforward_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Fast Forward"), wxEmptyString, NULL );

	m_toolBarAnimation->AddSeparator();

	m_toolIsolateObject = m_toolBarAnimation->AddTool( wxID_ANY, wxT("Isolate"), wxBitmap( obj_16_16_xpm ), wxNullBitmap, wxITEM_CHECK, wxT("Isolate Object"), wxEmptyString, NULL );

	m_staticText34 = new wxStaticText( m_toolBarAnimation, wxID_ANY, wxT("Speed:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34->Wrap( -1 );
	m_toolBarAnimation->AddControl( m_staticText34 );
	m_spinSpeed = new wxSpinCtrl( m_toolBarAnimation, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000, 0 );
	m_toolBarAnimation->AddControl( m_spinSpeed );
	m_toolBarAnimation->AddSeparator();

	m_chkExportLocalSpace = new wxCheckBox( m_toolBarAnimation, wxID_ANY, wxT("Export Local Space"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toolBarAnimation->AddControl( m_chkExportLocalSpace );
	m_toolBarAnimation->Realize();

	fgSizer24->Add( m_toolBarAnimation, 0, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer42;
	fgSizer42 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer42->AddGrowableRow( 1 );
	fgSizer42->SetFlexibleDirection( wxBOTH );
	fgSizer42->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* bSizer59;
	bSizer59 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText491 = new wxStaticText( this, wxID_ANY, wxT("Actor:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText491->Wrap( -1 );
	bSizer59->Add( m_staticText491, 0, wxALL, 5 );

	wxArrayString m_choiceActorChoices;
	m_choiceActor = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), m_choiceActorChoices, 0 );
	m_choiceActor->SetSelection( 0 );
	bSizer59->Add( m_choiceActor, 0, wxALL, 5 );

	m_textCurrentActor = new wxStaticText( this, wxID_ANY, wxT("[No actor selected]"), wxDefaultPosition, wxSize( 300,-1 ), 0 );
	m_textCurrentActor->Wrap( -1 );
	bSizer59->Add( m_textCurrentActor, 0, wxALL, 5 );

	m_staticText49 = new wxStaticText( this, wxID_ANY, wxT("Sprite animation:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText49->Wrap( -1 );
	bSizer59->Add( m_staticText49, 0, wxALL, 5 );

	wxArrayString m_choiceSpriteAnimChoices;
	m_choiceSpriteAnim = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), m_choiceSpriteAnimChoices, 0 );
	m_choiceSpriteAnim->SetSelection( 0 );
	bSizer59->Add( m_choiceSpriteAnim, 0, wxALL, 5 );


	fgSizer42->Add( bSizer59, 1, wxEXPAND, 5 );

	m_timeline = new KeyframePanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_timeline->SetScrollRate( 5, 5 );
	fgSizer42->Add( m_timeline, 1, wxEXPAND | wxALL, 5 );


	fgSizer24->Add( fgSizer42, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( fgSizer24 );
	this->Layout();

	// Connect Events
	m_choiceAnims->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TimelinePanelBase::OnSelectAnimation ), NULL, this );
	this->Connect( m_toolAddAnim->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolAddAnim ) );
	this->Connect( m_toolDeleteAnim->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolDeleteAnim ) );
	this->Connect( m_toolKeyframeReplace->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolKeyframeReplace ) );
	this->Connect( m_toolKeyframeInsert->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolKeyframeInsert ) );
	this->Connect( m_toolKeyframeEnd->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolKeyframeEnd ) );
	this->Connect( m_toolKeyframeDelete->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolKeyframeDelete ) );
	this->Connect( m_toolToggleLoop->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolLoopToggle ) );
	this->Connect( m_toolPlay->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolPlay ) );
	this->Connect( m_toolStop->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolStop ) );
	this->Connect( m_toolStepLeft->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolStepLeft ) );
	this->Connect( m_toolStepRight->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolStepRight ) );
	this->Connect( m_toolRewind->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolRewind ) );
	this->Connect( m_toolFastForward->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolFastForward ) );
	this->Connect( m_toolIsolateObject->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolIsolateObject ) );
	m_spinSpeed->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TimelinePanelBase::OnSpinSpeed ), NULL, this );
	m_chkExportLocalSpace->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnChkExportLocalSpace ), NULL, this );
	m_choiceActor->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TimelinePanelBase::OnSelectActor ), NULL, this );
	m_choiceSpriteAnim->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TimelinePanelBase::OnSelectSpriteAnim ), NULL, this );
}

TimelinePanelBase::~TimelinePanelBase()
{
	// Disconnect Events
	m_choiceAnims->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TimelinePanelBase::OnSelectAnimation ), NULL, this );
	this->Disconnect( m_toolAddAnim->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolAddAnim ) );
	this->Disconnect( m_toolDeleteAnim->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolDeleteAnim ) );
	this->Disconnect( m_toolKeyframeReplace->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolKeyframeReplace ) );
	this->Disconnect( m_toolKeyframeInsert->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolKeyframeInsert ) );
	this->Disconnect( m_toolKeyframeEnd->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolKeyframeEnd ) );
	this->Disconnect( m_toolKeyframeDelete->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolKeyframeDelete ) );
	this->Disconnect( m_toolToggleLoop->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolLoopToggle ) );
	this->Disconnect( m_toolPlay->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolPlay ) );
	this->Disconnect( m_toolStop->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolStop ) );
	this->Disconnect( m_toolStepLeft->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolStepLeft ) );
	this->Disconnect( m_toolStepRight->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolStepRight ) );
	this->Disconnect( m_toolRewind->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolRewind ) );
	this->Disconnect( m_toolFastForward->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolFastForward ) );
	this->Disconnect( m_toolIsolateObject->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolIsolateObject ) );
	m_spinSpeed->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TimelinePanelBase::OnSpinSpeed ), NULL, this );
	m_chkExportLocalSpace->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnChkExportLocalSpace ), NULL, this );
	m_choiceActor->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TimelinePanelBase::OnSelectActor ), NULL, this );
	m_choiceSpriteAnim->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TimelinePanelBase::OnSelectSpriteAnim ), NULL, this );

}

DialogNewObjectTypeBase::DialogNewObjectTypeBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* fgSizer43;
	fgSizer43 = new wxFlexGridSizer( 3, 1, 0, 0 );
	fgSizer43->AddGrowableRow( 2 );
	fgSizer43->SetFlexibleDirection( wxBOTH );
	fgSizer43->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer48;
	fgSizer48 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer48->AddGrowableCol( 1 );
	fgSizer48->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer48->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText61 = new wxStaticText( this, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	fgSizer48->Add( m_staticText61, 0, wxALL, 5 );

	m_textName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer48->Add( m_textName, 0, wxALL|wxEXPAND, 5 );


	fgSizer43->Add( fgSizer48, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer50;
	fgSizer50 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer50->AddGrowableCol( 1 );
	fgSizer50->SetFlexibleDirection( wxBOTH );
	fgSizer50->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText62 = new wxStaticText( this, wxID_ANY, wxT("Sprite actor:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText62->Wrap( -1 );
	fgSizer50->Add( m_staticText62, 0, wxALL, 5 );

	wxArrayString m_choiceActorChoices;
	m_choiceActor = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceActorChoices, 0 );
	m_choiceActor->SetSelection( 0 );
	fgSizer50->Add( m_choiceActor, 0, wxALL|wxEXPAND, 5 );


	fgSizer43->Add( fgSizer50, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer47;
	fgSizer47 = new wxFlexGridSizer( 1, 3, 0, 0 );
	fgSizer47->AddGrowableCol( 0 );
	fgSizer47->AddGrowableCol( 2 );
	fgSizer47->AddGrowableRow( 0 );
	fgSizer47->SetFlexibleDirection( wxBOTH );
	fgSizer47->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer44;
	fgSizer44 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer44->AddGrowableRow( 1 );
	fgSizer44->SetFlexibleDirection( wxBOTH );
	fgSizer44->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText59 = new wxStaticText( this, wxID_ANY, wxT("Auto Variables - Not Added"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText59->Wrap( -1 );
	fgSizer44->Add( m_staticText59, 0, wxALL|wxEXPAND, 5 );

	m_listVarsNotAdded = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	fgSizer44->Add( m_listVarsNotAdded, 0, wxALL|wxEXPAND, 5 );


	fgSizer47->Add( fgSizer44, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer45;
	fgSizer45 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer45->SetFlexibleDirection( wxBOTH );
	fgSizer45->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_btnAddVar = new wxButton( this, wxID_ANY, wxT(">"), wxDefaultPosition, wxSize( 20,-1 ), 0 );
	fgSizer45->Add( m_btnAddVar, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_btnRemoveVar = new wxButton( this, wxID_ANY, wxT("<"), wxDefaultPosition, wxSize( 20,-1 ), 0 );
	fgSizer45->Add( m_btnRemoveVar, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	fgSizer47->Add( fgSizer45, 1, wxALIGN_CENTER_VERTICAL, 5 );

	wxFlexGridSizer* fgSizer46;
	fgSizer46 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer46->AddGrowableRow( 1 );
	fgSizer46->SetFlexibleDirection( wxBOTH );
	fgSizer46->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText60 = new wxStaticText( this, wxID_ANY, wxT("Auto Variables - Added"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText60->Wrap( -1 );
	fgSizer46->Add( m_staticText60, 0, wxALL|wxEXPAND, 5 );

	m_listVarsAdded = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	fgSizer46->Add( m_listVarsAdded, 0, wxALL|wxEXPAND, 5 );


	fgSizer47->Add( fgSizer46, 1, wxEXPAND, 5 );


	fgSizer43->Add( fgSizer47, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer49;
	fgSizer49 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer49->AddGrowableCol( 0 );
	fgSizer49->SetFlexibleDirection( wxBOTH );
	fgSizer49->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_sdbSizer8 = new wxStdDialogButtonSizer();
	m_sdbSizer8OK = new wxButton( this, wxID_OK );
	m_sdbSizer8->AddButton( m_sdbSizer8OK );
	m_sdbSizer8Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer8->AddButton( m_sdbSizer8Cancel );
	m_sdbSizer8->Realize();

	fgSizer49->Add( m_sdbSizer8, 1, wxEXPAND, 5 );


	fgSizer43->Add( fgSizer49, 1, wxALIGN_RIGHT|wxEXPAND, 5 );


	this->SetSizer( fgSizer43 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_choiceActor->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DialogNewObjectTypeBase::OnChoiceActor ), NULL, this );
	m_btnAddVar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogNewObjectTypeBase::OnBtnAddVar ), NULL, this );
	m_btnRemoveVar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogNewObjectTypeBase::OnBtnRemoveVar ), NULL, this );
}

DialogNewObjectTypeBase::~DialogNewObjectTypeBase()
{
	// Disconnect Events
	m_choiceActor->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DialogNewObjectTypeBase::OnChoiceActor ), NULL, this );
	m_btnAddVar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogNewObjectTypeBase::OnBtnAddVar ), NULL, this );
	m_btnRemoveVar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogNewObjectTypeBase::OnBtnRemoveVar ), NULL, this );

}

SpriteAnimPanelBase::SpriteAnimPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxFlexGridSizer* fgSizer27;
	fgSizer27 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer27->AddGrowableCol( 0 );
	fgSizer27->AddGrowableCol( 1 );
	fgSizer27->SetFlexibleDirection( wxBOTH );
	fgSizer27->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_listSpriteAnims = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0|wxVSCROLL );
	fgSizer27->Add( m_listSpriteAnims, 0, wxALL|wxEXPAND, 5 );

	m_sliderTimeline = new wxSlider( this, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer27->Add( m_sliderTimeline, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( fgSizer27 );
	this->Layout();
}

SpriteAnimPanelBase::~SpriteAnimPanelBase()
{
}

ExportDialogMapTab::ExportDialogMapTab( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxFlexGridSizer* fgSizer34;
	fgSizer34 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer34->AddGrowableCol( 1 );
	fgSizer34->SetFlexibleDirection( wxBOTH );
	fgSizer34->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_chkMap = new wxCheckBox( this, wxID_ANY, wxT("Export tile map:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer34->Add( m_chkMap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerMap = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer34->Add( m_filePickerMap, 0, wxALL|wxEXPAND, 5 );

	m_chkBlockMap = new wxCheckBox( this, wxID_ANY, wxT("Export block map:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer34->Add( m_chkBlockMap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerBlockMap = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer34->Add( m_filePickerBlockMap, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	m_chkStampMap = new wxCheckBox( this, wxID_ANY, wxT("Export stamp map:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer34->Add( m_chkStampMap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerStampMap = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer34->Add( m_filePickerStampMap, 0, wxALL|wxEXPAND, 5 );

	m_chkCollisionMap = new wxCheckBox( this, wxID_ANY, wxT("Export terrain map:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer34->Add( m_chkCollisionMap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerCollisionMap = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer34->Add( m_filePickerCollisionMap, 0, wxALL|wxEXPAND, 5 );

	m_chkCollisionBlockMap = new wxCheckBox( this, wxID_ANY, wxT("Export terrain block map:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer34->Add( m_chkCollisionBlockMap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerCollisionBlockMap = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer34->Add( m_filePickerCollisionBlockMap, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	m_chkSceneAnims = new wxCheckBox( this, wxID_ANY, wxT("Export scene animations:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer34->Add( m_chkSceneAnims, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerSceneAnimations = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer34->Add( m_filePickerSceneAnimations, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_chkGameObj = new wxCheckBox( this, wxID_ANY, wxT("Export game objects:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer34->Add( m_chkGameObj, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_filePickerGameObj = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm;*.bee"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer34->Add( m_filePickerGameObj, 0, wxALL|wxEXPAND, 5 );


	fgSizer34->Add( 0, 0, 1, wxEXPAND, 5 );


	this->SetSizer( fgSizer34 );
	this->Layout();
	fgSizer34->Fit( this );
}

ExportDialogMapTab::~ExportDialogMapTab()
{
}

SceneExplorerPanelBase::SceneExplorerPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer69;
	bSizer69 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer62;
	fgSizer62 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer62->AddGrowableCol( 0 );
	fgSizer62->AddGrowableRow( 0 );
	fgSizer62->SetFlexibleDirection( wxBOTH );
	fgSizer62->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_tree = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_EDIT_LABELS );
	fgSizer62->Add( m_tree, 0, wxALL|wxEXPAND, 5 );


	bSizer69->Add( fgSizer62, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer69 );
	this->Layout();

	// Connect Events
	m_tree->Connect( wxEVT_COMMAND_TREE_END_DRAG, wxTreeEventHandler( SceneExplorerPanelBase::OnItemDragged ), NULL, this );
	m_tree->Connect( wxEVT_COMMAND_TREE_END_LABEL_EDIT, wxTreeEventHandler( SceneExplorerPanelBase::OnItemRenamed ), NULL, this );
	m_tree->Connect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( SceneExplorerPanelBase::OnItemActivated ), NULL, this );
	m_tree->Connect( wxEVT_COMMAND_TREE_ITEM_MENU, wxTreeEventHandler( SceneExplorerPanelBase::OnItemContextMenu ), NULL, this );
	m_tree->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( SceneExplorerPanelBase::OnItemSelected ), NULL, this );
}

SceneExplorerPanelBase::~SceneExplorerPanelBase()
{
	// Disconnect Events
	m_tree->Disconnect( wxEVT_COMMAND_TREE_END_DRAG, wxTreeEventHandler( SceneExplorerPanelBase::OnItemDragged ), NULL, this );
	m_tree->Disconnect( wxEVT_COMMAND_TREE_END_LABEL_EDIT, wxTreeEventHandler( SceneExplorerPanelBase::OnItemRenamed ), NULL, this );
	m_tree->Disconnect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( SceneExplorerPanelBase::OnItemActivated ), NULL, this );
	m_tree->Disconnect( wxEVT_COMMAND_TREE_ITEM_MENU, wxTreeEventHandler( SceneExplorerPanelBase::OnItemContextMenu ), NULL, this );
	m_tree->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( SceneExplorerPanelBase::OnItemSelected ), NULL, this );

}

PropertyPanelBase::PropertyPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxFlexGridSizer* fgSizer63;
	fgSizer63 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer63->AddGrowableCol( 0 );
	fgSizer63->AddGrowableRow( 0 );
	fgSizer63->SetFlexibleDirection( wxBOTH );
	fgSizer63->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_propertyGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	fgSizer63->Add( m_propertyGrid, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( fgSizer63 );
	this->Layout();

	// Connect Events
	m_propertyGrid->Connect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( PropertyPanelBase::OnPropertyChanged ), NULL, this );
	m_propertyGrid->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( PropertyPanelBase::OnRightClick ), NULL, this );
}

PropertyPanelBase::~PropertyPanelBase()
{
	// Disconnect Events
	m_propertyGrid->Disconnect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( PropertyPanelBase::OnPropertyChanged ), NULL, this );
	m_propertyGrid->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( PropertyPanelBase::OnRightClick ), NULL, this );

}

ScriptCompilePanelBase::ScriptCompilePanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxFlexGridSizer* fgSizer63;
	fgSizer63 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer63->AddGrowableCol( 0 );
	fgSizer63->AddGrowableRow( 0 );
	fgSizer63->SetFlexibleDirection( wxBOTH );
	fgSizer63->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_textOutput = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	fgSizer63->Add( m_textOutput, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( fgSizer63 );
	this->Layout();
}

ScriptCompilePanelBase::~ScriptCompilePanelBase()
{
}
