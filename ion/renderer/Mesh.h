///////////////////////////////////////////////////
// File:		Mesh.h
// Date:		8th August 2011
// Authors:		Matt Phillips
// Description:	Vertex mesh and file loader
///////////////////////////////////////////////////

#pragma once

#include "../core/Types.h"
#include "../core/Colour.h"
#include "../core/maths/Vector.h"
#include "../core/Archive.h"
#include "Vertex.h"
#include "TexCoord.h"
#include "Skeleton.h"

#include <string>
#include <list>
#include <vector>
#include <map>

#if defined ION_OGRE
#include <Ogre/OgreMesh.h>
#include <Ogre/OgreSubMesh.h>
#include <Ogre/OgreEntity.h>
#include <Ogre/OgreSubEntity.h>
#endif

namespace ion
{
	namespace renderer
	{
		//Forward declaration
		class Scene;

		struct Face
		{
			u16 mIndices[3];
			TexCoord mTexCoords[3];

			void Serialise(serialise::Archive& archive)
			{
				archive.Serialise(mIndices[0]);
				archive.Serialise(mIndices[1]);
				archive.Serialise(mIndices[2]);
				archive.Serialise(mTexCoords[0]);
				archive.Serialise(mTexCoords[1]);
				archive.Serialise(mTexCoords[2]);
			}
		};

		typedef u16 Index;

		class Mesh
		{
		public:
			Mesh();
			~Mesh();

			class SubMesh
			{
			public:
				SubMesh();
				~SubMesh();

				void AddVertex(Vertex& vertex);
				void AddNormal(Vector3& normal);
				void AddFace(Face& face);

				void MapBone(Bone& bone, u32 vertexIdx, float weight);

				void Finalise();

				int GetNumVertices();
				int GetNumNormals();
				int GetNumFaces();

				std::vector<Vertex>& GetVertices();
				std::vector<Vector3>& GetNormals();
				std::vector<Face>& GetFaces();

				void SetName(std::string name);
				const std::string& GetName();

				void SetMaterialName(std::string name);
				const std::string& GetMaterialName();

				//Serialise
				void Serialise(serialise::Archive& archive);

			protected:
				
				void SetParentMesh(Mesh* mesh);

			private:
				std::string mName;
				std::string mMaterialName;

				std::vector<Vertex> mVertices;
				std::vector<Vector3> mNormals;
				std::vector<Face> mFaces;

				#if defined ION_OGRE
				Ogre::SubMesh* mOgreSubMesh;
				Ogre::VertexData* mOgreVertexData;
				Ogre::HardwareVertexBufferSharedPtr mOgreVertexBuffer;
				Ogre::HardwareVertexBufferSharedPtr mOgreNormalBuffer;
				Ogre::HardwareVertexBufferSharedPtr mOgreTexCoordBuffer;
				Ogre::HardwareIndexBufferSharedPtr mOgreIndexBuffer;
				int mHardwareBufferIndex;
				#endif

				friend class Mesh;
			};

			SubMesh* CreateSubMesh();
			void Finalise();

			void SetSkeleton(Skeleton& skeleton);

			#if defined ION_OGRE
			Ogre::MeshPtr& GetOgreMesh();
			#endif

			//Get LOD set
			std::list<Mesh::SubMesh*>& GetSubMeshes(u32 lodLevel);

			//Calculate bounds
			void CalculateBounds();

			//Serialise
			void Serialise(serialise::Archive& archive);

		private:

			static const int sMinFileVersion;
			static const int sCurrentFileVersion;
			static const char* sFileType;
			
			//For unique name generation
			static int sMeshIndex;
			
			//Submesh/LOD hierarchy
			std::vector<std::list<SubMesh*>> mLodSets;

			//Bounds
			struct Bounds
			{
				Vector3 min;
				Vector3 max;
			} mBounds;

			#if defined ION_OGRE
			Ogre::MeshPtr mOgreMesh;
			#endif
		};

		class MeshInstance
		{
		public:
			MeshInstance(Mesh& mesh, Scene& scene);

			//TODO: Materials
			//void SetMaterial(Material& material);
			void SetCastShadows(bool castShadows);
			void SetDrawDebugSkeleton(bool drawSkeleton);

			//TODO: Copy skeleton locally
			void MapBone(Mesh::SubMesh& subMesh, Bone& bone, u32 vertexIdx, float weight);
			void SetBoneTransform(Bone& bone, const Matrix4& transform);

			#if defined ION_OGRE
			Ogre::Entity* GetOgreEntity() const { return mOgreEntity; }
			#endif

		protected:
			#if defined ION_OGRE
			Ogre::Entity* mOgreEntity;
			#endif
		};
	}
}