///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Primitive.cpp
// Date:		4th August 2011
// Authors:		Matt Phillips
// Description:	Manually built primitive
///////////////////////////////////////////////////

#include "core/debug/Debug.h"
#include "renderer/Primitive.h"
#include "renderer/Material.h"
#include "renderer/TexCoord.h"
#include "renderer/Face.h"

namespace ion
{
	namespace render
	{
		Primitive::Primitive(VertexBuffer::Pattern vertexPattern)
			: m_vertexBuffer(vertexPattern)
		{
		}

		Primitive::~Primitive()
		{

		}

		void Primitive::SetCastShadows(bool shadows)
		{

		}

		LineSegments::LineSegments(const std::vector<Vector3>& points)
			: Primitive(VertexBuffer::eLines)
		{
			for(int i = 0; i < points.size(); i++)
			{
				m_vertexBuffer.AddVertex(points[i], Vector3(0.0f, 0.0f, 1.0f), TexCoord(0.0f, 1.0f));
			}
		}

		LineStrip::LineStrip(const std::vector<Vector3>& points)
			: Primitive(VertexBuffer::eLineStrip)
		{
			for(int i = 0; i < points.size(); i++)
			{
				m_vertexBuffer.AddVertex(points[i], Vector3(0.0f, 0.0f, 1.0f), TexCoord(0.0f, 1.0f));
			}
		}

		Quad::Quad(Axis axis, const Vector2& halfExtents)
			: Primitive(VertexBuffer::eTriangles)
		{
			Vector3 offset(0.0f, 0.0f, 0.0f);

			if(axis == xy)
			{
				m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, 0.0f), Vector3(0.0f, 0.0f, 1.0f), TexCoord(0.0f, 0.0f));
				m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, 0.0f), Vector3(0.0f, 0.0f, 1.0f), TexCoord(0.0f, 1.0f));
				m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, 0.0f), Vector3(0.0f, 0.0f, 1.0f), TexCoord(1.0f, 1.0f));
				m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, 0.0f), Vector3(0.0f, 0.0f, 1.0f), TexCoord(1.0f, 0.0f));

				m_indexBuffer.Add(0, 1, 2);
				m_indexBuffer.Add(0, 2, 3);
			}
			else if(axis == xz)
			{
				m_vertexBuffer.AddVertex(Vector3(offset.x + -halfExtents.x, offset.y, offset.z + -halfExtents.y), Vector3(0.0f, 1.0f, 0.0f), TexCoord(0.0f, 0.0f));
				m_vertexBuffer.AddVertex(Vector3(offset.x + -halfExtents.x, offset.y, offset.z +  halfExtents.y), Vector3(0.0f, 1.0f, 0.0f), TexCoord(0.0f, 1.0f));
				m_vertexBuffer.AddVertex(Vector3(offset.x +  halfExtents.x, offset.y, offset.z +  halfExtents.y), Vector3(0.0f, 1.0f, 0.0f), TexCoord(1.0f, 1.0f));
				m_vertexBuffer.AddVertex(Vector3(offset.x +  halfExtents.x, offset.y, offset.z + -halfExtents.y), Vector3(0.0f, 1.0f, 0.0f), TexCoord(1.0f, 0.0f));

				m_indexBuffer.Add(0, 1, 2);
				m_indexBuffer.Add(2, 3, 0);
			}
			else if(axis == yz)
			{

			}
		}

		void Quad::SetTexCoords(TexCoord coords[4])
		{
			for(int i = 0; i < 4; i++)
			{
				Vertex pos = m_vertexBuffer.GetVertex(i);
				m_vertexBuffer.SetVertex(i, pos, Vector3(0.0f, 0.0f, 1.0f), coords[i]);
			}
		}

		LineQuad::LineQuad(Axis axis, const Vector2& halfExtents)
			: Primitive(VertexBuffer::eLines)
		{
			if(axis == xy)
			{
				m_vertexBuffer.AddVertex(Vector3(-halfExtents.x, -halfExtents.y, 0.0f), Vector3(), TexCoord());
				m_vertexBuffer.AddVertex(Vector3( halfExtents.x, -halfExtents.y, 0.0f), Vector3(), TexCoord());

				m_vertexBuffer.AddVertex(Vector3(halfExtents.x, -halfExtents.y, 0.0f), Vector3(), TexCoord());
				m_vertexBuffer.AddVertex(Vector3( halfExtents.x,  halfExtents.y, 0.0f), Vector3(), TexCoord());

				m_vertexBuffer.AddVertex(Vector3(halfExtents.x, halfExtents.y, 0.0f), Vector3(), TexCoord());
				m_vertexBuffer.AddVertex(Vector3(-halfExtents.x,  halfExtents.y, 0.0f), Vector3(), TexCoord());

				m_vertexBuffer.AddVertex(Vector3(-halfExtents.x, halfExtents.y, 0.0f), Vector3(), TexCoord());
				m_vertexBuffer.AddVertex(Vector3(-halfExtents.x, -halfExtents.y, 0.0f), Vector3(), TexCoord());
			}
		}

		Grid::Grid(Axis axis, const Vector2& halfExtents, int widthCells, int heightCells)
			: Primitive(VertexBuffer::eLines)
		{
			Vector2 cellSize((halfExtents.x * 2.0f) / (float)widthCells, (halfExtents.y * 2.0f) / (float)heightCells);
			Vector2 gridSize(halfExtents * 2.0f);

			if(axis == xy)
			{
				for(int x = 0; x < widthCells+1; x++)
				{
					m_vertexBuffer.AddVertex(Vector3((cellSize.x * x) - halfExtents.x, -halfExtents.y, 0.0f), Vector3(), TexCoord());
					m_vertexBuffer.AddVertex(Vector3((cellSize.x * x) - halfExtents.x, gridSize.y - halfExtents.y, 0.0f), Vector3(), TexCoord());
				}

				for(int y = 0; y < heightCells+1; y++)
				{
					m_vertexBuffer.AddVertex(Vector3(-halfExtents.x,				(cellSize.y * y) - halfExtents.y, 0.0f), Vector3(), TexCoord());
					m_vertexBuffer.AddVertex(Vector3(gridSize.x - halfExtents.x,	(cellSize.y * y) - halfExtents.y, 0.0f), Vector3(), TexCoord());
				}
			}
		}

		Chessboard::Chessboard(Axis axis, const Vector2& halfExtents, int widthCells, int heightCells, bool uniqueVerts)
			: Primitive(VertexBuffer::eTriangles)
		{
			Vector2 cellSize((halfExtents.x * 2.0f) / (float)widthCells, (halfExtents.y * 2.0f) / (float)heightCells);

			if(axis == xy)
			{
				int vertexCount = 0;
				for(int y = 0; y < heightCells; y++)
				{
					for(int x = 0; x < widthCells; x++)
					{
						if(uniqueVerts)
						{
							//Create quad per cell
							Vector2 cellPos((cellSize.x * x) - halfExtents.x, (cellSize.y * y) - halfExtents.y);
							m_vertexBuffer.AddVertex(Vector3(             cellPos.x, cellPos.y + cellSize.y, 0.0f), Vector3(0.0f, 0.0f, 1.0f), TexCoord(0.0f, 1.0f));
							m_vertexBuffer.AddVertex(Vector3(             cellPos.x, cellPos.y,              0.0f), Vector3(0.0f, 0.0f, 1.0f), TexCoord(1.0f, 1.0f));
							m_vertexBuffer.AddVertex(Vector3(cellPos.x + cellSize.x, cellPos.y,              0.0f), Vector3(0.0f, 0.0f, 1.0f), TexCoord(1.0f, 0.0f));
							m_vertexBuffer.AddVertex(Vector3(cellPos.x + cellSize.x, cellPos.y + cellSize.y, 0.0f), Vector3(0.0f, 0.0f, 1.0f), TexCoord(0.0f, 0.0f));

							m_indexBuffer.Add(vertexCount, vertexCount + 1, vertexCount+2);
							m_indexBuffer.Add(vertexCount, vertexCount + 2, vertexCount+3);

							vertexCount += 4;
						}
						else
						{
							//Shared verts
						}
					}
				}
			}
		}

		void Chessboard::SetTexCoords(int cellIndex, TexCoord coords[4])
		{
			for(int i = 0; i < 4; i++)
			{
				Vertex pos = m_vertexBuffer.GetVertex((cellIndex * 4) + i);
				m_vertexBuffer.SetVertex((cellIndex * 4) + i, pos, Vector3(0.0f, 0.0f, 1.0f), coords[i]);
			}
		}

		Box::Box(const Vector3& halfExtents, const Vector3& offset)
			: Primitive(VertexBuffer::eTriangles)
		{
			//Top
			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 1.0f, 0.0f), TexCoord(0.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 1.0f, 0.0f), TexCoord(1.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 1.0f, 0.0f), TexCoord(1.0f, 0.0f));

			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 1.0f, 0.0f), TexCoord(0.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 1.0f, 0.0f), TexCoord(1.0f, 0.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 1.0f, 0.0f), TexCoord(0.0f, 0.0f));

			//Bottom																															   
			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, -1.0f, 0.0f), TexCoord(0.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, -1.0f, 0.0f), TexCoord(1.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, -1.0f, 0.0f), TexCoord(1.0f, 0.0f));

			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, -1.0f, 0.0f), TexCoord(0.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, -1.0f, 0.0f), TexCoord(1.0f, 0.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, -1.0f, 0.0f), TexCoord(0.0f, 0.0f));

			//Left
			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(-1.0f, 0.0f, 0.0f), TexCoord(0.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(-1.0f, 0.0f, 0.0f), TexCoord(1.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(-1.0f, 0.0f, 0.0f), TexCoord(1.0f, 0.0f));

			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(-1.0f, 0.0f, 0.0f), TexCoord(0.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(-1.0f, 0.0f, 0.0f), TexCoord(1.0f, 0.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(-1.0f, 0.0f, 0.0f), TexCoord(0.0f, 0.0f));

			//Right
			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(1.0f, 0.0f, 0.0f), TexCoord(0.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(1.0f, 0.0f, 0.0f), TexCoord(1.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(1.0f, 0.0f, 0.0f), TexCoord(1.0f, 0.0f));

			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(1.0f, 0.0f, 0.0f), TexCoord(0.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(1.0f, 0.0f, 0.0f), TexCoord(1.0f, 0.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(1.0f, 0.0f, 0.0f), TexCoord(0.0f, 0.0f));

			//Front
			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(0.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(1.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(1.0f, 0.0f));

			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(0.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(1.0f, 0.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(0.0f, 0.0f));

			//Back
			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 0.0f, 1.0f), TexCoord(0.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 0.0f, 1.0f), TexCoord(1.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 0.0f, 1.0f), TexCoord(1.0f, 0.0f));

			m_vertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 0.0f, 1.0f), TexCoord(0.0f, 1.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 0.0f, 1.0f), TexCoord(1.0f, 0.0f));
			m_vertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 0.0f, 1.0f), TexCoord(0.0f, 0.0f));

			for(int i = 0; i < 36; i += 3)
			{
				m_indexBuffer.Add(i, i + 1, i + 2);
			}
		}

		Sphere::Sphere(float radius, int rings, int segments)
			: Primitive(VertexBuffer::eTriangles)
		{
			float deltaRingAngle = (maths::PI / rings);
			float deltaSegAngle = (2.0f * maths::PI / segments);
			int vertexIndex = 0;

			for(int ring = 0; ring <= rings; ring++)
			{
				float r0 = radius * sinf (ring * deltaRingAngle);
				float y0 = radius * cosf (ring * deltaRingAngle);

				for(int seg = 0; seg <= segments; seg++)
				{
					float x0 = r0 * sinf(seg * deltaSegAngle);
					float z0 = r0 * cosf(seg * deltaSegAngle);

					m_vertexBuffer.AddVertex(Vector3(x0, y0, z0), Vector3(x0, y0, z0).Normalise(), TexCoord((float)seg / (float)segments, (float)ring / (float)rings));

					if(ring != rings)
					{
						m_indexBuffer.Add(vertexIndex + segments + 1, vertexIndex, vertexIndex + segments);
						m_indexBuffer.Add(vertexIndex + segments + 1, vertexIndex + 1, vertexIndex);
						vertexIndex++;
					}
				}
			}
		}

		Cylinder::Cylinder(float radius, float height, int steps, const Vector3& offset)
			: Primitive(VertexBuffer::eTriangles)
		{
			float halfHeight = height * 0.5f;
			float a = 0.0f;
			float step = (maths::TWO_PI / (float)steps) * 2.0f;

			//Sides
			for(int i = 0; i < steps; i++)
			{
				float x = maths::Cos(a) * radius;
				float z = maths::Sin(a) * radius;

				m_vertexBuffer.AddVertex(Vector3(x, -halfHeight, z) + offset, Vector3(x, z, 0.0f).Normalise(), TexCoord(i%2==0 ? 0.0f : 1.0f, 1.0f));
				m_vertexBuffer.AddVertex(Vector3(x,  halfHeight, z) + offset, Vector3(x, z, 0.0f).Normalise(), TexCoord(i%2==0 ? 0.0f : 1.0f, 0.0f));
				a += step;

				m_indexBuffer.Add(i,   i+1, i+2);
				m_indexBuffer.Add(i+2, i+1, i+3);
			}

			//Caps
			m_vertexBuffer.AddVertex(Vector3(0.0f, 0.0f, -halfHeight), Vector3(0.0f, -1.0f, 0.0f).Normalise(), TexCoord(0.5f, 0.5f));
			m_vertexBuffer.AddVertex(Vector3(0.0f, 0.0f,  halfHeight), Vector3(0.0f,  1.0f, 0.0f).Normalise(), TexCoord(0.5f, 0.5f));

			for(int i = 0; i < steps; i++)
			{
				m_indexBuffer.Add(i*2, steps, (i+1)*2);
				m_indexBuffer.Add((i+1)*2+1, steps+1, i*2+1);
			}
		}
	}
}