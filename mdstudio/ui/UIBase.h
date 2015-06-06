///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __UIBASE_H__
#define __UIBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/ribbon/buttonbar.h>
#include <wx/ribbon/panel.h>
#include <wx/ribbon/page.h>
#include <wx/ribbon/control.h>
#include <wx/ribbon/art.h>
#include <wx/ribbon/bar.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/toolbar.h>
#include <wx/listctrl.h>
#include <wx/stattext.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_MAINWINDOW 1000
#define wxID_BTN_PROJ_NEW 1001
#define wxID_BTN_PROJ_OPEN 1002
#define wxID_BTN_PROJ_SAVE 1003
#define wxID_BTN_PROJ_EXPORT 1004
#define wxID_BTN_GRID_SHOW 1005
#define wxID_BTN_GRID_SNAP 1006
#define wxID_BTN_SHOW_OUTLINES 1007
#define wxID_BTN_TOOLS_MAPEDIT 1008
#define wxID_BTN_TOOLS_TILES 1009
#define wxID_BTN_TOOLS_STAMPS 1010
#define wxID_BTN_TOOLS_PALETTES 1011
#define wxID_BTN_MAP_CLEAR 1012
#define wxID_BTN_MAP_RESIZE 1013
#define wxID_BTN_TILES_IMPORT 1014
#define wxID_BTN_TILES_CREATE 1015
#define wxID_BTN_TILES_DELETE 1016
#define wxID_BTN_STAMPS_CREATE 1017
#define wxID_BTN_STAMPS_DELETE 1018
#define wxID_BTN_COLLISION_CONFIG 1019
#define wxID_BTN_COLLISION_TILEEDIT 1020
#define wxID_COL_TOOL_ADD 1021
#define wxID_COL_TOOL_DEL 1022
#define wxID_BTN_COL_IMG 1023
#define wxID_BTN_CANCEL 1024
#define wxID_TOOL_SELECTTILE 1025
#define wxID_TOOL_SELECTSTAMP 1026
#define wxID_TOOL_PAINT 1027
#define wxID_TOOL_STAMP 1028
#define wxID_TOOL_TILEPICKER 1029
#define wxID_TOOL_STAMPPICKER 1030
#define wxID_TOOL_FLIPX 1031
#define wxID_TOOL_FLIPY 1032
#define wxID_TOOL_FILL 1033
#define wxID_TOOL_CLONE 1034
#define wxID_TOOL_CREATESTAMP 1035
#define wxID_TOOL_REMOVESTAMP 1036

///////////////////////////////////////////////////////////////////////////////
/// Class MainWindowBase
///////////////////////////////////////////////////////////////////////////////
class MainWindowBase : public wxFrame 
{
	private:
	
	protected:
		wxRibbonBar* m_ribbonBar;
		wxRibbonPage* m_ribbonPageProject;
		wxRibbonPanel* m_ribbonPanelProject;
		wxRibbonButtonBar* m_ribbonButtonBarProject;
		wxRibbonPage* m_ribbonPageView;
		wxRibbonPanel* m_ribbonPanelView;
		wxRibbonButtonBar* m_ribbonButtonBarGrid;
		wxRibbonPage* m_ribbonPageTools;
		wxRibbonPanel* m_ribbonPanelTools;
		wxRibbonButtonBar* m_ribbonButtonBarTools;
		wxRibbonPanel* m_ribbonPanelMap;
		wxRibbonButtonBar* m_ribbonButtonBarMap;
		wxRibbonPanel* m_ribbonPanelTiles;
		wxRibbonButtonBar* m_ribbonButtonBarTiles;
		wxRibbonPanel* m_ribbonPanelStamps;
		wxRibbonButtonBar* m_ribbonButtonBarStamps;
		wxRibbonPanel* m_ribbonPanelCollision;
		wxRibbonButtonBar* m_ribbonButtonBarCollision;
		wxPanel* m_dockArea;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnBtnProjNew( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnProjOpen( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnProjSave( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnProjExport( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnGridShow( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnGridSnap( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnShowOutlines( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsMapEdit( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsTiles( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsStamps( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsPalettes( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnMapClear( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnMapResize( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnTilesImport( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnTilesCreate( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnTilesDelete( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnCollisionConfig( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnCollisionTileEdit( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainWindowBase( wxWindow* parent, wxWindowID id = wxID_MAINWINDOW, const wxString& title = wxT("BEEhive v0.1"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 720,489 ), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE|wxTAB_TRAVERSAL );
		
		~MainWindowBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CollisionTypeDialogBase
///////////////////////////////////////////////////////////////////////////////
class CollisionTypeDialogBase : public wxFrame 
{
	private:
	
	protected:
		wxToolBar* m_toolBar1;
		wxToolBarToolBase* m_toolAddCollisionType; 
		wxToolBarToolBase* m_toolRemoveCollisionType; 
		wxListCtrl* m_listCollisionTypes;
		wxStaticText* m_staticText6;
		wxBitmapButton* m_buttonIcon;
		wxStaticText* m_staticText4;
		wxTextCtrl* m_textName;
		wxStaticText* m_staticText5;
		wxSpinCtrl* m_spinBit;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnAddType( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemoveType( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTypeSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnIconChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNameChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBitChange( wxSpinEvent& event ) { event.Skip(); }
		
	
	public:
		
		CollisionTypeDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Collision Pixel Types"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 316,298 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_NO_TASKBAR|wxSTAY_ON_TOP|wxTAB_TRAVERSAL );
		
		~CollisionTypeDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogMapSizeBase
///////////////////////////////////////////////////////////////////////////////
class DialogMapSizeBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_text1;
		wxStaticText* m_text2;
		wxStaticText* m_text3;
		wxButton* m_buttonOk;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnBtnOk( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnCancel( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxSpinCtrl* m_spinCtrlWidth;
		wxSpinCtrl* m_spinCtrlHeight;
		
		DialogMapSizeBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Resize Map"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSTAY_ON_TOP ); 
		~DialogMapSizeBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MapToolbox
///////////////////////////////////////////////////////////////////////////////
class MapToolbox : public wxPanel 
{
	private:
	
	protected:
		wxBitmapButton* m_toolSelectTile;
		wxBitmapButton* m_toolSelectStamp;
		wxBitmapButton* m_toolPaint;
		wxBitmapButton* m_toolStamp;
		wxBitmapButton* m_toolTilePicker;
		wxBitmapButton* m_toolStampPicker;
		wxBitmapButton* m_toolFlipX;
		wxBitmapButton* m_toolFlipY;
		wxBitmapButton* m_toolFill;
		wxBitmapButton* m_toolClone;
		wxBitmapButton* m_toolCreateStamp;
		wxBitmapButton* m_toolRemoveStamp;
	
	public:
		
		MapToolbox( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); 
		~MapToolbox();
	
};

#endif //__UIBASE_H__
