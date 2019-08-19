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

#include "EditStampCollisionDialog.h"
#include "SpriteCanvas.h"

DialogEditStampCollision::DialogEditStampCollision(wxWindow* parent, Stamp& stamp, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources)
	: DialogEditStampCollisionBase(parent)
	, m_stamp(stamp)
	, m_project(project)
	, m_renderer(renderer)
	, m_renderResources(renderResources)
	, m_glContext(glContext)
{
	m_canvas->SetProject(&project);
	m_canvas->SetupRendering(&renderer, &glContext, &renderResources);

	Draw();
}

DialogEditStampCollision::~DialogEditStampCollision()
{

}

void DialogEditStampCollision::OnToolAddBezier(wxCommandEvent& event)
{
	m_canvas->SetTool(eToolDrawTerrainBezier);
}

void DialogEditStampCollision::OnToolEditBezier(wxCommandEvent& event)
{
	m_canvas->SetTool(eToolSelectTerrainBezier);
}

void DialogEditStampCollision::OnToolDeleteBezier(wxCommandEvent& event)
{
	m_canvas->SetTool(eToolDeleteTerrainBezier);
}

void DialogEditStampCollision::Draw()
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	m_canvas->CreateGrid(m_stamp.GetWidth() * tileWidth, m_stamp.GetHeight() * tileHeight, m_stamp.GetWidth(), m_stamp.GetHeight());
	m_canvas->SetGridColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	m_canvas->SetDrawGrid(true);
	m_canvas->SetStamp(m_stamp, ion::Vector2i());
	m_canvas->Refresh();
}