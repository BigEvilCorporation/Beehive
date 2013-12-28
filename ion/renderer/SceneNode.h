///////////////////////////////////////////////////
// File:		SceneNode.h
// Date:		4th August 2011
// Authors:		Matt Phillips
// Description:	Scene node
///////////////////////////////////////////////////

#pragma once

#include "renderer/Entity.h"

namespace ion
{
	namespace renderer
	{
		class SceneNode : public Entity
		{
		public:

			//Factory
			static SceneNode* Create();
			static void Release(SceneNode* scene);

			//Attach/remove entity
			virtual void AttachEntity(Entity& entity);
			virtual void RemoveEntity(Entity& entity);

		protected:

			//Use factory Create()/Release()
			SceneNode();
			virtual ~SceneNode();

		private:

			std::list<Entity*> mEntities;
		};
	}
}