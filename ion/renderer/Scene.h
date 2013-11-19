///////////////////////////////////////////////////
// File:		Scene.h
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Ogre scene manager
///////////////////////////////////////////////////

#pragma once

#include "../core/Types.h"
#include "../core/Colour.h"
#include "Light.h"
#include "../core/Archive.h"

#if defined ION_OGRE
#include <Ogre/OgreSceneManager.h>
#endif

#include <list>

namespace ion
{
	namespace renderer
	{
		//Forward declaration
		class Primitive;
		class Mesh;

		class Scene
		{
		public:
			Scene();
			~Scene();

			//Lighting and shadows
			void AddLight(Light& light);
			void SetAmbientLight(const ColourRGB& colour);
			void SetShadowColour(const ColourRGB& colour);
			void SetShadowFarDistance(float distance);
			void SetShadowTextureResolution(int resolution);
			void SetShadowTextureCount(int count);

			Mesh* GetSceneMesh();
			std::list<Light*>& GetLights();

			#if defined ION_OGRE
			Ogre::SceneManager* GetOgreSceneMgrIFace();
			#endif

			//Serialisation
			void Serialise(serialise::Archive& archive);
			
			static const int sSerialiseVersion;

		private:

			Mesh* mSceneMesh;
			ColourRGB mAmbientLight;
			std::list<Light*> mLights;

			#if defined ION_OGRE
			Ogre::SceneManager* mOgreSceneMgrIFace;
			#endif
		};
	}
}