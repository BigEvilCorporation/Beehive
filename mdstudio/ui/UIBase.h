///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __UIBASE_H__
#define __UIBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
class SpriteCanvas;

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
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/bmpbuttn.h>
#include <wx/listbox.h>
#include <wx/filepicker.h>
#include <wx/radiobut.h>
#include <wx/clrpicker.h>
#include <wx/grid.h>
#include <wx/slider.h>
#include <wx/toolbar.h>
#include <wx/listctrl.h>
#include <wx/choice.h>
#include <wx/statline.h>

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
#define wxID_BTN_TOOLS_GAMEOBJS 1012
#define wxID_BTN_TOOLS_GAMEOBJPARAMS 1013
#define wxID_BTN_MAP_CLEAR 1014
#define wxID_BTN_MAP_RESIZE 1015
#define wxID_BTN_TILES_IMPORT 1016
#define wxID_BTN_TILES_CREATE 1017
#define wxID_BTN_TILES_DELETE 1018
#define wxID_BTN_TILES_CLEANUP 1019
#define wxID_BTN_SPRITE_EDITOR 1020
#define wxID_BTN_STAMPS_CREATE 1021
#define wxID_BTN_STAMPS_DELETE 1022
#define wxID_BTN_COL_TILES_CREATE 1023
#define wxID_BTN_COL_TILES_DELETE 1024
#define wxID_BTN_COL_TILES_CLEANUP 1025
#define wxID_BTN_COL_MAP_CLEAR 1026
#define wxID_BTN_COL_GEN_BEZIER 1027
#define wxID_BTN_GAME_OBJ_TYPES 1028
#define wxID_BTN_CANCEL 1029
#define wxID_TOOL_SELECTTILE 1030
#define wxID_TOOL_PAINT 1031
#define wxID_TOOL_TILEPICKER 1032
#define wxID_TOOL_FLIPX 1033
#define wxID_TOOL_FLIPY 1034
#define wxID_TOOL_FILL 1035
#define wxID_TOOL_CLONE 1036
#define wxID_TOOL_SELECTSTAMP 1037
#define wxID_TOOL_STAMP 1038
#define wxID_TOOL_STAMPPICKER 1039
#define wxID_TOOL_CREATESTAMP 1040
#define wxID_TOOL_REMOVESTAMP 1041
#define wxID_TOOL_COL_PAINTTERRAIN 1042
#define wxID_TOOL_COL_PAINTSOLID 1043
#define wxID_TOOL_COL_DELETETERRTILE 1044
#define wxID_TOOL_COL_ADDTERRAINBEZIER 1045
#define wxID_TOOL_COL_EDITTERRAINBEZIER 1046
#define wxID_TOOL_SELECTGAMEOBJ 1047
#define wxID_TOOL_PLACEGAMEOBJ 1048
#define wxID_TOOL_REMOVEGAMEOBJ 1049

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
		wxRibbonPanel* m_ribbonPanelSprites;
		wxRibbonButtonBar* m_ribbonButtonBarSprites;
		wxRibbonPanel* m_ribbonPanelStamps;
		wxRibbonButtonBar* m_ribbonButtonBarStamps;
		wxRibbonPanel* m_ribbonPanelCollision;
		wxRibbonButtonBar* m_ribbonButtonBarCollision;
		wxRibbonPanel* m_ribbonPanelGameObj;
		wxRibbonButtonBar* m_ribbonButtonBarGameObj;
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
		virtual void OnBtnToolsGameObjs( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsGameObjParams( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnMapClear( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnMapResize( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnTilesImport( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnTilesCreate( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnTilesDelete( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnTilesCleanup( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnSpriteEditor( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnColTilesCreate( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnColTilesDelete( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnColTilesCleanup( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnColMapClear( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnColGenTerrainBezier( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnGameObjTypes( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainWindowBase( wxWindow* parent, wxWindowID id = wxID_MAINWINDOW, const wxString& title = wxT("BEEhive v0.1"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 720,489 ), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE|wxTAB_TRAVERSAL );
		
		~MainWindowBase();
	
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
/// Class DialogNewAnim
///////////////////////////////////////////////////////////////////////////////
class DialogNewAnim : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_text1;
		wxStdDialogButtonSizer* m_sdbSizer4;
		wxButton* m_sdbSizer4OK;
		wxButton* m_sdbSizer4Cancel;
	
	public:
		wxTextCtrl* m_textName;
		wxCheckBox* m_chkAutoGenerate;
		
		DialogNewAnim( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("New Animation"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 261,127 ), long style = wxCAPTION|wxSTAY_ON_TOP ); 
		~DialogNewAnim();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogNewActor
///////////////////////////////////////////////////////////////////////////////
class DialogNewActor : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_text1;
		wxStdDialogButtonSizer* m_sdbSizer4;
		wxButton* m_sdbSizer4OK;
		wxButton* m_sdbSizer4Cancel;
	
	public:
		wxTextCtrl* m_textName;
		
		DialogNewActor( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("New Actor/Prop"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 261,99 ), long style = wxCAPTION|wxSTAY_ON_TOP ); 
		~DialogNewActor();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogTerrainGenBase
///////////////////////////////////////////////////////////////////////////////
class DialogTerrainGenBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_text1;
		wxButton* m_buttonOk;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnBtnOk( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnCancel( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxSpinCtrl* m_spinCtrlGranularity;
		
		DialogTerrainGenBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Generate Terrain"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSTAY_ON_TOP ); 
		~DialogTerrainGenBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MapToolbox
///////////////////////////////////////////////////////////////////////////////
class MapToolbox : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* m_staticText11;
		wxBitmapButton* m_toolSelectTile;
		wxBitmapButton* m_toolPaint;
		wxBitmapButton* m_toolTilePicker;
		wxBitmapButton* m_toolFlipX;
		wxBitmapButton* m_toolFlipY;
		wxBitmapButton* m_toolFill;
		wxBitmapButton* m_toolClone;
		wxStaticText* m_staticText12;
		wxBitmapButton* m_toolSelectStamp;
		wxBitmapButton* m_toolPaintStamp;
		wxBitmapButton* m_toolStampPicker;
		wxBitmapButton* m_toolCreateStamp;
		wxBitmapButton* m_toolRemoveStamp;
		wxStaticText* m_staticText13;
		wxBitmapButton* m_toolPaintCollisionPixel;
		wxBitmapButton* m_toolPaintCollisionSolid;
		wxBitmapButton* m_toolDeleteTerrainTile;
		wxBitmapButton* m_toolAddTerrainBezier;
		wxBitmapButton* m_toolEditTerrainBezier1;
		wxStaticText* m_staticText14;
		wxBitmapButton* m_toolSelectGameObject;
		wxBitmapButton* m_toolPlaceGameObject;
		wxBitmapButton* m_toolRemoveGameObject;
	
	public:
		
		MapToolbox( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 124,627 ), long style = wxTAB_TRAVERSAL|wxVSCROLL ); 
		~MapToolbox();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GameObjTypesPanelBase
///////////////////////////////////////////////////////////////////////////////
class GameObjTypesPanelBase : public wxPanel 
{
	private:
	
	protected:
		wxListBox* m_listGameObjTypes;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnGameObjectTypeSelected( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GameObjTypesPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 342,231 ), long style = wxTAB_TRAVERSAL ); 
		~GameObjTypesPanelBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ExportDialog
///////////////////////////////////////////////////////////////////////////////
class ExportDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_static1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnBtnCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnOk( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxTextCtrl* m_txtProjectName;
		wxCheckBox* m_chkPalettes;
		wxFilePickerCtrl* m_filePickerPalettes;
		wxCheckBox* m_chkTileset;
		wxFilePickerCtrl* m_filePickerTileset;
		wxCheckBox* m_chkMap;
		wxFilePickerCtrl* m_filePickerMap;
		wxCheckBox* m_chkTerrainTiles;
		wxFilePickerCtrl* m_filePickerTerrainTiles;
		wxCheckBox* m_chkCollisionMap;
		wxFilePickerCtrl* m_filePickerCollisionMap;
		wxCheckBox* m_chkGameObj;
		wxFilePickerCtrl* m_filePickerGameObj;
		wxCheckBox* m_chkSprites;
		wxDirPickerCtrl* m_dirPickerSprites;
		wxRadioButton* m_btnText;
		wxRadioButton* m_btnBinary;
		wxStdDialogButtonSizer* m_buttons;
		wxButton* m_buttonsOK;
		wxButton* m_buttonsCancel;
		
		ExportDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Export"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 788,495 ), long style = wxCAPTION|wxSTAY_ON_TOP ); 
		~ExportDialog();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ImportDialogBase
///////////////////////////////////////////////////////////////////////////////
class ImportDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText8;
		wxTextCtrl* m_filenames;
		wxButton* m_btnSelectFiles;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		wxButton* m_sdbSizer2Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnBtnBrowse( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxCheckBox* m_chkClearTiles;
		wxCheckBox* m_chkClearMap;
		wxCheckBox* m_chkClearPalettes;
		wxCheckBox* m_chkImportPalette;
		wxCheckBox* m_chkInsertBGTile;
		wxCheckBox* m_chkPaintToMap;
		wxCheckBox* m_chkImportToStamp;
		wxCheckBox* m_chkPalette1;
		wxCheckBox* m_chkPalette2;
		wxCheckBox* m_chkPalette3;
		wxCheckBox* m_chkPalette4;
		
		ImportDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Import"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 512,393 ), long style = wxCAPTION|wxSTAY_ON_TOP ); 
		~ImportDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ImportDialogSpriteSheetBase
///////////////////////////////////////////////////////////////////////////////
class ImportDialogSpriteSheetBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText8;
		wxStaticText* m_staticText24;
		SpriteCanvas* m_canvas;
		wxStaticText* m_staticText241;
		wxColourPickerCtrl* m_gridColourPicker;
		wxStaticText* m_staticText21;
		wxStaticText* m_staticText22;
		wxStaticText* m_staticText23;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		wxButton* m_sdbSizer2Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnFileOpened( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnGridColourChanged( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void OnSpinWidthCells( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnSpinHeightCells( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnSpinCellCount( wxSpinEvent& event ) { event.Skip(); }
		
	
	public:
		wxFilePickerCtrl* m_filePicker;
		wxTextCtrl* m_textName;
		wxSpinCtrl* m_spinWidthCells;
		wxSpinCtrl* m_spinHeightCells;
		wxSpinCtrl* m_spinCellCount;
		
		ImportDialogSpriteSheetBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Import Sprite Sheet"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 816,656 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
		~ImportDialogSpriteSheetBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SpriteAnimEditorDialogBase
///////////////////////////////////////////////////////////////////////////////
class SpriteAnimEditorDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText36;
		wxStaticText* m_staticText38;
		wxStaticText* m_staticText39;
		wxListBox* m_listActors;
		wxListBox* m_listSpriteSheets;
		wxListBox* m_listAnimations;
		wxButton* m_btnNewActor;
		wxButton* m_btnDeleteActor;
		wxButton* m_btnActorExport;
		wxButton* m_btnImportSpriteSheet;
		wxButton* m_btnDeleteSprite;
		wxButton* m_btnNewAnim;
		wxButton* m_btnDeleteAnim;
		SpriteCanvas* m_canvas;
		wxStaticText* m_staticText33;
		wxGrid* m_gridSpriteFrames;
		wxStaticText* m_staticText32;
		wxGrid* m_gridTimeline;
		wxSlider* m_sliderTimeline;
		wxBitmapButton* m_btnPlay;
		wxBitmapButton* m_btnStop;
		wxStaticText* m_staticText40;
		wxSpinCtrl* m_spinCtrlSpeed;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnActorSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpriteSheetSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAnimSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnActorNew( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnActorDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnActorExport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnSpriteSheetImport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonSpriteDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnAnimNew( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnAnimDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSliderMove( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnBtnPlay( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnStop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpinSpeedChange( wxSpinEvent& event ) { event.Skip(); }
		
	
	public:
		
		SpriteAnimEditorDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sprite and Animation Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1378,772 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
		~SpriteAnimEditorDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GameObjTypeDialogBase
///////////////////////////////////////////////////////////////////////////////
class GameObjTypeDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxToolBar* m_toolBar1;
		wxToolBarToolBase* m_toolAddGameObjType; 
		wxToolBarToolBase* m_toolRemoveGameObjType; 
		wxToolBarToolBase* m_toolAddVariable; 
		wxToolBarToolBase* m_toolRemoveVariable; 
		wxListBox* m_listGameObjTypes;
		wxListCtrl* m_listVariables;
		wxStaticText* m_staticText10;
		wxTextCtrl* m_textGameObjName;
		wxStaticText* m_staticText25;
		wxChoice* m_choiceSprites;
		wxStaticText* m_staticText9;
		wxSpinCtrl* m_spinWidth;
		wxSpinCtrl* m_spinHeight;
		wxStaticLine* m_staticline2;
		wxStaticText* m_staticText6;
		wxTextCtrl* m_textVariableName;
		wxStaticText* m_staticText7;
		wxChoice* m_choiceSize;
		wxStaticText* m_staticText8;
		wxTextCtrl* m_textValue;
		wxButton* m_btnApply;
		wxButton* m_btnImport;
		wxButton* m_btnExport;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnToolGameObjAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolGameObjRemove( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolVariableAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolVariableRemove( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectGameObjType( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectVariable( wxListEvent& event ) { event.Skip(); }
		virtual void OnWidthChanged( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnHeightChanged( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnVariableSizeChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnApplyChanges( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnImport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnExport( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GameObjTypeDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Game Object Types"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 973,667 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~GameObjTypeDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GameObjParamsPanelBase
///////////////////////////////////////////////////////////////////////////////
class GameObjParamsPanelBase : public wxPanel 
{
	private:
	
	protected:
		wxToolBar* m_toolBar1;
		wxToolBarToolBase* m_toolAddVariable; 
		wxToolBarToolBase* m_toolRemoveVariable; 
		wxListCtrl* m_listVariables;
		wxStaticText* m_staticText6;
		wxTextCtrl* m_textVariableName;
		wxStaticText* m_staticText7;
		wxChoice* m_choiceSize;
		wxStaticText* m_staticText8;
		wxTextCtrl* m_textValue;
		wxButton* m_btnApply;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnToolVariableAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolVariableRemove( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectVariable( wxListEvent& event ) { event.Skip(); }
		virtual void OnVariableSizeChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnApplyChanges( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GameObjParamsPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 747,512 ), long style = wxTAB_TRAVERSAL ); 
		~GameObjParamsPanelBase();
	
};

#endif //__UIBASE_H__
