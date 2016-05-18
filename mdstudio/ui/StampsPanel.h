///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "ViewPanel.h"

class StampsPanel : public ViewPanel
{
public:
	StampsPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~StampsPanel();

	//Events
	virtual void OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta);
	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnResize(wxSizeEvent& event);

	//Set current project
	virtual void SetProject(Project* project);

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
		eMenuDeleteStamp
	};

	//Paint all stamps using position map to canvas
	void PaintStamps();

	//Recalc all stamp positions and canvas size
	void ArrangeStamps(const ion::Vector2& panelSize);

	//Render selection box
	void RenderBox(const ion::Vector2i& pos, const ion::Vector2& size, const ion::Colour& colour, ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	//Reset zoom/pan
	void ResetZoomPan();

	//Stamp position map
	std::vector< std::pair<StampId, ion::Vector2i> > m_stampPosMap;

	//Current/hover stamp
	StampId m_selectedStamp;
	StampId m_hoverStamp;

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