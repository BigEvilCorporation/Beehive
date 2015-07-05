///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "UIBase.h"

#include "../FormBuilderProj/add_16_16.xpm"
#include "../FormBuilderProj/clearmap.xpm"
#include "../FormBuilderProj/collision.xpm"
#include "../FormBuilderProj/config.xpm"
#include "../FormBuilderProj/deletestamp.xpm"
#include "../FormBuilderProj/deletetile.xpm"
#include "../FormBuilderProj/grid.xpm"
#include "../FormBuilderProj/gridsnap.xpm"
#include "../FormBuilderProj/importtiles.xpm"
#include "../FormBuilderProj/megadrive2.xpm"
#include "../FormBuilderProj/newproj.xpm"
#include "../FormBuilderProj/newstamp.xpm"
#include "../FormBuilderProj/newtile.xpm"
#include "../FormBuilderProj/open.xpm"
#include "../FormBuilderProj/palettespanel.xpm"
#include "../FormBuilderProj/qmark_16_16.xpm"
#include "../FormBuilderProj/remove_16_16.xpm"
#include "../FormBuilderProj/resizemap.xpm"
#include "../FormBuilderProj/save.xpm"
#include "../FormBuilderProj/showoutlines.xpm"
#include "../FormBuilderProj/stampspanel.xpm"
#include "../FormBuilderProj/tilespanel.xpm"
#include "../FormBuilderProj/tool_clone.xpm"
#include "../FormBuilderProj/tool_createstamp.xpm"
#include "../FormBuilderProj/tool_deletestamp.xpm"
#include "../FormBuilderProj/tool_fill.xpm"
#include "../FormBuilderProj/tool_flipx.xpm"
#include "../FormBuilderProj/tool_flipy.xpm"
#include "../FormBuilderProj/tool_paintstamp.xpm"
#include "../FormBuilderProj/tool_painttile.xpm"
#include "../FormBuilderProj/tool_picker.xpm"
#include "../FormBuilderProj/tool_select.xpm"
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
	m_ribbonBar->SetActivePage( m_ribbonPageProject ); 
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
	m_ribbonPanelTools = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Tool Panels") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarTools = new wxRibbonButtonBar( m_ribbonPanelTools, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_MAPEDIT, wxT("Toolbox"), wxBitmap( toolspanel_xpm ), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_TILES, wxT("Tiles"), wxBitmap( tilespanel_xpm ), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_STAMPS, wxT("Stamps"), wxBitmap( stampspanel_xpm ), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_PALETTES, wxT("Palettes"), wxBitmap( palettespanel_xpm ), wxEmptyString);
	m_ribbonPanelMap = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Map") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarMap = new wxRibbonButtonBar( m_ribbonPanelMap, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarMap->AddButton( wxID_BTN_MAP_CLEAR, wxT("Clear Map"), wxBitmap( clearmap_xpm ), wxEmptyString);
	m_ribbonButtonBarMap->AddButton( wxID_BTN_MAP_RESIZE, wxT("Resize Map"), wxBitmap( resizemap_xpm ), wxEmptyString);
	m_ribbonPanelTiles = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Tiles") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarTiles = new wxRibbonButtonBar( m_ribbonPanelTiles, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarTiles->AddButton( wxID_BTN_TILES_IMPORT, wxT("Import"), wxBitmap( importtiles_xpm ), wxEmptyString);
	m_ribbonButtonBarTiles->AddButton( wxID_BTN_TILES_CREATE, wxT("New"), wxBitmap( newtile_xpm ), wxEmptyString);
	m_ribbonButtonBarTiles->AddButton( wxID_BTN_TILES_DELETE, wxT("Delete"), wxBitmap( deletetile_xpm ), wxEmptyString);
	m_ribbonPanelStamps = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Stamps") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarStamps = new wxRibbonButtonBar( m_ribbonPanelStamps, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarStamps->AddButton( wxID_BTN_STAMPS_CREATE, wxT("Create"), wxBitmap( newstamp_xpm ), wxEmptyString);
	m_ribbonButtonBarStamps->AddButton( wxID_BTN_STAMPS_DELETE, wxT("Delete"), wxBitmap( deletestamp_xpm ), wxEmptyString);
	m_ribbonPanelCollision = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Collision") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarCollision = new wxRibbonButtonBar( m_ribbonPanelCollision, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarCollision->AddButton( wxID_BTN_COLLISION_CONFIG, wxT("Configuration"), wxBitmap( config_xpm ), wxEmptyString);
	m_ribbonButtonBarCollision->AddButton( wxID_BTN_COLLISION_TILEEDIT, wxT("Collision Tile Editor"), wxBitmap( collision_xpm ), wxEmptyString);
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
	this->Connect( wxID_BTN_MAP_CLEAR, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnMapClear ) );
	this->Connect( wxID_BTN_MAP_RESIZE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnMapResize ) );
	this->Connect( wxID_BTN_TILES_IMPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesImport ) );
	this->Connect( wxID_BTN_TILES_CREATE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesCreate ) );
	this->Connect( wxID_BTN_TILES_DELETE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesDelete ) );
	this->Connect( wxID_BTN_COLLISION_CONFIG, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnCollisionConfig ) );
	this->Connect( wxID_BTN_COLLISION_TILEEDIT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnCollisionTileEdit ) );
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
	this->Disconnect( wxID_BTN_MAP_CLEAR, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnMapClear ) );
	this->Disconnect( wxID_BTN_MAP_RESIZE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnMapResize ) );
	this->Disconnect( wxID_BTN_TILES_IMPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesImport ) );
	this->Disconnect( wxID_BTN_TILES_CREATE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesCreate ) );
	this->Disconnect( wxID_BTN_TILES_DELETE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesDelete ) );
	this->Disconnect( wxID_BTN_COLLISION_CONFIG, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnCollisionConfig ) );
	this->Disconnect( wxID_BTN_COLLISION_TILEEDIT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnCollisionTileEdit ) );
	
}

CollisionTypeDialogBase::CollisionTypeDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_toolBar1 = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 
	m_toolAddCollisionType = m_toolBar1->AddTool( wxID_COL_TOOL_ADD, wxT("tool"), wxBitmap( add_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_toolRemoveCollisionType = m_toolBar1->AddTool( wxID_COL_TOOL_DEL, wxT("tool"), wxBitmap( remove_16_16_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_toolBar1->Realize(); 
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_listCollisionTypes = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON|wxLC_SINGLE_SEL );
	bSizer5->Add( m_listCollisionTypes, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Icon:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer2->Add( m_staticText6, 0, wxALL, 5 );
	
	m_buttonIcon = new wxBitmapButton( this, wxID_BTN_COL_IMG, wxBitmap( qmark_16_16_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	fgSizer2->Add( m_buttonIcon, 0, wxALL, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer2->Add( m_staticText4, 0, wxALL, 5 );
	
	m_textName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_textName, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Bit:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer2->Add( m_staticText5, 0, wxALL, 5 );
	
	m_spinBit = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 8, 1 );
	fgSizer2->Add( m_spinBit, 0, wxALL, 5 );
	
	
	bSizer6->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	
	bSizer5->Add( bSizer6, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer5 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( m_toolAddCollisionType->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( CollisionTypeDialogBase::OnAddType ) );
	this->Connect( m_toolRemoveCollisionType->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( CollisionTypeDialogBase::OnRemoveType ) );
	m_listCollisionTypes->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( CollisionTypeDialogBase::OnTypeSelected ), NULL, this );
	m_buttonIcon->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CollisionTypeDialogBase::OnIconChange ), NULL, this );
	m_textName->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CollisionTypeDialogBase::OnNameChange ), NULL, this );
	m_spinBit->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CollisionTypeDialogBase::OnBitChange ), NULL, this );
}

CollisionTypeDialogBase::~CollisionTypeDialogBase()
{
	// Disconnect Events
	this->Disconnect( m_toolAddCollisionType->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( CollisionTypeDialogBase::OnAddType ) );
	this->Disconnect( m_toolRemoveCollisionType->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( CollisionTypeDialogBase::OnRemoveType ) );
	m_listCollisionTypes->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( CollisionTypeDialogBase::OnTypeSelected ), NULL, this );
	m_buttonIcon->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CollisionTypeDialogBase::OnIconChange ), NULL, this );
	m_textName->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CollisionTypeDialogBase::OnNameChange ), NULL, this );
	m_spinBit->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CollisionTypeDialogBase::OnBitChange ), NULL, this );
	
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

MapToolbox::MapToolbox( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_toolSelectTile = new wxBitmapButton( this, wxID_TOOL_SELECTTILE, wxBitmap( tool_select_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolSelectTile->SetToolTip( wxT("Select Tile(s)") );
	
	fgSizer1->Add( m_toolSelectTile, 0, wxALL, 5 );
	
	m_toolSelectStamp = new wxBitmapButton( this, wxID_TOOL_SELECTSTAMP, wxBitmap( tool_selectstamp_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolSelectStamp->SetToolTip( wxT("Select Stamp") );
	
	fgSizer1->Add( m_toolSelectStamp, 0, wxALL, 5 );
	
	m_toolPaint = new wxBitmapButton( this, wxID_TOOL_PAINT, wxBitmap( tool_painttile_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolPaint->SetToolTip( wxT("Paint Tile") );
	
	fgSizer1->Add( m_toolPaint, 0, wxALL, 5 );
	
	m_toolStamp = new wxBitmapButton( this, wxID_TOOL_STAMP, wxBitmap( tool_paintstamp_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolStamp->SetToolTip( wxT("Paint Stamp") );
	
	fgSizer1->Add( m_toolStamp, 0, wxALL, 5 );
	
	m_toolTilePicker = new wxBitmapButton( this, wxID_TOOL_TILEPICKER, wxBitmap( tool_picker_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolTilePicker->SetToolTip( wxT("Pick Tile") );
	
	fgSizer1->Add( m_toolTilePicker, 0, wxALL, 5 );
	
	m_toolStampPicker = new wxBitmapButton( this, wxID_TOOL_STAMPPICKER, wxBitmap( tool_stamppicker_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolStampPicker->SetToolTip( wxT("Pick Stamp") );
	
	fgSizer1->Add( m_toolStampPicker, 0, wxALL, 5 );
	
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
	
	m_toolCreateStamp = new wxBitmapButton( this, wxID_TOOL_CREATESTAMP, wxBitmap( tool_createstamp_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolCreateStamp->SetToolTip( wxT("Create stamp from selection") );
	
	fgSizer1->Add( m_toolCreateStamp, 0, wxALL, 5 );
	
	m_toolRemoveStamp = new wxBitmapButton( this, wxID_TOOL_REMOVESTAMP, wxBitmap( tool_deletestamp_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolRemoveStamp->SetToolTip( wxT("Remove Stamp") );
	
	fgSizer1->Add( m_toolRemoveStamp, 0, wxALL, 5 );
	
	
	this->SetSizer( fgSizer1 );
	this->Layout();
}

MapToolbox::~MapToolbox()
{
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
	
	m_chkPalettes = new wxCheckBox( this, wxID_ANY, wxT("Export palettes:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkPalettes->SetValue(true); 
	fgSizer3->Add( m_chkPalettes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_filePickerPalettes = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer3->Add( m_filePickerPalettes, 0, wxALL|wxEXPAND, 5 );
	
	m_chkTileset = new wxCheckBox( this, wxID_ANY, wxT("Export tileset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkTileset->SetValue(true); 
	fgSizer3->Add( m_chkTileset, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_filePickerTileset = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer3->Add( m_filePickerTileset, 0, wxALL|wxEXPAND, 5 );
	
	m_chkMap = new wxCheckBox( this, wxID_ANY, wxT("Export map:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkMap->SetValue(true); 
	fgSizer3->Add( m_chkMap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_filePickerMap = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer3->Add( m_filePickerMap, 0, wxALL|wxEXPAND, 5 );
	
	m_chkCollisionTiles = new wxCheckBox( this, wxID_ANY, wxT("Export collision tiles:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkCollisionTiles->SetValue(true); 
	fgSizer3->Add( m_chkCollisionTiles, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_filePickerCollisionTiles = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer3->Add( m_filePickerCollisionTiles, 0, wxALL|wxEXPAND, 5 );
	
	m_chkCollisionMap = new wxCheckBox( this, wxID_ANY, wxT("Export collision map:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkCollisionMap->SetValue(true); 
	fgSizer3->Add( m_chkCollisionMap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_filePickerCollisionMap = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.asm"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	fgSizer3->Add( m_filePickerCollisionMap, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
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

ImportDialog::ImportDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Bitmap file:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer11->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_fileBitmap = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.bmp"), wxDefaultPosition, wxDefaultSize, wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL );
	bSizer11->Add( m_fileBitmap, 1, wxALL|wxEXPAND, 5 );
	
	
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
	
	m_chkPaintToMap = new wxCheckBox( this, wxID_ANY, wxT("Paint to map"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_chkPaintToMap, 0, wxALL, 5 );
	
	m_chkImportToStamp = new wxCheckBox( this, wxID_ANY, wxT("Import as new stamp"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_chkImportToStamp, 0, wxALL, 5 );
	
	m_chkImportToSprite = new wxCheckBox( this, wxID_ANY, wxT("Import as new sprite"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_chkImportToSprite, 0, wxALL, 5 );
	
	
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
}

ImportDialog::~ImportDialog()
{
}
