///////////////////////////////////////////////////
// File:		Skeleton.cpp
// Date:		19th December 2012
// Authors:		Matt Phillips
// Description:	Skeletal animation
///////////////////////////////////////////////////

#include "renderer/Skeleton.h"
#include "renderer/Mesh.h"
#include "core/Debug.h"

#include <sstream>

namespace ion
{
	namespace renderer
	{
		//Class serialise version
		const int Skeleton::sSerialiseVersion = 1;

		Bone::Bone()
		{
			mParent = NULL;
		}

		Bone::Bone(const char* name)
		{
			mName = name;
			mParent = NULL;
		}

		void Bone::Serialise(serialise::Archive& archive)
		{
			archive.Serialise(mName);
			archive.Serialise(mLocalBindPoseMatrix);
			archive.Serialise(mWorldBindPoseMatrix);
			archive.Serialise(mParentName);
		}

		void Bone::SetOffsetTransform(const Matrix4& transform)
		{
			mLocalOffsetMatrix = transform;
		}

		void Bone::SetOffsetTranslation(const Vector3& translation)
		{
			mLocalOffsetMatrix.SetTranslation(translation);
		}

		void Bone::SetOffsetRotation(const Quaternion& rotation)
		{
			Matrix4 rotMtx = rotation.ToMatrix();
			mLocalOffsetMatrix.SetUp(rotMtx.GetUp());
			mLocalOffsetMatrix.SetRight(rotMtx.GetRight());
			mLocalOffsetMatrix.SetForward(rotMtx.GetForward());
		}

		const Matrix4& Bone::GetBindPoseLocalTransform() const
		{
			return mLocalBindPoseMatrix;
		}

		const Matrix4& Bone::GetBindPoseWorldTransform() const
		{
			return mWorldBindPoseMatrix;
		}

		const Matrix4 Bone::GetWorldTransform() const
		{
			return mWorldBindPoseMatrix * mLocalOffsetMatrix;
		}

		void Bone::Translate(const Vector3& positionDelta)
		{
			mLocalOffsetMatrix.SetTranslation(mLocalOffsetMatrix.GetTranslation() + positionDelta);
		}

		void Bone::Rotate(const Quaternion& rotationDelta)
		{
			Matrix4 rotMtx = rotationDelta.ToMatrix();
			mLocalOffsetMatrix = mLocalOffsetMatrix * rotMtx;
		}

		void Bone::FixBindingPose()
		{

		}

		void Bone::SetBindingPose()
		{
			mLocalOffsetMatrix.SetIdentity();
		}

		void Bone::SetParent(Bone* parent)
		{
			mParent = parent;

			if(parent)
			{
				mParentName = parent->GetName();
			}
			else
			{
				mParentName.clear();
			}
		}

		Bone* Bone::GetParent() const
		{
			return mParent;
		}

		#if defined ION_PLUGIN
		void Bone::SetLocalBindPoseTransform(const Matrix4& transform)
		{
			mLocalBindPoseMatrix = transform;
		}

		void Bone::SetWorldBindPoseTransform(const Matrix4& transform)
		{
			mWorldBindPoseMatrix = transform;
		}
		#endif

		const std::string& Bone::GetName() const
		{
			return mName;
		}

		const std::string& Bone::GetParentName() const
		{
			return mParentName;
		}

		void Bone::SetParentName(const char* name)
		{
			mParentName = name;
		}

		Skeleton::Skeleton()
		{
			mRootBone = NULL;
		}

		void Skeleton::Serialise(serialise::Archive& archive)
		{
			archive.Serialise(mBones);

			//If serialising in, finalise skeleton
			if(archive.GetDirection() == serialise::Archive::In)
			{
				Finalise();
			}
		}

		Bone* Skeleton::CreateBone(const char* name)
		{
			Bone* bone = NULL;

			//Insert into map
			std::pair<std::map<std::string, Bone>::iterator, bool> result = mBones.insert(std::pair<std::string, Bone>(name, Bone(name)));

			if(result.second)
			{
				bone = &(*result.first).second;
			}

			return bone;
		}

		Bone* Skeleton::FindBone(const char* name) const
		{
			Bone* bone = NULL;

			std::map<std::string, Bone>::const_iterator it = mBones.find(name);
			if(it != mBones.end())
			{
				bone = (Bone*)&it->second;
			}

			return bone;
		}

		void Skeleton::FixBindingPose()
		{
			for(std::map<std::string, Bone>::iterator it = mBones.begin(), end = mBones.end(); it != end; ++it)
			{
				it->second.FixBindingPose();
			}
		}

		void Skeleton::SetBindingPose()
		{
			for(std::map<std::string, Bone>::iterator it = mBones.begin(), end = mBones.end(); it != end; ++it)
			{
				it->second.SetBindingPose();
			}
		}

		void Skeleton::Finalise()
		{
			//Build tree
			for(std::map<std::string, Bone>::iterator it = mBones.begin(), end = mBones.end(); it != end; ++it)
			{
				const std::string& parentName = it->second.GetParentName();

				if(parentName.size() > 1)
				{
					//Find and set parent
					std::map<std::string, Bone>::iterator parentIt = mBones.find(parentName);
					ion::debug::Assert(parentIt != mBones.end(), "Parent bone not found");
					it->second.SetParent(&parentIt->second);
				}
				else
				{
					//Root bone
					mRootBone = &it->second;
				}
			}

			//Fix binding pose
			FixBindingPose();
		}

		SkeletalAnimation::SkeletalAnimation(MeshInstance& meshInstance)
			: mMeshInstance(meshInstance)
		{
		}

		void SkeletalAnimation::AddAnimationTrack(Bone& bone, const AnimationTrackTransform& animationTrack)
		{
			mTracks.push_back(std::pair<Bone*, const AnimationTrackTransform*>(&bone, &animationTrack));
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