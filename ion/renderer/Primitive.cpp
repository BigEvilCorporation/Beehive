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
			#if !defined ION_PLUGIN
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

				mManualPrimitive->setCastShadows(true);
			}
			#endif
		}

		Primitive::~Primitive()
		{
			//Manual primitive destroyed with scene manager
		}

		void Primitive::Begin(Material* material, Primitive::Pattern pattern)
		{
			#if !defined ION_PLUGIN
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

				std::string materialName = "BaseWhite";

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
			#if !defined ION_PLUGIN
			if(mManualPrimitive)
			{
				mManualPrimitive->end();
			}
			#endif
		}

		void Primitive::AddVertex(const Vertex& vertex)
		{
			#if !defined ION_PLUGIN
			if(mManualPrimitive)
			{
				mManualPrimitive->position(vertex.x, vertex.y, vertex.z);
			}
			#endif
		}

		void Primitive::AddNormal(const Vector3& normal)
		{
			#if !defined ION_PLUGIN
			if(mManualPrimitive)
			{
				mManualPrimitive->normal(normal.x, normal.y, normal.z);
			}
			#endif
		}

		void Primitive::AddTextureCoord(const TexCoord& coord)
		{
			#if !defined ION_PLUGIN
			if(mManualPrimitive)
			{
				mManualPrimitive->textureCoord(coord.x, coord.y);
			}
			#endif
		}

		void Primitive::AddIndex(unsigned int index)
		{
			#if !defined ION_PLUGIN
			if(mManualPrimitive)
			{
				mManualPrimitive->index(index);
			}
			#endif
		}

		void Primitive::AddQuad(float width, float height, Primitive::QuadAxis axis, const Vector3& offset)
		{
			float halfWidth = width * 0.5f;
			float halfHeight = height * 0.5f;

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

				AddVertex(Vertex(offset.x + -halfWidth, offset.y, offset.z + -halfHeight)); AddNormal(Vector3(0.0f, 1.0f, 0.0f)); AddTextureCoord(TexCoord(0.0f, 0.0f));
				AddVertex(Vertex(offset.x +  halfWidth, offset.y, offset.z +  halfHeight)); AddNormal(Vector3(0.0f, 1.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 1.0f));
				AddVertex(Vertex(offset.x +  halfWidth, offset.y, offset.z + -halfHeight)); AddNormal(Vector3(0.0f, 1.0f, 0.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));
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
		}

		void Primitive::AddBox(const Vector3& halfExtents, const Vector3& offset)
		{
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
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, 1.0f)); AddTextureCoord(TexCoord(0.0f, 1.0f));;
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, 1.0f)); AddTextureCoord(TexCoord(1.0f, 1.0f));;
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, 1.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));;
																																				  
			AddVertex(Vertex(offset.x - halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, 1.0f)); AddTextureCoord(TexCoord(0.0f, 1.0f));;
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y - halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, 1.0f)); AddTextureCoord(TexCoord(1.0f, 0.0f));;
			AddVertex(Vertex(offset.x + halfExtents.x, offset.y + halfExtents.y, offset.z + halfExtents.z)); AddNormal(Vector3(0.0f, 0.0f, 1.0f)); AddTextureCoord(TexCoord(0.0f, 0.0f));;
		}

		#if !defined ION_PLUGIN
		Ogre::ManualObject* Primitive::GetOgreManualObj()
		{
			return mManualPrimitive;
		}
		#endif
	}
}