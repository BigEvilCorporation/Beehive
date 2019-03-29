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

#include <ion/maths/Matrix.h>
#include <ion/renderer/Renderer.h>
#include <ion/beehive/Project.h>

#include "Mouse.h"

class MapPanel;

enum ToolType
{
	eToolSelectTiles,
	eToolSelectStamp,
	eToolPaintTile,
	eToolPaintTerrainTile,
	eToolPaintCollisionTerrain,
	eToolPaintCollisionSolid,
	eToolPaintCollisionHole,
	eToolDeleteTerrainTile,
	eToolDrawTerrainBezier,
	eToolSelectTerrainBezier,
	eToolDeleteTerrainBezier,
	eToolPaintStamp,
	eToolClone,
	eToolCreateStamp,
	eToolRemoveStamp,
	eToolMoveStamp,
	eToolFill,
	eToolTilePicker,
	eToolStampPicker,
	eToolFlipX,
	eToolFlipY,
	eToolSelectGameObject,
	eToolPlaceGameObject,
	eToolDrawGameObject,
	eToolMoveGameObject,
	eToolAnimateGameObject,
	eToolRemoveGameObject,
	eToolGenerateTerrain,
	eToolCreateStampAnim,
	eToolCopyToNewMap
};

class Tool
{
public:
	Tool(ToolType type, Project& project, MapPanel& mapPanel) : m_project(project), m_mapPanel(mapPanel), m_type(type) {}

	//Mouse click or changed tile callback
	virtual void OnMouseTileEvent(int buttonBits, ion::Vector2i tileDelta, int x, int y) = 0;

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset) = 0;

protected:


private:
	ToolType m_type;
	Project& m_project;
	MapPanel& m_mapPanel;
};