///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <ion/core/memory/Memory.h>
#include <ion/maths/Vector.h>
#include <ion/renderer/Renderer.h>
#include <ion/renderer/Viewport.h>
#include <ion/renderer/Camera.h>
#include <ion/renderer/Primitive.h>
#include <ion/renderer/Material.h>

#include <wx/event.h>
#include <wx/panel.h>
#include "wx/glcanvas.h"
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>

#include <stdint.h>

#include "UIBase.h"
#include "../Project.h"

class MainWindow;

class ViewPanel : public wxGLCanvas
{
public:
	enum MouseButtons
	{
		eMouseLeft = 1 << 0,
		eMouseMiddle = 1 << 1,
		eMouseRight = 1 << 2
	};

	ViewPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~ViewPanel();

	//Set current project
	virtual void SetProject(Project* project);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

protected:
	//Event callbacks
	virtual void OnMouse(wxMouseEvent& event);
	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnResize(wxSizeEvent& event);

	//Mouse click or changed tile callback
	virtual void OnMouseTileEvent(ion::Vector2 mouseDelta, int buttonBits, int x, int y) {}

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset) {}

	//Create canvas
	void CreateCanvas(int width, int height);

	//Create grid
	void CreateGrid(int width, int height, int cellsX, int cellsY);

	//Create and redraw tileset texture
	void CreateTilesetTexture(const Tileset& tileset);

	//Create TileID to index cache
	void CacheTileIndices();

	//Paint single tile to canvas
	void PaintTile(TileId tileId, int x, int y, u32 flipFlags);

	//Paint stamp to canvas
	void PaintStamp(const Stamp& stamp, int x, int y, u32 flipFlags);

	//Fill selection with single tile
	void FillTiles(TileId tileId, const ion::Vector2i& boxCorner1, const ion::Vector2i& boxCorner2);
	void FillTiles(TileId tileId, const std::vector<ion::Vector2i>& selection);

	//Find bounds from selected tile coords
	void FindBounds(const std::vector<ion::Vector2i>& tiles, int& left, int& top, int& right, int& bottom) const;

	//Get tile index into tileset
	int GetTileIndex(TileId tileId) const;

	//Get tileset UV coords for tile
	void GetTileTexCoords(TileId tileId, ion::render::TexCoord texCoords[4], u32 flipFlags) const;

	//Centre camera on canvas
	void CentreCamera();

	//Rendering
	void RenderCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderGrid(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	
	//Main project
	Project* m_project;

	//Main window
	MainWindow* m_mainWindow;

	//Renderer
	ion::render::Renderer& m_renderer;

	//Viewport
	ion::render::Viewport m_viewport;

	//Camera
	ion::render::Camera m_camera;
	float m_cameraZoom;
	float m_cameraPanSpeed;

	//Mouse
	ion::Vector2 m_mousePrevPos;
	ion::Vector2i m_prevMouseOverTilePos;
	int m_prevMouseBits;

	//Rendering materials and shaders
	ion::render::Shader* m_vertexShader;
	ion::render::Shader* m_pixelShader;
	ion::render::Material* m_canvasMaterial;
	ion::render::Material* m_gridMaterial;

	//Rendering primitives
	ion::render::Chessboard* m_canvasPrimitive;
	ion::render::Grid* m_gridPrimitive;

	//Tileset texture
	ion::render::Texture* m_tilesetTexture;

	//Map tile IDs to indices
	std::map<TileId, u32> m_tileIndexMap;

	//Canvas size (tiles)
	ion::Vector2i m_canvasSize;

	//Tileset size sq
	u32 m_tilesetSizeSq;

	//Tileset texture cell size sq
	float m_cellSizeTexSpaceSq;

	//Prev panel size (for filtering resize events)
	wxSize m_panelSize;

private:

	//Event handlers
	void EventHandlerMouse(wxMouseEvent& event);
	void EventHandlerKeyboard(wxKeyEvent& event);
	void EventHandlerPaint(wxPaintEvent& event);
	void EventHandlerErase(wxEraseEvent& event);
	void EventHandlerResize(wxSizeEvent& event);
};