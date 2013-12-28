///////////////////////////////////////////////////
// File:		Mesh.h
// Date:		8th August 2011
// Authors:		Matt Phillips
// Description:	Vertex mesh and file loader
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include "core/Colour.h"
#include "core/maths/Vector.h"
#include "core/Archive.h"
#include "renderer/Vertex.h"
#include "renderer/TexCoord.h"
#include "renderer/Skeleton.h"

#include <string>
#include <list>
#include <vector>
#include <map>

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
			//Factory
			static Mesh* Create();
			static void Release(Mesh* mesh);

			class SubMesh
			{
			public:
				//Factory
				static SubMesh* Create();
				static void Release(SubMesh* subMesh);

				void AddVertex(Vertex& vertex);
				void AddNormal(Vector3& normal);
				void AddFace(Face& face);

				virtual void MapBone(Bone& bone, u32 vertexIdx, float weight);

				virtual void Finalise();

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

				SubMesh();
				virtual ~SubMesh();
				
				virtual void SetParentMesh(Mesh* mesh);

				std::string mName;
				std::string mMaterialName;

				std::vector<Vertex> mVertices;
				std::vector<Vector3> mNormals;
				std::vector<Face> mFaces;

				friend class Mesh;
			};

			SubMesh* CreateSubMesh();

			virtual void Finalise();

			virtual void SetSkeleton(Skeleton& skeleton);

			//Get LOD set
			std::list<Mesh::SubMesh*>& GetSubMeshes(u32 lodLevel);

			//Calculate bounds
			void CalculateBounds();

			//Serialise
			void Serialise(serialise::Archive& archive);

			static const int sSerialiseVersion;

		protected:
			Mesh();
			virtual ~Mesh();
			
			//Submesh/LOD hierarchy
			std::vector<std::list<SubMesh*>> mLodSets;

			//Bounds
			struct Bounds
			{
				Vector3 min;
				Vector3 max;
			} mBounds;
		};

		class MeshInstance
		{
		public:
			//Factory
			static MeshInstance* Create();
			static void Release(MeshInstance* meshInstance);

			//TODO: Materials
			//void SetMaterial(Material& material);
			virtual void SetCastShadows(bool castShadows);
			virtual void SetDrawDebugSkeleton(bool drawSkeleton);

			//TODO: Copy skeleton locally
			virtual void MapBone(Mesh::SubMesh& subMesh, Bone& bone, u32 vertexIdx, float weight);
			virtual void SetBoneTransform(Bone& bone, const Matrix4& transform);

		protected:
			MeshInstance(Mesh& mesh);
			virtual ~MeshInstance();

			Mesh& mMesh;
		};
	}
}