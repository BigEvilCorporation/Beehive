///////////////////////////////////////////////////
// File:		SceneNode.cpp
// Date:		4th August 2011
// Authors:		Matt Phillips
// Description:	Scene node
///////////////////////////////////////////////////

#include "renderer/SceneNode.h"

namespace ion
{
	namespace renderer
	{
		SceneNode::SceneNode()
		{

		}

		SceneNode::~SceneNode()
		{

		}

		void SceneNode::AttachEntity(Entity& entity)
		{
			mEntities.push_back(&entity);
		}

		void SceneNode::RemoveEntity(Entity& entity)
		{
			mEntities.remove(&entity);
		}
	}
}