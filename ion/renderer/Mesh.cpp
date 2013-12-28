///////////////////////////////////////////////////
// File:		Mesh.cpp
// Date:		8th August 2011
// Authors:		Matt Phillips
// Description:	Vertex mesh and file loader
///////////////////////////////////////////////////

#pragma once

#include "core/Debug.h"
#include "renderer/Mesh.h"
#include "renderer/Scene.h"
#include "renderer/Factory.h"

#include <sstream>

namespace ion
{
	namespace renderer
	{
		//Stream version supported by Mesh::Serialise()
		const int Mesh::sSerialiseVersion = 2;

		Mesh::SubMesh* Mesh::SubMesh::Create()
		{
			return new SubMeshImpl();
		}

		void Mesh::SubMesh::Release(Mesh::SubMesh* subMesh)
		{
			delete subMesh;
		}

		Mesh::SubMesh::SubMesh()
		{

		}

		Mesh::SubMesh::~SubMesh()
		{

		}

		void Mesh::SubMesh::SetParentMesh(Mesh* mesh)
		{

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
			
		}

		void Mesh::SubMesh::Finalise()
		{

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

		Mesh* Mesh::Create()
		{
			return new MeshImpl();
		}

		void Mesh::Release(Mesh* mesh)
		{
			delete mesh;
		}

		Mesh::Mesh()
		{
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
		}

		void Mesh::SetSkeleton(Skeleton& skeleton)
		{

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

		MeshInstance* MeshInstance::Create()
		{
			return new MeshInstanceImpl();
		}

		void MeshInstance::Release(MeshInstance* meshInstance)
		{
			delete meshInstance;
		}

		MeshInstance::MeshInstance(Mesh& mesh)
			: mMesh(mesh)
		{
			//TODO: Create copy of ion skeleton
		}

		MeshInstance::~MeshInstance()
		{

		}

		void MeshInstance::SetCastShadows(bool castShadows)
		{

		}

		void MeshInstance::SetDrawDebugSkeleton(bool drawSkeleton)
		{

		}

		void MeshInstance::MapBone(Mesh::SubMesh& subMesh, Bone& bone, u32 vertexIdx, float weight)
		{

		}

		void MeshInstance::SetBoneTransform(Bone& bone, const Matrix4& transform)
		{

		}
	}
}