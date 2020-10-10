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
#include "Gizmo.h"

class MapPanel : public ViewPanel
{
public:

	MapPanel(MainWindow* mainWindow, Project& project, ion::render::Renderer& renderer, wxGLContext* glContext, RenderResources& renderResources, wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual ~MapPanel();

	//Events
	virtual void OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta);
	virtual void OnKeyboard(wxKeyEvent& event);
	virtual void OnResize(wxSizeEvent& event);

	//Camera control
	void CameraCentreOnObject(const GameObject& gameObject);
	ion::Vector2 GetCameraPos() const;

	//Set current tool
	void SetTool(ToolType tool);

	//Set selection
	void SelectGameObject(GameObjectId gameObjectId);

	//Set editing prefab
	void SetEditingPrefab(GameObjectId instanceId);

	GameObjectId FindGameObject(int x, int y, ion::Vector2i& topLeft) const;
	int FindGameObjects(int x, int y, int width, int height, std::vector<const GameObjectMapEntry*>& gameObjects) const;

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

protected:

	//Mouse click or changed tile callback
	virtual void OnMouseTileEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int x, int y);

	//Mouse click or changed pixel callback
	virtual void OnMousePixelEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int tileX, int tileY);

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);

private:

	enum ContextMenuItems
	{
		eContextMenuDeleteStamp,
		eContextMenuEditStampCollision,
#if !BEEHIVE_FIXED_STAMP_MODE //No tile/collision editing in fixed mode
		eContextMenuBakeStamp,
		eContextMenuStampBringToFront,
		eContextMenuStampSendToBack,
#endif
		eContextMenuGameObjAddToAnim,
		eContextMenuGameObjCreatePrefab,
		eContextMenuSetTerrainFlagSpecial,
		eContextMenuSetTerrainFlagWater,
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

	//Rendering
	void RenderPaintPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderStampPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderStampOutlines(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderStampGrid(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderPhysicsWorldOutline(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderDisplayFrame(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderBoxSelection(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderStampSelection(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderCollisionCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderCollisionBeziers(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderTerrainCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderGameObjects(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderGameObjectPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderReferenceImage(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	//Paint whole map to canvas
	void PaintMap(const Map& map);

	//Paint whole collision map to canvas
	void PaintCollisionMap(const Map& map, const CollisionMap& collisionMap);

	//Paint terrain beziers
	void PaintTerrainBeziers(Project& project);

	//Create stamp preview primitive
	void CreateStampPreview(Stamp* stamp);

	//Bucket fill (recursive)
	void BucketFill(Map& map, ion::Vector2i position, ion::Vector2i prevPosition, TileId originalTile, TileId newTile);

	//Clear all tool data
	void ResetToolData();

	//On right-click menu click
	void OnContextMenuClick(wxCommandEvent& event);

	//Set gizmo pos
	void SetGizmoCentre(const ion::Vector2i& centre);

	//Rendering primitives
	ion::render::Chessboard* m_stampPreviewPrimitive;
	std::vector<ion::render::Primitive*> m_primitiveBeziers;
	ion::render::Primitive* m_primitiveBezierPoints;
	ion::render::Primitive* m_primitiveBezierHandles;

	//Current tool
	ToolType m_currentTool;

	//Current prefab
	GameObjectId m_editingPrefabInstanceId;

	//Current cursor origin mode
	CursorOrigin m_cursorOrigin;

	//Move gizmo
	Gizmo m_gizmo;

	//Gizmo cursors
	wxCursor* m_cursorHorizontal;
	wxCursor* m_cursorVertical;

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
	std::vector<GameObjectId> m_selectedGameObjects;

	//Multiple (CTRL) selection
	bool m_multipleSelection;

	//Box selection
	bool m_boxSelection;
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
	GameObjectArchetypeId m_previewGameObjectArchetype;
	ion::Vector2i m_previewGameObjectPos;
	bool m_moveGameObjByPixel;

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
	u32 m_highlightedBezierIdx;
	int m_currentBezierControlIdx;
	eBezierModifyHandle m_currentBezierControlHndl;
	ion::Vector2 m_currentBezierControlPos;
};