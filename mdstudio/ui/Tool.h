///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <ion/maths/Matrix.h>
#include <ion/renderer/Renderer.h>

#include "Mouse.h"
#include "..\Project.h"

class MapPanel;

enum ToolType
{
	eToolSelectTiles,
	eToolSelectStamp,
	eToolPaintTile,
	eToolPaintTerrainTile,
	eToolPaintCollisionTerrain,
	eToolPaintCollisionSolid,
	eToolDeleteTerrainTile,
	eToolDrawTerrainBezier,
	eToolSelectTerrainBezier,
	eToolPaintStamp,
	eToolClone,
	eToolCreateStamp,
	eToolRemoveStamp,
	eToolFill,
	eToolTilePicker,
	eToolStampPicker,
	eToolFlipX,
	eToolFlipY,
	eToolSelectGameObject,
	eToolPlaceGameObject,
	eToolMoveGameObject,
	eToolRemoveGameObject,
	eToolGenerateTerrain
};

class Tool
{
public:
	Tool(ToolType type, Project& project, MapPanel& mapPanel) : m_project(project), m_mapPanel(mapPanel), m_type(type) {}

	//Mouse click or changed tile callback
	virtual void OnMouseTileEvent(int buttonBits, int x, int y) = 0;

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset) = 0;

protected:


private:
	ToolType m_type;
	Project& m_project;
	MapPanel& m_mapPanel;
};