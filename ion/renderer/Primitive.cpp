///////////////////////////////////////////////////
// File:		Primitive.cpp
// Date:		4th August 2011
// Authors:		Matt Phillips
// Description:	Manually built primitive
///////////////////////////////////////////////////

#include "core/Debug.h"
#include "renderer/Primitive.h"
#include "renderer/Material.h"
#include "renderer/TexCoord.h"
#include "renderer/Face.h"

namespace ion
{
	namespace render
	{
		Primitive::Primitive(Material* material)
		{
			mMaterial = material;
		}

		Primitive::~Primitive()
		{

		}

		void Primitive::SetCastShadows(bool shadows)
		{

		}

		Quad::Quad(Material* material, Axis axis, const Vector2& halfExtents, const Vector3& offset)
			: Primitive(material)
		{
			if(axis == xy)
			{
				mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(1.0f, 0.0f));
				mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(1.0f, 1.0f));
				mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(0.0f, 1.0f));
				mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(0.0f, 0.0f));

				mIndexBuffer.Add(2, 1, 0);
				mIndexBuffer.Add(3, 2, 0);
			}
			else if(axis == xz)
			{
				mVertexBuffer.AddVertex(Vector3(offset.x + -halfExtents.x, offset.y, offset.z + -halfExtents.y), Vector3(0.0f, 1.0f, 0.0f), TexCoord(0.0f, 0.0f));
				mVertexBuffer.AddVertex(Vector3(offset.x + -halfExtents.x, offset.y, offset.z +  halfExtents.y), Vector3(0.0f, 1.0f, 0.0f), TexCoord(0.0f, 1.0f));
				mVertexBuffer.AddVertex(Vector3(offset.x +  halfExtents.x, offset.y, offset.z +  halfExtents.y), Vector3(0.0f, 1.0f, 0.0f), TexCoord(1.0f, 1.0f));
				mVertexBuffer.AddVertex(Vector3(offset.x +  halfExtents.x, offset.y, offset.z + -halfExtents.y), Vector3(0.0f, 1.0f, 0.0f), TexCoord(1.0f, 0.0f));

				mIndexBuffer.Add(0, 1, 2);
				mIndexBuffer.Add(2, 3, 0);
			}
			else if(axis == yz)
			{

			}
		}

		Box::Box(Material* material, const Vector3& halfExtents, const Vector3& offset)
			: Primitive(material)
		{
			//Top
			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 1.0f, 0.0f), TexCoord(0.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 1.0f, 0.0f), TexCoord(1.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 1.0f, 0.0f), TexCoord(1.0f, 0.0f));

			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 1.0f, 0.0f), TexCoord(0.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 1.0f, 0.0f), TexCoord(1.0f, 0.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 1.0f, 0.0f), TexCoord(0.0f, 0.0f));

			//Bottom																															   
			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, -1.0f, 0.0f), TexCoord(0.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, -1.0f, 0.0f), TexCoord(1.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, -1.0f, 0.0f), TexCoord(1.0f, 0.0f));

			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, -1.0f, 0.0f), TexCoord(0.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, -1.0f, 0.0f), TexCoord(1.0f, 0.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, -1.0f, 0.0f), TexCoord(0.0f, 0.0f));

			//Left
			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(-1.0f, 0.0f, 0.0f), TexCoord(0.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(-1.0f, 0.0f, 0.0f), TexCoord(1.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(-1.0f, 0.0f, 0.0f), TexCoord(1.0f, 0.0f));

			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(-1.0f, 0.0f, 0.0f), TexCoord(0.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(-1.0f, 0.0f, 0.0f), TexCoord(1.0f, 0.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(-1.0f, 0.0f, 0.0f), TexCoord(0.0f, 0.0f));

			//Right
			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(1.0f, 0.0f, 0.0f), TexCoord(0.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(1.0f, 0.0f, 0.0f), TexCoord(1.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(1.0f, 0.0f, 0.0f), TexCoord(1.0f, 0.0f));

			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(1.0f, 0.0f, 0.0f), TexCoord(0.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(1.0f, 0.0f, 0.0f), TexCoord(1.0f, 0.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(1.0f, 0.0f, 0.0f), TexCoord(0.0f, 0.0f));

			//Front
			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(0.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(1.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(1.0f, 0.0f));

			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(0.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(1.0f, 0.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z), Vector3(0.0f, 0.0f, -1.0f), TexCoord(0.0f, 0.0f));

			//Back
			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 0.0f, 1.0f), TexCoord(0.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 0.0f, 1.0f), TexCoord(1.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 0.0f, 1.0f), TexCoord(1.0f, 0.0f));

			mVertexBuffer.AddVertex(Vector3(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 0.0f, 1.0f), TexCoord(0.0f, 1.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 0.0f, 1.0f), TexCoord(1.0f, 0.0f));
			mVertexBuffer.AddVertex(Vector3(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z), Vector3(0.0f, 0.0f, 1.0f), TexCoord(0.0f, 0.0f));

			for(int i = 0; i < 36; i += 3)
			{
				mIndexBuffer.Add(i, i + 1, i + 2);
			}
		}

		Sphere::Sphere(Material* material, float radius, int rings, int segments)
			: Primitive(material)
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

					mVertexBuffer.AddVertex(Vector3(x0, y0, z0), Vector3(x0, y0, z0).Normalise(), TexCoord((float)seg / (float)segments, (float)ring / (float)rings));

					if(ring != rings)
					{
						mIndexBuffer.Add(vertexIndex + segments + 1, vertexIndex, vertexIndex + segments);
						mIndexBuffer.Add(vertexIndex + segments + 1, vertexIndex + 1, vertexIndex);
						vertexIndex++;
					}
				}
			}
		}
	}
}