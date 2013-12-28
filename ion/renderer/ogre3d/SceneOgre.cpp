///////////////////////////////////////////////////
// File:		SceneOgre.cpp
// Date:		27th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d scene implementation
///////////////////////////////////////////////////

#include "SceneOgre.h"
#include "SceneNodeOgre.h"

#include "core/Colour.h"
#include "renderer/SceneNode.h"

#include <sstream>

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			SceneOgre::SceneOgre()
			{
				static int sceneId = 0;
				std::stringstream sceneName;
				sceneName << "Scene_" << sceneId++;
				mOgreSceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, sceneName.str().c_str());

				//Default shadow setup
				mOgreSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
			}

			SceneOgre::~SceneOgre()
			{
				Ogre::Root::getSingleton().destroySceneManager(mOgreSceneManager);
			}

			void SceneOgre::AddSceneNode(SceneNode& sceneNode)
			{
				((SceneNodeOgre&)sceneNode).AddToOgreScene(mOgreSceneManager);
				mOgreSceneManager->getRootSceneNode()->addChild(((SceneNodeOgre&)sceneNode).GetOgreSceneNode());
			}

			void SceneOgre::SetAmbientLight(const ColourRGB& colour)
			{
				Scene::SetAmbientLight(colour);
				mOgreSceneManager->setAmbientLight(Ogre::ColourValue(colour.r, colour.g, colour.b));
			}

			void SceneOgre::SetShadowColour(const ColourRGB& colour)
			{
				Scene::SetShadowColour(colour);
				mOgreSceneManager->setShadowColour(Ogre::ColourValue(colour.r, colour.g, colour.b));
			}

			void SceneOgre::SetShadowFarDistance(float distance)
			{
				Scene::SetShadowFarDistance(distance);
				mOgreSceneManager->setShadowFarDistance(distance);
			}

			void SceneOgre::SetShadowTextureResolution(int resolution)
			{
				Scene::SetShadowTextureResolution(resolution);
				mOgreSceneManager->setShadowTextureSize(resolution);
			}

			void SceneOgre::SetShadowTextureCount(int count)
			{
				Scene::SetShadowTextureCount(count);
				mOgreSceneManager->setShadowTextureCount(count);
			}
		}
	}
}