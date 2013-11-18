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

#if defined ION_OGRE
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

		Mesh::SubMesh::SubMesh()
		{
			#if defined ION_OGRE
			mOgreSubMesh = NULL;
			mOgreVertexData = NULL;
			#endif
		}

		Mesh::SubMesh::~SubMesh()
		{
		}

		void Mesh::SubMesh::SetParentMesh(Mesh* mesh)
		{
			#if defined ION_OGRE
			if(!mOgreSubMesh)
			{
				static int numSubMeshes = 0;
				std::stringstream subMeshName;
				subMeshName << "OgreSubMesh_" << numSubMeshes++;
				mName = subMeshName.str();
				mOgreSubMesh = mesh->GetOgreMesh()->createSubMesh(subMeshName.str());

				//Setup vertex declaration
				mOgreVertexData = new Ogre::VertexData;
				mOgreSubMesh->useSharedVertices = false;
				mOgreSubMesh->vertexData = mOgreVertexData;
				mOgreSubMesh->setMaterialName("BaseWhite");

				mHardwareBufferIndex = 0;
			}
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
			#if defined ION_OGRE
			Ogre::VertexBoneAssignment boneAssignment;
			boneAssignment.boneIndex = bone.GetOgreBone()->getHandle();
			boneAssignment.vertexIndex = vertexIdx;
			boneAssignment.weight = weight;
			mOgreSubMesh->addBoneAssignment(boneAssignment);
			#endif
		}

		void Mesh::SubMesh::Finalise()
		{
			#if defined ION_OGRE
			//Get hardware buffer manager
			Ogre::HardwareBufferManager& bufferManager = Ogre::HardwareBufferManager::getSingleton();

			if(mVertices.size() > 0)
			{
				//Add vertices to vertex declaration
				mOgreVertexData->vertexDeclaration->addElement(mHardwareBufferIndex, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);

				//Create vertex buffer
				mOgreVertexBuffer = bufferManager.createVertexBuffer(sizeof(Vertex), mVertices.size(), Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

				//Write vertices
				mOgreVertexBuffer->writeData(0, sizeof(Vertex) * mVertices.size(), &mVertices[0], true);

				//Bind vertex buffer
				mOgreVertexData->vertexCount = mVertices.size();
				mOgreSubMesh->vertexData->vertexBufferBinding->setBinding(mHardwareBufferIndex, mOgreVertexBuffer);

				mHardwareBufferIndex++;
			}

			if(mFaces.size() > 0)
			{
				//Create index buffer
				mOgreIndexBuffer = bufferManager.createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, mFaces.size() * 3, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

				//Write indices
				for(unsigned int i = 0; i < mFaces.size(); i++)
				{
					mOgreIndexBuffer->writeData(i * sizeof(Index) * 3, sizeof(Index) * 3, &mFaces[i].mIndices, true);
				}

				//Bind index buffer
				mOgreSubMesh->indexData->indexBuffer = mOgreIndexBuffer;
				mOgreSubMesh->indexData->indexCount = mOgreIndexBuffer->getNumIndexes();
				mOgreSubMesh->indexData->indexStart = 0;
			}

			if(mNormals.size() > 0)
			{
				//Add normals to vertex declaration
				mOgreVertexData->vertexDeclaration->addElement(mHardwareBufferIndex, 0, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
				
				//Create normal buffer
				mOgreNormalBuffer = bufferManager.createVertexBuffer(sizeof(Vector3), mNormals.size(), Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

				//Write normals
				mOgreNormalBuffer->writeData(0, sizeof(Vector3) * mNormals.size(), &mNormals[0], true);

				//Bind normal buffer
				mOgreSubMesh->vertexData->vertexBufferBinding->setBinding(mHardwareBufferIndex, mOgreNormalBuffer);

				mHardwareBufferIndex++;
			}

			if(mFaces.size() > 0)
			{
				//Add text coords to vertex declaration
				mOgreVertexData->vertexDeclaration->addElement(mHardwareBufferIndex, 0, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);

				//Create tex coord buffer
				mOgreTexCoordBuffer = bufferManager.createVertexBuffer(sizeof(TexCoord), mFaces.size() * 3, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

				//Write tex coords
				for(unsigned int i = 0; i < mFaces.size(); i++)
				{
					mOgreTexCoordBuffer->writeData(i * sizeof(TexCoord) * 3, sizeof(TexCoord) * 3, &mFaces[i].mTexCoords, true);
				}

				//Bind tex coord buffer
				mOgreSubMesh->vertexData->vertexBufferBinding->setBinding(mHardwareBufferIndex, mOgreTexCoordBuffer);

				mHardwareBufferIndex++;
			}

			mOgreSubMesh->_compileBoneAssignments();
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

		void Mesh::SubMesh::Serialise(serialise::Archive& archive)
		{
			//Serialise vertices, normals and faces
			archive.Serialise(mVertices);
			archive.Serialise(mNormals);
			archive.Serialise(mFaces);

			//Serialise material name
			archive.Serialise(mMaterialName);
		}

		Mesh::Mesh()
		{
			#if defined ION_OGRE
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

		}

		Mesh::SubMesh* Mesh::CreateSubMesh()
		{
			//TODO: LOD support, just add the one LOD set with one submesh
			std::list<SubMesh*> lodSet;

			//Create new submesh
			SubMesh* lod = new SubMesh();

			//Add to LOD set
			lodSet.push_back(lod);

			//Add LOD set to submesh list
			mLodSets.push_back(lodSet);

			//Set parent
			lod->SetParentMesh(this);

			return lod;
		}

		void Mesh::Finalise()
		{
			for(std::vector<std::list<SubMesh*>>::iterator subMesh = mLodSets.begin(), subMeshEnd = mLodSets.end(); subMesh != subMeshEnd; ++subMesh)
			{
				for(std::list<SubMesh*>::iterator lod = subMesh->begin(), lodEnd = subMesh->end(); lod != lodEnd; lod++)
				{
					(*lod)->SetParentMesh(this);
					(*lod)->Finalise();
				}
			}

			#if defined ION_OGRE
			mOgreMesh->_setBounds(Ogre::AxisAlignedBox(mBounds.min.x, mBounds.min.y, mBounds.min.z, mBounds.max.x, mBounds.max.y, mBounds.max.z));
			mOgreMesh->load();
			#endif
		}

		void Mesh::SetSkeleton(Skeleton& skeleton)
		{
			#if defined ION_OGRE
			mOgreMesh->_notifySkeleton(Ogre::SkeletonPtr(skeleton.GetOgreSkeleton()));
			#endif
		}

		void Mesh::CalculateBounds()
		{
			mBounds.min = Vector3(maths::FLOAT_MAX, maths::FLOAT_MAX, maths::FLOAT_MAX);
			mBounds.max = Vector3(maths::FLOAT_MIN, maths::FLOAT_MIN, maths::FLOAT_MIN);

			for(std::vector<std::list<SubMesh*>>::iterator subMesh = mLodSets.begin(), subMeshEnd = mLodSets.end(); subMesh != subMeshEnd; ++subMesh)
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

		#if defined ION_OGRE
		Ogre::MeshPtr& Mesh::GetOgreMesh()
		{
			return mOgreMesh;
		}
		#endif

		std::list<Mesh::SubMesh*>& Mesh::GetSubMeshes(u32 lodLevel)
		{
			debug::Assert(lodLevel >= 0 && lodLevel < mLodSets.size(), "Mesh::GetSubMeshes() - LOD level out of range");
			return mLodSets[lodLevel];
		}

		void Mesh::Serialise(serialise::Archive& archive)
		{
			//Register pointer types
			archive.RegisterPointerType<Mesh::SubMesh>();

			//If serialising out, calculate bounds first
			if(archive.GetDirection() == serialise::Archive::Out)
			{
				CalculateBounds();
			}

			//Serialise submeshes
			archive.Serialise(mLodSets);

			//Serialise bounds
			archive.Serialise(mBounds.min);
			archive.Serialise(mBounds.max);

			//If serialising in, finalise mesh
			if(archive.GetDirection() == serialise::Archive::In)
			{
				Finalise();
			}
		}

		MeshInstance::MeshInstance(Mesh& mesh, Scene& scene)
		{
			#if defined ION_OGRE
			mOgreEntity = scene.GetOgreSceneMgrIFace()->createEntity(mesh.GetOgreMesh()->getName());

			//Default material
			mOgreEntity->setMaterialName("BaseWhite");

			//Set all skeleton bones as manually controlled
			Ogre::Skeleton* ogreSkeleton = mOgreEntity->getSkeleton();
			if(ogreSkeleton)
			{
				for(Ogre::Skeleton::BoneIterator::iterator it = ogreSkeleton->getBoneIterator().begin(), end = ogreSkeleton->getBoneIterator().end(); it != end; ++it)
				{
					(*it)->setManuallyControlled(true);
				}

				//Set binding pose
				ogreSkeleton->setBindingPose();

				//Reset
				ogreSkeleton->reset();
			}
			#endif

			//TODO: Create copy of ion skeleton
		}

		void MeshInstance::SetCastShadows(bool castShadows)
		{
			#if defined ION_OGRE
			mOgreEntity->setCastShadows(castShadows);
			#endif
		}

		void MeshInstance::SetDrawDebugSkeleton(bool drawSkeleton)
		{
			#if defined ION_OGRE
			mOgreEntity->setDisplaySkeleton(drawSkeleton);
			#endif
		}

		void MeshInstance::MapBone(Mesh::SubMesh& subMesh, Bone& bone, u32 vertexIdx, float weight)
		{
			#if defined ION_OGRE
			Ogre::SubMesh* ogreSubMesh = mOgreEntity->getMesh()->getSubMesh(subMesh.GetName());
			if(ogreSubMesh)
			{
				Ogre::VertexBoneAssignment boneAssignment;
				boneAssignment.boneIndex = bone.GetOgreBone()->getHandle();
				boneAssignment.vertexIndex = vertexIdx;
				boneAssignment.weight = weight;
				ogreSubMesh->addBoneAssignment(boneAssignment);
			}
			#endif
		}

		void MeshInstance::SetBoneTransform(Bone& bone, const Matrix4& transform)
		{
			#if defined ION_OGRE
			Ogre::Bone* boneInstance = mOgreEntity->getSkeleton()->getBone(bone.GetOgreBone()->getHandle());
			if(boneInstance)
			{
				Vector3 position = transform.GetTranslation();
				Quaternion rotation;
				rotation.FromMatrix(transform);
				boneInstance->setPosition(position.x, position.y, position.z);
				boneInstance->setOrientation(rotation.w, rotation.x, rotation.y, rotation.z);
			}
			#endif
		}
	}
}