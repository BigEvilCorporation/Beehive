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

#include <ion/maths/Vector.h>
#include <ion/maths/Matrix.h>
#include <ion/maths/Geometry.h>

#include <ion/renderer/Renderer.h>
#include <ion/renderer/Colour.h>
#include <ion/renderer/Primitive.h>
#include <ion/renderer/Material.h>

#include <functional>

#include "RenderResources.h"
#include "Mouse.h"

class Gizmo
{
public:
	enum class Constraint
	{
		None,
		All,
		Horizontal,
		Vertical
	};

	enum class Action
	{
		None,
		ConstraintChanged,
		Started,
		Dragging,
		Finished
	};

	Gizmo();
	~Gizmo();

	void SetEnabled(bool enabled);
	bool IsEnabled() const;

	void SetObjectPosition(const ion::Vector2i& position);
	const ion::Vector2i& GetObjectPosition() const;
	const ion::Vector2i& GetGizmoPosition() const;
	const ion::Vector2i& GetMoveStartPosition() const;

	const ion::Vector2i& GetLastDelta() const;
	const ion::Vector2i& GetMoveDelta() const;

	void SetSensitivity(float unitsPerPixel);
	float GetSensitivity() const;

	Constraint GetCurrentConstraint() const;

	Action Update(const ion::Vector2i& position, const ion::Vector2i& delta, int buttonBits, float cameraZoom, const ion::Vector2i& mapSizePx);
	void OnRender(ion::render::Renderer& renderer, RenderResources& renderResources, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float cameraZoom, const ion::Vector2i& mapSizePx);

private:
	static const float s_drawLineWidth;
	static const float s_drawAxisLength;
	static const float s_drawTriangleSize;
	static const float s_drawBoxSize;
	static const float s_drawScale;
	static const ion::Colour s_drawColourAxisX;
	static const ion::Colour s_drawColourAxisY;
	static const ion::Colour s_drawColourBox;
	static const ion::Colour s_drawColourSelected;

	bool m_enabled;

	ion::Vector2i m_objectPosition;
	ion::Vector2i m_gizmoPosition;
	ion::Vector2i m_lastDelta;
	ion::Vector2i m_moveStartPosition;
	ion::Vector2i m_moveDelta;
	ion::Vector2 m_accumulatedDelta;
	int m_prevMouseButtonBits;
	Constraint m_hoverConstraint;
	Constraint m_currentConstraint;
	float m_sensitivity;
	int m_prevMouseBits;

	ion::render::LineSegments* m_unitLineX;
	ion::render::LineSegments* m_unitLineY;
	ion::render::Primitive* m_unitTriangleX;
	ion::render::Primitive* m_unitTriangleY;
	ion::render::Quad* m_unitBox;
};