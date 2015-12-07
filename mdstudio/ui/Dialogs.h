///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "UIBase.h"

class DialogMapSize : public DialogMapSizeBase
{
public:
	DialogMapSize(wxWindow* parent) : DialogMapSizeBase(parent) {}
	virtual void OnBtnOk(wxCommandEvent& event) { EndModal(wxID_OK); }
	virtual void OnBtnCancel(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
};

class ImportDialog : public ImportDialogBase
{
public:
	ImportDialog(wxWindow* parent)
		: ImportDialogBase(parent)
	{

	}

	wxArrayString m_paths;

protected:
	virtual void OnBtnBrowse(wxCommandEvent& event)
	{
		wxFileDialog dialog(this, _("Open BMP files"), "", "", "BMP files (*.bmp)|*.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
		if(dialog.ShowModal() == wxID_OK)
		{
			dialog.GetPaths(m_paths);

			if(m_paths.size() == 0)
			{
				m_filenames->Clear();
			}
			else if(m_paths.size() == 1)
			{
				m_filenames->SetValue(m_paths[0]);
			}
			else
			{
				char text[128] = { 0 };
				sprintf(text, "(%u) BMP files", m_paths.size());
				m_filenames->SetValue(wxString(text));
			}
		}
	}
};