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

	Gizmo(RenderResources& renderResources);
	~Gizmo();

	void SetEnabled(bool enabled);
	bool IsEnabled() const;

	void SetPosition(const ion::Vector2i& position);
	const ion::Vector2i& GetPosition() const;

	const ion::Vector2i& GetLastDelta() const;

	void SetSensitivity(float unitsPerPixel);
	float GetSensitivity() const;

	Constraint GetCurrentConstraint() const;

	void OnMouse(const ion::Vector2i& position, const ion::Vector2i& delta, int buttonBits, float cameraZoom, const ion::Vector2i& mapSizePx);
	void OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, const ion::Vector2i& mapSizePx);

private:
	static const float s_drawLineWidth;
	static const float s_drawAxisLength;
	static const float s_drawTriangleSize;
	static const float s_drawBoxSize;
	static const ion::Colour s_drawColourAxisX;
	static const ion::Colour s_drawColourAxisY;
	static const ion::Colour s_drawColourBox;

	bool m_enabled;

	ion::Vector2i m_position;
	ion::Vector2i m_lastDelta;
	ion::Vector2i m_moveStartPosition;
	ion::Vector2 m_accumulatedDelta;
	int m_prevMouseButtonBits;
	Constraint m_currentConstraint;
	float m_sensitivity;

	RenderResources& m_renderResources;
	ion::render::LineSegments* m_unitLineX;
	ion::render::LineSegments* m_unitLineY;
	ion::render::Primitive* m_unitTriangleX;
	ion::render::Primitive* m_unitTriangleY;
	ion::render::Quad* m_unitBox;
};