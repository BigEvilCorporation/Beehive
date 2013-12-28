///////////////////////////////////////////////////
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
		VertexBuffer::VertexBuffer()
		{
			mNumVertices = 0;
		}

		VertexBuffer::~VertexBuffer()
		{
			mBuffer.clear();
		}

		int VertexBuffer::GetStride() const
		{
			return (sPositionSize + sNormalSize + sTexCoordSize) * sizeof(float);
		}

		void VertexBuffer::AddVertex(const Vector3& position, const Vector3& normal, const TexCoord& texCoord)
		{
			mBuffer.push_back(position.x);
			mBuffer.push_back(position.y);
			mBuffer.push_back(position.z);

			mBuffer.push_back(normal.x);
			mBuffer.push_back(normal.y);
			mBuffer.push_back(normal.z);

			mBuffer.push_back(texCoord.x);
			mBuffer.push_back(texCoord.y);

			mNumVertices++;
		}

		void VertexBuffer::AddFace(const Face& face)
		{
			for(int i = 0; i < 3; i++)
			{
				AddVertex(face.mVertices[i], face.mNormals[i], face.mTexCoords[i]);
			}
		}

		Vertex VertexBuffer::GetVertex(int index) const
		{
			index *= sPositionSize;

			if(index >= 0 && index < GetNumVerts())
				return Vertex(mBuffer[index], mBuffer[index + 1], mBuffer[index + 2]);
			else
				return Vertex();
		}

		Face VertexBuffer::GetFace(int index) const
		{
			index *= sPositionSize * 3;

			if(index >=0 && index < (GetNumVerts() * 3))
			{
				return Face(Vertex(mBuffer[index],		mBuffer[index + 1],	mBuffer[index + 2]),
							Vertex(mBuffer[index + 3],	mBuffer[index + 4],	mBuffer[index + 5]),
							Vertex(mBuffer[index + 6],	mBuffer[index + 7],	mBuffer[index + 8]));
			}
			else
				return Face();
		}

		void VertexBuffer::Clear()
		{
			mBuffer.clear();
		}

		const float* VertexBuffer::GetVertexBuffer() const
		{
			return &mBuffer[0];
		}

		const float* VertexBuffer::GetNormalBuffer() const
		{
			return &mBuffer[0] + sPositionSize;
		}

		const float* VertexBuffer::GetTexCoordBuffer() const
		{
			return &mBuffer[0] + sPositionSize + sNormalSize;
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