///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "UIBase.h"
#include "RenderResources.h"

#include <ion/renderer/Renderer.h>

#include <wx/glcanvas.h>

class DialogNewProject : public DialogNewProjectBase
{
public:
	DialogNewProject(wxWindow* parent);
	virtual void OnChoicePreset(wxCommandEvent& event);
	virtual void OnBtnOk(wxCommandEvent& event) { EndModal(wxID_OK); }
	virtual void OnBtnCancel(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
	void PopulatePreset(int index);
};

class DialogMapSize : public DialogMapSizeBase
{
public:
	DialogMapSize(wxWindow* parent) : DialogMapSizeBase(parent) {}
	virtual void OnBtnOk(wxCommandEvent& event) { EndModal(wxID_OK); }
	virtual void OnBtnCancel(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
};

class DialogTerrainGen : public DialogTerrainGenBase
{
public:
	DialogTerrainGen(wxWindow* parent) : DialogTerrainGenBase(parent) {}
	virtual void OnBtnOk(wxCommandEvent& event) { EndModal(wxID_OK); }
	virtual void OnBtnCancel(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
};

class ImportDialog : public ImportDialogBase
{
public:
	ImportDialog(wxWindow* parent);

	wxArrayString m_paths;

protected:
	virtual void OnBtnBrowse(wxCommandEvent& event);
};

class ImportDialogSpriteSheet : public ImportDialogSpriteSheetBase
{
public:
	ImportDialogSpriteSheet(wxWindow* parent, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources);
	virtual void OnFileOpened(wxFileDirPickerEvent& event);
	virtual void OnGridColourChanged(wxColourPickerEvent& event);
	virtual void OnSpinWidthCells(wxSpinEvent& event);
	virtual void OnSpinHeightCells(wxSpinEvent& event);
	virtual void OnSpinCellCount(wxSpinEvent& event);

private:
	Project& m_project;
	RenderResources& m_renderResources;
};