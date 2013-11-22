///////////////////////////////////////////////////
// File:		Scene.cpp
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Ogre scene manager
///////////////////////////////////////////////////

#include "Scene.h"
#include "Mesh.h"
#include "SceneNode.h"
#include "../core/Colour.h"
#include "../core/BinaryFile.h"
#include "../core/Debug.h"

#include <sstream>

#if defined ION_OGRE
#include <Ogre/OgreRoot.h>
#include <Ogre/OgreSceneNode.h>
#include <Ogre/OgreEntity.h>
#endif

namespace ion
{
	namespace renderer
	{
		//Stream version supported by Scene::Serialise()
		const int Scene::sSerialiseVersion = 2;

		Scene::Scene()
		{
			mSceneMesh = new Mesh();

			#if defined ION_OGRE
			static int sceneId = 0;
			std::stringstream sceneName;
			sceneName << "Scene_" << sceneId++;
			mOgreSceneMgrIFace = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, sceneName.str().c_str());

			//Default shadow setup
			mOgreSceneMgrIFace->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
			SetShadowFarDistance(10.0f);
			SetShadowTextureResolution(2048);
			SetShadowTextureCount(2);
			SetShadowColour(ColourRGB(0.7f, 0.7f, 0.7f));
			#endif
		}

		Scene::~Scene()
		{
			if(mSceneMesh)
			{
				delete mSceneMesh;
			}

			//Ogre::SceneManager destroyed with the renderer
		}

		Mesh* Scene::GetSceneMesh()
		{
			return mSceneMesh;
		}

		std::list<Light*>& Scene::GetLights()
		{
			return mLights;
		}

		#if defined ION_OGRE
		Ogre::SceneManager* Scene::GetOgreSceneMgrIFace()
		{
			return mOgreSceneMgrIFace;
		}
		#endif

		void Scene::SetAmbientLight(const ColourRGB& colour)
		{
			#if defined ION_OGRE
			if(mOgreSceneMgrIFace)
			{
				mOgreSceneMgrIFace->setAmbientLight(Ogre::ColourValue(colour.r, colour.g, colour.b));
			}
			#endif

			mAmbientLight = colour;
		}

		void Scene::AddLight(Light& light)
		{
			mLights.push_back(&light);
		}

		void Scene::SetShadowFarDistance(float distance)
		{
			#if defined ION_OGRE
			mOgreSceneMgrIFace->setShadowFarDistance(distance);
			#endif
		}

		void Scene::SetShadowColour(const ColourRGB& colour)
		{
			#if defined ION_OGRE
			mOgreSceneMgrIFace->setShadowColour(Ogre::ColourValue(colour.r, colour.g, colour.b));
			#endif
		}

		void Scene::SetShadowTextureResolution(int resolution)
		{
			#if defined ION_OGRE
			mOgreSceneMgrIFace->setShadowTextureSize(resolution);
			#endif
		}

		void Scene::SetShadowTextureCount(int count)
		{
			#if defined ION_OGRE
			mOgreSceneMgrIFace->setShadowTextureCount(count);
			#endif
		}

		void Scene::Serialise(serialise::Archive& archive)
		{
			//Register pointer types
			archive.RegisterPointerType<ion::renderer::Mesh>();
			archive.RegisterPointerType<ion::renderer::Light>();

			//Serialise scene properties
			archive.Serialise(mAmbientLight);

			//Serialise scene mesh
			archive.Serialise(mSceneMesh);

			//Serialise lights
			archive.Serialise(mLights);

			//If serialising in, finalise
			if(archive.GetDirection() == serialise::Archive::In)
			{
				Finalise();
			}
		}

		void Scene::Finalise()
		{
			mSceneMeshInstance = new MeshInstance(*mSceneMesh, *this);
			mSceneMeshNode = new SceneNode(*this);
			mSceneMeshNode->Attach(*mSceneMeshInstance);
		}
	}
}