///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Face.h
// Date:		26th July 2011
// Authors:		Matt Phillips
// Description:	Renderering tri
///////////////////////////////////////////////////

#pragma once

#include "maths/Vector.h"
#include "renderer/colour.h"
#include "renderer/Vertex.h"
#include "renderer/TexCoord.h"

namespace ion
{
	namespace render
	{
		class Face
		{
		public:
			Face() {}
			Face(const Vertex& a, const Vertex& b, const Vertex& c)
			{
				m_vertices[0] = a;
				m_vertices[1] = b;
				m_vertices[2] = c;
			}

			Face(const Vertex& vertexA, const Vertex& vertexB, const Vertex& vertexC,
				const Vector3& normalA, const Vector3& normalB, const Vector3& normalC,
				const TexCoord& texCoordA, const TexCoord& texCoordB, const TexCoord& texCoordC)
			{
				m_vertices[0] = vertexA;
				m_vertices[1] = vertexB;
				m_vertices[2] = vertexC;
				m_normals[0] = normalA;
				m_normals[1] = normalB;
				m_normals[2] = normalC;
				m_texCoords[0] = texCoordA;
				m_texCoords[1] = texCoordB;
				m_texCoords[2] = texCoordC;
			}

			Vertex	m_vertices[3];
			Vector3 m_normals[3];
			TexCoord m_texCoords[3];
			Vector3 m_faceNormal;
		};
	}
}
