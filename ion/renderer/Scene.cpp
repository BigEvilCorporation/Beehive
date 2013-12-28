///////////////////////////////////////////////////
// File:		Scene.cpp
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Scene manager
///////////////////////////////////////////////////

#include "core/Colour.h"
#include "core/Debug.h"
#include "renderer/Scene.h"
#include "renderer/Mesh.h"
#include "renderer/SceneNode.h"
#include "renderer/Factory.h"

namespace ion
{
	namespace renderer
	{
		//Stream version supported by Scene::Serialise()
		const int Scene::sSerialiseVersion = 2;

		Scene* Scene::Create()
		{
			return new SceneImpl();
		}

		void Scene::Release(Scene* scene)
		{
			delete scene;
		}

		Scene::Scene()
		{
			//Create scene mesh
			mSceneMesh = Mesh::Create();

			//Set default properties
			SetShadowFarDistance(10.0f);
			SetShadowTextureResolution(2048);
			SetShadowTextureCount(2);
			SetShadowColour(ColourRGB(0.7f, 0.7f, 0.7f));
		}

		Scene::~Scene()
		{
			//Delete scene mesh
			Mesh::Release(mSceneMesh);
		}

		void Scene::AddSceneNode(SceneNode& sceneNode)
		{
			mSceneNodes.push_back(&sceneNode);
		}

		Mesh* Scene::GetSceneMesh()
		{
			return mSceneMesh;
		}

		std::list<Light*>& Scene::GetLights()
		{
			return mLights;
		}

		void Scene::SetAmbientLight(const ColourRGB& colour)
		{
			mAmbientLight = colour;
		}

		void Scene::SetShadowFarDistance(float distance)
		{
			mShadowFarDistance = distance;
		}

		void Scene::SetShadowColour(const ColourRGB& colour)
		{
			mAmbientLight = colour;
		}

		void Scene::SetShadowTextureResolution(int resolution)
		{
			mShadowTextureResolution = resolution;
		}

		void Scene::SetShadowTextureCount(int count)
		{
			mShadowTextureCount = count;
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
			mSceneMeshInstance = MeshInstance::Create();
			mSceneMeshNode = SceneNode::Create();
			AddSceneNode(*mSceneMeshNode);
			mSceneMeshNode->AttachEntity(*mSceneMeshInstance);
		}
	}
}