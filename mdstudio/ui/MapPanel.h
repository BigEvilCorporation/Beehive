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

#include <ion/maths/Geometry.h>
#include <ion/gamekit/Bezier.h>
#include "ViewPanel.h"

class MapPanel : public ViewPanel
{
public:

	MapPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~MapPanel();

	//Events
	virtual void OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta);
	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnResize(wxSizeEvent& event);

	//Set current tool
	void SetTool(ToolType tool);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

protected:

	//Mouse click or changed tile callback
	virtual void OnMouseTileEvent(int buttonBits, int x, int y);

	//Mouse click or changed pixel callback
	virtual void OnMousePixelEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, int buttonBits, int tileX, int tileY);

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

private:

	enum ContextMenuItems
	{
		eContextMenuDeleteStamp,
		eContextMenuBakeStamp,
		eContextMenuGameObjAddToAnim,
	};

	enum CursorOrigin
	{
		eCursorTopLeft,
		eCursorCentred,
		eCursorTopRight,
		eCursorBottomLeft,
		eCursorBottomRight,
		eCursor_MAX
	};

	//Create collision canvas
	void CreateCollisionCanvas(int width, int height);

	//Rendering
	void RenderPaintPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderStampPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderStampOutlines(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderTileSelection(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderStampSelection(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderCollisionCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderCollisionBeziers(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderTerrainCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderGameObjects(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderGameObjectPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	//Paint whole map to canvas
	void PaintMap(const Map& map);

	//Paint whole collision map to canvas
	void PaintCollisionMap(const CollisionMap& map);

	//Paint collision tile to canvas
	void PaintCollisionTile(TerrainTileId terrainTileId, u16 collisionFlags, int x, int y);

	//Paint terrain beziers
	void PaintTerrainBeziers(Project& project);

	//Create stamp preview primitive
	void CreateStampPreview(Stamp* stamp);

	//Clear all tool data
	void ResetToolData();

	//On right-click menu click
	void OnContextMenuClick(wxCommandEvent& event);

	//Rendering primitives
	ion::render::Chessboard* m_terrainCanvasPrimitive;
	ion::render::Chessboard* m_collisionCanvasPrimitive;
	ion::render::Chessboard* m_stampPreviewPrimitive;
	std::vector<ion::render::Primitive*> m_primitiveBeziers;
	ion::render::Primitive* m_primitiveBezierPoints;
	ion::render::Primitive* m_primitiveBezierHandles;

	//Current tool
	ToolType m_currentTool;

	//Current cursor origin mode
	CursorOrigin m_cursorOrigin;

	///////////////////////////////////////////////////
	// SELECT tool
	///////////////////////////////////////////////////

	//Selected tiles
	std::vector<ion::Vector2i> m_selectedTiles;

	//Selected stamp
	StampId m_hoverStamp;
	StampId m_selectedStamp;
	ion::Vector2i m_hoverStampPos;
	ion::Vector2i m_selectedStampPos;
	u32 m_hoverStampFlags;
	u32 m_hoverStampMapEntry;

	//Selected game object
	GameObjectId m_hoverGameObject;
	GameObjectId m_selectedGameObject;

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

	//Temp cloning stamp
	Stamp* m_tempStamp;

	//Paste pos
	ion::Vector2i m_stampPastePos;

	///////////////////////////////////////////////////
	// PLACE GAME OBJECT tool
	///////////////////////////////////////////////////

	//Current preview gameobj
	GameObjectTypeId m_previewGameObjectType;
	ion::Vector2i m_previewGameObjectPos;

	///TERRAIN BEZIER tool
	///////////////////////////////////////////////////
	enum eBezierModifyHandle
	{
		eBezierPosition,
		eBezierControlA,
		eBezierControlB
	};

	ion::gamekit::BezierPath* m_currentBezier;
	ion::gamekit::BezierPath* m_highlightedBezier;
	int m_currentBezierControlIdx;
	eBezierModifyHandle m_currentBezierControlHndl;
	ion::Vector2 m_currentBezierControlPos;
};