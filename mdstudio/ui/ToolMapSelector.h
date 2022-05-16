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

struct MapSelection
{
	MapSelection() {}
	MapSelection(ion::Vector2i _topLeft, ion::Vector2i _bottomRight)
		: topLeft(_topLeft)
		, bottomRight(_bottomRight)
	{
	}

	ion::Vector2i topLeft;
	ion::Vector2i bottomRight;
};

class MapSelector
{
public:
	MapSelector(Project& project, bool allowMultipleSelection, bool allowBoxSelection);

	void OnKeyboard(wxKeyEvent& event);
	void OnMouseTileEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int x, int y);
	void OnRender(ion::render::Renderer& renderer, RenderResources& renderResources, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

	ion::Vector2i GetHoverPos() const;
	const MapSelection& GetSelections() const;
	bool NeedsRedraw() const { return m_needsRedraw; }

private:
	void SanitiseBoxOrder(const ion::Vector2i& boxStart, const ion::Vector2i& boxEnd, ion::Vector2i& topLeft, ion::Vector2i& bottomRight);
	ion::Matrix4 CalcBoxDrawMatrix(const ion::Vector2i& boxStart, const ion::Vector2i& boxEnd, float mapWidth, float mapHeight, float z);

	Project& m_project;
	bool m_allowMultipleSelection;
	bool m_allowBoxSelection;
	bool m_inMultipleSelection;
	bool m_inBoxSelection;
	ion::Vector2i m_hoverPos;
	ion::Vector2i m_selectionStart;
	ion::Vector2i m_selectionEnd;
	std::vector<MapSelection> m_selections;
	int m_prevMouseBits;
	bool m_needsRedraw;
};