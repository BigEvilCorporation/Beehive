///////////////////////////////////////////////////
// File:		Skeleton.cpp
// Date:		19th December 2012
// Authors:		Matt Phillips
// Description:	Skeletal animation
///////////////////////////////////////////////////

#include "Skeleton.h"

#include <sstream>

namespace ion
{
	namespace renderer
	{
		int Skeleton::sSkeletonIndex = 0;

		Bone::Bone(const char* name, Ogre::Bone* ogreBone)
		{
			mOgreBone = ogreBone;
		}

		void Bone::SetLocalTransform(const Matrix4& transform)
		{
			mLocalMatrix = transform;
			UpdateOgreMtx();
		}

		void Bone::SetLocalTranslation(const Vector3& translation)
		{
			mLocalMatrix.SetTranslation(translation);
			UpdateOgreMtx();
		}

		void Bone::SetLocalRotation(const Quaternion& rotation)
		{
			Matrix4 rotMtx = rotation.ToMatrix();
			mLocalMatrix.SetUp(rotMtx.GetUp());
			mLocalMatrix.SetRight(rotMtx.GetRight());
			mLocalMatrix.SetForward(rotMtx.GetForward());
			UpdateOgreMtx();
		}

		void Bone::Translate(const Vector3& positionDelta)
		{
			mLocalMatrix.SetTranslation(mLocalMatrix.GetTranslation() + positionDelta);
			UpdateOgreMtx();
		}

		void Bone::Rotate(const Quaternion& rotationDelta)
		{
			Matrix4 rotMtx = rotationDelta.ToMatrix();
			mLocalMatrix = mLocalMatrix * rotMtx;
			UpdateOgreMtx();
		}

		void Bone::FixBindingPose()
		{
			mOgreBone->setBindingPose();
		}

		void Bone::SetBindingPose()
		{
			mOgreBone->reset();
		}

		const Matrix4& Bone::GetLocalTransform()
		{
			return mLocalMatrix;
		}

		void Bone::UpdateOgreMtx()
		{
			Vector3 position = mLocalMatrix.GetTranslation();
			Quaternion rotation;
			rotation.FromMatrix(mLocalMatrix);
			mOgreBone->setPosition(position.x, position.y, position.z);
			mOgreBone->setOrientation(rotation.w, rotation.x, rotation.y, rotation.z);
		}

		Skeleton::Skeleton()
		{
			std::stringstream skeletonName;
			skeletonName << "skeleton_" << sSkeletonIndex++;
			mOgreSkeleton = Ogre::SkeletonManager::getSingleton().create(skeletonName.str(), "General", true);
			mOgreSkeleton->load();

			//Create root bone
			Ogre::Bone* ogreRootBone = mOgreSkeleton->createBone("Root");
			ogreRootBone->setManuallyControlled(true);
			mRootBone = new Bone("Root", ogreRootBone);
			mBones.push_back(mRootBone);
		}

		Bone* Skeleton::CreateBone(const char* name)
		{
			Ogre::Bone* ogreBone = mRootBone->GetOgreBone()->createChild(mBones.size());
			ogreBone->setManuallyControlled(true);
			Bone* bone = new Bone(name, ogreBone);
			mBones.push_back(bone);
			return bone;
		}

		void Skeleton::FixBindingPose()
		{
			mOgreSkeleton->setBindingPose();

			for(std::list<Bone*>::iterator it = mBones.begin(), end = mBones.end(); it != end; ++it)
			{
				(*it)->FixBindingPose();
			}
		}

		void Skeleton::SetBindingPose()
		{
			mOgreSkeleton->reset();

			for(std::list<Bone*>::iterator it = mBones.begin(), end = mBones.end(); it != end; ++it)
			{
				(*it)->SetBindingPose();
			}
		}
	}
}