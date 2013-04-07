///////////////////////////////////////////////////
// File:		Skeleton.cpp
// Date:		19th December 2012
// Authors:		Matt Phillips
// Description:	Skeletal animation
///////////////////////////////////////////////////

#include "Skeleton.h"
#include "Mesh.h"

#include <sstream>

namespace ion
{
	namespace renderer
	{
		int Skeleton::sSkeletonIndex = 0;

		Bone::Bone(const char* name)
		{
		}

		#if defined ION_OGRE
		Bone::Bone(const char* name, Ogre::Bone* ogreBone)
		{
			mOgreBone = ogreBone;
		}
		#endif

		void Bone::SetLocalTransform(const Matrix4& transform)
		{
			mLocalMatrix = transform;

			#if defined ION_OGRE
			UpdateOgreMtx();
			#endif
		}

		void Bone::SetLocalTranslation(const Vector3& translation)
		{
			mLocalMatrix.SetTranslation(translation);

			#if defined ION_OGRE
			UpdateOgreMtx();
			#endif
		}

		void Bone::SetLocalRotation(const Quaternion& rotation)
		{
			Matrix4 rotMtx = rotation.ToMatrix();
			mLocalMatrix.SetUp(rotMtx.GetUp());
			mLocalMatrix.SetRight(rotMtx.GetRight());
			mLocalMatrix.SetForward(rotMtx.GetForward());

			#if defined ION_OGRE
			UpdateOgreMtx();
			#endif
		}

		void Bone::Translate(const Vector3& positionDelta)
		{
			mLocalMatrix.SetTranslation(mLocalMatrix.GetTranslation() + positionDelta);

			#if defined ION_OGRE
			UpdateOgreMtx();
			#endif
		}

		void Bone::Rotate(const Quaternion& rotationDelta)
		{
			Matrix4 rotMtx = rotationDelta.ToMatrix();
			mLocalMatrix = mLocalMatrix * rotMtx;

			#if defined ION_OGRE
			UpdateOgreMtx();
			#endif
		}

		void Bone::FixBindingPose()
		{
			#if defined ION_OGRE
			mOgreBone->setBindingPose();
			#endif
		}

		void Bone::SetBindingPose()
		{
			#if defined ION_OGRE
			mOgreBone->reset();
			#endif
		}

		const Matrix4& Bone::GetLocalTransform()
		{
			return mLocalMatrix;
		}

		#if defined ION_OGRE
		void Bone::UpdateOgreMtx()
		{
			Vector3 position = mLocalMatrix.GetTranslation();
			Quaternion rotation;
			rotation.FromMatrix(mLocalMatrix);
			mOgreBone->setPosition(position.x, position.y, position.z);
			mOgreBone->setOrientation(rotation.w, rotation.x, rotation.y, rotation.z);
		}
		#endif

		Skeleton::Skeleton()
		{
			#if defined ION_OGRE
			std::stringstream skeletonName;
			skeletonName << "skeleton_" << sSkeletonIndex++;
			mOgreSkeleton = Ogre::SkeletonManager::getSingleton().create(skeletonName.str(), "General", true);
			mOgreSkeleton->load();

			//Create root bone
			Ogre::Bone* ogreRootBone = mOgreSkeleton->createBone("Root");
			ogreRootBone->setManuallyControlled(true);
			mRootBone = new Bone("Root", ogreRootBone);
			mBones.push_back(mRootBone);
			#else
			mRootBone = new Bone("Root");
			mBones.push_back(mRootBone);
			#endif
		}

		Bone* Skeleton::CreateBone(const char* name)
		{
			#if defined ION_OGRE
			Ogre::Bone* ogreBone = mRootBone->GetOgreBone()->createChild(mBones.size());
			ogreBone->setManuallyControlled(true);
			Bone* bone = new Bone(name, ogreBone);
			mBones.push_back(bone);
			return bone;
			#else
			Bone* bone = new Bone(name);
			mBones.push_back(bone);
			return bone;
			#endif
		}

		void Skeleton::FixBindingPose()
		{
			#if defined ION_OGRE
			mOgreSkeleton->setBindingPose();
			#endif

			for(std::list<Bone*>::iterator it = mBones.begin(), end = mBones.end(); it != end; ++it)
			{
				(*it)->FixBindingPose();
			}
		}

		void Skeleton::SetBindingPose()
		{
			#if defined ION_OGRE
			mOgreSkeleton->reset();
			#endif

			for(std::list<Bone*>::iterator it = mBones.begin(), end = mBones.end(); it != end; ++it)
			{
				(*it)->SetBindingPose();
			}
		}

		SkeletalAnimation::SkeletalAnimation(MeshInstance& meshInstance)
			: mMeshInstance(meshInstance)
		{
		}

		void SkeletalAnimation::AddAnimationTrack(Bone& bone, const AnimationTrackTransform& animationTrack)
		{
			mTracks.push_back( std::pair<Bone*, const AnimationTrackTransform*>(&bone, &animationTrack));
		}

		void SkeletalAnimation::ApplyFrame(float frame)
		{
			for(unsigned int i = 0; i < mTracks.size(); i++)
			{
				Bone* bone = mTracks[i].first;
				const AnimationTrackTransform* track = mTracks[i].second;
				const Matrix4 transform = track->GetValue(frame);
				mMeshInstance.SetBoneTransform(*bone, transform);
			}
		}
	}
}