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

#include "Gizmo.h"

const float Gizmo::s_drawLineWidth = 4.0f;
const float Gizmo::s_drawAxisLength = 24.0f;
const float Gizmo::s_drawTriangleSize = 8.0f;
const float Gizmo::s_drawBoxSize = 8.0f;
const float Gizmo::s_drawScale = 2.0f;
const ion::Colour Gizmo::s_drawColourAxisX = ion::Colour(1.0f, 0.0f, 0.0f, 1.0f);
const ion::Colour Gizmo::s_drawColourAxisY = ion::Colour(0.0f, 1.0f, 0.0f, 1.0f);
const ion::Colour Gizmo::s_drawColourBox = ion::Colour(0.8f, 0.8f, 0.8f, 0.5f);
const ion::Colour Gizmo::s_drawColourSelected = ion::Colour(1.0f, 1.0f, 1.0f, 1.0f);

Gizmo::Gizmo()
{
	m_sensitivity = 1.0f;
	m_enabled = true;
	m_hoverConstraint = Constraint::None;
	m_currentConstraint = Constraint::None;
	m_prevMouseBits = 0;

	std::vector<ion::Vector3> lineX;
	std::vector<ion::Vector3> lineY;
	std::vector<ion::Vector3> triX;
	std::vector<ion::Vector3> triY;

	lineX.push_back(ion::Vector3(0.0f, 0.0f, 0.0f));
	lineX.push_back(ion::Vector3(s_drawAxisLength, 0.0f, 0.0f));

	lineY.push_back(ion::Vector3(0.0f, 0.0f, 0.0f));
	lineY.push_back(ion::Vector3(0.0f, s_drawAxisLength, 0.0f));

	float extent = s_drawTriangleSize / 2.0f;
	float offsetX = s_drawAxisLength;
	float offsetY = s_drawAxisLength;

	triX.push_back(ion::Vector3(offsetX + extent, 0.0f, 0.0f));
	triX.push_back(ion::Vector3(offsetX - extent, extent, 0.0f));
	triX.push_back(ion::Vector3(offsetX - extent, -extent, 0.0f));

	triY.push_back(ion::Vector3(0.0f, offsetY + extent, 0.0f));
	triY.push_back(ion::Vector3(-extent, offsetY - extent, 0.0f));
	triY.push_back(ion::Vector3(extent, offsetY - extent, 0.0f));

	m_unitLineX = new ion::render::LineSegments(lineX);
	m_unitLineY = new ion::render::LineSegments(lineY);
	m_unitTriangleX = new ion::render::Primitive(ion::render::VertexBuffer::Pattern::Triangles);
	m_unitTriangleY = new ion::render::Primitive(ion::render::VertexBuffer::Pattern::Triangles);
	m_unitBox = new ion::render::Quad(ion::render::Quad::Axis::xy, ion::Vector2(s_drawBoxSize / 2.0f, s_drawBoxSize / 2.0f), ion::Vector3(s_drawBoxSize / 2.0f, s_drawBoxSize / 2.0f, 0.0f));

	for (int i = 0; i < triX.size(); i++)
	{
		m_unitTriangleX->GetVertexBuffer().AddVertex(triX[i], ion::Vector3(), ion::Colour(1.0f, 1.0f, 1.0f, 1.0f), ion::render::TexCoord());
	}

	for (int i = 0; i < triY.size(); i++)
	{
		m_unitTriangleY->GetVertexBuffer().AddVertex(triY[i], ion::Vector3(), ion::Colour(1.0f, 1.0f, 1.0f, 1.0f), ion::render::TexCoord());
	}

	m_unitTriangleX->GetVertexBuffer().CompileBuffer();
	m_unitTriangleY->GetVertexBuffer().CompileBuffer();
}

Gizmo::~Gizmo()
{
	delete m_unitLineX;
	delete m_unitLineY;
}

void Gizmo::SetEnabled(bool enabled)
{
	m_enabled = enabled;
}

bool Gizmo::IsEnabled() const
{
	return m_enabled;
}

void Gizmo::SetObjectPosition(const ion::Vector2i& position)
{
	m_objectPosition = position;
}

const ion::Vector2i& Gizmo::GetObjectPosition() const
{
	return m_objectPosition;
}

const ion::Vector2i& Gizmo::GetGizmoPosition() const
{
	return m_gizmoPosition;
}

const ion::Vector2i& Gizmo::GetMoveStartPosition() const
{
	return m_moveStartPosition;
}

const ion::Vector2i& Gizmo::GetLastDelta() const
{
	return m_lastDelta;
}

const ion::Vector2i& Gizmo::GetMoveDelta() const
{
	return m_moveDelta;
}

void Gizmo::SetSensitivity(float unitsPerPixel)
{
	m_sensitivity = unitsPerPixel;
}

float Gizmo::GetSensitivity() const
{
	return m_sensitivity;
}

Gizmo::Constraint Gizmo::GetCurrentConstraint() const
{
	return m_currentConstraint;
}

void Gizmo::OnMouse(const ion::Vector2i& position, const ion::Vector2i& delta, int buttonBits, float cameraZoom, const ion::Vector2i& mapSizePx)
{
	if (m_enabled)
	{
		m_lastDelta.x = 0;
		m_lastDelta.y = 0;

		//Accumulate lost precision
		ion::Vector2i intDelta(delta.x, delta.y);
		m_accumulatedDelta += ion::Vector2(delta.x - intDelta.x, delta.y - intDelta.y);

		ion::Vector2i overflow(m_accumulatedDelta.x - intDelta.x, m_accumulatedDelta.y - intDelta.y);
		m_accumulatedDelta.x -= overflow.x;
		m_accumulatedDelta.y -= overflow.y;
		intDelta += overflow;

		//If already constrained + mouse button down + mouse moved, update pos and process callbacks
		if ((buttonBits & eMouseLeft) && (m_prevMouseBits & eMouseLeft))
		{
			if (m_currentConstraint == Constraint::Horizontal && delta.x != 0)
			{
				m_lastDelta.x = intDelta.x;
			}
			else if (m_currentConstraint == Constraint::Vertical && delta.y != 0)
			{
				m_lastDelta.y = intDelta.y;
			}
			else if (m_currentConstraint == Constraint::All)
			{
				m_lastDelta = intDelta;
			}

			m_moveDelta += m_lastDelta;
			m_gizmoPosition += m_lastDelta;
		}
		else if ((buttonBits & eMouseLeft) && !(m_prevMouseBits & eMouseLeft))
		{
			//Start action
			m_currentConstraint = m_hoverConstraint;
			m_moveStartPosition = m_gizmoPosition;
			m_moveDelta = ion::Vector2i();
		}
		else
		{
			///Mouse up, snap back to original state
			m_currentConstraint = Constraint::None;
			m_gizmoPosition = m_objectPosition;

			//Bounds check against axis
			ion::Vector2 pos(position.x + m_lastDelta.x, position.y + m_lastDelta.y);

			float halfWidth = s_drawTriangleSize / 2.0f;
			float length = (s_drawAxisLength + (s_drawTriangleSize / 2.0f)) * s_drawScale;

			ion::Vector2 minBox(m_gizmoPosition.x, m_gizmoPosition.y);
			ion::Vector2 maxBox(m_gizmoPosition.x + s_drawBoxSize * s_drawScale, m_gizmoPosition.y + s_drawBoxSize * s_drawScale);

			ion::Vector2 minX(m_gizmoPosition.x, m_gizmoPosition.y - halfWidth);
			ion::Vector2 maxX(m_gizmoPosition.x + length, m_gizmoPosition.y + halfWidth);

			ion::Vector2 minY(m_gizmoPosition.x - halfWidth, m_gizmoPosition.y - length);
			ion::Vector2 maxY(m_gizmoPosition.x + halfWidth, m_gizmoPosition.y);

			if (ion::maths::PointInsideBox(pos, minBox, maxBox))
			{
				m_hoverConstraint = Constraint::All;
			}
			else if (ion::maths::PointInsideBox(pos, minX, maxX))
			{
				m_hoverConstraint = Constraint::Horizontal;
			}
			else if (ion::maths::PointInsideBox(pos, minY, maxY))
			{
				m_hoverConstraint = Constraint::Vertical;
			}
			else
			{
				m_hoverConstraint = Constraint::None;
			}
		}

		m_prevMouseBits = buttonBits;
	}
}

void Gizmo::OnRender(ion::render::Renderer& renderer, RenderResources& renderResources, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float cameraZoom, const ion::Vector2i& mapSizePx)
{
	if (m_enabled)
	{
		ion::render::Material* material = renderResources.GetMaterial(RenderResources::eMaterialFlatColour);

		ion::Matrix4 mtx;
		ion::Vector2 coordSysCorrection(-mapSizePx.x / 2.0f, -mapSizePx.y / 2.0f);
		mtx.SetTranslation(ion::Vector3(m_gizmoPosition.x + coordSysCorrection.x, (mapSizePx.y - 1 - m_gizmoPosition.y) + coordSysCorrection.y, z));
		mtx.SetScale(ion::Vector3(s_drawScale, s_drawScale, 1.0f));

		bool selectedX = m_hoverConstraint == Constraint::Horizontal || m_hoverConstraint == Constraint::All;
		bool selectedY = m_hoverConstraint == Constraint::Vertical || m_hoverConstraint == Constraint::All;
		bool selectedBox = m_hoverConstraint == Constraint::All;

		material->SetDiffuseColour(selectedX ? s_drawColourSelected : s_drawColourAxisX);
		renderer.BindMaterial(*material, mtx, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(s_drawLineWidth);
		renderer.DrawVertexBuffer(m_unitLineX->GetVertexBuffer());
		renderer.DrawVertexBuffer(m_unitTriangleX->GetVertexBuffer());
		renderer.SetLineWidth(1.0f);
		renderer.UnbindMaterial(*material);

		material->SetDiffuseColour(selectedY ? s_drawColourSelected : s_drawColourAxisY);
		renderer.BindMaterial(*material, mtx, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(s_drawLineWidth);
		renderer.DrawVertexBuffer(m_unitLineY->GetVertexBuffer());
		renderer.DrawVertexBuffer(m_unitTriangleY->GetVertexBuffer());
		renderer.SetLineWidth(1.0f);
		renderer.UnbindMaterial(*material);

		renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
		material->SetDiffuseColour(selectedBox ? s_drawColourSelected : s_drawColourBox);
		renderer.BindMaterial(*material, mtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(m_unitBox->GetVertexBuffer(), m_unitBox->GetIndexBuffer());
		renderer.UnbindMaterial(*material);
	}
}
