///////////////////////////////////////////////////
// File:		Primitive.cpp
// Date:		4th August 2011
// Authors:		Matt Phillips
// Description:	Manually built primitive
///////////////////////////////////////////////////

#include "Primitive.h"
#include "Scene.h"
#include "Material.h"
#include "TexCoord.h"
#include "../core/Debug.h"

namespace ion
{
	namespace renderer
	{
		Primitive::Primitive(Scene& scene, Projection projection)
		{
			mProjection = projection;

			#if defined ION_OGRE
			mManualPrimitive = scene.GetOgreSceneMgrIFace()->createManualObject();

			if(mManualPrimitive)
			{
				if(projection == Proj2D)
				{
					//Use identity view/projection matrices
					mManualPrimitive->setUseIdentityProjection(true);
					mManualPrimitive->setUseIdentityView(true);

					//Infinite clip bounds
					Ogre::AxisAlignedBox bounds;
					bounds.setInfinite();
					mManualPrimitive->setBoundingBox(bounds);

					//Render before overlay renderstage
					mManualPrimitive->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY - 1);
				}
			}
			#endif
		}

		Primitive::~Primitive()
		{
			//Manual primitive destroyed with scene manager
		}

		void Primitive::Begin(Material* material, Primitive::Pattern pattern)
		{
			#if defined ION_OGRE
			if(mManualPrimitive)
			{
				Ogre::RenderOperation::OperationType renderOp;

				switch(pattern)
				{
				case Line:
					renderOp = Ogre::RenderOperation::OT_LINE_LIST;
					break;

				case Triangle:
					renderOp = Ogre::RenderOperation::OT_TRIANGLE_LIST;
					break;

				case Quad:
					renderOp = Ogre::RenderOperation::OT_LINE_STRIP;
					break;

				default:
					debug::Error("Primitive::Begin() - Invalid render pattern");
				}

				std::string materialName = "BaseWhiteNoLighting";

				if(material)
				{
					materialName = material->GetOgreMaterialName();
				}

				mManualPrimitive->begin(materialName, renderOp);
			}
			#endif
		}

		void Primitive::End()
		{
			#if defined ION_OGRE
			if(mManualPrimitive)
			{
				mManualPrimitive->end();
			}
			#endif
		}

		void Primitive::Clear()
		{
			#if defined ION_OGRE
			if(mManualPrimitive)
			{
				mManualPrimitive->clear();

				if(mProjection == Proj2D)
				{
					//Use identity view/projection matrices
					mManualPrimitive->setUseIdentityProjection(true);
					mManualPrimitive->setUseIdentityView(true);

					//Infinite clip bounds
					Ogre::AxisAlignedBox bounds;
					bounds.setInfinite();
					mManualPrimitive->setBoundingBox(bounds);

					//Render before overlay renderstage
					mManualPrimitive->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY - 1);
				}
			}
			#endif
		}

		void Primitive::AddVertex(const Vertex& vertex)
		{
			#if defined ION_OGRE
			if(mManualPrimitive)
			{
				mManualPrimitive->position(vertex.x, vertex.y, vertex.z);
			}
			#endif
		}

		void Primitive::AddNormal(const Vector3& normal)
		{
			#if defined ION_OGRE
			if(mManualPrimitive)
			{
				mManualPrimitive->normal(normal.x, normal.y, normal.z);
			}
			#endif
		}

		void Primitive::AddColour(const Colour& colour)
		{
			#if defined ION_OGRE
			if(mManualPrimitive)
			{
				mManualPrimitive->colour(colour.r, colour.g, colour.b, colour.a);
			}
			#endif
		}

		void Primitive::AddTextureCoord(const TexCoord& coord)
		{
			#if defined ION_OGRE
			if(mManualPrimitive)
			{
				mManualPrimitive->textureCoord(coord.x, coord.y);
			}
			#endif
		}

		void Primitive::AddIndex(unsigned int index)
		{
			#if defined ION_OGRE
			if(mManualPrimitive)
			{
				mManualPrimitive->index(index);
			}
			#endif
		}

		void Primitive::AddQuad(Material* material, float width, float height, Primitive::QuadAxis axis, const Vector3& offset)
		{
			float halfWidth = width * 0.5f;
			float halfHeight = height * 0.5f;

			Begin(material, Triangle);

			if(axis == xy)
			{
				AddVertex(Vertex(offset.x + -halfWidth, offset.y + -halfHeight, offset.z));
				AddVertex(Vertex(offset.x +  halfWidth, offset.y + -halfHeight, offset.z));
				AddVertex(Vertex(offset.x +  halfWidth, offset.y +  halfHeight, offset.z));

				AddVertex(Vertex(offset.x + -halfWidth, offset.y + -halfHeight, offset.z));
				AddVertex(Vertex(offset.x +  halfWidth, offset.y +  halfHeight, offset.z));
				AddVertex(Vertex(offset.x + -halfWidth, offset.y +  halfHeight, offset.z));
			}
			else if(axis == xz)
			{
				AddVertex(Vertex(offset.x + -halfWidth, offset.y, offset.z + -halfHeight)); AddNormal(Vector3(0.0f, 1.0f, 0.0f)); AddTextureCoord(TexCoord(0.0f, 0.0f));
				AddVertex(Vertex(offset.x + -halfWidth, offset.y, offset.z +  halfHeight)); AddNormal(Vector3(0.0f, 1.0f, 0.0f)); AddTextureCoord(TexCoord(0.0f, 1.0f));
				AddVertex(Vertex(offset.x +  halfWidth, offset.y, offset.z +  halfHeight)); AddNormal(Vector3(0.0f, 1.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 1.0f));
				AddVertex(Vertex(offset.x +  halfWidth, offset.y, offset.z + -halfHeight)); AddNormal(Vector3(0.0f, 1.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));

				AddIndex(0);
				AddIndex(1);
				AddIndex(2);
				AddIndex(2);
				AddIndex(3);
				AddIndex(0);
			}
			else if(axis == yz)
			{
				AddVertex(Vertex(offset.x, offset.y + -halfHeight, offset.z + -halfWidth));
				AddVertex(Vertex(offset.x, offset.y +  halfHeight, offset.z + -halfWidth));
				AddVertex(Vertex(offset.x, offset.y +  halfHeight, offset.z +  halfWidth));
								
				AddVertex(Vertex(offset.x, offset.y + -halfHeight, offset.z + -halfWidth));
				AddVertex(Vertex(offset.x, offset.y +  halfHeight, offset.z +  halfWidth));
				AddVertex(Vertex(offset.x, offset.y + -halfHeight, offset.z +  halfWidth));
			}

			End();
		}

		void Primitive::AddBox(Material* material, const Vector3& halfExtents, const Vector3& offset)
		{
			Begin(material, Triangle);

			//Add indices

			//Top
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, 1.0f, 0.0f)); AddTextureCoord(TexCoord(0.0f, 1.0f));
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, 1.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 1.0f));
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(0.0f, 1.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));

			AddVertex(Vertex(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, 1.0f, 0.0f)); AddTextureCoord(TexCoord(0.0f, 1.0f));
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(0.0f, 1.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(0.0f, 1.0f, 0.0f)); AddTextureCoord(TexCoord(0.0f, 0.0f));

			//Bottom																															   
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, -1.0f, 0.0f)); AddTextureCoord(TexCoord(0.0f, 1.0f));
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, -1.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 1.0f));
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(0.0f, -1.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));
																																				   
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, -1.0f, 0.0f)); AddTextureCoord(TexCoord(0.0f, 1.0f));
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(0.0f, -1.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(0.0f, -1.0f, 0.0f)); AddTextureCoord(TexCoord(0.0f, 0.0f));

			//Left
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(-1.0f, 0.0f, 0.0f)); AddTextureCoord(TexCoord(0.0f, 1.0f));
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(-1.0f, 0.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 1.0f));
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(-1.0f, 0.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));
																																				   
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(-1.0f, 0.0f, 0.0f)); AddTextureCoord(TexCoord(0.0f, 1.0f));
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(-1.0f, 0.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(-1.0f, 0.0f, 0.0f)); AddTextureCoord(TexCoord(0.0f, 0.0f));

			//Right
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(1.0f, 0.0f, 0.0f)); AddTextureCoord(TexCoord(0.0f, 1.0f));
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(1.0f, 0.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 1.0f));
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(1.0f, 0.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));
																																				  
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(1.0f, 0.0f, 0.0f)); AddTextureCoord(TexCoord(0.0f, 1.0f));
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(1.0f, 0.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(1.0f, 0.0f, 0.0f)); AddTextureCoord(TexCoord(0.0f, 0.0f));

			//Front
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, -1.0f)); AddTextureCoord(TexCoord(0.0f, 1.0f));
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, -1.0f)); AddTextureCoord(TexCoord(1.0f, 1.0f));
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, -1.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));
																																				   
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, -1.0f)); AddTextureCoord(TexCoord(0.0f, 1.0f));
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, -1.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z - halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, -1.0f)); AddTextureCoord(TexCoord(0.0f, 0.0f));

			//Back
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, 1.0f)); AddTextureCoord(TexCoord(0.0f, 1.0f));
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, 1.0f)); AddTextureCoord(TexCoord(1.0f, 1.0f));
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, 1.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));
																																				  
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, 1.0f)); AddTextureCoord(TexCoord(0.0f, 1.0f));
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, 1.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, 1.0f)); AddTextureCoord(TexCoord(0.0f, 0.0f));

			for(int i = 0; i < 36; i++)
			{
				AddIndex(i);
			}

			End();
		}

		void Primitive::AddSphere(Material* material, float radius, int rings, int segments)
		{
			Begin(material, Triangle);

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

					AddVertex(Vertex(x0, y0, z0));
					AddNormal(Vector3(x0, y0, z0).Normalise());
					AddTextureCoord(TexCoord((float)seg / (float)segments, (float)ring / (float)rings));

					if(ring != rings)
					{
						AddIndex(vertexIndex + segments + 1);
						AddIndex(vertexIndex);               
						AddIndex(vertexIndex + segments);
						AddIndex(vertexIndex + segments + 1);
						AddIndex(vertexIndex + 1);
						AddIndex(vertexIndex);
						vertexIndex++;
					}
				}
			}

			End();
		}

		void Primitive::SetCastShadows(bool shadows)
		{
			mManualPrimitive->setCastShadows(shadows);
		}

		#if defined ION_OGRE
		Ogre::ManualObject* Primitive::GetOgreManualObj()
		{
			return mManualPrimitive;
		}
		#endif
	}
}