///////////////////////////////////////////////////
// File:		PrimitiveOgre.cpp
// Date:		30th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d primitive implementation
///////////////////////////////////////////////////

#include "PrimitiveOgre.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			PrimitiveOgre::PrimitiveOgre()
			{
			}

			PrimitiveOgre::~PrimitiveOgre()
			{
			}

			void PrimitiveOgre::Begin(Material* material, Pattern pattern)
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

				mOgreManualObject->begin(materialName, renderOp);
			}

			void PrimitiveOgre::End()
			{
				mOgreManualObject->end();
			}

			void PrimitiveOgre::Clear()
			{
				mOgreManualObject->clear();
			}

			void PrimitiveOgre::AddVertex(const Vertex& vertex)
			{
				mOgreManualObject->position(vertex.x, vertex.y, vertex.z);
			}

			void PrimitiveOgre::AddNormal(const Vector3& normal)
			{
				if(mOgreManualObject)
				{
					mOgreManualObject->normal(normal.x, normal.y, normal.z);
				}
			}

			void PrimitiveOgre::AddColour(const Colour& colour)
			{
				mOgreManualObject->colour(colour.r, colour.g, colour.b, colour.a);
			}

			void PrimitiveOgre::AddTextureCoord(const TexCoord& coord)
			{
				mOgreManualObject->textureCoord(coord.x, coord.y);
			}

			void PrimitiveOgre::AddIndex(unsigned int index)
			{
				mOgreManualObject->index(index);
			}

			void PrimitiveOgre::SetCastShadows(bool shadows)
			{
				mOgreManualObject->setCastShadows(shadows);
			}

			void PrimitiveOgre::AddToOgreScene(Ogre::SceneManager* sceneManager)
			{
				mOgreManualObject = sceneManager->createManualObject();

				if(mProjection == Proj2D)
				{
					//Use identity view/projection matrices
					mOgreManualObject->setUseIdentityProjection(true);
					mOgreManualObject->setUseIdentityView(true);

					//Infinite clip bounds
					Ogre::AxisAlignedBox bounds;
					bounds.setInfinite();
					mOgreManualObject->setBoundingBox(bounds);

					//Render before overlay renderstage
					mOgreManualObject->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY - 1);
				}
			}
		}
	}
}
