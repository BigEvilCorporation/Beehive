///////////////////////////////////////////////////
// File:		LightOgre.h
// Date:		28th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d light implementation
///////////////////////////////////////////////////

#pragma once

#include <Ogre/OgreRoot.h>
#include <Ogre/OgreLight.h>

#include "renderer/Light.h"

namespace ion
{
	//Forward declaration
	class ColourRGB;

	namespace renderer
	{
		namespace ogre
		{
			class LightOgre : public Light
			{
			public:
				LightOgre();
				~LightOgre();

				virtual void SetType(Type type);
				virtual void SetPosition(const Vector3& position);
				virtual void SetDirection(const Vector3& direction);
				virtual void SetDiffuse(const ColourRGB& diffuse);
				virtual void SetSpecular(const ColourRGB& specular);
				virtual void SetAttenuation(float maxDistance);
				virtual void SetSpotRange(float minAngle, float maxAngle);
				virtual void CastShadows(bool enabled);

				void AddToOgreScene(Ogre::SceneManager* sceneManager);

				Ogre::Light* mOgreLight;
			};
		}

		typedef ogre::LightOgre LightImpl;
	}
}