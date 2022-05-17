///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2019 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#pragma once

#include "SpriteCanvas.h"
#include "MapTool.h"

#include <ion/renderer/Renderer.h>
#include <ion/renderer/Viewport.h>
#include <ion/renderer/Camera.h>
#include <ion/renderer/Primitive.h>
#include <ion/renderer/Material.h>
#include <ion/gamekit/Bezier.h>
#include <ion/beehive/Stamp.h>

class StampCanvas : public SpriteCanvas
{
public:
	StampCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxFrameNameStr);
	virtual ~StampCanvas();

	//Set current stamp
	void SetStamp(Stamp& stamp, const ion::Vector2i& offset);

	//Set current tool
	void SetTool(ToolType tool);

	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);

private:
	static const int s_maxTerrainLayers = 2;

	enum ContextMenuItems
	{
		eContextMenuSetTerrainFlagSpecial,
		eContextMenuSetTerrainFlagWater,
		eContextMenuSetTerrainGenerateWidthData,
		eContextMenuSetTerrainLayerFirst,
		eContextMenuSetTerrainLayerLast = eContextMenuSetTerrainLayerFirst + s_maxTerrainLayers,
	};

	//Mouse click or changed tile callback
	virtual void OnMouseTileEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int x, int y);

	//Mouse click or changed pixel callback
	virtual void OnMousePixelEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int tileX, int tileY);

	//To end/cancel current tool
	virtual void OnKeyboard(wxKeyEvent& event);

	//On right-click menu click
	void OnContextMenuClick(wxCommandEvent& event);

	//Rendering
	void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset);
	void RenderCollisionBeziers(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderTerrainCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);
	void RenderCollisionCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z);

	//Canvas drawing
	void PaintTerrainBeziers(const Stamp& stamp);
	void PaintCollisionStamp(const Stamp& stamp);
	void PaintCollisionTile(TerrainTileId terrainTileId, int x, int y, u16 collisionFlags);

	//Rendering primitives
	ion::render::Chessboard* m_terrainCanvasPrimitive;
	ion::render::Chessboard* m_collisionCanvasPrimitive;
	std::vector<ion::render::Primitive*> m_primitiveBeziers;
	ion::render::Primitive* m_primitiveBezierPoints;
	ion::render::Primitive* m_primitiveBezierHandles;
	ion::render::Primitive* m_primitiveBezierNormals;

	bool m_terrainPrimitiveDirty;
	bool m_collisionPrimitiveDirty;

	//Beziers
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

	//Currently editing stamp
	Stamp* m_stamp;

	//Current tool
	ToolType m_currentToolType;
};