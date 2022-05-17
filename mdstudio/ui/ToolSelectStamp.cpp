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
	, m_mapSelector(project, ion::Vector2i(project.GetPlatformConfig().stampWidth * project.GetPlatformConfig().tileWidth, project.GetPlatformConfig().stampHeight * project.GetPlatformConfig().tileHeight), true, true, true)
{

}

void ToolSelectStamp::OnKeyboard(wxKeyEvent& event)
{
	m_mapSelector.OnKeyboard(event);
}

void ToolSelectStamp::OnMousePixelEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int tileX, int tileY)
{
	m_mapSelector.OnMousePixelEvent(mousePos, mouseDelta, tileDelta, buttonBits, tileX, tileY);

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