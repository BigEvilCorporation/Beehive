///////////////////////////////////////////////////
// File:		SceneNode.h
// Date:		4th August 2011
// Authors:		Matt Phillips
// Description:	Scene node
///////////////////////////////////////////////////

#pragma once

#include "../core/maths/Vector.h"
#include "../core/maths/Matrix.h"
#include "../core/maths/Quaternion.h"

#if defined ION_OGRE
#include <Ogre/OgreSceneNode.h>
#include <Ogre/OgreSceneManager.h>
#endif

namespace ion
{
	namespace renderer
	{
		//Forward declaration
		class Scene;
		class Primitive;
		class Camera;
		class MeshInstance;
		class Light;

		class SceneNode
		{
		public:
			SceneNode(Scene& scene);
			~SceneNode();

			void SetPosition(const Vector3& position);
			void SetTransform(const Matrix4& matrix);
			void SetLookAt(const Vector3& position);

			void Attach(Primitive& primitive);
			void Attach(Camera& camera);
			void Attach(MeshInstance& meshInstance);
			void Attach(Light& light);

			void Detach(Primitive& primitive);

			#if defined ION_OGRE
			Ogre::SceneNode* GetOgreSceneNode();
			#endif

		private:
			#if defined ION_OGRE
			Ogre::SceneNode* mOgreSceneNode;
			Ogre::SceneManager* mOgreScene;
			#endif
		};
	}
}