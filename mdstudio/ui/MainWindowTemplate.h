///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MAINWINDOWTEMPLATE_H__
#define __MAINWINDOWTEMPLATE_H__

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
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/toolbar.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainWindowTemplate
///////////////////////////////////////////////////////////////////////////////
class MainWindowTemplate : public wxFrame 
{
	private:
	
	protected:
		wxRibbonBar* m_ribbonBarMain;
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
		wxScrolledWindow* m_scrolledWindowMap;
		wxStatusBar* m_statusBarMain;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnBtnToolsMapEdit( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainWindowTemplate( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("MD Studio"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE|wxTAB_TRAVERSAL );
		
		~MainWindowTemplate();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ToolboxMapEditTemplate
///////////////////////////////////////////////////////////////////////////////
class ToolboxMapEditTemplate : public wxFrame 
{
	private:
	
	protected:
		wxToolBar* m_toolBarMapEdit;
		wxToolBarToolBase* m_toolMapPaint; 
		wxToolBarToolBase* m_toolMapFlipH; 
		wxToolBarToolBase* m_toolMapFlipV; 
		wxToolBarToolBase* m_toolMapAssignPal; 
	
	public:
		
		ToolboxMapEditTemplate( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Map Editing"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxFRAME_TOOL_WINDOW|wxTAB_TRAVERSAL );
		
		~ToolboxMapEditTemplate();
	
};

#endif //__MAINWINDOWTEMPLATE_H__
