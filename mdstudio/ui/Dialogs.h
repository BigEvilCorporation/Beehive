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