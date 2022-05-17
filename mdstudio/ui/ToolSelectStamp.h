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

#pragma once

#include "Tool.h"
#include "ToolMapSelector.h"

class ToolSelectStamp : public Tool
{
public:
	static const ToolType StaticType() { return ToolType::eToolSelectStamp; }

	ToolSelectStamp(Project& project, MapPanel& mapPanel, TUndoStack& undoStack);

	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnMousePixelEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int tileX, int tileY);
	virtual void OnRender(ion::render::Renderer& renderer, RenderResources& renderResources, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

protected:
	struct Selection
	{
		Selection()
			: stampId(InvalidStampId)
			, stampFlags(0)
			, stampMapEntry(0)
		{ }

		StampId stampId;
		ion::Vector2i stampPos;
		u32 stampFlags;
		u32 stampMapEntry;
	};

	MapSelector m_mapSelector;
	Selection m_hovered;
	Selection m_selected;
};