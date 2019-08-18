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

class DialogEditStampCollision : public DialogEditStampCollisionBase
{
public:
	DialogEditStampCollision(wxWindow* parent, Stamp& stamp, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources);
	virtual ~DialogEditStampCollision();

private:
	void Draw();

	Stamp& m_stamp;

	Project& m_project;
	ion::render::Renderer& m_renderer;
	RenderResources& m_renderResources;
	wxGLContext& m_glContext;
};