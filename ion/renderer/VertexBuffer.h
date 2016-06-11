///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
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
			enum Pattern { eLines, eLineStrip, eTriangles, eQuads };

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
			bool IsLocked() const { return m_locked; }

			Pattern GetPattern() const { return m_pattern; }
			int GetNumVerts() const { return m_numVertices; }
			int GetStrideBytes() const;

			int GetVertexSize() const { return s_positionSize; }
			int GetNormalSize() const { return s_normalSize; }
			int GetTexCoordSize() const { return s_texCoordSize; }

			int GetFirstVertexOffset() const;
			int GetFirstNormalOffset() const;
			int GetFirstTexCoordOffset() const;

		protected:
			//Size of one position, normal and texcoord
			static const int s_positionSize = 3;
			static const int s_normalSize = 3;
			static const int s_texCoordSize = 2;

			std::vector<float> m_buffer;

			Pattern m_pattern;
			int m_numVertices;
			bool m_locked;
		};
	}
}