///////////////////////////////////////////////////
// File:		MeshOgre.cpp
// Date:		28th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d mesh/submesh/instance implementation
///////////////////////////////////////////////////

#include "MeshOgre.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
				SubMeshOgre::SubMeshOgre()
				{
					mOgreSubMesh = NULL;
					mOgreVertexData = NULL;
				}

				SubMeshOgre::~SubMeshOgre()
				{
				}

				void SubMeshOgre::MapBone(Bone& bone, u32 vertexIdx, float weight)
				{
					SubMesh::MapBone(bone, vertexIdx, weight);

					Ogre::VertexBoneAssignment boneAssignment;
					boneAssignment.boneIndex = bone.GetOgreBone()->getHandle();
					boneAssignment.vertexIndex = vertexIdx;
					boneAssignment.weight = weight;
					mOgreSubMesh->addBoneAssignment(boneAssignment);
				}

				void SubMeshOgre::SetParentMesh(Mesh* mesh)
				{
					SubMesh::SetParentMesh(mesh);

					if(!mOgreSubMesh)
					{
						static int numSubMeshes = 0;
						std::stringstream subMeshName;
						subMeshName << "OgreSubMesh_" << numSubMeshes++;
						mOgreSubMesh = ((MeshOgre*)mesh)->mOgreMesh->createSubMesh(subMeshName.str());

						//Setup vertex declaration
						mOgreVertexData = new Ogre::VertexData;
						mOgreSubMesh->useSharedVertices = false;
						mOgreSubMesh->vertexData = mOgreVertexData;
						mOgreSubMesh->setMaterialName("BaseWhite");

						mOgreHardwareBufferIndex = 0;
					}
				}

				void SubMeshOgre::Finalise()
				{
					SubMesh::Finalise();

					//Get hardware buffer manager
					Ogre::HardwareBufferManager& bufferManager = Ogre::HardwareBufferManager::getSingleton();

					if(mVertices.size() > 0)
					{
						//Add vertices to vertex declaration
						mOgreVertexData->vertexDeclaration->addElement(mOgreHardwareBufferIndex, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);

						//Create vertex buffer
						mOgreVertexBuffer = bufferManager.createVertexBuffer(sizeof(Vertex), mVertices.size(), Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

						//Write vertices
						mOgreVertexBuffer->writeData(0, sizeof(Vertex) * mVertices.size(), &mVertices[0], true);

						//Bind vertex buffer
						mOgreVertexData->vertexCount = mVertices.size();
						mOgreSubMesh->vertexData->vertexBufferBinding->setBinding(mOgreHardwareBufferIndex, mOgreVertexBuffer);

						mOgreHardwareBufferIndex++;
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
						mOgreVertexData->vertexDeclaration->addElement(mOgreHardwareBufferIndex, 0, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
				
						//Create normal buffer
						mOgreNormalBuffer = bufferManager.createVertexBuffer(sizeof(Vector3), mNormals.size(), Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

						//Write normals
						mOgreNormalBuffer->writeData(0, sizeof(Vector3) * mNormals.size(), &mNormals[0], true);

						//Bind normal buffer
						mOgreSubMesh->vertexData->vertexBufferBinding->setBinding(mOgreHardwareBufferIndex, mOgreNormalBuffer);

						mOgreHardwareBufferIndex++;
					}

					if(mFaces.size() > 0)
					{
						//Add text coords to vertex declaration
						mOgreVertexData->vertexDeclaration->addElement(mOgreHardwareBufferIndex, 0, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);

						//Create tex coord buffer
						mOgreTexCoordBuffer = bufferManager.createVertexBuffer(sizeof(TexCoord), mFaces.size() * 3, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

						//Write tex coords
						for(unsigned int i = 0; i < mFaces.size(); i++)
						{
							mOgreTexCoordBuffer->writeData(i * sizeof(TexCoord) * 3, sizeof(TexCoord) * 3, &mFaces[i].mTexCoords, true);
						}

						//Bind tex coord buffer
						mOgreSubMesh->vertexData->vertexBufferBinding->setBinding(mOgreHardwareBufferIndex, mOgreTexCoordBuffer);

						mOgreHardwareBufferIndex++;
					}

					mOgreSubMesh->_compileBoneAssignments();
				}

				MeshOgre::MeshOgre()
				{
					static int meshIndex = 0;
					std::stringstream meshName;
					meshName << "mesh_" << meshIndex++;
					mOgreMesh = Ogre::MeshManager::getSingleton().createManual(meshName.str(), "General");
				}

				MeshOgre::~MeshOgre()
				{
				}

				void MeshOgre::SetSkeleton(Skeleton& skeleton)
				{
					Mesh::SetSkeleton(skeleton);

					mOgreMesh->_notifySkeleton(Ogre::SkeletonPtr(skeleton.GetOgreSkeleton()));
				}

				void MeshOgre::Finalise()
				{
					Mesh::Finalise();

					mOgreMesh->_setBounds(Ogre::AxisAlignedBox(mBounds.min.x, mBounds.min.y, mBounds.min.z, mBounds.max.x, mBounds.max.y, mBounds.max.z));
					mOgreMesh->load();
				}

				MeshInstanceOgre::MeshInstanceOgre()
				{
				}

				MeshInstanceOgre::~MeshInstanceOgre()
				{
				}

				void MeshInstanceOgre::AddToOgreScene(Ogre::SceneManager* sceneManager)
				{
					mOgreEntity = sceneManager->createEntity(mMesh.GetOgreMesh()->getName());

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
				}

				void MeshInstanceOgre::SetCastShadows(bool castShadows)
				{
					MeshInstance::SetCastShadows(castShadows);
					mOgreEntity->setCastShadows(castShadows);
				}

				void MeshInstanceOgre::SetDrawDebugSkeleton(bool drawSkeleton)
				{
					MeshInstance::SetDrawDebugSkeleton(drawSkeleton);
					mOgreEntity->setDisplaySkeleton(drawSkeleton);
				}

				void MeshInstanceOgre::MapBone(Mesh::SubMesh& subMesh, Bone& bone, u32 vertexIdx, float weight)
				{
					MeshInstance::MapBone(subMesh, bone, vertexIdx, weight);

					Ogre::SubMesh* ogreSubMesh = mOgreEntity->getMesh()->getSubMesh(subMesh.GetName());
					if(ogreSubMesh)
					{
						Ogre::VertexBoneAssignment boneAssignment;
						boneAssignment.boneIndex = bone.GetOgreBone()->getHandle();
						boneAssignment.vertexIndex = vertexIdx;
						boneAssignment.weight = weight;
						ogreSubMesh->addBoneAssignment(boneAssignment);
					}
				}

				void MeshInstanceOgre::SetBoneTransform(Bone& bone, const Matrix4& transform)
				{
					MeshInstance::SetBoneTransform(bone, transform);

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
	}
}
