///////////////////////////////////////////////////
// File:		SceneNodeOgre.h
// Date:		27th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d scene node implementation
///////////////////////////////////////////////////

#pragma once

#include <Ogre/OgreRoot.h>
#include <Ogre/OgreSceneNode.h>
#include <Ogre/OgreSceneManager.h>
#include <Ogre/OgreEntity.h>

#include "renderer/SceneNode.h"

namespace ion
{
	namespace renderer
	{
		//Forward declaration
		class Entity;

		namespace ogre
		{
			class SceneNodeOgre : public SceneNode
			{
			public:
				SceneNodeOgre();
				~SceneNodeOgre();
				virtual void AttachEntity(Entity& entity);
				virtual void RemoveEntity(Entity& entity);

				void AddToOgreScene(Ogre::SceneManager* sceneManager);

				Ogre::SceneNode* mOgreSceneNode;
				Ogre::SceneManager* mOgreSceneManager;
			};
		}

		typedef ogre::SceneNodeOgre SceneNodeImpl;
	}
}