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

#include "ViewPanel.h"

class BlocksPanel : public ViewPanel
{
public:
	BlocksPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~BlocksPanel();

	//Events
	virtual void OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta);
	virtual void OnResize(wxSizeEvent& event);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

	int GetUniqueBlockCount() { return m_blockCache.size(); }

protected:

	//Mouse click or changed tile callback
	virtual void OnMouseTileEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int x, int y);

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

private:

	//Render selection box
	void RenderBox(const ion::Vector2i& pos, const ion::Vector2& size, const ion::Colour& colour, ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	//Paint all blocks to canvas
	void PaintBlocks();

	//Paint block to canvas
	void PaintBlock(const Map::Block& block, int x, int y, int canvasSizeTilesX, int canvasSizeTilesY);

	//Recalc all stamp positions and canvas size
	void ArrangeStamps(const ion::Vector2& panelSize);

	//Reset zoom/pan
	void ResetZoomPan();

	ion::Vector2i m_hoverPos;
	std::vector<const Map::Block*> m_blockCache;
};