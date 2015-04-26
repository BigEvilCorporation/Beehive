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

class MapPanel : public wxGLCanvas //wxPanel
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
	//Paint tile to canvas
	void PaintTile(ion::Vector2 mousePos, TileId tile);

	//Paint whole map to dc
	void PaintMapToDc(wxMemoryDC& dc);

	//Centre camera on canvas
	void CentreCamera();

	//Resource manager
	ion::io::ResourceManager& m_resourceManager;

	//Main project
	Project* m_project;

	//Local drawing canvas
	wxBitmap m_canvas;

	//OpenGL context
	wxGLContext* m_context;

	//Renderer
	ion::render::Renderer* m_renderer;

	//Camera
	ion::render::Camera* m_camera;

	//Rendering material and shaders
	ion::io::ResourceHandle<ion::render::Shader> m_vertexShader;
	ion::io::ResourceHandle<ion::render::Shader> m_pixelShader;
	ion::render::Material* m_material;

	//Rendering primitive
	ion::render::Grid* m_primitive;

	//Tileset texture
	ion::render::Texture* m_tilesetTexture;
	ion::io::ResourceHandle<ion::render::Texture> m_tilesetTextureHndl;

	//Camera
	ion::Vector2 m_cameraPos;
	float m_cameraZoom;
	float m_cameraPanSpeed;

	//For mouse delta
	ion::Vector2 m_mousePrevPos;
};