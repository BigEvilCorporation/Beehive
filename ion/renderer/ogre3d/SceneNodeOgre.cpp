///////////////////////////////////////////////////
// File:		SceneNodeOgre.cpp
// Date:		27th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d scene node implementation
///////////////////////////////////////////////////

#include "SceneNodeOgre.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			SceneNodeOgre::SceneNodeOgre()
			{
				//Only created when added to Ogre scene manager
				mOgreSceneNode = NULL;
			}

			SceneNodeOgre::~SceneNodeOgre()
			{
				if(mOgreSceneNode)
				{
					mOgreSceneNode->getCreator()->destroySceneNode(mOgreSceneNode);
				}
			}

			void SceneNodeOgre::SetPosition(const Vector3& position)
			{
				SceneNode::SetPosition(position);
				SetOgreTransform();
			}

			void SceneNodeOgre::SetOrientation(const Quaternion& orientation)
			{
				SceneNode::SetOrientation(orientation);
				SetOgreTransform();
			}

			void SceneNodeOgre::SetTransform(const Matrix4& matrix)
			{
				SceneNode::SetTransform(matrix);
				SetOgreTransform();
			}

			void SceneNodeOgre::SetLookAt(const Vector3& position)
			{
				SceneNode::SetLookAt(position);
				SetOgreTransform();
			}

			void SceneNodeOgre::Move(const Vector3& moveVector)
			{
				SceneNode::Move(moveVector);
				SetOgreTransform();
			}

			void SceneNodeOgre::Pitch(float pitch, TransformSpace transformSpace)
			{
				SceneNode::Pitch(pitch, transformSpace);
				SetOgreTransform();
			}

			void SceneNodeOgre::Yaw(float yaw, TransformSpace transformSpace)
			{
				SceneNode::Yaw(yaw, transformSpace);
				SetOgreTransform();
			}

			void SceneNodeOgre::Roll(float roll, TransformSpace transformSpace)
			{
				SceneNode::Roll(roll, transformSpace);
				SetOgreTransform();
			}

			void SceneNodeOgre::AttachEntity(Entity& entity)
			{
				SceneNode::AttachEntity(entity);
			}

			void SceneNodeOgre::RemoveEntity(Entity& entity)
			{
				SceneNode::RemoveEntity(entity);
			}

			void SceneNodeOgre::AddToOgreScene(Ogre::SceneManager* sceneManager)
			{
				mOgreSceneNode = sceneManager->createSceneNode();
				mOgreSceneNode->setFixedYawAxis(true);
				SetOgreTransform();
			}
		}
	}
}