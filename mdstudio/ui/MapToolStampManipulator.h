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

#include "MapTool.h"
#include "MapToolRegionSelector.h"
#include "Gizmo.h"

#include <ion/maths/Bounds.h>

class MapToolStampManipulator : public wxEvtHandler, Tool
{
public:
	static const ToolType StaticType() { return ToolType::eToolSelectStamp; }

	MapToolStampManipulator(Project& project, MapPanel& mapPanel, TUndoStack& undoStack);

	virtual std::string GetName();
	virtual std::string GetDescription();
	virtual std::string GetStatusText();

	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnMousePixelEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int tileX, int tileY);
	virtual void OnRender(ion::render::Renderer& renderer, RenderResources& renderResources, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

protected:

	enum class ContextMenu
	{
		Delete,
		EditCollision,
	};

	struct Selection
	{
		Selection()
			: stampId(InvalidStampId)
			, stamp(nullptr)
			, stampFlags(0)
			, stampMapEntryIdx(0)
		{ }

		bool operator == (const Selection& rhs) const
		{
			return stampPos == rhs.stampPos;
		}

		StampId stampId;
		Stamp* stamp;
		ion::Vector2i stampPos;
		u32 stampFlags;
		u32 stampMapEntryIdx;
	};

	void SetTooltip(const Selection& stamp, const ion::Vector2i& cursorPosTiles);
	void OnContextMenuClick(wxCommandEvent& event);
	void EnumerateSelection();
	void DeleteSelection();
	void SetupGizmo();

	MapToolRegionSelector m_mapSelector;
	Gizmo m_gizmo;
	Selection m_cursor;
	std::vector<Selection> m_selectedStamps;
	ion::maths::Bounds2i m_selectedBoundsPx;
};