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

template <typename T> class MapToolManipulator : public wxEvtHandler, public Tool
{
public:
	static const ToolType StaticType() { return ToolType::eToolSelectStamp; }

	MapToolManipulator(Project& project, MapPanel& mapPanel, TUndoStack& undoStack);
	virtual ~MapToolManipulator();

	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnMousePixelEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int tileX, int tileY);
	virtual void OnRender(ion::render::Renderer& renderer, RenderResources& renderResources, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

protected:
	void OnContextMenuClick(wxCommandEvent& event);
	void EnumerateSelection();
	void SetupGizmo();

	void AddContextMenuItem(int id, const std::string& name, std::function<void(int, std::vector<T>&)> const& callback);

	virtual std::string GetTooltipText(const T& object, const ion::Vector2i& cursorPosPx) = 0;

	virtual ion::Vector2i GetUnitSizePx() = 0;
	virtual int GetRegionSelectorFlags() = 0;

	virtual void FindObjects(const Map& map, const std::vector<MapRegion>& regions, std::vector<T>& selection) = 0;
	virtual void MoveObjects(Map& map, std::vector<T>& selection, const ion::Vector2i& delta) = 0;
	virtual void DeleteObjects(Map& map, std::vector<T>& selection) = 0;

private:
	struct ContextMenuItem
	{
		typedef std::function<void(int, std::vector<T>&)> Callback;
		std::string name;
		int id;
		Callback callback;
	};

	void OnContextMenuDelete(int id, std::vector<T>& objects);

	MapToolRegionSelector* m_mapSelector;
	Gizmo m_gizmo;
	T m_objUnderCursor;
	std::vector<T> m_selectedObjs;
	ion::maths::Bounds2i m_selectedBoundsPx;
	std::vector<ContextMenuItem> m_contextMenuItems;
};

#include "MapToolManipulator.inl"