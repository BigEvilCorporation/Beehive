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

#include "RenderResources.h"

#include <ion/renderer/Renderer.h>
#include <ion/renderer/Viewport.h>
#include <ion/renderer/Camera.h>
#include <ion/renderer/Primitive.h>
#include <ion/renderer/Material.h>

#include <wx/wx.h>
#include <wx/glcanvas.h>

class SpriteCanvas : public wxGLCanvas
{
public:
	SpriteCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxFrameNameStr);
	virtual ~SpriteCanvas();

	void SetProject(Project* project) { m_project = project; }
	void SetupRendering(ion::render::Renderer* renderer, wxGLContext* glContext, RenderResources* renderResources);
	void SetSpriteSheetDimentionsPixels(const ion::Vector2i& spriteSheetDimentions);
	void SetSpriteSheetDimentionsCells(const ion::Vector2i& spriteSheetDimentionsCells);

	//Grid
	void CreateGrid(int width, int height, int cellsX, int cellsY);
	void SetGridColour(const ion::Colour& colour);

	void SetDrawPreview(bool drawPreview, u32 maxFrames);
	void SetDrawGrid(bool drawGrid);
	void SetDrawSpriteSheet(SpriteSheetId spriteSheet, u32 frame, const ion::Vector2i& size, const ion::Vector2i& offset, const ion::Vector2i& topLeft, const ion::Vector2i& bottomRight);

	//Refresh panel
	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);
	virtual void OnResize(wxSizeEvent& event);
	virtual void OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta);

protected:
	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

	//Event handlers
	void EventHandlerPaint(wxPaintEvent& event);
	void EventHandlerResize(wxSizeEvent& event);
	void EventHandlerMouse(wxMouseEvent& event);

	//Mouse click or changed tile callback
	virtual void OnMouseTileEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int x, int y) {}

	//Mouse click or changed pixel callback
	virtual void OnMousePixelEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int tileX, int tileY) {}

	//Rendering
	void RenderSpriteSheet(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderTileFrame(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderGrid(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderBounds(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	void SetCameraZoom(float zoom);

	Project* m_project;

	//Rendering resources
	ion::render::Renderer* m_renderer;
	ion::render::Camera m_camera;
	ion::render::Viewport m_viewport;
	RenderResources* m_renderResources;

	//Rendering primitives
	ion::render::Grid* m_gridPrimitive;
	ion::render::LineQuad* m_boundsPrimitive;
	ion::render::Chessboard* m_tileFramePrimitive;

	bool m_drawPreview;
	u32 m_drawPreviewMaxFrames;
	SpriteSheetId m_drawSpriteSheet;
	u32 m_drawSpriteSheetFrame;
	ion::Vector2i m_drawOffset;
	ion::Vector2i m_topLeft;
	ion::Vector2i m_bottomRight;
	bool m_drawGrid;

	//Mouse
	ion::Vector2i m_mousePrevPos;
	ion::Vector2i m_prevMouseOverTilePos;
	ion::Vector2i m_prevMouseOverPixelPos;
	int m_prevMouseBits;
	float m_cameraZoom;

	//Canvas size (tiles)
	ion::Vector2i m_canvasSize;

	//Panel size (pixels)
	ion::Vector2i m_panelSize;

	//Prev panel size (for filtering resize events)
	ion::Vector2i m_prevPanelSize;

	//SpriteSheet sheet dimentions
	ion::Vector2i m_spriteSheetDimentions;
	ion::Vector2i m_spriteSheetDimentionsCells;

	//Grid colour
	ion::Colour m_gridColour;
};
