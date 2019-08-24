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
#include "Dialogs.h"
#include "MainWindow.h"
#include "RenderResources.h"

DialogEditStampCollision::DialogEditStampCollision(MainWindow& mainWindow, Stamp& stamp, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources)
	: DialogEditStampCollisionBase(&mainWindow)
	, m_mainWindow(mainWindow)
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

void DialogEditStampCollision::OnToolPaintSolid(wxCommandEvent& event)
{
	m_canvas->SetTool(eToolPaintCollisionSolid);
}

void DialogEditStampCollision::OnToolGenerateTerrain(wxCommandEvent& event)
{
	m_canvas->SetTool(eToolNone);

	DialogTerrainGen dialog(this);

	if (dialog.ShowModal() == wxID_OK)
	{
		int granularity = dialog.m_spinCtrlGranularity->GetValue();

		if (wxMessageBox("This will clear all terrain tiles and regenerate for all stamps, are you sure?", "Generate Terrain", wxOK | wxCANCEL) == wxOK)
		{
			if (!m_project.GenerateTerrainFromBeziers(granularity))
			{
				wxMessageBox("Error generating terrain - out of tile space", "Error", wxOK, this);
			}

			//Refresh all to redraw terrain tiles
			m_mainWindow.RefreshAll();

			//Invalid terrain tiles and refresh stamp canvas
			m_project.InvalidateTerrainTiles(true);
			m_canvas->Refresh();
			m_project.InvalidateTerrainTiles(false);
		}
	}
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