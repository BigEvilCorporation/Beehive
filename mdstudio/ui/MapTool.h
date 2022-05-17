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

#include <ion/maths/Matrix.h>
#include <ion/renderer/Renderer.h>
#include <ion/beehive/Project.h>

#include <wx/event.h>

#include "Mouse.h"
#include "RenderResources.h"

class MapPanel;

enum ToolType
{
	eToolNone,

#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
	eToolPaintTile,
	eToolPaintTerrainTile,
	eToolPaintCollisionTerrain,
	eToolDeleteTerrainTile,
	eToolClone,
	eToolCreateStamp,
	eToolFill,
	eToolFlipX,
	eToolFlipY,
	eToolGenerateTerrain,
	eToolCopyToNewMap,
#endif

	//Tiles
	eToolSelectTiles,
	eToolTilePicker,

	//Stamps
	eToolSelectStamp,
	eToolPaintStamp,
	eToolMoveStamp,
	eToolRemoveStamp,
	eToolStampPicker,
	eToolCreateStampAnim,

	//Game objects
	eToolSelectGameObject,
	eToolPlaceGameObject,
	eToolDrawGameObject,
	eToolAnimateGameObject,

	//Collision
	eToolDrawTerrainBezier,
	eToolSelectTerrainBezier,
	eToolDeleteTerrainBezier,
	eToolPaintCollisionSolid,
	eToolPaintCollisionHole,

	eToolTypeCount
};

typedef std::vector<std::pair<ToolType, ion::io::MemoryStream*>> TUndoStack;

class Tool
{
public:
	Tool(ToolType type, Project& project, MapPanel& mapPanel, TUndoStack& undoStack)
		: m_project(project)
		, m_mapPanel(mapPanel)
		, m_type(type)
		, m_undoStack(undoStack)
		, m_needsRedraw(false)
	{
	}

	//Use help text
	virtual std::string GetName() = 0;
	virtual std::string GetDescription() = 0;
	virtual std::string GetStatusText() = 0;

	//TODO - populate toolboxes automatically
	// //virtual something GetDefaultToolbox() = 0;
	//virtual something GetDefaultIcon() = 0;

	//Input callbacks
	virtual void OnKeyboard(wxKeyEvent& event) {}
	virtual void OnMouseTileEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int x, int y) {}
	virtual void OnMousePixelEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int tileX, int tileY) {}

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, RenderResources& renderResources, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

	//Reverses an action from serialised stream
	virtual void UndoAction(const ion::io::MemoryStream& stream) {}

	//Triggers redraw
	bool NeedsRedraw() const { return m_needsRedraw; }

protected:
	//Pushes serialised action to undo stack
	void PushAction(ion::io::MemoryStream* stream)
	{
		m_undoStack.push_back(std::make_pair(m_type, stream));
	}

	bool Redraw() { m_needsRedraw = true; }

	Project& m_project;
	MapPanel& m_mapPanel;
	TUndoStack& m_undoStack;

private:
	ToolType m_type;
	bool m_needsRedraw;
};

class ToolFactory
{
public:
	template <typename T> void RegisterTool(Project& project, MapPanel& mapPanel, TUndoStack& undoStack)
	{
		if (m_tools.size() == 0)
			m_tools.resize(ToolType::eToolTypeCount);
		m_tools[T::StaticType()] = (Tool*)new T(project, mapPanel, undoStack);
	}

	template <typename T> T* GetTool()
	{
		return (T*)m_tools[T::StaticType()];
	}

	Tool* GetTool(ToolType type)
	{
		return m_tools[type];
	}

private:
	std::vector<Tool*> m_tools;
};