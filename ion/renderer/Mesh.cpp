///////////////////////////////////////////////////
// File:		Mesh.cpp
// Date:		8th August 2011
// Authors:		Matt Phillips
// Description:	Vertex mesh and file loader
///////////////////////////////////////////////////

#pragma once

#include "Mesh.h"
#include "Scene.h"
#include "../core/Debug.h"

#if !defined ION_PLUGIN
#include <Ogre/OgreMesh.h>
#include <Ogre/OgreMeshManager.h>
#include <Ogre/OgreHardwareBufferManager.h>
#endif

#include <sstream>

namespace ion
{
	namespace renderer
	{
		//Minimum file version supported by Mesh::Load()
		const int Mesh::sMinFileVersion = 2;

		//File version output by Mesh::Save()
		const int Mesh::sCurrentFileVersion = 2;

		//Mesh file header type
		const char* Mesh::sFileType = "ion::mesh";

		int Mesh::sMeshIndex = 0;

		Mesh::SubMesh::SubMesh(Mesh* mesh)
		{
			#if !defined ION_PLUGIN
			mOgreSubMesh = mesh->GetOgreMesh()->createSubMesh();

			mOgreVertexData = new Ogre::VertexData;

			//Setup vertex declaration
			mOgreVertexData->vertexDeclaration->addElement(VertexBuffer, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);

			mOgreSubMesh->useSharedVertices = false;
			mOgreSubMesh->vertexData = mOgreVertexData;
			mOgreSubMesh->setMaterialName("BaseWhite");
			#endif
		}

		Mesh::SubMesh::~SubMesh()
		{
		}

		void Mesh::SubMesh::CreateVertexBuffer(int numVertices)
		{
			#if !defined ION_PLUGIN
			mOgreVertexData->vertexCount = numVertices;
			mOgreVertexBuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(sizeof(Vertex),
																								numVertices,
																								Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
			#endif
		}

		void Mesh::SubMesh::CreateNormalBuffer(int numNormals)
		{
			#if !defined ION_PLUGIN
			//Add normals to vertex declaration
			mOgreVertexData->vertexDeclaration->addElement(NormalBuffer, 0, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
			
			//Create normal buffer
			mOgreNormalBuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(sizeof(Vector3),
																								numNormals,
																								Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
			#endif
		}

		void Mesh::SubMesh::CreateIndexBuffer(int numIndices)
		{
			#if !defined ION_PLUGIN
			mOgreIndexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT,
																							numIndices,
																							Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
			#endif
		}

		void Mesh::SubMesh::CreateTexCoordBuffer(int numTexCoords)
		{
			#if !defined ION_PLUGIN
			//Add text coords to vertex declaration
			mOgreVertexData->vertexDeclaration->addElement(TexCoordBuffer, 0, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);

			//Create tex coord buffer
			mOgreTexCoordBuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(sizeof(TexCoord),
																								numTexCoords,
																								Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
			#endif
		}

		void Mesh::SubMesh::AddVertex(Vertex& vertex)
		{
			mVertices.push_back(vertex);
		}

		void Mesh::SubMesh::AddNormal(Vector3& normal)
		{
			mNormals.push_back(normal);
		}

		void Mesh::SubMesh::AddFace(Face& face)
		{
			mFaces.push_back(face);
		}

		void Mesh::SubMesh::MapBone(Bone& bone, u32 vertexIdx, float weight)
		{
			#if !defined ION_PLUGIN
			Ogre::VertexBoneAssignment boneAssignment;
			boneAssignment.boneIndex = bone.GetOgreBone()->getHandle();
			boneAssignment.vertexIndex = vertexIdx;
			boneAssignment.weight = weight;
			mOgreSubMesh->addBoneAssignment(boneAssignment);
			#endif
		}

		void Mesh::SubMesh::Finalise()
		{
			#if !defined ION_PLUGIN
			if(mOgreVertexBuffer->getNumVertices() == 0 || mOgreVertexBuffer->getNumVertices() != mVertices.size())
			{
				ion::debug::Error("Mesh::SubMesh::Finalise() - Cannot finalise submesh, incorrect number of vertices");
			}

			if(mOgreIndexBuffer->getNumIndexes() == 0 || mOgreIndexBuffer->getNumIndexes() != mFaces.size() * 3)
			{
				ion::debug::Error("Mesh::SubMesh::Finalise() - Cannot finalise submesh, incorrect number of indices");
			}

			if(!mOgreNormalBuffer.isNull() && mOgreNormalBuffer->getNumVertices() != mNormals.size())
			{
				ion::debug::Error("Mesh::SubMesh::Finalise() - Cannot finalise submesh, incorrect number of normals");
			}

			if(!mOgreTexCoordBuffer.isNull() && mOgreTexCoordBuffer->getNumVertices() != mFaces.size() * 3)
			{
				ion::debug::Error("Mesh::SubMesh::Finalise() - Cannot finalise submesh, incorrect number of texture coordinates");
			}

			//Copy mesh data to Ogre hardware buffers
			mOgreVertexBuffer->writeData(0, sizeof(Vertex) * mVertices.size(), &mVertices[0], true);

			if(!mOgreNormalBuffer.isNull())
			{
				mOgreNormalBuffer->writeData(0, sizeof(Vector3) * mNormals.size(), &mNormals[0], true);
			}

			for(unsigned int i = 0; i < mFaces.size(); i++)
			{
				mOgreIndexBuffer->writeData(i * sizeof(Index) * 3, sizeof(Index) * 3, &mFaces[i].mIndices, true);

				if(!mOgreTexCoordBuffer.isNull())
				{
					mOgreTexCoordBuffer->writeData(i * sizeof(TexCoord) * 3, sizeof(TexCoord) * 3, &mFaces[i].mTexCoords, true);
				}
			}

			//Bind vertex, normal and texcoord buffers
			mOgreSubMesh->vertexData->vertexBufferBinding->setBinding(VertexBuffer, mOgreVertexBuffer);

			if(!mOgreNormalBuffer.isNull())
			{
				mOgreSubMesh->vertexData->vertexBufferBinding->setBinding(NormalBuffer, mOgreNormalBuffer);
			}

			if(!mOgreTexCoordBuffer.isNull())
			{
				mOgreSubMesh->vertexData->vertexBufferBinding->setBinding(TexCoordBuffer, mOgreTexCoordBuffer);
			}

			//Set index buffer
			mOgreSubMesh->indexData->indexBuffer = mOgreIndexBuffer;
			mOgreSubMesh->indexData->indexCount = mOgreIndexBuffer->getNumIndexes();
			mOgreSubMesh->indexData->indexStart = 0;
			#endif
		}

		int Mesh::SubMesh::GetNumVertices()
		{
			return mVertices.size();
		}

		int Mesh::SubMesh::GetNumNormals()
		{
			return mNormals.size();
		}

		int Mesh::SubMesh::GetNumFaces()
		{
			return mFaces.size();
		}

		std::vector<Vertex>& Mesh::SubMesh::GetVertices()
		{
			return mVertices;
		}

		std::vector<Vector3>& Mesh::SubMesh::GetNormals()
		{
			return mNormals;
		}

		std::vector<Face>& Mesh::SubMesh::GetFaces()
		{
			return mFaces;
		}

		void Mesh::SubMesh::SetName(std::string name)
		{
			mName = name;
		}

		const std::string& Mesh::SubMesh::GetName()
		{
			return mName;
		}

		void Mesh::SubMesh::SetMaterialName(std::string name)
		{
			mMaterialName = name;
		}

		const std::string& Mesh::SubMesh::GetMaterialName()
		{
			return mMaterialName;
		}

		bool Mesh::SubMesh::Read(io::BinaryFile::Chunk& binaryChunk)
		{
			//For all mesh data chunks
			for(io::BinaryFile::Chunk dataChunk = binaryChunk.Begin(), dataEnd = binaryChunk.End(); dataChunk != dataEnd; dataChunk = dataChunk.Next())
			{
				if(dataChunk.GetID() == ChunkId_Vertices)
				{
					//Read in vertices
					u64 numVerts = dataChunk.GetNumItems();

					CreateVertexBuffer((int)numVerts);

					for(int i = 0; i < numVerts; i++)
					{
						Vertex vertex;
						dataChunk.ReadData(&vertex, sizeof(Vertex));
						AddVertex(vertex);
					}
				}
				else if(dataChunk.GetID() == ChunkId_Normals)
				{
					//Read in normals
					u64 numNorms = dataChunk.GetNumItems();

					CreateNormalBuffer((int)numNorms);

					for(int i = 0; i < numNorms; i++)
					{
						Vector3 normal;
						dataChunk.ReadData(&normal, sizeof(Vector3));
						AddNormal(normal);
					}
				}
				else if(dataChunk.GetID() == ChunkId_Faces)
				{
					//Read in faces
					u64 numFaces = dataChunk.GetNumItems();

					CreateIndexBuffer((int)numFaces * 3);
					CreateTexCoordBuffer((int)numFaces * 3);

					for(int i = 0; i < numFaces; i++)
					{
						Face face;
						dataChunk.ReadData(&face, sizeof(Face));
						AddFace(face);
					}
				}
				else if(dataChunk.GetID() == ChunkId_MatName)
				{
					//Read in material name
					char text[128] = {0};
					u64 stringLength = dataChunk.GetDataSize();
					dataChunk.ReadData(text, stringLength);
					SetMaterialName(text);
				}
			}

			//Finished reading
			Finalise();

			return true;
		}

		u64 Mesh::SubMesh::Write(io::BinaryFile::Chunk& binaryChunk)
		{
			//Create vertex chunk
			io::BinaryFile::Chunk vertsChunk;
			vertsChunk.SetID(ChunkId_Vertices);

			//Create normals chunk
			io::BinaryFile::Chunk normsChunk;
			normsChunk.SetID(ChunkId_Normals);

			//Create faces chunk
			io::BinaryFile::Chunk facesChunk;
			facesChunk.SetID(ChunkId_Faces);

			//Set verts ptr
			vertsChunk.SetData(&GetVertices()[0], GetNumVertices() * sizeof(Vertex), GetNumVertices());

			//Set normals ptr
			normsChunk.SetData(&GetNormals()[0], GetNumNormals() * sizeof(Vector3), GetNumNormals());

			//Set faces ptr
			facesChunk.SetData(&GetFaces()[0], GetNumFaces() * sizeof(Face), GetNumFaces());

			//Add verts/norms/faces chunks
			binaryChunk.AddChild(vertsChunk);
			binaryChunk.AddChild(normsChunk);
			binaryChunk.AddChild(facesChunk);
						
			if(GetMaterialName().size() > 0)
			{
				//Create material name chunk
				io::BinaryFile::Chunk materialChunk;
				materialChunk.SetID(ChunkId_MatName);
						
				//Set material name ptr
				materialChunk.SetData((void*)&GetMaterialName()[0], GetMaterialName().size(), 1);

				//Add to chunk
				binaryChunk.AddChild(materialChunk);
			}

			return binaryChunk.GetChunkSize();
		}

		Mesh::Mesh()
		{
			#if !defined ION_PLUGIN
			std::stringstream meshName;
			meshName << "mesh_" << sMeshIndex++;
			mOgreMesh = Ogre::MeshManager::getSingleton().createManual(meshName.str(), "General");
			#endif

			//Default bounding box
			mBounds.min = Vector3(-0.5f, -0.5f, -0.5f);
			mBounds.max = Vector3(0.5f, 0.5f, 0.5f);
		}

		Mesh::~Mesh()
		{
			for(std::list<std::list<SubMesh*>>::iterator subMesh = mSubMeshes.begin(), subMeshEnd = mSubMeshes.end(); subMesh != subMeshEnd; ++subMesh)
			{
				for(std::list<SubMesh*>::iterator lod = subMesh->begin(), lodEnd = subMesh->end(); lod != lodEnd; ++lod)
				{
					delete (*lod);
				}
			}
		}

		Mesh::SubMesh* Mesh::CreateSubMesh()
		{
			//TODO: LOD support
			std::list<SubMesh*> subMeshSet;
			SubMesh* lod = new SubMesh(this);
			subMeshSet.push_back(lod);
			mSubMeshes.push_back(subMeshSet);

			return lod;
		}

		void Mesh::Finalise()
		{
			#if !defined ION_PLUGIN
			mOgreMesh->_setBounds(Ogre::AxisAlignedBox(mBounds.min.x, mBounds.min.y, mBounds.min.z, mBounds.max.x, mBounds.max.y, mBounds.max.z));
			mOgreMesh->load();
			#endif
		}

		void Mesh::SetSkeleton(Skeleton& skeleton)
		{
			#if !defined ION_PLUGIN
			mOgreMesh->_notifySkeleton(Ogre::SkeletonPtr(skeleton.GetOgreSkeleton()));
			#endif
		}

		void Mesh::CalculateBounds()
		{
			mBounds.min = Vector3(maths::FLOAT_MAX, maths::FLOAT_MAX, maths::FLOAT_MAX);
			mBounds.max = Vector3(maths::FLOAT_MIN, maths::FLOAT_MIN, maths::FLOAT_MIN);

			for(std::list<std::list<SubMesh*>>::iterator subMesh = mSubMeshes.begin(), subMeshEnd = mSubMeshes.end(); subMesh != subMeshEnd; ++subMesh)
			{
				for(std::list<SubMesh*>::iterator lod = subMesh->begin(), lodEnd = subMesh->end(); lod != lodEnd; lod++)
				{
					for(int i = 0; i < (*lod)->GetNumVertices(); i++)
					{
						Vertex& vert = (*lod)->GetVertices()[i];

						if(vert.x < mBounds.min.x) mBounds.min.x = vert.x;
						if(vert.x > mBounds.max.x) mBounds.max.x = vert.x;
						if(vert.y < mBounds.min.y) mBounds.min.y = vert.y;
						if(vert.y > mBounds.max.y) mBounds.max.y = vert.y;
						if(vert.z < mBounds.min.z) mBounds.min.z = vert.z;
						if(vert.z > mBounds.max.z) mBounds.max.z = vert.z;
					}
				}
			}
		}

		#if !defined ION_PLUGIN
		Ogre::MeshPtr& Mesh::GetOgreMesh()
		{
			return mOgreMesh;
		}
		#endif

		std::list<std::list<Mesh::SubMesh*>>& Mesh::GetSubMeshes()
		{
			return mSubMeshes;
		}

		bool Mesh::Load(std::string filename)
		{
			//Open file for reading
			io::BinaryFile file(filename, ion::io::File::OpenRead);

			if(file.IsOpen())
			{
				//Check header
				if(stricmp(sFileType, file.GetFileType().c_str()) != 0)
				{
					ion::debug::Error("Mesh::Load() - Not an ion::mesh file");
				}

				//Check version
				if(file.GetFileVersion() < sMinFileVersion)
				{
					ion::debug::Error("Mesh::Load() - Unsupported file version");
				}

				//If no bounds chunk found, we need to manually calculate from vertices
				bool boundsFound = false;

				//For all root chunks
				for(io::BinaryFile::Chunk rootChunk = file.GetTrunk().Begin(), rootEnd = file.GetTrunk().End(); rootChunk != rootEnd; rootChunk = rootChunk.Next())
				{
					if(rootChunk.GetID() == ChunkId_SubMesh)
					{
						//For all LOD chunks
						for(io::BinaryFile::Chunk lodChunk = rootChunk.Begin(), lodEnd = rootChunk.End(); lodChunk != lodEnd; lodChunk = lodChunk.Next())
						{
							//Create submesh
							SubMesh* subMesh = CreateSubMesh();

							//Read submesh data
							subMesh->Read(lodChunk);
						}
					}
					else if(rootChunk.GetID() == ChunkId_Bounds)
					{
						//Read bounds
						rootChunk.ReadData(&mBounds, sizeof(Bounds));

						//Bounds chunk found
						boundsFound = true;
					}
				}

				//If bounds not found, calculate manually
				if(!boundsFound)
				{
					CalculateBounds();
				}

				//Done, finalise
				Finalise();

				return true;
			}

			return false;
		}

		u64 Mesh::Save(std::string filename)
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

				//For all submeshes
				for(std::list<std::list<SubMesh*>>::iterator subMesh = mSubMeshes.begin(), subMeshEnd = mSubMeshes.end(); subMesh != subMeshEnd; ++subMesh)
				{
					//Create submesh chunk
					io::BinaryFile::Chunk subMeshChunk;
					subMeshChunk.SetID(ChunkId_SubMesh);

					//For all LODs
					for(std::list<SubMesh*>::iterator lod = subMesh->begin(), lodEnd = subMesh->end(); lod != lodEnd; lod++)
					{
						//Create LOD chunk
						io::BinaryFile::Chunk lodChunk;
						lodChunk.SetID(ChunkId_LOD);

						//Write mesh data
						(*lod)->Write(lodChunk);

						//Add LOD chunk to submesh chunk
						subMeshChunk.AddChild(lodChunk);
					}

					//Add submesh chunk to trunk
					file.GetTrunk().AddChild(subMeshChunk);
				}

				//Calculate bounds
				CalculateBounds();

				//Create bounds chunk
				io::BinaryFile::Chunk boundsChunk;
				boundsChunk.SetID(ChunkId_Bounds);
				boundsChunk.SetData(&mBounds, sizeof(mBounds), 1);
				file.GetTrunk().AddChild(boundsChunk);

				//Get total filesize
				fileSize = file.GetTrunk().GetChunkSize() + sizeof(io::BinaryFile::FileHeader);

				//Write file
				file.Write();

				//Close file
				file.Close();
			}

			return fileSize;
		}

		MeshInstance::MeshInstance(Mesh& mesh, Scene& scene)
		{
			mOgreEntity = scene.GetOgreSceneMgrIFace()->createEntity(mesh.GetOgreMesh()->getName());

			//Default material
			mOgreEntity->setMaterialName("BaseWhite");

			//TODO: Create copy of ion skeleton
		}

		void MeshInstance::SetCastShadows(bool castShadows)
		{
			mOgreEntity->setCastShadows(castShadows);
		}

		void MeshInstance::SetDrawDebugSkeleton(bool drawSkeleton)
		{
			mOgreEntity->setDisplaySkeleton(drawSkeleton);
		}

		void MeshInstance::SetBoneTransform(Bone& bone, const Matrix4& transform)
		{
			Ogre::Bone* boneInstance = mOgreEntity->getSkeleton()->getBone(bone.GetOgreBone()->getHandle());
			if(boneInstance)
			{
				Vector3 position = transform.GetTranslation();
				Quaternion rotation;
				rotation.FromMatrix(transform);
				boneInstance->setPosition(position.x, position.y, position.z);
				boneInstance->setOrientation(rotation.w, rotation.x, rotation.y, rotation.z);
			}
		}
	}
}