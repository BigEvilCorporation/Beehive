///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "StampsPanel.h"

StampsPanel::StampsPanel(ion::render::Renderer& renderer, wxGLContext* glContext, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(renderer, glContext, parent, winid, pos, size, style, name)
{

}

StampsPanel::~StampsPanel()
{

}

void StampsPanel::OnMouse(wxMouseEvent& event)
{
	ViewPanel::OnMouse(event);
}

void StampsPanel::OnKeyboard(wxKeyEvent& event)
{
	ViewPanel::OnKeyboard(event);
}

void StampsPanel::OnErase(wxEraseEvent& event)
{
	ViewPanel::OnErase(event);
}

void StampsPanel::OnResize(wxSizeEvent& event)
{
	ViewPanel::OnResize(event);
}

void StampsPanel::SetProject(Project* project)
{
	ViewPanel::SetProject(project);
}

void StampsPanel::HandleMouseTileEvent(ion::Vector2 mouseDelta, int buttonBits, int x, int y)
{
	ViewPanel::HandleMouseTileEvent(mouseDelta, buttonBits, x, y);
}

void StampsPanel::RenderCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	ViewPanel::RenderCanvas(renderer, cameraInverseMtx, projectionMtx, z, zOffset);
}