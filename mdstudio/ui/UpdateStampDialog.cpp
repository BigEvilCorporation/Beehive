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

#include "UpdateStampDialog.h"
#include "SpriteCanvas.h"

DialogUpdateStamp::DialogUpdateStamp(wxWindow* parent, Stamp& stamp, Project& project, ion::render::Renderer& renderer, wxGLContext& glContext, RenderResources& renderResources)
	: DialogUpdateStampBase(parent)
	, m_stampOld(stamp)
	, m_project(project)
	, m_renderer(renderer)
	, m_renderResources(renderResources)
	, m_glContext(glContext)
{
	m_canvasOld->SetProject(&project);
	m_canvasOld->SetupRendering(&renderer, &glContext, &renderResources);
	m_canvasNew->SetProject(&project);
	m_canvasNew->SetupRendering(&renderer, &glContext, &renderResources);

	m_stampNew.Resize(m_stampOld.GetWidth(), m_stampOld.GetHeight(), false, false);

	Draw(&m_stampOld, NULL);
}

DialogUpdateStamp::~DialogUpdateStamp()
{

}

void DialogUpdateStamp::OnFileBrowse(wxFileDirPickerEvent& event)
{
	std::string filename = event.GetPath().GetData().AsChar();

	//Get palette from original stamp
	int paletteIdx = 0;
	TileId firstTile = m_stampOld.GetTile(0, 0);
	if(Tile* tile = m_project.GetTileset().GetTile(firstTile))
	{
		paletteIdx = (int)tile->GetPaletteId();
	}

	//Import bitmap to temp stamp
	u32 flags = Project::eBMPImportReplaceStamp;
	//if(m_chkReplacePalette->GetValue())
	//{
	//	flags |= Project::eBMPImportWholePalette;
	//}

	//'Replace' temp stamp to show preview
	if(m_project.ImportBitmap(filename, flags, 1 << paletteIdx, &m_stampNew))
	{
		//Render
		Draw(&m_stampOld, &m_stampNew);

		//Save settings for 'OK' button
		m_filename = filename;
		m_paletteIdx = paletteIdx;
	}
}

void DialogUpdateStamp::OnBtnOk(wxCommandEvent& event)
{
	//Replace stamp for real
	m_project.ImportBitmap(m_filename, Project::eBMPImportReplaceStamp, 1 << m_paletteIdx, &m_stampOld);

	EndModal(wxID_OK);
}

void DialogUpdateStamp::OnBtnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void DialogUpdateStamp::Draw(Stamp* stampA, Stamp* stampB)
{
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;

	if(stampA)
	{
		m_canvasOld->CreateGrid(stampA->GetWidth() * tileWidth, stampA->GetHeight() * tileHeight, stampA->GetWidth(), stampA->GetHeight());
		m_canvasOld->SetGridColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
		m_canvasOld->SetDrawGrid(true);
		m_canvasOld->SetStamp(*stampA, /* TODO: */ ion::Vector2i());
		m_canvasOld->Refresh();
	}

	if(stampB)
	{
		m_canvasNew->CreateGrid(stampB->GetWidth() * tileWidth, stampB->GetHeight() * tileHeight, stampB->GetWidth(), stampB->GetHeight());
		m_canvasNew->SetGridColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
		m_canvasNew->SetDrawGrid(true);
		m_canvasNew->SetStamp(*stampB, /* TODO: */ ion::Vector2i());
		m_canvasNew->Refresh();
	}
}