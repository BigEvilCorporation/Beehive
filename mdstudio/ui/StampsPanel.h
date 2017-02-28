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

class StampsPanel : public ViewPanel
{
public:
	StampsPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~StampsPanel();

	//Events
	virtual void OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta);
	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnResize(wxSizeEvent& event);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

protected:

	//Mouse click or changed tile callback
	virtual void OnMouseTileEvent(int buttonBits, int x, int y);

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

	//Right-click menu callback
	void OnContextMenuClick(wxCommandEvent& event);

private:

	enum MenuItems
	{
		eMenuRenameStamp,
		eMenuUpdateStamp,
		eMenuUpdatePalette,
		eMenuSubstituteStamp,
		eMenuDeleteStamp,
		eMenuSetStampLowDrawPrio,
		eMenuSetStampHighDrawPrio,
		eMenuSortTilesSequentially,
		eMenuOpenInAnimEditor,
	};

	enum Mode
	{
		eModeSelect,
		eModeSubstitute
	};

	//Paint all stamps using position map to canvas
	void PaintStamps();

	//Recalc all stamp positions and canvas size
	void ArrangeStamps(const ion::Vector2& panelSize);

	//Render selection box
	void RenderBox(const ion::Vector2i& pos, const ion::Vector2& size, const ion::Colour& colour, ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	//Render stamp outlines
	void RenderStampOutlines(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	//Reset zoom/pan
	void ResetZoomPan();

	//Stamp position map
	std::vector< std::pair<StampId, ion::Vector2i> > m_stampPosMap;

	//Current/hover/substitute stamp
	StampId m_selectedStamp;
	StampId m_hoverStamp;
	StampId m_stampToSubstitute;

	//Current selection mode
	Mode m_mode;

	//Current/hover stamp pos
	ion::Vector2i m_selectedStampPos;
	ion::Vector2i m_hoverStampPos;

	//Rendering materials and shaders
	ion::render::Shader* m_selectionVertexShader;
	ion::render::Shader* m_selectionPixelShader;
	ion::render::Material* m_selectionMaterial;

	//Rendering primitives
	ion::render::Quad* m_selectionPrimitive;
};