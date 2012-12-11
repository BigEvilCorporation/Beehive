///////////////////////////////////////////////////
// File:		Scene.cpp
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Ogre scene manager
///////////////////////////////////////////////////

#include "Scene.h"
#include "Mesh.h"
#include "../core/Colour.h"
#include "../core/BinaryFile.h"
#include "../core/Debug.h"

#if !defined ION_PLUGIN
#include <Ogre/OgreRoot.h>
#include <Ogre/OgreSceneNode.h>
#include <Ogre/OgreEntity.h>
#endif

namespace ion
{
	namespace renderer
	{
		//Minimum file version supported by Scene::Load()
		const int Scene::sMinFileVersion = 1;

		//File version output by Scene::Save()
		const int Scene::sCurrentFileVersion = 1;

		//Scene file header type
		const char* Scene::sFileType = "ion::scene";

		Scene::Scene()
		{
			mSceneMesh = new Mesh();

			#if !defined ION_PLUGIN
			mOgreSceneMgrIFace = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, "Scene");

			Ogre::Entity* sceneMeshEntity = mOgreSceneMgrIFace->createEntity(mSceneMesh->GetOgreMesh()->getName());
			mOgreSceneMgrIFace->getRootSceneNode()->attachObject(sceneMeshEntity);

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

		#if defined ION_PLUGIN
		Mesh* Scene::GetSceneMesh()
		{
			return mSceneMesh;
		}

		std::list<Light*>& Scene::GetLights()
		{
			return mLights;
		}
		#endif

		#if !defined ION_PLUGIN
		Ogre::SceneManager* Scene::GetOgreSceneMgrIFace()
		{
			return mOgreSceneMgrIFace;
		}
		#endif

		void Scene::SetAmbientLight(const ColourRGB& colour)
		{
			#if !defined ION_PLUGIN
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
			#if !defined ION_PLUGIN
			mOgreSceneMgrIFace->setShadowFarDistance(distance);
			#endif
		}

		void Scene::SetShadowColour(const ColourRGB& colour)
		{
			mOgreSceneMgrIFace->setShadowColour(Ogre::ColourValue(colour.r, colour.g, colour.b));
		}

		void Scene::SetShadowTextureResolution(int resolution)
		{
			mOgreSceneMgrIFace->setShadowTextureSize(resolution);
		}

		void Scene::SetShadowTextureCount(int count)
		{
			mOgreSceneMgrIFace->setShadowTextureCount(count);
		}

		bool Scene::Load(std::string filename)
		{
			//Open file for reading
			io::BinaryFile file(filename, ion::io::File::OpenRead);

			if(file.IsOpen())
			{
				//Check header
				if(stricmp(sFileType, file.GetFileType().c_str()) != 0)
				{
					ion::debug::Error("Scene::Load() - Not an ion::scene file");
				}

				//Check version
				if(file.GetFileVersion() < sMinFileVersion)
				{
					ion::debug::Error("Scene::Load() - Unsupported file version");
				}

				//If no bounds chunk found, we need to manually calculate from vertices
				bool boundsFound = false;

				//For all root chunks
				for(io::BinaryFile::Chunk rootChunk = file.GetTrunk().Begin(), rootEnd = file.GetTrunk().End(); rootChunk != rootEnd; rootChunk = rootChunk.Next())
				{
					if(rootChunk.GetID() == ChunkId_SceneMeshData)
					{
						for(io::BinaryFile::Chunk meshChunk = rootChunk.Begin(), meshEnd = rootChunk.End(); meshChunk != meshEnd; meshChunk = meshChunk.Next())
						{
							if(meshChunk.GetID() == ChunkId_SubMesh)
							{
								//Create submesh
								Mesh::SubMesh* subMesh = mSceneMesh->CreateSubMesh();

								//Read submesh data
								subMesh->Read(meshChunk);
							}
							else if(meshChunk.GetID() == ChunkId_Bounds)
							{
								//TODO
							}
						}
					}
					else if(rootChunk.GetID() == ChunkId_Light)
					{
						//Create light
						Light* light = new Light(*this);

						//Read data
						light->Read(rootChunk);

						//Add
						mLights.push_back(light);
					}
				}

				//Done, finalise mesh
				if(mSceneMesh)
				{
					if(!boundsFound)
					{
						mSceneMesh->CalculateBounds();
					}

					mSceneMesh->Finalise();
				}

				return true;
			}

			return false;
		}

		u64 Scene::Save(std::string filename)
		{
			u64 fileSize = 0;

			//Open file for writing
			io::BinaryFile file(filename, ion::io::File::OpenWrite);

			if(file.IsOpen())
			{
				//Setup header
				file.SetFileType(sFileType);
				file.SetFileVersion(sCurrentFileVersion);

				//Set trunk node id
				file.GetTrunk().SetID(ChunkId_Root);

				if(mSceneMesh)
				{
					if(mSceneMesh->GetSubMeshes().size() > 0)
					{
						//Create scene mesh data chunk
						io::BinaryFile::Chunk sceneMeshChunk;
						sceneMeshChunk.SetID(ChunkId_SceneMeshData);

						//For all submeshes
						for(std::list<std::list<Mesh::SubMesh*>>::iterator subMesh = mSceneMesh->GetSubMeshes().begin(), subMeshEnd = mSceneMesh->GetSubMeshes().end(); subMesh != subMeshEnd; ++subMesh)
						{
							if((*subMesh).size() > 0)
							{
								//Create submesh chunk
								io::BinaryFile::Chunk subMeshChunk;
								subMeshChunk.SetID(ChunkId_SubMesh);

								//Only use one lod for scene data
								Mesh::SubMesh* firstLod = (*(*subMesh).begin());

								//Write
								firstLod->Write(subMeshChunk);

								//Add submesh chunk to scene data chunk
								sceneMeshChunk.AddChild(subMeshChunk);
							}
						}

						//Add scene mesh chunk to trunk
						file.GetTrunk().AddChild(sceneMeshChunk);
					}
				}

				//For all lights
				for(std::list<Light*>::iterator light = mLights.begin(), end = mLights.end(); light != end; ++light)
				{
					//Create light chunk
					io::BinaryFile::Chunk lightChunk;
					lightChunk.SetID(ChunkId_Light);

					//Write data
					(*light)->Write(lightChunk);

					//Add to trunk
					file.GetTrunk().AddChild(lightChunk);
				}

				/*
				//Calculate world bounds
				CalculateBounds();

				//Create bounds chunk
				io::BinaryFile::Chunk boundsChunk;
				boundsChunk.SetID(ChunkId_Bounds);
				boundsChunk.SetData(&mBounds, sizeof(mBounds), 1);
				file.GetTrunk().AddChild(boundsChunk);
				*/

				//Get total filesize
				fileSize = file.GetTrunk().GetChunkSize() + sizeof(io::BinaryFile::FileHeader);

				//Write file
				file.Write();

				//Close file
				file.Close();
			}

			return fileSize;
		}
	}
}