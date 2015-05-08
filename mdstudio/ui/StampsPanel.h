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

#include <stdint.h>

#include "UIBase.h"
#include "../Project.h"

class StampsPanel : public wxGLCanvas
{
public:
	StampsPanel(ion::io::ResourceManager& resourceManager, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~StampsPanel();

	//Events
	void OnMouse(wxMouseEvent& event);
	void OnKeyboard(wxKeyEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnResize(wxSizeEvent& event);

	//Set current project
	void SetProject(Project* project);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

private:
	//Create canvas
	void CreateCanvas();

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

	//Rendering materials and shaders
	ion::io::ResourceHandle<ion::render::Shader> m_vertexShader;
	ion::io::ResourceHandle<ion::render::Shader> m_pixelShader;
	ion::render::Material* m_material;

	//Rendering primitives
	ion::render::Chessboard* m_primitive;
};