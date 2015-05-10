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

class ViewPanel : public wxGLCanvas
{
public:
	enum MouseButtons
	{
		eMouseLeft = 1 << 0,
		eMouseMiddle = 1 << 1,
		eMouseRight = 1 << 2
	};

	ViewPanel(ion::render::Renderer& renderer, wxGLContext* glContext, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~ViewPanel();

	//Set current project
	virtual void SetProject(Project* project);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

protected:
	//Event callbacks
	virtual void OnMouse(wxMouseEvent& event);
	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnPaint(wxPaintEvent& event);
	virtual void OnErase(wxEraseEvent& event);
	virtual void OnResize(wxSizeEvent& event);

	//Mouse click or changed tile
	virtual void HandleMouseTileEvent(ion::Vector2 mouseDelta, int buttonBits, int x, int y) {}

	//Render to canvas callback (between map and grid)
	virtual void RenderCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset) {}

	//Paint whole map to canvas
	void PaintMap(const Map& map);

	//Paint single tile to canvas
	void PaintTile(TileId tileId, int x, int y, bool flipX, bool flipY);

	//Paint stamp to canvas
	void PaintStamp(const Stamp& stamp, int x, int y);

	//Fill selection with single tile
	void FillTiles(TileId tileId, const ion::Vector2i& boxCorner1, const ion::Vector2i& boxCorner2);
	void FillTiles(TileId tileId, const std::vector<ion::Vector2i>& selection);

	//Find bounds from selected tile coords
	void FindBounds(const std::vector<ion::Vector2i>& tiles, int& left, int& top, int& right, int& bottom) const;

	//Get tile index into tileset
	int GetTileIndex(TileId tileId) const;

	//Get tileset UV coords for tile
	void GetTileTexCoords(TileId tileId, ion::render::TexCoord texCoords[4], bool flipX, bool flipY) const;

	//Centre camera on canvas
	void CentreCamera();

	//Rendering
	void RenderMap(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderGrid(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	
	//Main project
	Project* m_project;

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
	ion::render::Material* m_material;
	ion::render::Material* m_gridMaterial;

	//Rendering primitives
	ion::render::Chessboard* m_primitive;
	ion::render::Grid* m_gridPrimitive;

	//Tileset texture
	ion::render::Texture* m_tilesetTexture;

	//Map tile IDs to indices
	std::map<TileId, u32> m_tileIndexMap;

	//Tileset size sq
	u32 m_tilesetSizeSq;

	//Tileset texture cell size sq
	float m_cellSizeTexSpaceSq;

	//Prev panel size (for filtering resize events)
	wxSize m_panelSize;

private:
	//Create canvas
	void CreateCanvas(int width, int height);

	//Create grid
	void CreateGrid(int width, int height, int cellsX, int cellsY);

	//Create and redraw tileset texture
	void CreateTilesetTexture(const Tileset& tileset);

	//Create TileID to index cache
	void CacheTileIndices();

	//Event handlers
	void EventHandlerMouse(wxMouseEvent& event);
	void EventHandlerKeyboard(wxKeyEvent& event);
	void EventHandlerPaint(wxPaintEvent& event);
	void EventHandlerErase(wxEraseEvent& event);
	void EventHandlerResize(wxSizeEvent& event);
};