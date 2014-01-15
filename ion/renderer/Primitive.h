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
			Primitive(VertexBuffer::Pattern vertexPattern, Material* material);
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
			Quad(Material* material, Axis axis, const Vector2& halfExtents, const Vector3& offset = Vector3());
		};

		class Box : public Primitive
		{
		public:
			Box(Material* material, const Vector3& halfExtents, const Vector3& offset = Vector3());
		};

		class Sphere : public Primitive
		{
		public:
			Sphere(Material* material, float radius, int rings, int segments);
		};
	}
}