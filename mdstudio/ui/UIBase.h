///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
class KeyframePanel;
class SpriteCanvas;

#include "StampCanvas.h"
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
#include <wx/radiobox.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/filepicker.h>
#include <wx/checkbox.h>
#include <wx/bmpbuttn.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/toolbar.h>
#include <wx/listctrl.h>
#include <wx/listbox.h>
#include <wx/radiobut.h>
#include <wx/scrolwin.h>
#include <wx/notebook.h>
#include <wx/clrpicker.h>
#include <wx/grid.h>
#include <wx/slider.h>
#include <wx/statline.h>
#include <wx/treectrl.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_MAINWINDOW 1000
#define wxID_BTN_PROJ_NEW 1001
#define wxID_BTN_PROJ_OPEN 1002
#define wxID_BTN_PROJ_SAVE 1003
#define wxID_BTN_PROJ_SETTINGS 1004
#define wxID_BTN_PROJ_EXPORT 1005
#define wxID_BTN_SAVE_LAYOUT 1006
#define wxID_BTN_GRID_SHOW 1007
#define wxID_BTN_GRID_SNAP 1008
#define wxID_BTN_SHOW_OUTLINES 1009
#define wxID_BTN_SHOW_COLLISION 1010
#define wxID_BTN_SHOW_DISPLAYFRAME 1011
#define wxID_BTN_TOOLS_MAPEDIT 1012
#define wxID_BTN_TOOLS_MAP_LIST 1013
#define wxID_BTN_TOOLS_TILES 1014
#define wxID_BTN_TOOLS_COLLISION_TILES 1015
#define wxID_BTN_TOOLS_STAMPS 1016
#define wxID_BTN_TOOLS_PALETTES 1017
#define wxID_BTN_TOOLS_GAMEOBJS 1018
#define wxID_BTN_TOOLS_GAMEOBJPARAMS 1019
#define wxID_BTN_TOOLS_TIMELINE 1020
#define wxID_BTN_TOOLS_SPRITEANIM 1021
#define wxID_BTN_MAP_CLEAR 1022
#define wxID_BTN_MAP_RESIZE 1023
#define wxID_BTN_MAP_EXPORT_BMP 1024
#define wxID_BTN_TILES_IMPORT 1025
#define wxID_BTN_TILES_CREATE 1026
#define wxID_BTN_TILES_DELETE 1027
#define wxID_BTN_TILES_CLEANUP 1028
#define wxID_BTN_SPRITE_EDITOR 1029
#define wxID_BTN_STAMPS_IMPORT 1030
#define wxID_BTN_STAMPS_CREATE 1031
#define wxID_BTN_STAMPS_DELETE 1032
#define wxID_BTN_STAMPS_EXPORT_BMP 1033
#define wxID_BTN_STAMPS_CLEANUP 1034
#define wxID_BTN_COL_TILES_CREATE 1035
#define wxID_BTN_COL_TILES_DELETE 1036
#define wxID_BTN_COL_TILES_CLEANUP 1037
#define wxID_BTN_COL_MAP_CLEAR 1038
#define wxID_BTN_COL_GEN_BEZIER 1039
#define wxID_BTN_GAME_OBJ_TYPES 1040
#define wxID_BTN_CANCEL 1041
#define wxID_TOOL_COL_ADDTERRAINBEZIER 1042
#define wxID_TOOL_COL_EDITTERRAINBEZIER 1043
#define wxID_TOOL_COL_DELETETERRAINBEZIER 1044
#define wxID_TOOL_COL_PAINTSOLID 1045
#define wxID_TOOL_GENERATETERRAIN 1046
#define wxID_TOOL_SELECTSTAMP 1047
#define wxID_TOOL_STAMP 1048
#define wxID_TOOL_MOVESTAMP 1049
#define wxID_TOOL_STAMPPICKER 1050
#define wxID_TOOL_CREATESTAMP 1051
#define wxID_TOOL_REMOVESTAMP 1052
#define wxID_TOOL_COL_PAINTTERRAIN 1053
#define wxID_TOOL_COL_PAINTHOLE 1054
#define wxID_TOOL_COL_DELETETERRTILE 1055
#define wxID_TOOL_SELECTTILE 1056
#define wxID_TOOL_PAINT 1057
#define wxID_TOOL_TILEPICKER 1058
#define wxID_TOOL_FLIPX 1059
#define wxID_TOOL_FLIPY 1060
#define wxID_TOOL_FILL 1061
#define wxID_TOOL_CLONE 1062
#define wxID_TOOL_CREATE_SCENE_ANIM 1063
#define wxID_TOOL_COPY_TO_NEW_MAP 1064
#define wxID_TOOL_SELECTGAMEOBJ 1065
#define wxID_TOOL_PLACEGAMEOBJ 1066
#define wxID_TOOL_MOVEGAMEOBJ 1067
#define wxID_TOOL_ANIMATEGAMEOBJ 1068
#define wxID_TOOL_REMOVEGAMEOBJ 1069
#define wxID_TOOL_DRAWGAMEOBJ 1070
#define wxID_TOOL_DUPLICATEGAMEOBJ 1071

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
		virtual void OnBtnProjSettings( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnProjExport( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnSaveLayout( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnGridShow( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnGridSnap( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnShowOutlines( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnShowCollision( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnShowDisplayFrame( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsMapEdit( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsMapList( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsTiles( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsCollisionTiles( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsStamps( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsPalettes( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsGameObjs( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsGameObjParams( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsTimeline( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnToolsSpriteAnim( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnMapClear( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnMapResize( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnMapExportBMP( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnTilesImport( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnTilesCreate( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnTilesDelete( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnTilesCleanup( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnSpriteEditor( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnStampsImport( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnStampsExportBMPs( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnStampsCleanup( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnColTilesCreate( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnColTilesDelete( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnColTilesCleanup( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnColMapClear( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnColGenTerrainBezier( wxRibbonButtonBarEvent& event ) { event.Skip(); }
		virtual void OnBtnGameObjTypes( wxRibbonButtonBarEvent& event ) { event.Skip(); }


	public:
		wxRibbonPanel* m_ribbonPanelStamps;
		wxRibbonButtonBar* m_ribbonButtonBarStamps;

		MainWindowBase( wxWindow* parent, wxWindowID id = wxID_MAINWINDOW, const wxString& title = wxT("Beehive Studio v0.5"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 971,240 ), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE|wxTAB_TRAVERSAL );

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
		wxRadioBox* m_radioBoxShiftY;
		wxRadioBox* m_radioBoxShiftX;

		DialogMapSizeBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Resize Map"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 415,189 ), long style = wxCAPTION|wxSTAY_ON_TOP );
		~DialogMapSizeBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogUpdateStampBase
///////////////////////////////////////////////////////////////////////////////
class DialogUpdateStampBase : public wxDialog
{
	private:

	protected:
		wxFilePickerCtrl* m_filePicker10;
		wxCheckBox* m_chkReplacePalette;
		StampCanvas* m_canvasOld;
		StampCanvas* m_canvasNew;
		wxCheckBox* m_chkFlipX;
		wxCheckBox* m_chkFlipY;
		wxButton* m_buttonOk;
		wxButton* m_buttonCancel;

		// Virtual event handlers, overide them in your derived class
		virtual void OnFileBrowse( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnBtnOk( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnCancel( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxRadioBox* m_radioBoxShiftY;
		wxRadioBox* m_radioBoxShiftX;

		DialogUpdateStampBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Update Stamp"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 688,485 ), long style = wxCAPTION|wxSTAY_ON_TOP );
		~DialogUpdateStampBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogEditStampCollisionBase
///////////////////////////////////////////////////////////////////////////////
class DialogEditStampCollisionBase : public wxDialog
{
	private:

	protected:
		wxPanel* m_toolbox;
		wxBitmapButton* m_toolAddTerrainBezier;
		wxBitmapButton* m_toolEditTerrainBezier;
		wxBitmapButton* m_toolDeleteTerrainBezier;
		wxBitmapButton* m_toolPaintCollisionSolid;
		wxBitmapButton* m_toolGenerateTerrain;
		StampCanvas* m_canvas;

		// Virtual event handlers, overide them in your derived class
		virtual void OnToolAddBezier( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolEditBezier( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolDeleteBezier( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolPaintSolid( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolGenerateTerrain( wxCommandEvent& event ) { event.Skip(); }


	public:

		DialogEditStampCollisionBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Edit Stamp Collision"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 688,485 ), long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
		~DialogEditStampCollisionBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogNewMap
///////////////////////////////////////////////////////////////////////////////
class DialogNewMap : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText50;
		wxStaticText* m_text1;
		wxStaticText* m_text2;
		wxStaticText* m_text3;
		wxStdDialogButtonSizer* m_sdbSizer7;
		wxButton* m_sdbSizer7OK;
		wxButton* m_sdbSizer7Cancel;

	public:
		wxTextCtrl* m_textMapName;
		wxSpinCtrl* m_spinCtrlWidth;
		wxSpinCtrl* m_spinCtrlHeight;

		DialogNewMap( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("New Map"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 415,139 ), long style = wxCAPTION|wxSTAY_ON_TOP );
		~DialogNewMap();

};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogNewProjectBase
///////////////////////////////////////////////////////////////////////////////
class DialogNewProjectBase : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText49;
		wxChoice* m_choicePreset;
		wxStaticText* m_text1;
		wxStaticText* m_text3;
		wxStaticText* m_text11;
		wxStaticText* m_text31;
		wxStaticText* m_staticText53;
		wxStaticText* m_staticText54;
		wxButton* m_buttonOk;
		wxButton* m_buttonCancel;

		// Virtual event handlers, overide them in your derived class
		virtual void OnChoicePreset( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnOk( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnCancel( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxSpinCtrl* m_spinCtrlTileWidth;
		wxSpinCtrl* m_spinCtrlTileHeight;
		wxSpinCtrl* m_spinCtrlMapWidth;
		wxSpinCtrl* m_spinCtrlMapHeight;
		wxSpinCtrl* m_spinCtrlStampWidth;
		wxSpinCtrl* m_spinCtrlStampHeight;

		DialogNewProjectBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("New Project"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 454,202 ), long style = wxCAPTION|wxSTAY_ON_TOP );
		~DialogNewProjectBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogNewSpriteAnim
///////////////////////////////////////////////////////////////////////////////
class DialogNewSpriteAnim : public wxDialog
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

		DialogNewSpriteAnim( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("New sprite animation"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 261,127 ), long style = wxCAPTION|wxSTAY_ON_TOP );
		~DialogNewSpriteAnim();

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

		DialogNewAnim( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("New animation"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 261,127 ), long style = wxCAPTION|wxSTAY_ON_TOP );
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
		wxBitmapButton* m_toolSelectStamp;
		wxBitmapButton* m_toolMveStamp;
		wxBitmapButton* m_toolStampPicker;
		wxBitmapButton* m_toolCreateStamp;
		wxBitmapButton* m_toolRemoveStamp;
		wxStaticText* m_staticText13;
		wxBitmapButton* m_toolPaintCollisionSolid;
		wxBitmapButton* m_toolPaintCollisionHole;
		wxBitmapButton* m_toolDeleteTerrainTile;
		wxBitmapButton* m_toolAddTerrainBezier;
		wxBitmapButton* m_toolEditTerrainBezier1;
		wxBitmapButton* m_toolDeleteTerrainBezier;
		wxStaticText* m_staticText12;
		wxBitmapButton* m_toolSelectTile;
		wxBitmapButton* m_toolPaint;
		wxBitmapButton* m_toolTilePicker;
		wxBitmapButton* m_toolFlipX;
		wxBitmapButton* m_toolFlipY;
		wxBitmapButton* m_toolFill;
		wxStaticText* m_staticText14;
		wxBitmapButton* m_toolSelectGameObject;
		wxBitmapButton* m_toolPlaceGameObject;
		wxBitmapButton* m_toolMoveGameObject;
		wxBitmapButton* m_toolRemoveGameObject;

	public:
		wxBitmapButton* m_toolPaintStamp;
		wxBitmapButton* m_toolPaintCollisionPixel;
		wxBitmapButton* m_toolClone;
		wxBitmapButton* m_toolCreateSceneAnim;
		wxBitmapButton* m_toolCopyToNewMap;
		wxBitmapButton* m_toolAnimateGameObject;

		MapToolbox( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 259,597 ), long style = wxTAB_TRAVERSAL|wxVSCROLL, const wxString& name = wxEmptyString );
		~MapToolbox();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MapToolboxGameObjs
///////////////////////////////////////////////////////////////////////////////
class MapToolboxGameObjs : public wxPanel
{
	private:

	protected:
		wxBitmapButton* m_toolSelectGameObject;
		wxBitmapButton* m_toolPlaceGameObject;
		wxBitmapButton* m_toolDrawGameObject;
		wxBitmapButton* m_toolMoveGameObject;
		wxBitmapButton* m_toolRemoveGameObject;
		wxBitmapButton* m_toolDuplicateGameObject;

	public:
		wxBitmapButton* m_toolAnimateGameObject;

		MapToolboxGameObjs( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 115,271 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~MapToolboxGameObjs();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MapToolboxTerrain
///////////////////////////////////////////////////////////////////////////////
class MapToolboxTerrain : public wxPanel
{
	private:

	protected:
		wxBitmapButton* m_toolPaintCollisionSolid;
		wxBitmapButton* m_toolPaintCollisionHole;
		wxBitmapButton* m_toolDeleteTerrainTile;
		wxBitmapButton* m_toolAddTerrainBezier;
		wxBitmapButton* m_toolEditTerrainBezier1;
		wxBitmapButton* m_toolDeleteTerrainBezier;

	public:
		wxBitmapButton* m_toolPaintCollisionPixel;

		MapToolboxTerrain( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 110,228 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~MapToolboxTerrain();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MapToolboxTiles
///////////////////////////////////////////////////////////////////////////////
class MapToolboxTiles : public wxPanel
{
	private:

	protected:

	public:
		wxBitmapButton* m_toolSelectTile;
		wxBitmapButton* m_toolPaint;
		wxBitmapButton* m_toolTilePicker;
		wxBitmapButton* m_toolFlipX;
		wxBitmapButton* m_toolFlipY;
		wxBitmapButton* m_toolFill;
		wxBitmapButton* m_toolClone;
		wxBitmapButton* m_toolCreateSceneAnim;
		wxBitmapButton* m_toolCopyToNewMap;

		MapToolboxTiles( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 108,282 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~MapToolboxTiles();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MapToolboxStamps
///////////////////////////////////////////////////////////////////////////////
class MapToolboxStamps : public wxPanel
{
	private:

	protected:
		wxBitmapButton* m_toolSelectStamp;
		wxBitmapButton* m_toolMveStamp;
		wxBitmapButton* m_toolStampPicker;
		wxBitmapButton* m_toolRemoveStamp;

	public:
		wxBitmapButton* m_toolPaintStamp;
		wxBitmapButton* m_toolCreateStamp;

		MapToolboxStamps( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 116,181 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~MapToolboxStamps();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MapListPanelBase
///////////////////////////////////////////////////////////////////////////////
class MapListPanelBase : public wxPanel
{
	private:

	protected:
		wxToolBar* m_toolBar1;
		wxToolBarToolBase* m_toolAddMap;
		wxToolBarToolBase* m_toolRemoveMap;
		wxToolBarToolBase* m_toolRename;
		wxToolBarToolBase* m_toolImport;
		wxListCtrl* m_listMaps;

		// Virtual event handlers, overide them in your derived class
		virtual void OnToolAddMap( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolRemoveMap( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolRenameMap( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolImportMap( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapRightClick( wxListEvent& event ) { event.Skip(); }
		virtual void OnMapSelected( wxListEvent& event ) { event.Skip(); }


	public:

		MapListPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 375,425 ), long style = wxTAB_TRAVERSAL|wxVSCROLL, const wxString& name = wxEmptyString );
		~MapListPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class GameObjTypesPanelBase
///////////////////////////////////////////////////////////////////////////////
class GameObjTypesPanelBase : public wxPanel
{
	private:

	protected:
		wxStaticText* m_staticText77;
		wxStaticText* m_staticText78;
		wxListBox* m_listGameObjTypes;
		wxListBox* m_listGameObjArchetypes;

		// Virtual event handlers, overide them in your derived class
		virtual void OnGameObjectTypeSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnArchetypeSelected( wxCommandEvent& event ) { event.Skip(); }


	public:

		GameObjTypesPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 342,231 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~GameObjTypesPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class ExportDialogBase
///////////////////////////////////////////////////////////////////////////////
class ExportDialogBase : public wxDialog
{
	private:

	protected:
		wxStaticText* m_static1;
		wxScrolledWindow* m_scrolledWindow1;
		wxNotebook* m_notebookMapTabs;
		wxButton* m_btnCancel;
		wxButton* m_btnExportAll;

		// Virtual event handlers, overide them in your derived class
		virtual void OnBtnCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnExportAll( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxTextCtrl* m_txtProjectName;
		wxRadioButton* m_btnText;
		wxRadioButton* m_btnBinary;
		wxRadioButton* m_btnCompressed;
		wxRadioButton* m_btnBeeFormat;
		wxCheckBox* m_chkPalettes;
		wxFilePickerCtrl* m_filePickerPalettes;
		wxCheckBox* m_chkTileset;
		wxFilePickerCtrl* m_filePickerTileset;
		wxCheckBox* m_chkBlocks;
		wxFilePickerCtrl* m_filePickerBlocks;
		wxCheckBox* m_chkStamps;
		wxFilePickerCtrl* m_filePickerStamps;
		wxCheckBox* m_chkStampAnims;
		wxFilePickerCtrl* m_filePickerStampAnims;
		wxCheckBox* m_chkTerrainTiles;
		wxFilePickerCtrl* m_filePickerTerrainTiles;
		wxCheckBox* m_chkTerrainAngles;
		wxFilePickerCtrl* m_filePickerTerrainAngles;
		wxCheckBox* m_chkGameObjTypes;
		wxFilePickerCtrl* m_filePickerGameObjTypes;
		wxCheckBox* m_chkTerrainBlocks;
		wxFilePickerCtrl* m_filePickerTerrainBlocks;
		wxCheckBox* m_chkSpriteSheets;
		wxDirPickerCtrl* m_dirPickerSpriteSheets;
		wxCheckBox* m_chkSpritePalettes;
		wxDirPickerCtrl* m_dirPickerSpritePalettes;
		wxCheckBox* m_chkSpriteAnims;
		wxDirPickerCtrl* m_dirPickerSpriteAnims;

		ExportDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Export"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 976,667 ), long style = wxCAPTION|wxRESIZE_BORDER );
		~ExportDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class ProjectSettingsDialogBase
///////////////////////////////////////////////////////////////////////////////
class ProjectSettingsDialogBase : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText691;
		wxTextCtrl* m_textProjectName;
		wxStaticText* m_staticText511;
		wxButton* m_btnParseProject;
		wxStaticText* m_staticText51112;
		wxStaticText* m_staticText5111;
		wxStaticText* m_staticText51111;
		wxStaticText* m_staticText511111;
		wxStaticText* m_staticText5111111;
		wxStaticText* m_staticText51111111;
		wxStaticText* m_staticText511111111;
		wxStaticText* m_staticText51;
		wxStaticText* m_staticText53;
		wxStaticText* m_staticText531;
		wxStaticText* m_staticText68;
		wxSpinCtrl* m_spinStampWidth;
		wxStaticText* m_staticText69;
		wxSpinCtrl* m_spinStampHeight;
		wxStdDialogButtonSizer* m_sdbSizer7;
		wxButton* m_sdbSizer7OK;
		wxButton* m_sdbSizer7Cancel;

		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnBtnScanProject( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxDirPickerCtrl* m_dirPickerProject;
		wxDirPickerCtrl* m_dirPickerEntities;
		wxDirPickerCtrl* m_dirPickerScene;
		wxDirPickerCtrl* m_dirPickerSprites;
		wxDirPickerCtrl* m_dirPickerSpriteAnims;
		wxDirPickerCtrl* m_dirPickerSpritePalettes;
		wxDirPickerCtrl* m_dirPickerScripts;
		wxDirPickerCtrl* m_dirPickerScriptsInclude;
		wxFilePickerCtrl* m_filePickerSpritesProj;
		wxFilePickerCtrl* m_filePickerGameObjTypesFile;
		wxFilePickerCtrl* m_filePickerReference;

		ProjectSettingsDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Project Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 879,596 ), long style = wxCAPTION|wxRESIZE_BORDER );
		~ProjectSettingsDialogBase();

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
		wxStaticText* m_staticText81;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		wxButton* m_sdbSizer2Cancel;

		// Virtual event handlers, overide them in your derived class
		virtual void OnBtnBrowse( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxDirPickerCtrl* m_dirStamps;
		wxCheckBox* m_chkClearTiles;
		wxCheckBox* m_chkClearMap;
		wxCheckBox* m_chkClearPalettes;
		wxCheckBox* m_chkImportPalette;
		wxCheckBox* m_chkInsertBGTile;
		wxCheckBox* m_chkPaintToMap;
		wxCheckBox* m_chkImportToStamp;
		wxCheckBox* m_chkReplaceStamps;
		wxCheckBox* m_chkOnlyExisting;
		wxCheckBox* m_chkNoDuplicateTileCheck;
		wxCheckBox* m_chkPalette1;
		wxCheckBox* m_chkPalette2;
		wxCheckBox* m_chkPalette3;
		wxCheckBox* m_chkPalette4;

		ImportDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Import"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 503,466 ), long style = wxCAPTION|wxSTAY_ON_TOP );
		~ImportDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class ImportStampsDialogBase
///////////////////////////////////////////////////////////////////////////////
class ImportStampsDialogBase : public wxDialog
{
	private:

	protected:
		wxTextCtrl* m_filenames;
		wxButton* m_btnSelectFiles;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		wxButton* m_sdbSizer2Cancel;

		// Virtual event handlers, overide them in your derived class
		virtual void OnRadioImportFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnBrowse( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRadioImportDir( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxRadioButton* m_radioSingleStamp;
		wxRadioButton* m_radioStampDir;
		wxDirPickerCtrl* m_dirStamps;
		wxCheckBox* m_chkClearPalettes;
		wxCheckBox* m_chkImportPalette;
		wxCheckBox* m_chkReplaceStamps;
		wxRadioBox* m_radioBoxPal;

		ImportStampsDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Import"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 503,332 ), long style = wxCAPTION|wxSTAY_ON_TOP );
		~ImportStampsDialogBase();

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

		ImportDialogSpriteSheetBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Import Sprite/Stamp Sheet"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 816,656 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
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
		wxButton* m_btnImport;
		wxButton* m_btnExport;
		wxButton* m_btnRenameActor;
		wxButton* m_btnImportSpriteSheet;
		wxButton* m_btnDeleteSprite;
		wxButton* m_btnUsePalette;
		wxButton* m_btnRenameSheet;
		wxButton* m_btnNewAnim;
		wxButton* m_btnDeleteAnim;
		wxButton* m_btnRenameAnim;
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
		wxStaticText* m_staticText43;
		wxRadioButton* m_radioBlendLerp;
		wxRadioButton* m_radioBlendSnap;

		// Virtual event handlers, overide them in your derived class
		virtual void OnActorSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpriteSheetSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAnimSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnActorNew( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnActorDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnImport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnExport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnRenameActor( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnSpriteSheetImport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnSpriteSheetDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnSpriteSheetUsePalette( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnRenameSheet( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnAnimNew( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnAnimDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnRenameAnim( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSliderMove( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnBtnPlay( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnStop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpinSpeedChange( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnRadioBlendLerp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRadioBlendSnap( wxCommandEvent& event ) { event.Skip(); }


	public:

		SpriteAnimEditorDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sprite and Stamp Animation Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1378,772 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
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
		wxToolBarToolBase* m_toolOrderUp;
		wxToolBarToolBase* m_toolOrderDown;
		wxToolBarToolBase* m_toolAddVariable;
		wxToolBarToolBase* m_toolRemoveVariable;
		wxListBox* m_listGameObjTypes;
		wxListCtrl* m_listVariables;
		wxStaticText* m_staticText10;
		wxTextCtrl* m_textGameObjName;
		wxStaticText* m_staticText9;
		wxSpinCtrl* m_spinWidth;
		wxSpinCtrl* m_spinHeight;
		wxStaticText* m_staticText49;
		wxStaticText* m_staticText50;
		wxChoice* m_choiceSpriteActor;
		wxStaticText* m_staticText501;
		wxChoice* m_choiceSpriteSheet;
		wxStaticText* m_staticText5011;
		wxChoice* m_choiceSpriteAnim;
		wxButton* m_btnApplyObjSettings;
		wxButton* m_btnLoadSprite;
		wxStaticLine* m_staticline2;
		wxStaticText* m_staticText6;
		wxTextCtrl* m_textVariableName;
		wxStaticText* m_staticText7;
		wxChoice* m_choiceSize;
		wxStaticText* m_staticText8;
		wxTextCtrl* m_textValue;
		wxButton* m_btnApplyVarSettings;
		wxButton* m_btnImport;
		wxButton* m_btnExport;

		// Virtual event handlers, overide them in your derived class
		virtual void OnToolGameObjAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolGameObjRemove( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolOrderUp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolOrderDown( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolVariableAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolVariableRemove( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectGameObjType( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectVariable( wxListEvent& event ) { event.Skip(); }
		virtual void OnWidthChanged( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnHeightChanged( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnSelectSpriteActor( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectSpriteSheet( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectSpriteAnim( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnApplyObjChanges( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnLoadSprite( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnVariableSizeChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnApplyVarChanges( wxCommandEvent& event ) { event.Skip(); }
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
		wxStaticText* m_staticText42;
		wxTextCtrl* m_textObjectName;
		wxButton* m_btnApplyObjectName;
		wxStaticText* m_staticText63;
		wxChoice* m_choiceArchetypes;
		wxButton* m_btnNewArchetype;
		wxStaticText* m_staticText631;
		wxChoice* m_choiceSpriteActor;
		wxStaticText* m_staticText641;
		wxChoice* m_choiceSpriteSheet;
		wxStaticText* m_staticText64;
		wxChoice* m_choiceSpriteAnim;
		wxListCtrl* m_listVariables;
		wxStaticText* m_staticText6;
		wxTextCtrl* m_textVariableName;
		wxStaticText* m_staticText7;
		wxChoice* m_choiceSize;
		wxStaticText* m_staticText8;
		wxTextCtrl* m_textValue;
		wxStaticText* m_staticText81;
		wxChoice* m_choiceValue;

		// Virtual event handlers, overide them in your derived class
		virtual void OnToolVariableAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolVariableRemove( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonApplyObjectName( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectArchetype( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonNewArchetype( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectSpriteActor( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectSpriteSheet( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectSpriteAnim( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectVariable( wxListEvent& event ) { event.Skip(); }
		virtual void OnEnterTextVariable( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnVariableSizeChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEnterTextValue( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectValue( wxCommandEvent& event ) { event.Skip(); }


	public:

		GameObjParamsPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 676,512 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~GameObjParamsPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class TimelinePanelBase
///////////////////////////////////////////////////////////////////////////////
class TimelinePanelBase : public wxPanel
{
	private:

	protected:
		wxToolBar* m_toolBarAnimation;
		wxChoice* m_choiceAnims;
		wxToolBarToolBase* m_toolAddAnim;
		wxToolBarToolBase* m_toolDeleteAnim;
		wxToolBarToolBase* m_toolKeyframeReplace;
		wxToolBarToolBase* m_toolKeyframeInsert;
		wxToolBarToolBase* m_toolKeyframeEnd;
		wxToolBarToolBase* m_toolKeyframeDelete;
		wxToolBarToolBase* m_toolPlay;
		wxToolBarToolBase* m_toolStop;
		wxToolBarToolBase* m_toolStepLeft;
		wxToolBarToolBase* m_toolStepRight;
		wxToolBarToolBase* m_toolRewind;
		wxToolBarToolBase* m_toolFastForward;
		wxToolBarToolBase* m_toolIsolateObject;
		wxStaticText* m_staticText34;
		wxSpinCtrl* m_spinSpeed;
		wxCheckBox* m_chkExportLocalSpace;
		wxStaticText* m_staticText491;
		wxChoice* m_choiceActor;
		wxStaticText* m_textCurrentActor;
		wxStaticText* m_staticText49;
		wxChoice* m_choiceSpriteAnim;
		KeyframePanel* m_timeline;

		// Virtual event handlers, overide them in your derived class
		virtual void OnSelectAnimation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolAddAnim( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolDeleteAnim( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolKeyframeReplace( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolKeyframeInsert( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolKeyframeEnd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolKeyframeDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolLoopToggle( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolPlay( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolStop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolStepLeft( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolStepRight( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolRewind( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolFastForward( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToolIsolateObject( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpinSpeed( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnChkExportLocalSpace( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectActor( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectSpriteAnim( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxToolBarToolBase* m_toolToggleLoop;

		TimelinePanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 993,220 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~TimelinePanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogNewObjectTypeBase
///////////////////////////////////////////////////////////////////////////////
class DialogNewObjectTypeBase : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText61;
		wxStaticText* m_staticText62;
		wxStaticText* m_staticText59;
		wxButton* m_btnAddVar;
		wxButton* m_btnRemoveVar;
		wxStaticText* m_staticText60;
		wxStdDialogButtonSizer* m_sdbSizer8;
		wxButton* m_sdbSizer8OK;
		wxButton* m_sdbSizer8Cancel;

		// Virtual event handlers, overide them in your derived class
		virtual void OnChoiceActor( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnAddVar( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnRemoveVar( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxTextCtrl* m_textName;
		wxChoice* m_choiceActor;
		wxListBox* m_listVarsNotAdded;
		wxListBox* m_listVarsAdded;

		DialogNewObjectTypeBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("New Game Object Type"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 346,413 ), long style = wxCAPTION );
		~DialogNewObjectTypeBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class SpriteAnimPanelBase
///////////////////////////////////////////////////////////////////////////////
class SpriteAnimPanelBase : public wxPanel
{
	private:

	protected:
		wxListBox* m_listSpriteAnims;
		wxSlider* m_sliderTimeline;

	public:

		SpriteAnimPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,130 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~SpriteAnimPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class ExportDialogMapTab
///////////////////////////////////////////////////////////////////////////////
class ExportDialogMapTab : public wxPanel
{
	private:

	protected:

	public:
		wxCheckBox* m_chkMap;
		wxFilePickerCtrl* m_filePickerMap;
		wxCheckBox* m_chkBlockMap;
		wxFilePickerCtrl* m_filePickerBlockMap;
		wxCheckBox* m_chkStampMap;
		wxFilePickerCtrl* m_filePickerStampMap;
		wxCheckBox* m_chkCollisionMap;
		wxFilePickerCtrl* m_filePickerCollisionMap;
		wxCheckBox* m_chkCollisionBlockMap;
		wxFilePickerCtrl* m_filePickerCollisionBlockMap;
		wxCheckBox* m_chkSceneAnims;
		wxFilePickerCtrl* m_filePickerSceneAnimations;
		wxCheckBox* m_chkGameObj;
		wxFilePickerCtrl* m_filePickerGameObj;

		ExportDialogMapTab( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~ExportDialogMapTab();

};

///////////////////////////////////////////////////////////////////////////////
/// Class SceneExplorerPanelBase
///////////////////////////////////////////////////////////////////////////////
class SceneExplorerPanelBase : public wxPanel
{
	private:

	protected:
		wxTreeCtrl* m_tree;

		// Virtual event handlers, overide them in your derived class
		virtual void OnItemDragged( wxTreeEvent& event ) { event.Skip(); }
		virtual void OnItemRenamed( wxTreeEvent& event ) { event.Skip(); }
		virtual void OnItemActivated( wxTreeEvent& event ) { event.Skip(); }
		virtual void OnItemContextMenu( wxTreeEvent& event ) { event.Skip(); }
		virtual void OnItemSelected( wxTreeEvent& event ) { event.Skip(); }


	public:

		SceneExplorerPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 169,613 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~SceneExplorerPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class PropertyPanelBase
///////////////////////////////////////////////////////////////////////////////
class PropertyPanelBase : public wxPanel
{
	private:

	protected:
		wxPropertyGrid* m_propertyGrid;

		// Virtual event handlers, overide them in your derived class
		virtual void OnPropertyChanged( wxPropertyGridEvent& event ) { event.Skip(); }
		virtual void OnRightClick( wxMouseEvent& event ) { event.Skip(); }


	public:

		PropertyPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 356,614 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~PropertyPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class ScriptCompilePanelBase
///////////////////////////////////////////////////////////////////////////////
class ScriptCompilePanelBase : public wxPanel
{
	private:

	protected:
		wxTextCtrl* m_textOutput;

	public:

		ScriptCompilePanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~ScriptCompilePanelBase();

};

