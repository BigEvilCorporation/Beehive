///////////////////////////////////////////////////
// File:		VertexBuffer.h
// Date:		3rd December 2013
// Authors:		Matt Phillips
// Description:	Vertex buffer class
///////////////////////////////////////////////////

#pragma once

#include "maths/Vector.h"
#include "renderer/colour.h"
#include "renderer/Vertex.h"
#include "renderer/Face.h"

#include <vector>

namespace ion
{
	namespace render
	{
		class VertexBuffer
		{
		public:
			enum Pattern { Lines, Triangles, Quads };

			VertexBuffer(Pattern pattern);
			~VertexBuffer();

			void AddVertex(const Vector3& position, const Vector3& normal, const TexCoord& texCoord);
			void AddFace(const Face& face);

			void Reserve(int size);
			void SetVertex(int vertexIdx, const Vector3& position, const Vector3& normal, const TexCoord& texCoord);

			Vertex GetVertex(int index) const;
			Face GetFace(int index) const;

			//Returns start address of each buffer type
			const float* GetVertexBuffer() const;
			const float* GetNormalBuffer() const;
			const float* GetTexCoordBuffer() const;

			void Clear();

			bool Lock();
			bool Unlock();
			bool IsLocked() const { return mLocked; }

			Pattern GetPattern() const { return mPattern; }
			int GetNumVerts() const { return mNumVertices; }
			int GetStrideBytes() const;

			int GetVertexSize() const { return sPositionSize; }
			int GetNormalSize() const { return sNormalSize; }
			int GetTexCoordSize() const { return sTexCoordSize; }

			int GetFirstVertexOffset() const;
			int GetFirstNormalOffset() const;
			int GetFirstTexCoordOffset() const;

		protected:
			//Size of one position, normal and texcoord
			static const int sPositionSize = 3;
			static const int sNormalSize = 3;
			static const int sTexCoordSize = 2;

			std::vector<float> mBuffer;

			Pattern mPattern;
			int mNumVertices;
			bool mLocked;
		};
	}
}