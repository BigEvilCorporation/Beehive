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

#if !defined ION_PLUGIN
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

			bool Load(std::string filename);
			u64 Save(std::string filename);

			#if defined ION_PLUGIN
			Mesh* GetSceneMesh();
			std::list<Light*>& GetLights();
			#else
			Ogre::SceneManager* GetOgreSceneMgrIFace();
			#endif

		private:
			enum ChunkIds
			{
				ChunkId_Root				= CHUNK_ID('ROOT'),

					ChunkId_AmbientLight	= CHUNK_ID('ALIT'),
					ChunkId_WorldBounds		= CHUNK_ID('WBND'),
					ChunkId_Skybox			= CHUNK_ID('SKYB'),

					ChunkId_SceneMeshData	= CHUNK_ID('MESH'),
						ChunkId_SubMesh		= CHUNK_ID('SUB_'),
						ChunkId_Bounds		= CHUNK_ID('BNDS'),	//TODO: Belongs in the SubMesh chunk

					ChunkId_MeshInstance	= CHUNK_ID('MSHI'),
						//ChunkId_Name
						//ChunkId_Position
						//ChunkId_Orientation

					ChunkId_Light			= CHUNK_ID('LIT_'),

					//ChunkId_Entity		= CHUNK_ID('ENTY'),
					//ChunkId_Script		= CHUNK_ID('SCRP'),

				ChunkId_Name				= CHUNK_ID('NAME'),
				ChunkId_Position			= CHUNK_ID('POS_'),
				ChunkId_Direction			= CHUNK_ID('DIR_'),
				ChunkId_Rotation			= CHUNK_ID('ROT_')
			};

			static const int sMinFileVersion;
			static const int sCurrentFileVersion;
			static const char* sFileType;

			Mesh* mSceneMesh;
			ColourRGB mAmbientLight;
			std::list<Light*> mLights;

			#if !defined ION_PLUGIN
			Ogre::SceneManager* mOgreSceneMgrIFace;
			#endif
		};
	}
}