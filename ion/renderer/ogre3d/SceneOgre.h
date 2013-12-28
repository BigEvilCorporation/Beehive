///////////////////////////////////////////////////
// File:		SceneOgre.h
// Date:		27th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d scene implementation
///////////////////////////////////////////////////

#pragma once

#include <Ogre/OgreRoot.h>
#include <Ogre/OgreSceneManager.h>

#include "renderer/Scene.h"

namespace ion
{
	//Forward declaration
	class ColourRGB;

	namespace renderer
	{
		//Forward declaration
		class SceneNode;

		namespace ogre
		{
			class SceneOgre : public Scene
			{
			public:
				SceneOgre();
				~SceneOgre();

				virtual void AddSceneNode(SceneNode& sceneNode);
				virtual void SetAmbientLight(const ColourRGB& colour);
				virtual void SetShadowColour(const ColourRGB& colour);
				virtual void SetShadowFarDistance(float distance);
				virtual void SetShadowTextureResolution(int resolution);
				virtual void SetShadowTextureCount(int count);

				Ogre::SceneManager* mOgreSceneManager;
			};
		}

		typedef ogre::SceneOgre SceneImpl;
	}
}