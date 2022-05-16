///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#include "ToolSelectStamp.h"

ToolSelectStamp::ToolSelectStamp(Project& project, MapPanel& mapPanel, TUndoStack& undoStack)
	: Tool(StaticType(), project, mapPanel, undoStack)
	, m_mapSelector(project, true, true)
{

}

void ToolSelectStamp::OnKeyboard(wxKeyEvent& event)
{
	m_mapSelector.OnKeyboard(event);
}

void ToolSelectStamp::OnMouseTileEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int x, int y)
{
	m_mapSelector.OnMouseTileEvent(mousePos, mouseDelta, tileDelta, buttonBits, x, y);

	if (m_mapSelector.NeedsRedraw())
	{
		Redraw();
	}
}


void ToolSelectStamp::OnRender(ion::render::Renderer& renderer, RenderResources& renderResources, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	m_mapSelector.OnRender(renderer, renderResources, cameraInverseMtx, projectionMtx, z, zOffset);
	Tool::OnRender(renderer, renderResources, cameraInverseMtx, projectionMtx, z, zOffset);
}