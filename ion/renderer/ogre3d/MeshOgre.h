///////////////////////////////////////////////////
// File:		MeshOgre.h
// Date:		28th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d mesh/submesh/instance implementation
///////////////////////////////////////////////////

#pragma once

#include <Ogre/OgreMesh.h>
#include <Ogre/OgreSubMesh.h>
#include <Ogre/OgreEntity.h>
#include <Ogre/OgreSubEntity.h>
#include <Ogre/OgreMeshManager.h>
#include <Ogre/OgreHardwareBufferManager.h>
#include <Ogre/OgreSceneManager.h>

#include "renderer/Mesh.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			class SubMeshOgre : public Mesh::SubMesh
			{
			public:
				SubMeshOgre();
				~SubMeshOgre();

				virtual void MapBone(Bone& bone, u32 vertexIdx, float weight);
				virtual void Finalise();
				virtual void SetParentMesh(Mesh* mesh);

				Ogre::SubMesh* mOgreSubMesh;
				Ogre::VertexData* mOgreVertexData;
				Ogre::HardwareVertexBufferSharedPtr mOgreVertexBuffer;
				Ogre::HardwareVertexBufferSharedPtr mOgreNormalBuffer;
				Ogre::HardwareVertexBufferSharedPtr mOgreTexCoordBuffer;
				Ogre::HardwareIndexBufferSharedPtr mOgreIndexBuffer;
				int mOgreHardwareBufferIndex;
			};

			class MeshOgre : public Mesh
			{
			public:
				MeshOgre();
				~MeshOgre();

				virtual void SetSkeleton(Skeleton& skeleton);
				virtual void Finalise();

				Ogre::MeshPtr mOgreMesh;
			};

			class MeshInstanceOgre : public MeshInstance
			{
			public:
				MeshInstanceOgre();
				~MeshInstanceOgre();

				void AddToOgreScene(Ogre::SceneManager* sceneManager);

				virtual void SetCastShadows(bool castShadows);
				virtual void SetDrawDebugSkeleton(bool drawSkeleton);
				virtual void MapBone(Mesh::SubMesh& subMesh, Bone& bone, u32 vertexIdx, float weight);
				virtual void SetBoneTransform(Bone& bone, const Matrix4& transform);

				Ogre::Entity* mOgreEntity;
			};
		}

		typedef ogre::SubMeshOgre SubMeshImpl;
		typedef ogre::MeshOgre MeshImpl;
		typedef ogre::MeshInstanceOgre MeshInstanceImpl;
	}
}