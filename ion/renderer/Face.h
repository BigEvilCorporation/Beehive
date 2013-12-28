///////////////////////////////////////////////////
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
				mVertices[0] = a;
				mVertices[1] = b;
				mVertices[2] = c;
			}

			Face(const Vertex& vertexA, const Vertex& vertexB, const Vertex& vertexC,
				const Vector3& normalA, const Vector3& normalB, const Vector3& normalC,
				const TexCoord& texCoordA, const TexCoord& texCoordB, const TexCoord& texCoordC)
			{
				mVertices[0] = vertexA;
				mVertices[1] = vertexB;
				mVertices[2] = vertexC;
				mNormals[0] = normalA;
				mNormals[1] = normalB;
				mNormals[2] = normalC;
				mTexCoords[0] = texCoordA;
				mTexCoords[1] = texCoordB;
				mTexCoords[2] = texCoordC;
			}

			Vertex	mVertices[3];
			Vector3 mNormals[3];
			TexCoord mTexCoords[3];
			Vector3 mFaceNormal;
		};
	}
}
