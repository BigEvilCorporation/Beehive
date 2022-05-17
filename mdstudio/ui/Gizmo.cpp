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
const ion::Colour Gizmo::s_drawColourAxisX = ion::Colour(1.0f, 0.0f, 0.0f, 1.0f);
const ion::Colour Gizmo::s_drawColourAxisY = ion::Colour(0.0f, 1.0f, 0.0f, 1.0f);
const ion::Colour Gizmo::s_drawColourBox = ion::Colour(0.8f, 0.8f, 0.8f, 0.5f);

Gizmo::Gizmo()
{
	m_sensitivity = 1.0f;
	m_enabled = true;

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

void Gizmo::SetPosition(const ion::Vector2i& position)
{
	m_position = position;
}

const ion::Vector2i& Gizmo::GetPosition() const
{
	return m_position;
}

const ion::Vector2i& Gizmo::GetLastDelta() const
{
	return m_lastDelta;
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

		//Apply camera zoom
		ion::Vector2 zoomedDelta((float)delta.x / cameraZoom, (float)delta.y / cameraZoom);

		//Accumulate lost precision
		ion::Vector2i intDelta(zoomedDelta.x, zoomedDelta.y);
		m_accumulatedDelta += ion::Vector2(zoomedDelta.x - intDelta.x, zoomedDelta.y - intDelta.y);

		ion::Vector2i overflow(m_accumulatedDelta.x - intDelta.x, m_accumulatedDelta.y - intDelta.y);
		m_accumulatedDelta.x -= overflow.x;
		m_accumulatedDelta.y -= overflow.y;
		intDelta += overflow;

		//If already constrained + mouse button down + mouse moved, update pos and process callbacks
		if (buttonBits & eMouseLeft)
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
		}
		else
		{
			//Mouse up, bounds check against axis
			ion::Vector2 pos(position.x + m_lastDelta.x, mapSizePx.y - position.y + m_lastDelta.y);

			float halfWidth = s_drawTriangleSize / 2.0f;
			float length = s_drawAxisLength + (s_drawTriangleSize / 2.0f);

			ion::Vector2 minBox(m_position.x, m_position.y);
			ion::Vector2 maxBox(m_position.x + s_drawBoxSize, m_position.y + s_drawBoxSize);

			ion::Vector2 minX(m_position.x, m_position.y - halfWidth);
			ion::Vector2 maxX(m_position.x + length, m_position.y + halfWidth);

			ion::Vector2 minY(m_position.x - halfWidth, m_position.y);
			ion::Vector2 maxY(m_position.x + halfWidth, m_position.y + length);

			if (ion::maths::PointInsideBox(pos, minBox, maxBox))
			{
				m_currentConstraint = Constraint::All;
			}
			else if (ion::maths::PointInsideBox(pos, minX, maxX))
			{
				m_currentConstraint = Constraint::Horizontal;
			}
			else if (ion::maths::PointInsideBox(pos, minY, maxY))
			{
				m_currentConstraint = Constraint::Vertical;
			}
			else
			{
				m_currentConstraint = Constraint::None;
			}
		}
	}
}

void Gizmo::OnRender(ion::render::Renderer& renderer, RenderResources& renderResources, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, const ion::Vector2i& mapSizePx)
{
	if (m_enabled)
	{
		ion::render::Material* material = renderResources.GetMaterial(RenderResources::eMaterialFlatColour);

		ion::Matrix4 mtx;
		ion::Vector2 coordSysCorrection(-mapSizePx.x / 2.0f, -mapSizePx.y / 2.0f);
		mtx.SetTranslation(ion::Vector3(m_position.x + coordSysCorrection.x, m_position.y + coordSysCorrection.y, z));

		material->SetDiffuseColour(s_drawColourAxisX);
		renderer.BindMaterial(*material, mtx, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(s_drawLineWidth);
		renderer.DrawVertexBuffer(m_unitLineX->GetVertexBuffer());
		renderer.DrawVertexBuffer(m_unitTriangleX->GetVertexBuffer());
		renderer.SetLineWidth(1.0f);
		renderer.UnbindMaterial(*material);

		material->SetDiffuseColour(s_drawColourAxisY);
		renderer.BindMaterial(*material, mtx, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(s_drawLineWidth);
		renderer.DrawVertexBuffer(m_unitLineY->GetVertexBuffer());
		renderer.DrawVertexBuffer(m_unitTriangleY->GetVertexBuffer());
		renderer.SetLineWidth(1.0f);
		renderer.UnbindMaterial(*material);

		renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
		material->SetDiffuseColour(s_drawColourBox);
		renderer.BindMaterial(*material, mtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(m_unitBox->GetVertexBuffer(), m_unitBox->GetIndexBuffer());
		renderer.UnbindMaterial(*material);
	}
}
