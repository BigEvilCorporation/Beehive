///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2017 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#pragma once

#include "UIBase.h"
#include "RenderResources.h"

#include <wx/glcanvas.h>

class DialogUpdateStamp : public DialogUpdateStampBase
{
public:
	DialogUpdateStamp(wxWindow* parent, Stamp& stamp, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources);
	virtual ~DialogUpdateStamp();

	virtual void OnFileBrowse(wxFileDirPickerEvent& event);
	virtual void OnBtnOk(wxCommandEvent& event);
	virtual void OnBtnCancel(wxCommandEvent& event);

private:
	void Draw(Stamp* stampA, Stamp* stampB);

	Stamp& m_stampOld;
	Stamp m_stampNew;
	std::string m_filename;
	int m_paletteIdx;

	Project& m_project;
	ion::render::Renderer& m_renderer;
	RenderResources& m_renderResources;
	wxGLContext& m_glContext;
};