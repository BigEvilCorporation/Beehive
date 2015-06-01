///////////////////////////////////////////////////
// File:		Primitive.h
// Date:		4th August 2011
// Authors:		Matt Phillips
// Description:	Manually built primitive
///////////////////////////////////////////////////

#pragma once

#include "maths/Vector.h"
#include "renderer/colour.h"
#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"
#include "renderer/Entity.h"

#include <string>

namespace ion
{
	namespace render
	{
		//Forward declaration
		class Material;

		class Primitive : public Entity
		{
		public:
			Primitive(VertexBuffer::Pattern vertexPattern);
			virtual ~Primitive();

			void SetCastShadows(bool shadows);

			const VertexBuffer& GetVertexBuffer() const { return mVertexBuffer; }
			const IndexBuffer& GetIndexBuffer() const { return mIndexBuffer; }

		protected:
			VertexBuffer mVertexBuffer;
			IndexBuffer mIndexBuffer;
			Material* mMaterial;
		};

		class Quad : public Primitive
		{
		public:
			enum Axis { xy, xz, yz };
			Quad(Axis axis, const Vector2& halfExtents);
			void SetTexCoords(TexCoord coords[4]);
		};

		class LineQuad : public Primitive
		{
		public:
			enum Axis { xy, xz, yz };
			LineQuad(Axis axis, const Vector2& halfExtents);
		};

		class Grid : public Primitive
		{
		public:
			enum Axis { xy, xz, yz };
			Grid(Axis axis, const Vector2& halfExtents, int widthCells, int heightCells);
		};

		class Chessboard : public Primitive
		{
		public:
			enum Axis { xy, xz, yz };
			Chessboard(Axis axis, const Vector2& halfExtents, int widthCells, int heightCells, bool uniqueVerts);
			void SetTexCoords(int cellIndex, TexCoord coords[4]);
		};

		class Box : public Primitive
		{
		public:
			Box(const Vector3& halfExtents, const Vector3& offset = Vector3());
		};

		class Sphere : public Primitive
		{
		public:
			Sphere(float radius, int rings, int segments);
		};

		class Cylinder : public Primitive
		{
		public:
			Cylinder(float radius, float height, int steps, const Vector3& offset = Vector3());
		};
	}
}