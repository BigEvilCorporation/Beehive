///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "UIBase.h"

#include "icons/arrow_plain_blue_NE_SE_SW_NW.bmp.h"
#include "icons/arrow_plain_green_E.bmp.h"
#include "icons/file_yellow_empty.bmp.h"
#include "icons/floppy_35inch_blue.bmp.h"
#include "icons/minus_red.bmp.h"
#include "icons/plus_green.bmp.h"
#include "icons/tables.bmp.h"
#include "icons/window.bmp.h"

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
	m_ribbonButtonBarProject->AddButton( wxID_BTN_PROJ_NEW, wxT("New"), file_yellow_empty_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonButtonBarProject->AddButton( wxID_BTN_PROJ_OPEN, wxT("Open"), floppy_35inch_blue_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonButtonBarProject->AddButton( wxID_BTN_PROJ_SAVEAS, wxT("Save as..."), floppy_35inch_blue_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonButtonBarProject->AddButton( wxID_BTN_PROJ_SAVE, wxT("Save"), floppy_35inch_blue_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonButtonBarProject->AddButton( wxID_BTN_PROJ_EXPORT, wxT("Export"), arrow_plain_green_E_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonPageView = new wxRibbonPage( m_ribbonBar, wxID_ANY, wxT("View") , wxNullBitmap , 0 );
	m_ribbonPanelView = new wxRibbonPanel( m_ribbonPageView, wxID_ANY, wxT("Grid") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarGrid = new wxRibbonButtonBar( m_ribbonPanelView, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarGrid->AddToggleButton( wxID_BTN_GRID_SHOW, wxT("Show Grid"), tables_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonButtonBarGrid->AddToggleButton( wxID_BTN_GRID_SNAP, wxT("Snap to Grid"), tables_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonPageTools = new wxRibbonPage( m_ribbonBar, wxID_ANY, wxT("Tools") , wxNullBitmap , 0 );
	m_ribbonPanelTools = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Tool Panels") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarTools = new wxRibbonButtonBar( m_ribbonPanelTools, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_MAPEDIT, wxT("Map Editing"), window_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_TILES, wxT("Tiles"), window_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_ANY, wxT("Stamps"), window_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonButtonBarTools->AddButton( wxID_BTN_TOOLS_PALETTES, wxT("Palettes"), window_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonPanelMap = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Map") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarMap = new wxRibbonButtonBar( m_ribbonPanelMap, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarMap->AddButton( wxID_ANY, wxT("Resize"), arrow_plain_blue_NE_SE_SW_NW_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonPanelTiles = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Tiles") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarTiles = new wxRibbonButtonBar( m_ribbonPanelTiles, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarTiles->AddButton( wxID_BTN_TILES_IMPORT, wxT("Import"), plus_green_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonButtonBarTiles->AddButton( wxID_ANY, wxT("Delete"), minus_red_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonPanelPalettes = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Pal") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarPalettes = new wxRibbonButtonBar( m_ribbonPanelPalettes, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarPalettes->AddButton( wxID_ANY, wxT("Import"), plus_green_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonPanelStamps = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Stamps") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarStamps = new wxRibbonButtonBar( m_ribbonPanelStamps, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarStamps->AddButton( wxID_ANY, wxT("Create"), plus_green_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonButtonBarStamps->AddButton( wxID_ANY, wxT("Delete"), minus_red_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonPanelCollision = new wxRibbonPanel( m_ribbonPageTools, wxID_ANY, wxT("Collision") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBarCollision = new wxRibbonButtonBar( m_ribbonPanelCollision, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBarCollision->AddButton( wxID_ANY, wxT("Configuration"), tables_bmp_to_wx_bitmap(), wxEmptyString);
	m_ribbonButtonBarCollision->AddButton( wxID_ANY, wxT("Collision Tile Editor"), tables_bmp_to_wx_bitmap(), wxEmptyString);
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
	this->Connect( wxID_BTN_PROJ_SAVEAS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjSaveAs ) );
	this->Connect( wxID_BTN_PROJ_SAVE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjSave ) );
	this->Connect( wxID_BTN_PROJ_EXPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjExport ) );
	this->Connect( wxID_BTN_GRID_SHOW, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnGridShow ) );
	this->Connect( wxID_BTN_GRID_SNAP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnGridSnap ) );
	this->Connect( wxID_BTN_TOOLS_MAPEDIT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsMapEdit ) );
	this->Connect( wxID_BTN_TOOLS_TILES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsTiles ) );
	this->Connect( wxID_ANY, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsStamps ) );
	this->Connect( wxID_BTN_TOOLS_PALETTES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsPalettes ) );
	this->Connect( wxID_BTN_TILES_IMPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesImport ) );
	this->Connect( wxID_ANY, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesDelete ) );
}

MainWindowBase::~MainWindowBase()
{
	// Disconnect Events
	this->Disconnect( wxID_BTN_PROJ_NEW, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjNew ) );
	this->Disconnect( wxID_BTN_PROJ_OPEN, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjOpen ) );
	this->Disconnect( wxID_BTN_PROJ_SAVEAS, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjSaveAs ) );
	this->Disconnect( wxID_BTN_PROJ_SAVE, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjSave ) );
	this->Disconnect( wxID_BTN_PROJ_EXPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnProjExport ) );
	this->Disconnect( wxID_BTN_GRID_SHOW, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnGridShow ) );
	this->Disconnect( wxID_BTN_GRID_SNAP, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxCommandEventHandler( MainWindowBase::OnBtnGridSnap ) );
	this->Disconnect( wxID_BTN_TOOLS_MAPEDIT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsMapEdit ) );
	this->Disconnect( wxID_BTN_TOOLS_TILES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsTiles ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsStamps ) );
	this->Disconnect( wxID_BTN_TOOLS_PALETTES, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnToolsPalettes ) );
	this->Disconnect( wxID_BTN_TILES_IMPORT, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesImport ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxRibbonButtonBarEventHandler( MainWindowBase::OnBtnTilesDelete ) );
	
}

MapToolbox::MapToolbox( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_toolSelect = new wxBitmapButton( this, wxID_TOOL_SELECT, window_bmp_to_wx_bitmap(), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolSelect->SetToolTip( wxT("Select Tile(s)") );
	
	fgSizer1->Add( m_toolSelect, 0, wxALL, 5 );
	
	m_toolPaint = new wxBitmapButton( this, wxID_TOOL_PAINT, window_bmp_to_wx_bitmap(), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolPaint->SetToolTip( wxT("Paint Tile") );
	
	fgSizer1->Add( m_toolPaint, 0, wxALL, 5 );
	
	m_toolPicker = new wxBitmapButton( this, wxID_TOOL_PICKER, window_bmp_to_wx_bitmap(), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolPicker->SetToolTip( wxT("Pick Tile") );
	
	fgSizer1->Add( m_toolPicker, 0, wxALL, 5 );
	
	m_toolFlipX = new wxBitmapButton( this, wxID_TOOL_FLIPX, window_bmp_to_wx_bitmap(), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolFlipX->SetToolTip( wxT("Flip Horizontal") );
	
	fgSizer1->Add( m_toolFlipX, 0, wxALL, 5 );
	
	m_toolFlipY = new wxBitmapButton( this, wxID_TOOL_FLIPY, window_bmp_to_wx_bitmap(), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolFlipY->SetToolTip( wxT("Flip Vertical") );
	
	fgSizer1->Add( m_toolFlipY, 0, wxALL, 5 );
	
	m_toolFill = new wxBitmapButton( this, wxID_TOOL_FILL, window_bmp_to_wx_bitmap(), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_toolFill->SetToolTip( wxT("Fill Selection") );
	
	fgSizer1->Add( m_toolFill, 0, wxALL, 5 );
	
	
	this->SetSizer( fgSizer1 );
	this->Layout();
}

MapToolbox::~MapToolbox()
{
}
