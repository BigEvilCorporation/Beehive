///////////////////////////////////////////////////
// File:		SceneNode.cpp
// Date:		4th August 2011
// Authors:		Matt Phillips
// Description:	Scene node
///////////////////////////////////////////////////

#include "SceneNode.h"
#include "Scene.h"
#include "Camera.h"
#include "Primitive.h"
#include "Mesh.h"

#if !defined ION_PLUGIN
#include <Ogre/OgreEntity.h>
#endif

namespace ion
{
	namespace renderer
	{
		SceneNode::SceneNode(Scene& scene)
		{
			#if !defined ION_PLUGIN
			mOgreSceneNode = scene.GetOgreSceneMgrIFace()->getRootSceneNode()->createChildSceneNode();
			mOgreSceneNode->setPosition(0.0f, 0.0f, 0.0f);
			mOgreScene = scene.GetOgreSceneMgrIFace();
			#endif
		}

		SceneNode::~SceneNode()
		{
			//Ogre::SceneNode destroyed with scene manager
		}

		#if !defined ION_PLUGIN
		Ogre::SceneNode* SceneNode::GetOgreSceneNode()
		{
			return mOgreSceneNode;
		}
		#endif

		void SceneNode::SetPosition(const Vector3& position)
		{
			#if !defined ION_PLUGIN
			if(mOgreSceneNode)
			{
				mOgreSceneNode->setPosition(position.x, position.y, position.z);
			}
			#endif		
		}

		void SceneNode::SetTransform(const Matrix4& matrix)
		{
			#if !defined ION_PLUGIN
			if(mOgreSceneNode)
			{
				Quaternion quat;
				quat.FromMatrix(matrix);
				SetPosition(matrix.GetTranslation());
				mOgreSceneNode->setOrientation(Ogre::Quaternion(quat.w, quat.x, quat.y, quat.z));
			}
			#endif
		}

		void SceneNode::SetLookAt(const Vector3& position)
		{
			#if !defined ION_PLUGIN
			if(mOgreSceneNode)
			{
				mOgreSceneNode->lookAt(Ogre::Vector3(position.x, position.y, position.z), Ogre::Node::TS_WORLD);
			}
			#endif	
		}

		void SceneNode::Attach(Primitive& primitive)
		{
			#if !defined ION_PLUGIN
			if(mOgreSceneNode)
			{
				mOgreSceneNode->attachObject(primitive.GetOgreManualObj());
			}
			#endif
		}

		void SceneNode::Attach(Camera& camera)
		{
			#if !defined ION_PLUGIN
			if(mOgreSceneNode)
			{
				mOgreSceneNode->attachObject(camera.GetOgreCameraIFace());
			}
			#endif
		}

		void SceneNode::Attach(Mesh& mesh)
		{
			#if !defined ION_PLUGIN
			if(mOgreSceneNode)
			{
				Ogre::Entity* entity = mOgreScene->createEntity(mesh.GetOgreMesh()->getName());
				entity->setMaterialName("BaseWhite");
				entity->setCastShadows(true);
				mOgreSceneNode->attachObject(entity);
				mOgreSceneNode->setVisible(true);
			}
			#endif
		}

		void SceneNode::Attach(Light& light)
		{
			#if !defined ION_PLUGIN
			if(mOgreSceneNode)
			{
				mOgreSceneNode->attachObject(light.GetOgreLightIFace());
			}
			#endif
		}
	}
}