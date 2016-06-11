///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		VertexBuffer.cpp
// Date:		3rd December 2013
// Authors:		Matt Phillips
// Description:	Vertex buffer class
///////////////////////////////////////////////////

#include "VertexBuffer.h"

namespace ion
{
	namespace render
	{
		VertexBuffer::VertexBuffer(Pattern pattern)
		{
			m_pattern = pattern;
			m_numVertices = 0;
		}

		VertexBuffer::~VertexBuffer()
		{
			m_buffer.clear();
		}

		int VertexBuffer::GetStrideBytes() const
		{
			return (s_positionSize + s_normalSize + s_texCoordSize) * sizeof(float);
		}

		void VertexBuffer::AddVertex(const Vector3& position, const Vector3& normal, const TexCoord& texCoord)
		{
			m_buffer.push_back(position.x);
			m_buffer.push_back(position.y);
			m_buffer.push_back(position.z);

			m_buffer.push_back(normal.x);
			m_buffer.push_back(normal.y);
			m_buffer.push_back(normal.z);

			m_buffer.push_back(texCoord.x);
			m_buffer.push_back(texCoord.y);

			m_numVertices++;
		}

		void VertexBuffer::AddFace(const Face& face)
		{
			for(int i = 0; i < 3; i++)
			{
				AddVertex(face.m_vertices[i], face.m_normals[i], face.m_texCoords[i]);
			}
		}

		void VertexBuffer::Reserve(int size)
		{
			int stride = GetStrideBytes();
			m_buffer.resize(size * stride);
			m_numVertices = size;
		}

		void VertexBuffer::SetVertex(int vertexIdx, const Vector3& position, const Vector3& normal, const TexCoord& texCoord)
		{
			debug::Assert(vertexIdx >= 0 && vertexIdx < m_numVertices, "Bad vertex id");

			int stride = GetStrideBytes() / sizeof(float);
			int floatIdx = stride * vertexIdx;

			m_buffer[floatIdx  ] = (position.x);
			m_buffer[floatIdx+1] = (position.y);
			m_buffer[floatIdx+2] = (position.z);

			m_buffer[floatIdx+3] = (normal.x);
			m_buffer[floatIdx+4] = (normal.y);
			m_buffer[floatIdx+5] = (normal.z);

			m_buffer[floatIdx+6] = (texCoord.x);
			m_buffer[floatIdx+7] = (texCoord.y);
		}

		Vertex VertexBuffer::GetVertex(int index) const
		{
			int stride = GetStrideBytes() / sizeof(float);
			int floatIdx = stride * index;

			if(index >= 0 && index < GetNumVerts())
				return Vertex(m_buffer[floatIdx], m_buffer[floatIdx + 1], m_buffer[floatIdx + 2]);
			else
				return Vertex();
		}

		Face VertexBuffer::GetFace(int index) const
		{
			index *= s_positionSize * 3;

			if(index >=0 && index < (GetNumVerts() * 3))
			{
				return Face(Vertex(m_buffer[index],		m_buffer[index + 1],	m_buffer[index + 2]),
							Vertex(m_buffer[index + 3],	m_buffer[index + 4],	m_buffer[index + 5]),
							Vertex(m_buffer[index + 6],	m_buffer[index + 7],	m_buffer[index + 8]));
			}
			else
				return Face();
		}

		void VertexBuffer::Clear()
		{
			m_buffer.clear();
		}

		const float* VertexBuffer::GetVertexBuffer() const
		{
			return &m_buffer[0];
		}

		const float* VertexBuffer::GetNormalBuffer() const
		{
			return &m_buffer[0] + s_positionSize;
		}

		const float* VertexBuffer::GetTexCoordBuffer() const
		{
			return &m_buffer[0] + s_positionSize + s_normalSize;
		}

		bool VertexBuffer::Lock()
		{
			return false;
		}

		bool VertexBuffer::Unlock()
		{
			return false;
		}
	}
}