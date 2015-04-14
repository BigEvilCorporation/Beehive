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

///////////////////////////////////////////////////////////////////////////

#define wxID_MAINWINDOW 1000
#define wxID_BTN_PROJ_NEW 1001
#define wxID_BTN_PROJ_OPEN 1002
#define wxID_BTN_PROJ_SAVEAS 1003
#define wxID_BTN_PROJ_SAVE 1004
#define wxID_BTN_PROJ_EXPORT 1005
#define wxID_BTN_TILES_IMPORT 1006
#define wxID_BTN_TOOLS_MAPEDIT 1007
#define wxID_BTN_TOOLS_TILES 1008
#define wxID_BTN_TOOLS_PALETTES 1009

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
		wxRibbonPage* m_ribbonPageTiles;
		wxRibbonPanel* m_ribbonPanelTiles;
		wxRibbonButtonBar* m_ribbonButtonBarTiles;
		wxRibbonPage* m_ribbonPageStamps;
		wxRibbonPanel* m_ribbonPanelStamps;
		wxRibbonButtonBar* m_ribbonButtonBarStamps;
		wxRibbonPage* m_ribbonPageMap;
		wxRibbonPanel* m_ribbonPanelMap;
		wxRibbonButtonBar* m_ribbonButtonBarMap;
		wxRibbonPage* m_ribbonPagePalettes;
		wxRibbonPanel* m_ribbonPanelPalettes;
		wxRibbonButtonBar* m_ribbonButtonBarPal;
		wxRibbonPage* m_ribbonPageCollision;
		wxRibbonPanel* m_ribbonPanelCollision;
		wxRibbonButtonBar* m_ribbonButtonBarCollision;
		wxRibbonPage* m_ribbonPageTools;
		wxRibbonPanel* m_ribbonPanelTools;
		wxRibbonButtonBar* m_ribbonButtonBarTools;
		wxPanel* m_dockArea;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnBtnProjNew( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnProjOpen( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnProjSaveAs( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnProjSave( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnProjExport( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnTilesImport( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnTilesDelete( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsMapEdit( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsTiles( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsStamps( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsPalettes( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainWindowBase( wxWindow* parent, wxWindowID id = wxID_MAINWINDOW, const wxString& title = wxT("BEE v0.1"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 720,489 ), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE|wxTAB_TRAVERSAL );
		
		~MainWindowBase();
	
};

#endif //__UIBASE_H__
