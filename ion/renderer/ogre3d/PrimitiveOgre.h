///////////////////////////////////////////////////
// File:		PrimitiveOgre.h
// Date:		30th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d primitive implementation
///////////////////////////////////////////////////

#pragma once

#include <Ogre/OgreRoot.h>
#include <Ogre/OgreManualObject.h>

#include "core/Debug.h"
#include "renderer/Primitive.h"
#include "renderer/TexCoord.h"
#include "renderer/Material.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			class PrimitiveOgre : public Primitive
			{
			public:
				PrimitiveOgre();
				~PrimitiveOgre();

				virtual void Begin(Material* material, Pattern pattern);
				virtual void End();
				virtual void Clear();
				virtual void AddVertex(const Vertex& vertex);
				virtual void AddNormal(const Vector3& normal);
				virtual void AddColour(const Colour& colour);
				virtual void AddTextureCoord(const TexCoord& coord);
				virtual void AddIndex(unsigned int index);
				virtual void SetCastShadows(bool shadows);

				void AddToOgreScene(Ogre::SceneManager* sceneManager);

				Ogre::ManualObject* mOgreManualObject;
			};
		}

		typedef ogre::PrimitiveOgre PrimitiveImpl;
	}
}
