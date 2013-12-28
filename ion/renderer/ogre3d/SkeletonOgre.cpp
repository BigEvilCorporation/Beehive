///////////////////////////////////////////////////
// File:		SkeletonOgre.cpp
// Date:		30th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d skeleton implementation
///////////////////////////////////////////////////

#include "SkeletonOgre.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			BoneOgre::BoneOgre()
			{
				mOgreBone = NULL;
			}

			BoneOgre::~BoneOgre()
			{
			}

			void BoneOgre::SetOffsetTransform(const Matrix4& transform)
			{
				Bone::SetOffsetTransform(transform);
				UpdateOgreMtx();
			}

			void BoneOgre::SetOffsetTranslation(const Vector3& translation)
			{
				Bone::SetOffsetTranslation(translation);
				UpdateOgreMtx();
			}

			void BoneOgre::SetOffsetRotation(const Quaternion& rotation)
			{
				Bone::SetOffsetRotation(rotation);
				UpdateOgreMtx();
			}

			void BoneOgre::Translate(const Vector3& positionDelta)
			{
				Bone::Translate(positionDelta);
				UpdateOgreMtx();
			}

			void BoneOgre::Rotate(const Quaternion& rotationDelta)
			{
				Bone::Rotate(rotationDelta);
				UpdateOgreMtx();
			}

			void BoneOgre::FixBindingPose()
			{
				Bone::FixBindingPose();
				mOgreBone->setBindingPose();
			}

			void BoneOgre::SetBindingPose()
			{
				Bone::SetBindingPose();
				mOgreBone->reset();
			}

			void BoneOgre::UpdateOgreMtx()
			{
				Matrix4 objectMtx = mLocalBindPoseMatrix * mLocalOffsetMatrix;
				Vector3 position = objectMtx.GetTranslation();
				Quaternion rotation;
				rotation.FromMatrix(objectMtx);
				mOgreBone->setPosition(position.x, position.y, position.z);
				mOgreBone->setOrientation(rotation.w, rotation.x, rotation.y, rotation.z);
			}

			SkeletonOgre::SkeletonOgre()
			{
			}

			SkeletonOgre::~SkeletonOgre()
			{
			}

			void SkeletonOgre::FixBindingPose()
			{
				Skeleton::FixBindingPose();
				mOgreSkeleton->setBindingPose();
			}

			void SkeletonOgre::SetBindingPose()
			{
				Skeleton::SetBindingPose();
				mOgreSkeleton->reset();
			}

			void SkeletonOgre::Finalise()
			{
				Skeleton::Finalise();

				//Create Ogre skeleton
				static int skeletonIndex = 0;
				std::stringstream skeletonName;
				skeletonName << "skeleton_" << skeletonIndex++;
				mOgreSkeleton = Ogre::SkeletonManager::getSingleton().create(skeletonName.str(), "General", true);
				mOgreSkeleton->load();

				//Create root Ogre bone
				Ogre::Bone* ogreRootBone = mOgreSkeleton->createBone("Root");

				//Manually controlled
				ogreRootBone->setManuallyControlled(true);

				//Set Ogre root bone
				((BoneOgre*)mRootBone)->mOgreBone = ogreRootBone;

				//Update matrix
				((BoneOgre*)mRootBone)->UpdateOgreMtx();

				//Loop until all bones have been created. Quick and dirty and very inefficient, but we don't yet have a Tree container
				int numOgreBonesCreated = mBones.size() - 1;
				while(numOgreBonesCreated)
				{
					for(std::map<std::string, Bone>::iterator it = mBones.begin(), end = mBones.end(); it != end; ++it)
					{
						Ogre::Bone* ogreBone = NULL;

						if(it->second.GetParent())
						{
							static int ogreBoneId = 1;

							//Get Ogre parent bone
							Ogre::Bone* ogreParent = ((BoneOgre*)it->second.GetParent())->mOgreBone;
						
							if(ogreParent)
							{
								//Create child bone
								ogreBone = ogreParent->createChild(ogreBoneId++);

								//Manually controlled
								ogreBone->setManuallyControlled(true);

								//Set Ogre bone
								((BoneOgre&)it->second).mOgreBone = ogreBone;

								//Update Ogre matrix
								((BoneOgre&)it->second).UpdateOgreMtx();

								numOgreBonesCreated--;
							}
						}
					}

					FixBindingPose();
				}
			}
		}
	}
}