///////////////////////////////////////////////////
// File:		Scene.h
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Scene manager
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include "core/Colour.h"
#include "core/Archive.h"

#include <list>

namespace ion
{
	namespace renderer
	{
		//Forward declaration
		class Mesh;
		class MeshInstance;
		class SceneNode;
		class Light;

		class Scene
		{
		public:
			
			//Factory
			static Scene* Create();
			static void Release(Scene* scene);

			//Scene nodes
			virtual void AddSceneNode(SceneNode& sceneNode);

			//Lighting and shadows
			virtual void SetAmbientLight(const ColourRGB& colour);
			virtual void SetShadowColour(const ColourRGB& colour);
			virtual void SetShadowFarDistance(float distance);
			virtual void SetShadowTextureResolution(int resolution);
			virtual void SetShadowTextureCount(int count);

			Mesh* GetSceneMesh();
			std::list<Light*>& GetLights();

			//Serialisation
			void Serialise(serialise::Archive& archive);
			
			static const int sSerialiseVersion;

		protected:

			//Use factory Create()/Release()
			Scene();
			virtual ~Scene();

		private:

			//Post-serialisation finalise
			void Finalise();

			//Scene mesh, instance and node
			Mesh* mSceneMesh;
			MeshInstance* mSceneMeshInstance;
			SceneNode* mSceneMeshNode;

			//Scene properties
			ColourRGB mAmbientLight;
			ColourRGB mShadowColour;
			float mShadowFarDistance;
			int mShadowTextureResolution;
			int mShadowTextureCount;

			//Scene nodes
			std::list<SceneNode*> mSceneNodes;

			//Lights
			std::list<Light*> mLights;
		};
	}
}