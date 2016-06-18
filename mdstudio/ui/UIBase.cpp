///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "SpriteCanvas.h"

#include "UIBase.h"

#include "../FormBuilderProj/add_16_16.xpm"
#include "../FormBuilderProj/clearmap.xpm"
#include "../FormBuilderProj/deletestamp.xpm"
#include "../FormBuilderProj/deletetile.xpm"
#include "../FormBuilderProj/deleteunusedtiles.xpm"
#include "../FormBuilderProj/gameobj.xpm"
#include "../FormBuilderProj/gameobjpanel.xpm"
#include "../FormBuilderProj/genterrain_beziers.xpm"
#include "../FormBuilderProj/grid.xpm"
#include "../FormBuilderProj/gridsnap.xpm"
#include "../FormBuilderProj/importtiles.xpm"
#include "../FormBuilderProj/key_16_16.xpm"
#include "../FormBuilderProj/keyall_16_16.xpm"
#include "../FormBuilderProj/megadrive2.xpm"
#include "../FormBuilderProj/newproj.xpm"
#include "../FormBuilderProj/newstamp.xpm"
#include "../FormBuilderProj/newtile.xpm"
#include "../FormBuilderProj/obj_16_16.xpm"
#include "../FormBuilderProj/open.xpm"
#include "../FormBuilderProj/palettespanel.xpm"
#include "../FormBuilderProj/play_16_16.xpm"
#include "../FormBuilderProj/remove_16_16.xpm"
#include "../FormBuilderProj/resizemap.xpm"
#include "../FormBuilderProj/rewind_16_16.xpm"
#include "../FormBuilderProj/save.xpm"
#include "../FormBuilderProj/showoutlines.xpm"
#include "../FormBuilderProj/stampspanel.xpm"
#include "../FormBuilderProj/stop_16_16.xpm"
#include "../FormBuilderProj/tilespanel.xpm"
#include "../FormBuilderProj/tool_addterrainbezier.xpm"
#include "../FormBuilderProj/tool_clone.xpm"
#include "../FormBuilderProj/tool_createstamp.xpm"
#include "../FormBuilderProj/tool_deletegameobj.xpm"
#include "../FormBuilderProj/tool_deletestamp.xpm"
#include "../FormBuilderProj/tool_deleteterraintile.xpm"
#include "../FormBuilderProj/tool_editterrainbezier.xpm"
#include "../FormBuilderProj/tool_fill.xpm"
#include "../FormBuilderProj/tool_flipx.xpm"
#include "../FormBuilderProj/tool_flipy.xpm"
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

///////////////////////////////////////////////////////////////////////////

MainWindowBase::MainWindowBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_ribbonBar = new wxRibbonBar( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxRIBBON_BAR_DEFAULT_STYLE|wxRIBBON_BAR_FLOW_HORIZONTAL|wxRIBBON_BAR_SHOW_PAGE_ICONS|wxRIBBON_BAR_SHOW_PAGE_LABELS|wxRIBBON_BAR_SHOW_PANEL_MINIMISE_BUTTONS|wxNO_BORDER );
	m_ribbonBar->SetArtProvider(new wxRibbonDefaultArtProvider); 
	m_ribbonPageProject = new wxRibbonPage( m_ribbonBar, wxID_ANY, wxT("Project") , wxNullBitmap , 0 );
	m_ribbonPanelProject = new wxRibbonPanel( m_ribbonPageProject, wxID_ANY, wxT("Project") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarProject = new wxRibbonButtonBar( m_ribbonPanelProject, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarProject->AddButton( wxID_BTN_PROJ_NEW, wxT("New"), wxBitmap( newproj_xpm ), wxEmptyString);
	m_ribbonButtonBarProject->AddButton( wxID_BTN_PROJ_OPEN, wxT("Open"), wxBitmap( open_xpm ), wxEmptyString);
	m_ribbonButtonBarProject->AddButton( wxID_BTN_PROJ_SAVE, wxT("Save"), wxBitmap( save_xpm ), wxEmptyString);
	m_ribbonButtonBarProject->AddButton( wxID_BTN_PROJ_EXPORT, wxT("Export"), wxBitmap( megadrive2_xpm ), wxEmptyString);
	m_ribbonPageView = new wxRibbonPage( m_ribbonBar, wxID_ANY, wxT("View") , wxNullBitmap , 0 );
	m_ribbonPanelView = new wxRibbonPanel( m_ribbonPageView, wxID_ANY, wxT("Grid") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarGrid = new wxRibbonButtonBar( m_ribbonPanelView, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarGrid->AddToggleButton( wxID_BTN_GRID_SHOW, wxT("Show Grid"), wxBitmap( grid_xpm ), wxEmptyString);
	m_ribbonButtonBarGrid->AddToggleButton( wxID_BTN_GRID_SNAP, wxT("Snap to Grid"), wxBitmap( gridsnap_xpm ), wxEmptyString);
	m_ribbonButtonBarGrid->AddToggleButton( wxID_BTN_SHOW_OUTLINES, wxT("Show Outlines"), wxBitmap( showoutlines_xpm ), wxEmptyString);
	m_ribbonPageTools = new wxRibbonPage( m_ribbonBar, wxID_ANY, wxT("Tools") , wxNullBitmap , 0 );
	m_ribbonBar->SetActivePage( m_ribbonPageTools ); 
	m_ribbonPanelTools = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Tool Panels") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarTools = new wxRibbonButtonBar( m_ribbonPanelTools, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_MAPEDIT, wxT("Toolbox"), wxBitmap( toolspanel_xpm ), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_TILES, wxT("Tiles"), wxBitmap( tilespanel_xpm ), wxEmptyString);
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
	m_ribbonButtonBarStamps->AddButton( wxID_BTN_STAMPS_CREATE, wxT("Create"), wxBitmap( newstamp_xpm ), wxEmptyString);
	m_ribbonButtonBarStamps->AddButton( wxID_BTN_STAMPS_DELETE, wxT("Delete"), wxBitmap( deletestamp_xpm ), wxEmptyString);
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
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxID_BTN_PROJ_NEW, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjNew ) );
	this->Connect( wxID_BTN_PROJ_OPEN, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjOpen ) );
	this->Connect( wxID_BTN_PROJ_SAVE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjSave ) );
	this->Connect( wxID_BTN_PROJ_EXPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjExport ) );
	this->Connect( wxID_BTN_GRID_SHOW, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnGridShow ) );
	this->Connect( wxID_BTN_GRID_SNAP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnGridSnap ) );
	this->Connect( wxID_BTN_SHOW_OUTLINES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnShowOutlines ) );
	this->Connect( wxID_BTN_TOOLS_MAPEDIT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsMapEdit ) );
	this->Connect( wxID_BTN_TOOLS_TILES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsTiles ) );
	this->Connect( wxID_BTN_TOOLS_STAMPS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsStamps ) );
	this->Connect( wxID_BTN_TOOLS_PALETTES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsPalettes ) );
	this->Connect( wxID_BTN_TOOLS_GAMEOBJS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsGameObjs ) );
	this->Connect( wxID_BTN_TOOLS_GAMEOBJPARAMS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsGameObjParams ) );
	this->Connect( wxID_BTN_TOOLS_TIMELINE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsTimeline ) );
	this->Connect( wxID_BTN_TOOLS_SPRITEANIM, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsSpriteAnim ) );
	this->Connect( wxID_BTN_MAP_CLEAR, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnMapClear ) );
	this->Connect( wxID_BTN_MAP_RESIZE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnMapResize ) );
	this->Connect( wxID_BTN_TILES_IMPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesImport ) );
	this->Connect( wxID_BTN_TILES_CREATE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesCreate ) );
	this->Connect( wxID_BTN_TILES_DELETE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesDelete ) );
	this->Connect( wxID_BTN_TILES_CLEANUP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesCleanup ) );
	this->Connect( wxID_BTN_SPRITE_EDITOR, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnSpriteEditor ) );
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
	this->Disconnect( wxID_BTN_PROJ_EXPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjExport ) );
	this->Disconnect( wxID_BTN_GRID_SHOW, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnGridShow ) );
	this->Disconnect( wxID_BTN_GRID_SNAP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnGridSnap ) );
	this->Disconnect( wxID_BTN_SHOW_OUTLINES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnShowOutlines ) );
	this->Disconnect( wxID_BTN_TOOLS_MAPEDIT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsMapEdit ) );
	this->Disconnect( wxID_BTN_TOOLS_TILES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsTiles ) );
	this->Disconnect( wxID_BTN_TOOLS_STAMPS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsStamps ) );
	this->Disconnect( wxID_BTN_TOOLS_PALETTES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsPalettes ) );
	this->Disconnect( wxID_BTN_TOOLS_GAMEOBJS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsGameObjs ) );
	this->Disconnect( wxID_BTN_TOOLS_GAMEOBJPARAMS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsGameObjParams ) );
	this->Disconnect( wxID_BTN_TOOLS_TIMELINE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsTimeline ) );
	this->Disconnect( wxID_BTN_TOOLS_SPRITEANIM, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsSpriteAnim ) );
	this->Disconnect( wxID_BTN_MAP_CLEAR, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnMapClear ) );
	this->Disconnect( wxID_BTN_MAP_RESIZE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnMapResize ) );
	this->Disconnect( wxID_BTN_TILES_IMPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesImport ) );
	this->Disconnect( wxID_BTN_TILES_CREATE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesCreate ) );
	this->Disconnect( wxID_BTN_TILES_DELETE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesDelete ) );
	this->Disconnect( wxID_BTN_TILES_CLEANUP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesCleanup ) );
	this->Disconnect( wxID_BTN_SPRITE_EDITOR, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnSpriteEditor ) );
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
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOk = new wxButton( this, wxID_BTN_CANCEL, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_buttonOk, 0, wxALIGN_RIGHT, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_BTN_CANCEL, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_buttonCancel, 0, wxALIGN_RIGHT, 5 );
	
	
	bSizer2->Add( bSizer4, 1, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( bSizer2 );
	this->Layout();
	bSizer2->Fit( this );
	
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
	fgSizer28->Add( m_text31, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_spinCtrlMapHeight = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 64 );
	fgSizer28->Add( m_spinCtrlMapHeight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
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
	bSizer2->Fit( this );
	
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
	bSizer3->Add( m_text1, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	m_spinCtrlGranularity = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100000, 1000 );
	bSizer3->Add( m_spinCtrlGranularity, 0, wxALL, 5 );
	
	
	bSizer2->Add( bSizer3, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOk = new wxButton( this, wxID_BTN_CANCEL, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_buttonOk, 0, wxALIGN_RIGHT, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_BTN_CANCEL, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_buttonCancel, 0, wxALIGN_RIGHT, 5 );
	
	
	bSizer2->Add( bSizer4, 1, wxALIGN_RIGHT, 5 );
	
	
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

MapToolbox::MapToolbox( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Tile Tools"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer16->Add( m_staticText11, 0, wxALIGN_CENTER, 5 );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_toolSelectTile = new wxBitmapButton( this, wxID_TOOL_SELECTTILE, wxBitmap( tool_select_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolSelectTile->SetToolTip( wxT("Select Tile(s)") );
	
	fgSizer1->Add( m_toolSelectTile, 0, wxALL, 5 );
	
	m_toolPaint = new wxBitmapButton( this, wxID_TOOL_PAINT, wxBitmap( tool_painttile_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolPaint->SetToolTip( wxT("Paint Tile") );
	
	fgSizer1->Add( m_toolPaint, 0, wxALL, 5 );
	
	m_toolTilePicker = new wxBitmapButton( this, wxID_TOOL_TILEPICKER, wxBitmap( tool_picker_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolTilePicker->SetToolTip( wxT("Pick Tile") );
	
	fgSizer1->Add( m_toolTilePicker, 0, wxALL, 5 );
	
	m_toolFlipX = new wxBitmapButton( this, wxID_TOOL_FLIPX, wxBitmap( tool_flipx_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolFlipX->SetToolTip( wxT("Flip Horizontal") );
	
	fgSizer1->Add( m_toolFlipX, 0, wxALL, 5 );
	
	m_toolFlipY = new wxBitmapButton( this, wxID_TOOL_FLIPY, wxBitmap( tool_flipy_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolFlipY->SetToolTip( wxT("Flip Vertical") );
	
	fgSizer1->Add( m_toolFlipY, 0, wxALL, 5 );
	
	m_toolFill = new wxBitmapButton( this, wxID_TOOL_FILL, wxBitmap( tool_fill_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolFill->SetToolTip( wxT("Fill Selection") );
	
	fgSizer1->Add( m_toolFill, 0, wxALL, 5 );
	
	m_toolClone = new wxBitmapButton( this, wxID_TOOL_CLONE, wxBitmap( tool_clone_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolClone->SetToolTip( wxT("Clone Selection") );
	
	fgSizer1->Add( m_toolClone, 0, wxALL, 5 );
	
	
	bSizer16->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	m_staticText12 = new wxStaticText( this, wxID_ANY, wxT("Stamp Tools"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer16->Add( m_staticText12, 0, wxALIGN_CENTER, 5 );
	
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_toolSelectStamp = new wxBitmapButton( this, wxID_TOOL_SELECTSTAMP, wxBitmap( tool_selectstamp_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolSelectStamp->SetToolTip( wxT("Select Stamp") );
	
	fgSizer7->Add( m_toolSelectStamp, 0, wxALL, 5 );
	
	m_toolPaintStamp = new wxBitmapButton( this, wxID_TOOL_STAMP, wxBitmap( tool_paintstamp_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolPaintStamp->SetToolTip( wxT("Paint Stamp") );
	
	fgSizer7->Add( m_toolPaintStamp, 0, wxALL, 5 );
	
	m_toolStampPicker = new wxBitmapButton( this, wxID_TOOL_STAMPPICKER, wxBitmap( tool_stamppicker_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolStampPicker->SetToolTip( wxT("Pick Stamp") );
	
	fgSizer7->Add( m_toolStampPicker, 0, wxALL, 5 );
	
	m_toolCreateStamp = new wxBitmapButton( this, wxID_TOOL_CREATESTAMP, wxBitmap( tool_createstamp_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolCreateStamp->SetToolTip( wxT("Create stamp from selection") );
	
	fgSizer7->Add( m_toolCreateStamp, 0, wxALL, 5 );
	
	m_toolRemoveStamp = new wxBitmapButton( this, wxID_TOOL_REMOVESTAMP, wxBitmap( tool_deletestamp_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolRemoveStamp->SetToolTip( wxT("Remove Stamp") );
	
	fgSizer7->Add( m_toolRemoveStamp, 0, wxALL, 5 );
	
	
	bSizer16->Add( fgSizer7, 1, wxEXPAND, 5 );
	
	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("Collision Tools"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer16->Add( m_staticText13, 0, wxALIGN_CENTER, 5 );
	
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_toolPaintCollisionPixel = new wxBitmapButton( this, wxID_TOOL_COL_PAINTTERRAIN, wxBitmap( tool_paintcolpixel_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolPaintCollisionPixel->SetToolTip( wxT("Paint Terrain (direct to map)") );
	
	fgSizer8->Add( m_toolPaintCollisionPixel, 0, wxALL, 5 );
	
	m_toolPaintCollisionSolid = new wxBitmapButton( this, wxID_TOOL_COL_PAINTSOLID, wxBitmap( tool_paintcolwall_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolPaintCollisionSolid->SetToolTip( wxT("Paint Solid Wall/Ceiling") );
	
	fgSizer8->Add( m_toolPaintCollisionSolid, 0, wxALL, 5 );
	
	m_toolDeleteTerrainTile = new wxBitmapButton( this, wxID_TOOL_COL_DELETETERRTILE, wxBitmap( tool_deleteterraintile_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolDeleteTerrainTile->SetToolTip( wxT("Delete Terrain Tile") );
	
	fgSizer8->Add( m_toolDeleteTerrainTile, 0, wxALL, 5 );
	
	m_toolAddTerrainBezier = new wxBitmapButton( this, wxID_TOOL_COL_ADDTERRAINBEZIER, wxBitmap( tool_addterrainbezier_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolAddTerrainBezier->SetToolTip( wxT("Add Terrain Bezier") );
	
	fgSizer8->Add( m_toolAddTerrainBezier, 0, wxALL, 5 );
	
	m_toolEditTerrainBezier1 = new wxBitmapButton( this, wxID_TOOL_COL_EDITTERRAINBEZIER, wxBitmap( tool_editterrainbezier_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolEditTerrainBezier1->SetToolTip( wxT("Edit Terrain Bezier") );
	
	fgSizer8->Add( m_toolEditTerrainBezier1, 0, wxALL, 5 );
	
	
	bSizer16->Add( fgSizer8, 1, wxEXPAND, 5 );
	
	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("Game Object Tools"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer16->Add( m_staticText14, 0, wxALIGN_CENTER, 5 );
	
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_toolSelectGameObject = new wxBitmapButton( this, wxID_TOOL_SELECTGAMEOBJ, wxBitmap( tool_selectgameobj_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolSelectGameObject->SetToolTip( wxT("Select Game Object") );
	
	fgSizer9->Add( m_toolSelectGameObject, 0, wxALL, 5 );
	
	m_toolPlaceGameObject = new wxBitmapButton( this, wxID_TOOL_PLACEGAMEOBJ, wxBitmap( tool_placegameobj_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolPlaceGameObject->SetToolTip( wxT("Place Game Object") );
	
	fgSizer9->Add( m_toolPlaceGameObject, 0, wxALL, 5 );
	
	m_toolMoveGameObject = new wxBitmapButton( this, wxID_TOOL_MOVEGAMEOBJ, wxBitmap( tool_selectgameobj_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolMoveGameObject->SetToolTip( wxT("Move Game Object") );
	
	fgSizer9->Add( m_toolMoveGameObject, 0, wxALL, 5 );
	
	m_toolAnimateGameObject = new wxBitmapButton( this, wxID_TOOL_ANIMATEGAMEOBJ, wxBitmap( tool_selectgameobj_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolAnimateGameObject->SetToolTip( wxT("Animate Game Object") );
	
	fgSizer9->Add( m_toolAnimateGameObject, 0, wxALL, 5 );
	
	m_toolRemoveGameObject = new wxBitmapButton( this, wxID_TOOL_REMOVEGAMEOBJ, wxBitmap( tool_deletegameobj_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolRemoveGameObject->SetToolTip( wxT("Remove Game Object") );
	
	fgSizer9->Add( m_toolRemoveGameObject, 0, wxALL, 5 );
	
	
	bSizer16->Add( fgSizer9, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer16 );
	this->Layout();
}

MapToolbox::~MapToolbox()
{
}

GameObjTypesPanelBase::GameObjTypesPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer13;
	fgSizer13 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer13->AddGrowableCol( 0 );
	fgSizer13->AddGrowableRow( 0 );
	fgSizer13->SetFlexibleDirection( wxBOTH );
	fgSizer13->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_listGameObjTypes = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer13->Add( m_listGameObjTypes, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer13 );
	this->Layout();
	
	// Connect Events
	m_listGameObjTypes->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GameObjTypesPanelBase::OnGameObjectTypeSelected ), NULL, this );
}

GameObjTypesPanelBase::~GameObjTypesPanelBase()
{
	// Disconnect Events
	m_listGameObjTypes->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GameObjTypesPanelBase::OnGameObjectTypeSelected ), NULL, this );
	
}

ExportDialog::ExportDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_static1 = new wxStaticText( this, wxID_ANY, wxT("Project name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_static1->Wrap( -1 );
	fgSizer3->Add( m_static1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_txtProjectName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_txtProjectName, 0, wxALL|wxEXPAND, 5 );
	
	m_chkPalettes = new wxCheckBox( this, wxID_ANY, wxT("Export map palettes:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkPalettes->SetValue(true); 
	fgSizer3->Add( m_chkPalettes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_filePickerPalettes = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer3->Add( m_filePickerPalettes, 0, wxALL|wxEXPAND, 5 );
	
	m_chkTileset = new wxCheckBox( this, wxID_ANY, wxT("Export map tileset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkTileset->SetValue(true); 
	fgSizer3->Add( m_chkTileset, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_filePickerTileset = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer3->Add( m_filePickerTileset, 0, wxALL|wxEXPAND, 5 );
	
	m_chkMap = new wxCheckBox( this, wxID_ANY, wxT("Export map:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkMap->SetValue(true); 
	fgSizer3->Add( m_chkMap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_filePickerMap = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer3->Add( m_filePickerMap, 0, wxALL|wxEXPAND, 5 );
	
	m_chkTerrainTiles = new wxCheckBox( this, wxID_ANY, wxT("Export terrain tiles:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkTerrainTiles->SetValue(true); 
	fgSizer3->Add( m_chkTerrainTiles, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_filePickerTerrainTiles = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer3->Add( m_filePickerTerrainTiles, 0, wxALL|wxEXPAND, 5 );
	
	m_chkCollisionMap = new wxCheckBox( this, wxID_ANY, wxT("Export collision map:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkCollisionMap->SetValue(true); 
	fgSizer3->Add( m_chkCollisionMap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_filePickerCollisionMap = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer3->Add( m_filePickerCollisionMap, 0, wxALL|wxEXPAND, 5 );
	
	m_chkGameObj = new wxCheckBox( this, wxID_ANY, wxT("Export game objects:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkGameObj->SetValue(true); 
	fgSizer3->Add( m_chkGameObj, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_filePickerGameObj = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer3->Add( m_filePickerGameObj, 0, wxALL|wxEXPAND, 5 );
	
	m_chkSpriteSheets = new wxCheckBox( this, wxID_ANY, wxT("Export sprite sheets:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkSpriteSheets->SetValue(true); 
	fgSizer3->Add( m_chkSpriteSheets, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_dirPickerSpriteSheets = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select sprites directory"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer3->Add( m_dirPickerSpriteSheets, 0, wxALL|wxEXPAND, 5 );
	
	m_chkSpritePalettes = new wxCheckBox( this, wxID_ANY, wxT("Export sprite palettes:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkSpritePalettes->SetValue(true); 
	fgSizer3->Add( m_chkSpritePalettes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_dirPickerSpritePalettes = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select sprites directory"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer3->Add( m_dirPickerSpritePalettes, 0, wxALL|wxEXPAND, 5 );
	
	m_chkSpriteAnims = new wxCheckBox( this, wxID_ANY, wxT("Export sprite animations:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkSpriteAnims->SetValue(true); 
	fgSizer3->Add( m_chkSpriteAnims, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_dirPickerSpriteAnims = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select sprites directory"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer3->Add( m_dirPickerSpriteAnims, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btnText = new wxRadioButton( this, wxID_ANY, wxT("Text"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_btnText->SetValue( true ); 
	bSizer16->Add( m_btnText, 0, wxALL, 5 );
	
	m_btnBinary = new wxRadioButton( this, wxID_ANY, wxT("Binary"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_btnBinary, 0, wxALL, 5 );
	
	
	fgSizer3->Add( bSizer16, 1, wxEXPAND, 5 );
	
	m_buttons = new wxStdDialogButtonSizer();
	m_buttonsOK = new wxButton( this, wxID_OK );
	m_buttons->AddButton( m_buttonsOK );
	m_buttonsCancel = new wxButton( this, wxID_CANCEL );
	m_buttons->AddButton( m_buttonsCancel );
	m_buttons->Realize();
	
	fgSizer3->Add( m_buttons, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttonsCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExportDialog::OnBtnCancel ), NULL, this );
	m_buttonsOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExportDialog::OnBtnOk ), NULL, this );
}

ExportDialog::~ExportDialog()
{
	// Disconnect Events
	m_buttonsCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExportDialog::OnBtnCancel ), NULL, this );
	m_buttonsOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ExportDialog::OnBtnOk ), NULL, this );
	
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
	
	m_chkImportToStamp = new wxCheckBox( this, wxID_ANY, wxT("Import as new stamp"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_chkImportToStamp, 0, wxALL, 5 );
	
	
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
	m_textName->SetMaxLength( 64 ); 
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
	
	m_staticText36 = new wxStaticText( this, wxID_ANY, wxT("Actors/props:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText36->Wrap( -1 );
	fgSizer25->Add( m_staticText36, 0, wxALL, 5 );
	
	m_staticText38 = new wxStaticText( this, wxID_ANY, wxT("Sprite Sheets:"), wxDefaultPosition, wxDefaultSize, 0 );
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
	
	m_btnActorExport = new wxButton( this, wxID_ANY, wxT("Export..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer35->Add( m_btnActorExport, 0, wxALL, 5 );
	
	
	fgSizer25->Add( bSizer35, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btnImportSpriteSheet = new wxButton( this, wxID_ANY, wxT("Import..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer26->Add( m_btnImportSpriteSheet, 0, wxALL, 5 );
	
	m_btnDeleteSprite = new wxButton( this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer26->Add( m_btnDeleteSprite, 0, wxALL, 5 );
	
	
	fgSizer25->Add( bSizer26, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btnNewAnim = new wxButton( this, wxID_ANY, wxT("New..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer27->Add( m_btnNewAnim, 0, wxALL, 5 );
	
	m_btnDeleteAnim = new wxButton( this, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer27->Add( m_btnDeleteAnim, 0, wxALL, 5 );
	
	
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
	m_gridSpriteFrames->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gridSpriteFrames->SetRowSize( 0, 64 );
	m_gridSpriteFrames->EnableDragRowSize( false );
	m_gridSpriteFrames->SetRowLabelSize( 0 );
	m_gridSpriteFrames->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
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
	m_gridTimeline->EnableEditing( false );
	m_gridTimeline->EnableGridLines( true );
	m_gridTimeline->EnableDragGridSize( false );
	m_gridTimeline->SetMargins( 0, 0 );
	
	// Columns
	m_gridTimeline->SetColSize( 0, 128 );
	m_gridTimeline->EnableDragColMove( false );
	m_gridTimeline->EnableDragColSize( false );
	m_gridTimeline->SetColLabelSize( 0 );
	m_gridTimeline->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gridTimeline->SetRowSize( 0, 64 );
	m_gridTimeline->EnableDragRowSize( false );
	m_gridTimeline->SetRowLabelSize( 0 );
	m_gridTimeline->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gridTimeline->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	m_sizerTimeline->Add( m_gridTimeline, 0, wxALL|wxEXPAND, 5 );
	
	m_sliderTimeline = new wxSlider( this, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	m_sizerTimeline->Add( m_sliderTimeline, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btnPlay = new wxBitmapButton( this, wxID_ANY, wxBitmap( play_16_16_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer28->Add( m_btnPlay, 0, wxALL, 5 );
	
	m_btnStop = new wxBitmapButton( this, wxID_ANY, wxBitmap( stop_16_16_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer28->Add( m_btnStop, 0, wxALL, 5 );
	
	m_staticText40 = new wxStaticText( this, wxID_ANY, wxT("Speed:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText40->Wrap( -1 );
	bSizer28->Add( m_staticText40, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_spinCtrlSpeed = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 10 );
	bSizer28->Add( m_spinCtrlSpeed, 0, wxALL, 5 );
	
	
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
	m_btnActorExport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnActorExport ), NULL, this );
	m_btnImportSpriteSheet->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnSpriteSheetImport ), NULL, this );
	m_btnDeleteSprite->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnSpriteSheetDelete ), NULL, this );
	m_btnNewAnim->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnAnimNew ), NULL, this );
	m_btnDeleteAnim->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnAnimDelete ), NULL, this );
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
}

SpriteAnimEditorDialogBase::~SpriteAnimEditorDialogBase()
{
	// Disconnect Events
	m_listActors->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnActorSelected ), NULL, this );
	m_listSpriteSheets->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnSpriteSheetSelected ), NULL, this );
	m_listAnimations->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnAnimSelected ), NULL, this );
	m_btnNewActor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnActorNew ), NULL, this );
	m_btnDeleteActor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnActorDelete ), NULL, this );
	m_btnActorExport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnActorExport ), NULL, this );
	m_btnImportSpriteSheet->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnSpriteSheetImport ), NULL, this );
	m_btnDeleteSprite->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnSpriteSheetDelete ), NULL, this );
	m_btnNewAnim->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnAnimNew ), NULL, this );
	m_btnDeleteAnim->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpriteAnimEditorDialogBase::OnBtnAnimDelete ), NULL, this );
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
	
}

GameObjTypeDialogBase::GameObjTypeDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	m_toolBar1 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL ); 
	m_toolAddGameObjType = m_toolBar1->AddTool( wxID_ANY, wxT("Add Game Object Type"), wxBitmap( add_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_toolRemoveGameObjType = m_toolBar1->AddTool( wxID_ANY, wxT("Remove Game Object Type"), wxBitmap( remove_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
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
	
	m_staticText25 = new wxStaticText( this, wxID_ANY, wxT("Preview Sprite:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	fgSizer5->Add( m_staticText25, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	wxArrayString m_choiceSpritesChoices;
	m_choiceSprites = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceSpritesChoices, 0 );
	m_choiceSprites->SetSelection( 0 );
	m_choiceSprites->Enable( false );
	
	fgSizer5->Add( m_choiceSprites, 0, wxALL, 5 );
	
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
	
	
	fgSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnApplyObjSettings = new wxButton( this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnApplyObjSettings->Enable( false );
	
	fgSizer5->Add( m_btnApplyObjSettings, 0, wxALL, 5 );
	
	
	fgSizer4->Add( fgSizer5, 1, wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer4->Add( m_staticline2, 0, wxALL|wxEXPAND, 5 );
	
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
	this->Connect( m_toolAddVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolVariableAdd ) );
	this->Connect( m_toolRemoveVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolVariableRemove ) );
	m_listGameObjTypes->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GameObjTypeDialogBase::OnSelectGameObjType ), NULL, this );
	m_listVariables->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( GameObjTypeDialogBase::OnSelectVariable ), NULL, this );
	m_spinWidth->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( GameObjTypeDialogBase::OnWidthChanged ), NULL, this );
	m_spinHeight->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( GameObjTypeDialogBase::OnHeightChanged ), NULL, this );
	m_btnApplyObjSettings->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnBtnApplyObjChanges ), NULL, this );
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
	this->Disconnect( m_toolAddVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolVariableAdd ) );
	this->Disconnect( m_toolRemoveVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnToolVariableRemove ) );
	m_listGameObjTypes->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GameObjTypeDialogBase::OnSelectGameObjType ), NULL, this );
	m_listVariables->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( GameObjTypeDialogBase::OnSelectVariable ), NULL, this );
	m_spinWidth->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( GameObjTypeDialogBase::OnWidthChanged ), NULL, this );
	m_spinHeight->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( GameObjTypeDialogBase::OnHeightChanged ), NULL, this );
	m_btnApplyObjSettings->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnBtnApplyObjChanges ), NULL, this );
	m_choiceSize->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjTypeDialogBase::OnVariableSizeChanged ), NULL, this );
	m_btnApplyVarSettings->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnBtnApplyVarChanges ), NULL, this );
	m_btnImport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnBtnImport ), NULL, this );
	m_btnExport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjTypeDialogBase::OnBtnExport ), NULL, this );
	
}

GameObjParamsPanelBase::GameObjParamsPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer31;
	fgSizer31 = new wxFlexGridSizer( 3, 1, 0, 0 );
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
	fgSizer30->Add( m_staticText42, 0, wxALL, 5 );
	
	m_textObjectName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_textObjectName, 0, wxALL|wxEXPAND, 5 );
	
	m_btnApplyObjectName = new wxButton( this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_btnApplyObjectName, 0, wxALL, 5 );
	
	
	fgSizer31->Add( fgSizer30, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_listVariables = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxLC_REPORT );
	bSizer11->Add( m_listVariables, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Variable:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer3->Add( m_staticText6, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_textVariableName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 300,-1 ), 0 );
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
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnApplyVarParams = new wxButton( this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnApplyVarParams->SetDefault(); 
	fgSizer3->Add( m_btnApplyVarParams, 0, wxALL, 5 );
	
	
	bSizer11->Add( fgSizer3, 1, wxEXPAND, 5 );
	
	
	fgSizer31->Add( bSizer11, 1, wxEXPAND, 5 );
	
	
	bSizer10->Add( fgSizer31, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer10 );
	this->Layout();
	
	// Connect Events
	this->Connect( m_toolAddVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnToolVariableAdd ) );
	this->Connect( m_toolRemoveVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnToolVariableRemove ) );
	m_btnApplyObjectName->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnButtonApplyObjectName ), NULL, this );
	m_listVariables->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( GameObjParamsPanelBase::OnSelectVariable ), NULL, this );
	m_choiceSize->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjParamsPanelBase::OnVariableSizeChanged ), NULL, this );
	m_btnApplyVarParams->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnBtnApplyVariableChanges ), NULL, this );
}

GameObjParamsPanelBase::~GameObjParamsPanelBase()
{
	// Disconnect Events
	this->Disconnect( m_toolAddVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnToolVariableAdd ) );
	this->Disconnect( m_toolRemoveVariable->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnToolVariableRemove ) );
	m_btnApplyObjectName->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnButtonApplyObjectName ), NULL, this );
	m_listVariables->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( GameObjParamsPanelBase::OnSelectVariable ), NULL, this );
	m_choiceSize->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GameObjParamsPanelBase::OnVariableSizeChanged ), NULL, this );
	m_btnApplyVarParams->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameObjParamsPanelBase::OnBtnApplyVariableChanges ), NULL, this );
	
}

TimelinePanelBase::TimelinePanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer24;
	fgSizer24 = new wxFlexGridSizer( 2, 1, 0, 0 );
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
	
	m_toolKeyframeActor = m_toolBarAnimation->AddTool( wxID_ANY, wxT("Keyframe Actor"), wxBitmap( key_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Keyframe Actor"), wxEmptyString, NULL ); 
	
	m_toolKeyframeAll = m_toolBarAnimation->AddTool( wxID_ANY, wxT("Keyframe"), wxBitmap( keyall_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Keyframe All"), wxEmptyString, NULL ); 
	
	m_toolBarAnimation->AddSeparator(); 
	
	m_toolPlay = m_toolBarAnimation->AddTool( wxID_ANY, wxT("tool"), wxBitmap( play_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Play"), wxEmptyString, NULL ); 
	
	m_toolStop = m_toolBarAnimation->AddTool( wxID_ANY, wxT("tool"), wxBitmap( stop_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Stop"), wxEmptyString, NULL ); 
	
	m_toolRewind = m_toolBarAnimation->AddTool( wxID_ANY, wxT("tool"), wxBitmap( rewind_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Rewind"), wxEmptyString, NULL ); 
	
	m_toolBarAnimation->AddSeparator(); 
	
	m_toolIsolateObject = m_toolBarAnimation->AddTool( wxID_ANY, wxT("tool"), wxBitmap( obj_16_16_xpm ), wxNullBitmap, wxITEM_CHECK, wxT("Isolate Object"), wxEmptyString, NULL ); 
	
	m_staticText34 = new wxStaticText( m_toolBarAnimation, wxID_ANY, wxT("Speed:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34->Wrap( -1 );
	m_toolBarAnimation->AddControl( m_staticText34 );
	m_spinSpeed = new wxSpinCtrl( m_toolBarAnimation, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000, 0 );
	m_toolBarAnimation->AddControl( m_spinSpeed );
	m_toolBarAnimation->Realize(); 
	
	fgSizer24->Add( m_toolBarAnimation, 0, wxEXPAND, 5 );
	
	m_textCurrentActor = new wxStaticText( this, wxID_ANY, wxT("[All keyframes]"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCurrentActor->Wrap( -1 );
	fgSizer24->Add( m_textCurrentActor, 0, wxALL, 5 );
	
	m_gridTimeline = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_gridTimeline->CreateGrid( 2, 2 );
	m_gridTimeline->EnableEditing( false );
	m_gridTimeline->EnableGridLines( true );
	m_gridTimeline->EnableDragGridSize( false );
	m_gridTimeline->SetMargins( 0, 0 );
	
	// Columns
	m_gridTimeline->EnableDragColMove( false );
	m_gridTimeline->EnableDragColSize( true );
	m_gridTimeline->SetColLabelSize( 30 );
	m_gridTimeline->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gridTimeline->EnableDragRowSize( true );
	m_gridTimeline->SetRowLabelSize( 80 );
	m_gridTimeline->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gridTimeline->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	fgSizer24->Add( m_gridTimeline, 0, wxALL|wxEXPAND, 5 );
	
	m_sliderTimeline = new wxSlider( this, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer24->Add( m_sliderTimeline, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer24 );
	this->Layout();
	
	// Connect Events
	m_choiceAnims->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TimelinePanelBase::OnSelectAnimation ), NULL, this );
	this->Connect( m_toolAddAnim->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolAddAnim ) );
	this->Connect( m_toolDeleteAnim->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolDeleteAnim ) );
	this->Connect( m_toolKeyframeActor->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolKeyframeActor ) );
	this->Connect( m_toolKeyframeAll->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolKeyframeAll ) );
	this->Connect( m_toolPlay->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolPlay ) );
	this->Connect( m_toolStop->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolStop ) );
	this->Connect( m_toolRewind->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolRewind ) );
	this->Connect( m_toolIsolateObject->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolIsolateObject ) );
	m_sliderTimeline->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
}

TimelinePanelBase::~TimelinePanelBase()
{
	// Disconnect Events
	m_choiceAnims->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TimelinePanelBase::OnSelectAnimation ), NULL, this );
	this->Disconnect( m_toolAddAnim->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolAddAnim ) );
	this->Disconnect( m_toolDeleteAnim->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolDeleteAnim ) );
	this->Disconnect( m_toolKeyframeActor->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolKeyframeActor ) );
	this->Disconnect( m_toolKeyframeAll->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolKeyframeAll ) );
	this->Disconnect( m_toolPlay->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolPlay ) );
	this->Disconnect( m_toolStop->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolStop ) );
	this->Disconnect( m_toolRewind->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolRewind ) );
	this->Disconnect( m_toolIsolateObject->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( TimelinePanelBase::OnToolIsolateObject ) );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	m_sliderTimeline->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TimelinePanelBase::OnSliderTimelineChange ), NULL, this );
	
}

SpriteAnimPanelBase::SpriteAnimPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
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
