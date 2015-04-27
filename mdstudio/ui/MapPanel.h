///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <ion/maths/Vector.h>
#include <ion/io/ResourceManager.h>
#include <ion/renderer/Renderer.h>
#include <ion/renderer/Camera.h>
#include <ion/renderer/Primitive.h>
#include <ion/renderer/Material.h>

#include <wx/event.h>
#include <wx/panel.h>
#include "wx/glcanvas.h"
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>

#include "UIBase.h"
#include "../Project.h"

class MapPanel : public wxGLCanvas
{
public:
	MapPanel(ion::io::ResourceManager& resourceManager, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~MapPanel();

	//Events
	void OnMouse(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnResize(wxSizeEvent& event);

	//Set current project
	void SetProject(Project* project);

	virtual void Refresh(bool eraseBackground=true, const wxRect *rect=NULL);

private:
	//Create canvas
	void CreateCanvas();

	//Create grid
	void CreateGrid();

	//Create and redraw tileset texture
	void CreateTilesetTexture();

	//Create TileID to index cache
	void CacheTileIndices();

	//Paint whole map to canvas
	void PaintWholeMap();

	//Paint single tile to canvas
	void PaintTile(TileId tileId, int x, int y);

	//Centre camera on canvas
	void CentreCamera();

	//Resource manager
	ion::io::ResourceManager& m_resourceManager;

	//Main project
	Project* m_project;

	//OpenGL context
	wxGLContext* m_context;

	//Renderer
	ion::render::Renderer* m_renderer;

	//Camera
	ion::render::Camera* m_camera;
	float m_cameraZoom;
	float m_cameraPanSpeed;

	//Rendering materials and shaders
	ion::io::ResourceHandle<ion::render::Shader> m_vertexShader;
	ion::io::ResourceHandle<ion::render::Shader> m_pixelShader;
	ion::render::Material* m_mapMaterial;
	ion::render::Material* m_gridMaterial;

	//Rendering primitives
	ion::render::Chessboard* m_mapPrimitive;
	ion::render::Grid* m_gridPrimitive;

	//Tileset texture
	ion::io::ResourceHandle<ion::render::Texture> m_tilesetTextureHndl;

	//Map tile IDs to indices
	std::map<TileId, u32> m_tileIndexMap;

	//Tileset size sq
	u32 m_tilesetSizeSq;

	//Tileset texture cell size sq
	float m_cellSizeTexSpaceSq;

	//Grid
	int m_gridSize;
	bool m_snapToGrid;

	//For mouse delta
	ion::Vector2 m_mousePrevPos;
};