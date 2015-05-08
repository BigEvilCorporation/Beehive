///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "ViewPanel.h"

class MapPanel : public ViewPanel
{
public:
	enum Tool
	{
		eToolSelect,
		eToolPaint,
		eToolPicker,
		eToolFlipX,
		eToolFlipY,
		eToolFill,
		eToolClone
	};

	enum MouseButtons
	{
		eMouseLeft		= 1<<0,
		eMouseMiddle	= 1<<1,
		eMouseRight		= 1<<2
	};

	MapPanel(ion::io::ResourceManager& resourceManager, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~MapPanel();

	//Events
	virtual void OnMouse(wxMouseEvent& event);
	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnErase(wxEraseEvent& event);
	virtual void OnResize(wxSizeEvent& event);

	//Set current project
	void SetProject(Project* project);

	//Set current tool
	void SetTool(Tool tool);

private:

	//Clear all tool data
	void ResetToolData();

	//Mouse click or changed tile
	virtual void HandleMouseTileEvent(ion::Vector2 mouseDelta, int buttonBits, int x, int y);

	virtual void RenderCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

	//Rendering materials and shaders
	ion::io::ResourceHandle<ion::render::Shader> m_selectionVertexShader;
	ion::io::ResourceHandle<ion::render::Shader> m_selectionPixelShader;
	ion::render::Material* m_selectionMaterial;

	//Rendering primitives
	//ion::render::Chessboard* m_mapPrimitive;
	//ion::render::Grid* m_gridPrimitive;
	ion::render::Quad* m_previewPrimitive;
	ion::render::Chessboard* m_clonePreviewPrimitive;

	//Rendering colours
	ion::Colour m_previewColour;
	ion::Colour m_clonePreviewColour;
	ion::Colour m_boxSelectColour;

	//Current tool
	Tool m_currentTool;

	///////////////////////////////////////////////////
	// SELECT tool
	///////////////////////////////////////////////////

	//Selected tiles
	std::vector<ion::Vector2i> m_selectedTiles;

	//Multiple (CTRL) selection
	bool m_multipleSelection;

	//Box selection
	ion::Vector2i m_boxSelectStart;
	ion::Vector2i m_boxSelectEnd;

	///////////////////////////////////////////////////
	// PAINT tool
	///////////////////////////////////////////////////

	//Current preview tile
	TileId m_previewTile;
	ion::Vector2i m_previewTilePos;
	bool m_previewTileFlipX;
	bool m_previewTileFlipY;

	///////////////////////////////////////////////////
	// CLONE tool
	///////////////////////////////////////////////////

	//Clipboard stamp
	Stamp* m_clipboard;
	ion::Vector2i m_clonePastePos;
};