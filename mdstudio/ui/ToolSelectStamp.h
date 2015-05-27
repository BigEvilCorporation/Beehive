///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "Tool.h"

class ToolSelectStamp : public Tool
{
public:
	ToolSelectStamp(MapPanel& mapPanel);

	//Mouse click or changed tile callback
	virtual void OnMouseTileEvent(ion::Vector2 mouseDelta, int buttonBits, int x, int y) = 0;

	//Render callback
	virtual void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset) = 0;
};