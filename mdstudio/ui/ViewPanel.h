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

#include <ion/core/memory/Memory.h>
#include <ion/maths/Vector.h>
#include <ion/renderer/Renderer.h>
#include <ion/renderer/Viewport.h>
#include <ion/renderer/Camera.h>
#include <ion/renderer/Primitive.h>
#include <ion/renderer/Material.h>
#include <ion/beehive/Project.h>

#include <wx/event.h>
#include <wx/panel.h>
#include <wx/glcanvas.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>

#include <stdint.h>

#include "UIBase.h"
#include "Mouse.h"
#include "Tool.h"
#include "RenderResources.h"

class MainWindow;

class ViewPanel : public wxGLCanvas
{
public:
	ViewPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, wxGLAttributes& glAttributes, RenderResources& renderResources, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~ViewPanel();

	//Refresh panel
	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

	//Always take focus, even if we have child widgets
	virtual bool AcceptsFocus() { return true; }

	//Allow default zoom/pan behaviour
	void EnableZoom(bool enabled) { m_enableZoom = enabled; }
	void EnablePan(bool enabled) { m_enablePan = enabled; }

	//Centre camera on canvas
	void CentreCamera();

protected:
	//Event callbacks
	virtual void OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta);
	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnResize(wxSizeEvent& event);

	//Mouse click or changed tile callback
	virtual void OnMouseTileEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int x, int y) {}

	//Mouse click or changed pixel callback
	virtual void OnMousePixelEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int tileX, int tileY) {}

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset) {}

	//Create canvas
	void CreateCanvas(int width, int height);

	//Create collision canvas
	void CreateCollisionCanvas(int width, int height);

	//Create grid
	void CreateGrid(int width, int height, int cellsX, int cellsY);

	//Paint single tile to canvas
	void PaintTile(TileId tileId, int x, int y, u32 flipFlags);

	//Paint collision tile to canvas
	void PaintCollisionTile(TerrainTileId terrainTileId, int x, int y, u16 collisionFlags);

	//Paint stamp to canvas
	void PaintStamp(const Stamp& stamp, int x, int y, u32 flipFlags);

	//Paint stamp collision to canvas
	void PaintStampCollision(const Stamp& stamp, int x, int y, u32 flipFlags);

	//Fill selection with single tile
	void FillTiles(TileId tileId, const ion::Vector2i& boxCorner1, const ion::Vector2i& boxCorner2);
	void FillTiles(TileId tileId, const std::vector<ion::Vector2i>& selection);

	//Find bounds from selected tile coords
	void FindBounds(const std::vector<ion::Vector2i>& tiles, int& left, int& top, int& right, int& bottom) const;

	//Set camera zoom
	void SetCameraZoom(float zoom);

	//Rendering
	void RenderCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderGrid(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	
	//Main project
	Project& m_project;

	//Main window
	MainWindow* m_mainWindow;

	//Renderer
	ion::render::Renderer& m_renderer;

	//Rendering resources
	RenderResources& m_renderResources;

	//GL context
	wxGLContext* m_glContext;

	//Viewport
	ion::render::Viewport m_viewport;

	//Camera
	ion::render::Camera m_camera;
	float m_cameraZoom;
	float m_cameraPanSpeed;

	//Mouse
	ion::Vector2i m_mousePrevPos;
	ion::Vector2i m_prevMouseOverTilePos;
	ion::Vector2i m_prevMouseOverPixelPos;
	int m_prevMouseBits;

	//Rendering primitives
	ion::render::Chessboard* m_canvasPrimitive;
	ion::render::Chessboard* m_terrainCanvasPrimitive;
	ion::render::Chessboard* m_collisionCanvasPrimitive;
	ion::render::Grid* m_gridPrimitive;

	bool m_canvasPrimitiveDirty;
	bool m_terrainCanvasDirty;
	bool m_collisionCanvasDirty;

	//Canvas size (tiles)
	ion::Vector2i m_canvasSize;

	//Panel size (pixels)
	ion::Vector2i m_panelSize;

	//Prev panel size (for filtering resize events)
	ion::Vector2i m_prevPanelSize;

private:

	//Event handlers
	void EventHandlerMouse(wxMouseEvent& event);
	void EventHandlerKeyboard(wxKeyEvent& event);
	void EventHandlerPaint(wxPaintEvent& event);
	void EventHandlerErase(wxEraseEvent& event);
	void EventHandlerResize(wxSizeEvent& event);

	bool m_enableZoom;
	bool m_enablePan;
};