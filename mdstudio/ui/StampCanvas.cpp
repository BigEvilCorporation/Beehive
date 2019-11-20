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

#include "StampCanvas.h"

#include <ion/maths/Geometry.h>
#include <ion/core/utils/STL.h>

StampCanvas::StampCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: SpriteCanvas(parent, id, pos, size, style, name)
{
	m_terrainCanvasPrimitive = NULL;
	m_collisionCanvasPrimitive = NULL;
	m_primitiveBezierPoints = NULL;
	m_primitiveBezierHandles = NULL;
	m_primitiveBezierNormals = NULL;
	m_highlightedBezier = NULL;
	m_currentBezier = NULL;
	m_currentBezierControlIdx = -1;
	m_currentTool = eToolNone;
	m_stamp = NULL;
}

StampCanvas::~StampCanvas()
{
	if (m_tileFramePrimitive)
		delete m_tileFramePrimitive;
	if (m_terrainCanvasPrimitive)
		delete m_terrainCanvasPrimitive;
	if (m_collisionCanvasPrimitive)
		delete m_collisionCanvasPrimitive;
	if (m_primitiveBezierPoints)
		delete m_primitiveBezierPoints;
	if (m_primitiveBezierHandles)
		delete m_primitiveBezierHandles;

	if (m_currentBezier)
	{
		m_currentBezier->CalculateBounds();
	}
}

void StampCanvas::Refresh(bool eraseBackground, const wxRect *rect)
{
	if (m_stamp)
	{
		//If collision tilset invalidated
		if (m_project->TerrainTilesAreInvalidated())
		{
			//Redraw collision map
			PaintCollisionStamp(*m_stamp);
		}

		//If terrain beziers invalidated
		if (m_project->TerrainBeziersAreInvalidated())
		{
			PaintTerrainBeziers(*m_stamp);
		}
	}

	SpriteCanvas::Refresh(eraseBackground, rect);
}

void StampCanvas::SetStamp(Stamp& stamp, const ion::Vector2i& offset)
{
	m_drawOffset = offset;

	if (m_tileFramePrimitive)
		delete m_tileFramePrimitive;
	if (m_terrainCanvasPrimitive)
		delete m_terrainCanvasPrimitive;
	if (m_collisionCanvasPrimitive)
		delete m_collisionCanvasPrimitive;

	const int tileWidth = m_project->GetPlatformConfig().tileWidth;
	const int tileHeight = m_project->GetPlatformConfig().tileHeight;
	const int width = stamp.GetWidth();
	const int height = stamp.GetHeight();

	m_canvasSize.x = width;
	m_canvasSize.y = height;

	m_tileFramePrimitive = new ion::render::Chessboard(ion::render::Chessboard::xy, ion::Vector2((float)width * (tileWidth / 2.0f), (float)height * (tileHeight / 2.0f)), width, height, true);
	m_terrainCanvasPrimitive = new ion::render::Chessboard(ion::render::Chessboard::xy, ion::Vector2((float)(width * tileWidth) / 2.0f, (float)(height * tileHeight) / 2.0f), width, height, true);
	m_collisionCanvasPrimitive = new ion::render::Chessboard(ion::render::Chessboard::xy, ion::Vector2((float)(width * tileWidth) / 2.0f, (float)(height * tileHeight) / 2.0f), width, height, true);

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			TileId tileId = stamp.GetTile(x, y);
			u32 tileFlags = stamp.GetTileFlags(x, y);
			int y_inv = height - 1 - y;

			//Set texture coords for cell
			ion::render::TexCoord coords[4];
			m_renderResources->GetTileTexCoords(tileId, coords, tileFlags);
			m_tileFramePrimitive->SetTexCoords((y_inv * width) + x, coords);
		}
	}

	m_stamp = &stamp;

	PaintTerrainBeziers(stamp);
	PaintCollisionStamp(stamp);

	Refresh();
}

void StampCanvas::SetTool(ToolType tool)
{
	ToolType previousTool = m_currentTool;
	m_currentTool = tool;

	switch (previousTool)
	{
		case eToolDrawTerrainBezier:
		{
			if (m_currentBezier)
			{
				m_currentBezier->CalculateBounds();
			}
			break;
		}
	}

	//Invalidate bezier
	m_currentBezier = NULL;
	m_highlightedBezier = NULL;
	m_currentBezierControlIdx = -1;

	if (m_primitiveBezierPoints)
	{
		delete m_primitiveBezierPoints;
		m_primitiveBezierPoints = NULL;
	}

	if (m_primitiveBezierHandles)
	{
		delete m_primitiveBezierHandles;
		m_primitiveBezierHandles = NULL;
	}

	Refresh();
}

void StampCanvas::OnKeyboard(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_ESCAPE || event.GetKeyCode() == WXK_SPACE || event.GetKeyCode() == WXK_RETURN)
	{
		SetTool(eToolNone);
		Refresh();
	}

	SpriteCanvas::OnKeyboard(event);
}

void StampCanvas::OnMouseTileEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int x, int y)
{
	if (m_stamp)
	{
		const int stampWidth = m_stamp->GetWidth();
		const int stampHeight = m_stamp->GetHeight();
		const int tileWidth = m_project->GetPlatformConfig().tileWidth;
		const int tileHeight = m_project->GetPlatformConfig().tileHeight;

		//Invert for OpenGL
		int y_inv = (stampHeight - 1 - y);

		//Check in map range
		bool inMaprange = ((x >= 0) && (x < stampWidth) && (y >= 0) && (y < stampHeight));

		TerrainTileId hoverTileId = inMaprange ? m_stamp->GetTerrainTile(x, y) : InvalidTerrainTileId;

		switch (m_currentTool)
		{
			case eToolNone:
			{
				if (const TerrainTile* tile = m_project->GetTerrainTileset().GetTerrainTile(hoverTileId))
				{
					float radians = tile->GetAngleRadians();
					float degrees = tile->GetAngleDegrees();
					u8 angleByte = tile->GetAngleByte();
					int quadrant = ion::maths::Round(degrees / 90.0f) % 4;

					std::stringstream tipStr;
					tipStr << "Cursor: 0x" << SSTREAM_HEX2(x) << ", 0x" << SSTREAM_HEX2(y) << " (" << x << ", " << y << ")" << std::endl;
					tipStr << "Tile: 0x" << SSTREAM_HEX4(hoverTileId) << " (" << hoverTileId << ")" << std::endl;
					tipStr << "Angle: " << radians << " (deg: " << degrees << ", hex: 0x" << SSTREAM_HEX2(angleByte) << ", quadrant: " << quadrant << ")" << std::endl;
					tipStr << "Normal: " << tile->GetNormal().x << ", " << tile->GetNormal().y << std::endl;
					tipStr << "Addr: 0x" << SSTREAM_HEX8(hoverTileId * tileWidth) << std::endl;
					tipStr << "Data:" << std::endl;
					tipStr << " H: ";

					for (int i = 0; i < tileWidth; i++)
					{
						tipStr << SSTREAM_HEX2(tile->GetHeight(i)) << " ";
					}

					tipStr << std::endl;

					tipStr << " W: ";

					for (int i = 0; i < tileHeight; i++)
					{
						tipStr << SSTREAM_HEX2(tile->GetWidth(i)) << " ";
					}

					tipStr << std::endl;

					SetToolTip(tipStr.str().c_str());
				}
				else
				{
					UnsetToolTip();
				}
			}
		}
	}
}

void StampCanvas::OnMousePixelEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int tileX, int tileY)
{
	if (m_stamp)
	{
		const int stampWidth = m_stamp->GetWidth();
		const int stampHeight = m_stamp->GetHeight();
		const int tileWidth = m_project->GetPlatformConfig().tileWidth;
		const int tileHeight = m_project->GetPlatformConfig().tileHeight;

		//Invert for OpenGL
		ion::Vector2 mousePosF((float)mousePos.x, (float)(stampHeight * tileHeight) - (float)mousePos.y);
		int y_inv = (stampHeight - 1 - tileY);

		bool inRange = ((tileX >= 0) && (tileX < stampWidth) && (tileY >= 0) && (tileY < stampHeight));

		switch (m_currentTool)
		{
			case eToolPaintCollisionSolid:
			{
				if (inRange)
				{
					//If clicking/dragging, paint solid collision tile
					if (buttonBits & eMouseLeft)
					{
						//Set solid tile
						u16 collisionTileFlags = m_stamp->GetCollisionTileFlags(tileX, tileY) | eCollisionTileFlagSolid;
						m_stamp->SetCollisionTileFlags(tileX, tileY, collisionTileFlags);

						//Paint to canvas
						PaintCollisionTile(m_stamp->GetTerrainTile(tileX, tileY), tileX, y_inv, collisionTileFlags);
						Refresh();
					}
					else if (buttonBits & eMouseRight)
					{
						//Clear solid tile
						u16 collisionTileFlags = m_stamp->GetCollisionTileFlags(tileX, tileY) & ~eCollisionTileFlagSolid;
						m_stamp->SetCollisionTileFlags(tileX, tileY, collisionTileFlags);

						//Paint to canvas
						PaintCollisionTile(m_stamp->GetTerrainTile(tileX, tileY), tileX, y_inv, collisionTileFlags);
						Refresh();
					}
				}

				break;
			}

			case eToolDrawTerrainBezier:
			{
				const float boxHalfExtents = 2.0f;

				//Check if touching an existing control handle
				bool redraw = false;
				if (m_currentBezier)
				{
					ion::Vector2 position;
					ion::Vector2 controlA;
					ion::Vector2 controlB;

					if (!(m_prevMouseBits & eMouseLeft))
					{
						//Not previously clicking, determine control point under cursor
						m_currentBezierControlIdx = -1;
						const int numPoints = m_currentBezier->GetNumPoints();

						for (int i = 0; i < numPoints && m_currentBezierControlIdx == -1; i++)
						{
							//Ignore first and last control handles (not drawn)
							bool modifyHandleA = (i != 0);
							bool modifyHandleB = (i != (numPoints - 1));

							m_currentBezier->GetPoint(i, position, controlA, controlB);

							const ion::Vector2 boxMinP(position.x - boxHalfExtents, position.y - boxHalfExtents);
							const ion::Vector2 boxMaxP(position.x + boxHalfExtents, position.y + boxHalfExtents);
							const ion::Vector2 boxMinA(controlA.x + position.x - boxHalfExtents, controlA.y + position.y - boxHalfExtents);
							const ion::Vector2 boxMaxA(controlA.x + position.x + boxHalfExtents, controlA.y + position.y + boxHalfExtents);
							const ion::Vector2 boxMinB(controlB.x + position.x - boxHalfExtents, controlB.y + position.y - boxHalfExtents);
							const ion::Vector2 boxMaxB(controlB.x + position.x + boxHalfExtents, controlB.y + position.y + boxHalfExtents);

							if (ion::maths::PointInsideBox(mousePosF, boxMinP, boxMaxP))
							{
								//Touching position handle
								m_currentBezierControlPos = position;
								m_currentBezierControlIdx = i;
								m_currentBezierControlHndl = eBezierPosition;
							}
							else if (modifyHandleA && ion::maths::PointInsideBox(mousePosF, boxMinA, boxMaxA))
							{
								//Touching control A handle
								m_currentBezierControlPos = controlA + position;
								m_currentBezierControlIdx = i;
								m_currentBezierControlHndl = eBezierControlA;
							}
							else if (modifyHandleB && ion::maths::PointInsideBox(mousePosF, boxMinB, boxMaxB))
							{
								//Touching control B handle
								m_currentBezierControlPos = controlB + position;
								m_currentBezierControlIdx = i;
								m_currentBezierControlHndl = eBezierControlB;
							}

							//Refresh this panel to redraw preview
							Refresh();
						}
					}

					if ((m_currentBezierControlIdx != -1))
					{
						if (buttonBits & eMouseLeft)
						{
							//Control point is under cursor, and left button is held
							m_currentBezier->GetPoint(m_currentBezierControlIdx, position, controlA, controlB);

							//Set new point/control handle position
							switch (m_currentBezierControlHndl)
							{
							case eBezierPosition:
								position = mousePosF;
								break;
							case eBezierControlA:
								controlA = mousePosF - position;
								break;
							case eBezierControlB:
								controlB = mousePosF - position;
								break;
							}

							//Set new preview pos
							m_currentBezierControlPos = mousePosF;

							//Set new point
							m_currentBezier->SetPoint(m_currentBezierControlIdx, position, controlA, controlB);
							redraw = true;
						}
						else if ((buttonBits & eMouseRight) && !(m_prevMouseBits & eMouseRight))
						{
							//Control point is under cursor, and right button was clicked
							if (m_currentBezierControlHndl == eBezierPosition)
							{
								//Remove point
								m_currentBezier->RemovePoint(m_currentBezierControlIdx);
								redraw = true;
							}
						}
					}
				}

				//If not handling a control point
				if (m_currentBezierControlIdx == -1)
				{
					if ((buttonBits & eMouseLeft) && !(m_prevMouseBits & eMouseLeft))
					{
						//If no current bezier, create new
						if (!m_currentBezier)
						{
							m_currentBezier = m_stamp->AddTerrainBezier();
						}

						//If new point is to the right of the prev, point handles -1,1 on X axis, else 1,-1
						ion::Vector2 prevPosition(0.0f, 0.0f);
						ion::Vector2 prevControlA(0.0f, 0.0f);
						ion::Vector2 prevControlB(0.0f, 0.0f);

						int numExistingPoints = m_currentBezier->GetNumPoints();

						if (numExistingPoints > 0)
						{
							m_currentBezier->GetPoint(numExistingPoints - 1, prevPosition, prevControlA, prevControlB);
						}

						const float defaultControlLen = (mousePos.x > prevPosition.x) ? 10.0f : -10.0f;

						m_currentBezier->AddPoint(ion::Vector2(mousePos.x, (stampHeight * tileHeight) - mousePos.y), ion::Vector2(-defaultControlLen, 0.0f), ion::Vector2(defaultControlLen, 0.0f));
						redraw = true;
					}
					else if ((buttonBits & eMouseRight) && !(m_prevMouseBits & eMouseRight))
					{
						//Finalise bezier
						SetTool(eToolNone);
						redraw = true;
					}
				}

				if (redraw)
				{
					//Invalidate beziers and refresh this panel
					PaintTerrainBeziers(*m_stamp);
					Refresh();
				}

				break;
			}

			case eToolSelectTerrainBezier:
			case eToolDeleteTerrainBezier:
			{
				m_currentBezier = NULL;

				//Find bezier(s) under cursor
				ion::gamekit::BezierPath* smallestBezier = NULL;
				float smallestBezierSize = ion::maths::FLOAT_MAX;
				int smallestBezierIndex = 0;

				for (int i = 0; i < m_stamp->GetNumTerrainBeziers(); i++)
				{
					ion::gamekit::BezierPath* bezier = m_stamp->GetTerrainBezier(i);

					ion::Vector2 boundsMin;
					ion::Vector2 boundsMax;

					bezier->GetBounds(boundsMin, boundsMax);

					//Ensure bounds are at least 2 tiles thick
					if ((ion::maths::Abs(boundsMax.x - boundsMin.x) / (float)tileWidth) < 2.0f)
					{
						boundsMin.x -= (float)tileWidth;
						boundsMax.x += (float)tileWidth;
					}

					if ((ion::maths::Abs(boundsMax.y - boundsMin.y) / (float)tileHeight) < 2.0f)
					{
						boundsMin.y -= (float)tileHeight;
						boundsMax.y += (float)tileHeight;
					}

					if (ion::maths::PointInsideBox(mousePosF, boundsMin, boundsMax))
					{
						//Found a bezier, check if smallest
						float size = (boundsMax - boundsMin).GetLength();

						if (size < smallestBezierSize)
						{
							smallestBezier = bezier;
							smallestBezierSize = size;
							smallestBezierIndex = i;
						}
					}
				}

				ion::gamekit::BezierPath* highlightedBezier = nullptr;
				u32 highlightedBezierIdx = 0;

				if (smallestBezier)
				{
					if (buttonBits & eMouseLeft)
					{
						if (m_currentTool == eToolSelectTerrainBezier)
						{
							//Set current bezier
							m_currentBezier = smallestBezier;

							//Set bezier draw tool
							m_currentTool = eToolDrawTerrainBezier;
						}
						else if (m_currentTool == eToolDeleteTerrainBezier)
						{
							//Delete bezier
							m_stamp->RemoveTerrainBezier(smallestBezierIndex);
						}

						//Invalidate beziers and refresh this panel
						PaintTerrainBeziers(*m_stamp);
						Refresh();
					}
					else
					{
						highlightedBezier = smallestBezier;
						highlightedBezierIdx = smallestBezierIndex;
					}
				}

				//Set highlighted bezier
				if (m_highlightedBezier != highlightedBezier)
				{
					m_highlightedBezier = highlightedBezier;
					m_highlightedBezierIdx = highlightedBezierIdx;
					Refresh();
				}

				if ((buttonBits & eMouseRight) && (m_currentTool == eToolSelectTerrainBezier) && m_highlightedBezier)
				{
					//Right-click menu
					wxMenu contextMenu;

					for (int i = 0; i < s_maxTerrainLayers; i++)
					{
						std::string label = "Terrain layer " + std::to_string(i);
						wxMenuItem* item = contextMenu.Append(eContextMenuSetTerrainLayerFirst + i, wxString(label.c_str()));
						item->SetCheckable(true);
						item->Check(m_stamp->GetTerrainBezierLayer(m_highlightedBezierIdx) == i);
					}

					wxMenuItem* item = contextMenu.Append(eContextMenuSetTerrainGenerateWidthData, "Generate width data");
					item->SetCheckable(true);
					item->Check(m_stamp->GetTerrainBezierGenerateWidth(m_highlightedBezierIdx));

					contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&StampCanvas::OnContextMenuClick, NULL, this);
					PopupMenu(&contextMenu);
				}

				break;
			}
		}
	}
}

void StampCanvas::OnContextMenuClick(wxCommandEvent& event)
{
	if (event.GetId() >= eContextMenuSetTerrainLayerFirst && event.GetId() < eContextMenuSetTerrainLayerLast)
	{
		int layer = (event.GetId() - eContextMenuSetTerrainLayerFirst);
		m_stamp->SetTerrainBezierLayer(m_highlightedBezierIdx, layer);
	}
	else if (event.GetId() == eContextMenuSetTerrainGenerateWidthData)
	{
		m_stamp->SetTerrainBezierGenerateWidth(m_highlightedBezierIdx, event.IsChecked());
	}
}

void StampCanvas::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	//Render tile frame
	RenderTileFrame(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;

	//Render preview
	RenderPreview(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;

	//Render grid
	if (m_project->GetShowGrid())
	{
		RenderGrid(renderer, cameraInverseMtx, projectionMtx, z);
		z += zOffset;
	}

	//Render collision
	RenderCollisionCanvas(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;

	//Render terrain
	RenderTerrainCanvas(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;

	//Render beziers
	RenderCollisionBeziers(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;
}

void StampCanvas::RenderCollisionBeziers(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//No depth test
	renderer.SetDepthTest(ion::render::Renderer::eAlways);

	ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialFlatColour);

	const float stampWidth = m_stamp->GetWidth();
	const float stampHeight = m_stamp->GetHeight();
	const float tileWidth = m_project->GetPlatformConfig().tileWidth;
	const float tileHeight = m_project->GetPlatformConfig().tileHeight;

	ion::Matrix4 bezierMatrix;
	bezierMatrix.SetTranslation(ion::Vector3(-(stampWidth * tileWidth) / 2.0f, -(stampHeight * tileHeight) / 2.0f, z));

	//Draw curves
	if (m_primitiveBeziers.size() > 0)
	{
		material->SetDiffuseColour(ion::Colour(1.0f, 0.4f, 0.4f, 1.0f));
		material->Bind(bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(3.0f);

		for (int i = 0; i < m_primitiveBeziers.size(); i++)
		{
			renderer.DrawVertexBuffer(m_primitiveBeziers[i]->GetVertexBuffer());
		}

		renderer.SetLineWidth(1.0f);
		material->Unbind();
	}

	//Draw curve points
	if (m_primitiveBezierPoints)
	{
		material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
		material->Bind(bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(2.0f);
		renderer.DrawVertexBuffer(m_primitiveBezierPoints->GetVertexBuffer());
		renderer.SetLineWidth(1.0f);
		material->Unbind();
	}

	//Draw curve control handles
	if (m_primitiveBezierHandles)
	{
		material->SetDiffuseColour(ion::Colour(1.0f, 0.6f, 0.6f, 1.0f));
		material->Bind(bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(2.0f);
		renderer.DrawVertexBuffer(m_primitiveBezierHandles->GetVertexBuffer());
		renderer.SetLineWidth(1.0f);
		material->Unbind();
	}

	//Draw normals
	if (m_primitiveBezierNormals)
	{
		material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 0.0f, 1.0f));
		material->Bind(bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(2.0f);
		renderer.DrawVertexBuffer(m_primitiveBezierNormals->GetVertexBuffer());
		renderer.SetLineWidth(1.0f);
		material->Unbind();
	}

	//Draw selected handle
	if (m_currentBezierControlIdx != -1)
	{
		ion::render::Primitive* primitive = m_renderResources->GetPrimitive(RenderResources::ePrimitiveTileQuad);
		const ion::Colour& colour = m_renderResources->GetColour(RenderResources::eColourSelected);

		renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
		material->SetDiffuseColour(colour);

		const float x = m_currentBezierControlPos.x;
		const float y = m_currentBezierControlPos.y;
		const float width = m_currentBezierControlHndl == eBezierPosition ? 1.0f : 0.5f;
		const float height = m_currentBezierControlHndl == eBezierPosition ? 1.0f : 0.5f;

		ion::Vector3 previewScale(width, height, 1.0f);
		ion::Matrix4 previewMtx;
		ion::Vector3 previewPos(x, y, z);
		previewMtx.SetTranslation(previewPos);
		previewMtx.SetScale(previewScale);

		material->Bind(previewMtx * bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		material->Unbind();

		renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);
	}

	//Draw selected bezier
	if (m_highlightedBezier)
	{
		ion::render::Primitive* primitive = m_renderResources->GetPrimitive(RenderResources::ePrimitiveTileQuad);
		const ion::Colour& colour = m_renderResources->GetColour(RenderResources::eColourSelected);

		renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
		material->SetDiffuseColour(colour);

		ion::Vector2 boundsMin;
		ion::Vector2 boundsMax;

		m_highlightedBezier->GetBounds(boundsMin, boundsMax);

		//Ensure bounds are at least 2 tiles thick
		if ((ion::maths::Abs(boundsMax.x - boundsMin.x) / (float)tileWidth) < 2.0f)
		{
			boundsMin.x -= (float)tileWidth;
			boundsMax.x += (float)tileWidth;
		}

		if ((ion::maths::Abs(boundsMax.y - boundsMin.y) / (float)tileHeight) < 2.0f)
		{
			boundsMin.y -= (float)tileHeight;
			boundsMax.y += (float)tileHeight;
		}

		ion::Vector2 size = boundsMax - boundsMin;

		const float x = boundsMin.x + (size.x / 2.0f);
		const float y = boundsMin.y + (size.y / 2.0f);

		ion::Vector3 previewScale(size.x / (float)tileWidth, size.y / (float)tileWidth, 1.0f);
		ion::Matrix4 previewMtx;
		ion::Vector3 previewPos(x, y, z);
		previewMtx.SetTranslation(previewPos);
		previewMtx.SetScale(previewScale);

		material->Bind(previewMtx * bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		material->Unbind();

		renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);
	}

	renderer.SetDepthTest(ion::render::Renderer::eLessEqual);
}

void StampCanvas::RenderTerrainCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//No depth test (stops grid cells Z fighting)
	renderer.SetDepthTest(ion::render::Renderer::eAlways);

	renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);

	//Draw terrain heightmaps
	{
		ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialTerrainTilesetHeight);
		material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
		material->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(m_terrainCanvasPrimitive->GetVertexBuffer(), m_terrainCanvasPrimitive->GetIndexBuffer());
		material->Unbind();
	}

	//Draw terrain widthmaps
	{
		ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialTerrainTilesetWidth);
		material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
		material->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(m_terrainCanvasPrimitive->GetVertexBuffer(), m_terrainCanvasPrimitive->GetIndexBuffer());
		material->Unbind();
	}

	renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);

	renderer.SetDepthTest(ion::render::Renderer::eLessEqual);
}

void StampCanvas::RenderCollisionCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//No depth test (stops grid cells Z fighting)
	renderer.SetDepthTest(ion::render::Renderer::eAlways);

	ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialCollisionTypes);

	//Draw map
	renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	material->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_collisionCanvasPrimitive->GetVertexBuffer(), m_collisionCanvasPrimitive->GetIndexBuffer());
	material->Unbind();
	renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);

	renderer.SetDepthTest(ion::render::Renderer::eLessEqual);
}

void StampCanvas::PaintCollisionStamp(const Stamp& stamp)
{
	int width = stamp.GetWidth();
	int height = stamp.GetHeight();

	//Paint all collision tiles
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			//Get terrain tile and collision flags
			TerrainTileId terrainTileId = stamp.GetTerrainTile(x, y);
			u16 collisionFlags = stamp.GetCollisionTileFlags(x, y);

			//Invert Y for OpenGL
			int yInv = height - 1 - y;

			//Paint tile
			PaintCollisionTile(terrainTileId, x, yInv, collisionFlags);
		}
	}
}

void StampCanvas::PaintCollisionTile(TerrainTileId terrainTileId, int x, int y, u16 collisionFlags)
{
	//Set texture coords for terrain cell
	ion::render::TexCoord coords[4];
	m_renderResources->GetTerrainTileTexCoords(terrainTileId, coords);
	m_terrainCanvasPrimitive->SetTexCoords((y * m_canvasSize.x) + x, coords);

	//Set texture coords for collision cell
	m_renderResources->GetCollisionTypeTexCoords(collisionFlags, coords);
	m_collisionCanvasPrimitive->SetTexCoords((y * m_canvasSize.x) + x, coords);
}

void StampCanvas::PaintTerrainBeziers(const Stamp& stamp)
{
	for (int i = 0; i < m_primitiveBeziers.size(); i++)
	{
		delete m_primitiveBeziers[i];
	}

	m_primitiveBeziers.clear();

	if (m_primitiveBezierPoints)
	{
		delete m_primitiveBezierPoints;
		m_primitiveBezierPoints = NULL;
	}

	if (m_primitiveBezierHandles)
	{
		delete m_primitiveBezierHandles;
		m_primitiveBezierHandles = NULL;
	}

	if (m_primitiveBezierNormals)
	{
		delete m_primitiveBezierNormals;
		m_primitiveBezierNormals = NULL;
	}

	for (int i = 0; i < stamp.GetNumTerrainBeziers(); i++)
	{
		const ion::gamekit::BezierPath* bezier = stamp.GetTerrainBezier(i);
		m_primitiveBeziers.push_back(m_renderResources->CreateBezierPrimitive(*bezier));
	}

	if (m_currentBezier && m_currentBezier->GetNumPoints() > 0)
	{
		m_primitiveBezierPoints = m_renderResources->CreateBezierPointsPrimitive(*m_currentBezier, 2.0f);
		m_primitiveBezierHandles = m_renderResources->CreateBezierHandlesPrimitive(*m_currentBezier, 1.0f);
		m_primitiveBezierNormals = m_renderResources->CreateBezierNormalsPrimitive(*m_currentBezier, 10.0f, 10.0f);
	}
}