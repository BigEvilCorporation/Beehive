///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
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
		eToolPaintTile,
		eToolPaintStamp,
		eToolClone,
		eToolCreateStamp,
		eToolFill,
		eToolPicker,
		eToolFlipX,
		eToolFlipY
	};

	enum MouseButtons
	{
		eMouseLeft		= 1<<0,
		eMouseMiddle	= 1<<1,
		eMouseRight		= 1<<2
	};

	MapPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~MapPanel();

	//Events
	virtual void OnMouse(wxMouseEvent& event);
	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnResize(wxSizeEvent& event);

	//Set current project
	virtual void SetProject(Project* project);

	//Set current tool
	void SetTool(Tool tool);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

protected:

	//Mouse click or changed tile callback
	virtual void OnMouseTileEvent(ion::Vector2 mouseDelta, int buttonBits, int x, int y);

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

private:

	//Rendering
	void RenderPaintPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderStampPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderSelection(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	//Paint whole map to canvas
	void PaintMap(const Map& map);

	//Create stamp preview primitive
	void CreateStampPreview(Stamp* stamp);

	//Clear all tool data
	void ResetToolData();

	//Rendering materials and shaders
	ion::render::Shader* m_selectionVertexShader;
	ion::render::Shader* m_selectionPixelShader;
	ion::render::Material* m_selectionMaterial;

	//Rendering primitives
	ion::render::Quad* m_previewPrimitive;
	ion::render::Chessboard* m_stampPreviewPrimitive;

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
	// PAINT TILE tool
	///////////////////////////////////////////////////

	//Current preview tile
	TileId m_previewTile;
	ion::Vector2i m_previewTilePos;
	bool m_previewTileFlipX;
	bool m_previewTileFlipY;

	///////////////////////////////////////////////////
	// PAINT STAMP tool
	///////////////////////////////////////////////////

	//Current paste stamp
	StampId m_currentStampId;

	//Temp cloning stamp
	Stamp* m_tempStamp;

	//Paste pos
	ion::Vector2i m_stampPastePos;
};