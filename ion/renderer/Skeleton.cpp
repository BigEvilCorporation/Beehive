///////////////////////////////////////////////////
// File:		Skeleton.cpp
// Date:		19th December 2012
// Authors:		Matt Phillips
// Description:	Skeletal animation
///////////////////////////////////////////////////

#include "Skeleton.h"
#include "Mesh.h"
#include "../core/Debug.h"

#include <sstream>

namespace ion
{
	namespace renderer
	{
		//For name generation
		int Skeleton::sSkeletonIndex = 0;

		//Class serialise version
		const int Skeleton::sSerialiseVersion = 1;

		Bone::Bone()
		{
			mParent = NULL;

			#if defined ION_OGRE
			mOgreBone = NULL;
			#endif
		}

		Bone::Bone(const char* name)
		{
			mName = name;
			mParent = NULL;

			#if defined ION_OGRE
			mOgreBone = NULL;
			#endif
		}

		#if defined ION_OGRE
		Bone::Bone(const char* name, Ogre::Bone* ogreBone)
		{
			mOgreBone = ogreBone;
			mName = name;
			mParent = NULL;
		}
		#endif

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

			#if defined ION_OGRE
			UpdateOgreMtx();
			#endif
		}

		void Bone::SetOffsetTranslation(const Vector3& translation)
		{
			mLocalOffsetMatrix.SetTranslation(translation);

			#if defined ION_OGRE
			UpdateOgreMtx();
			#endif
		}

		void Bone::SetOffsetRotation(const Quaternion& rotation)
		{
			Matrix4 rotMtx = rotation.ToMatrix();
			mLocalOffsetMatrix.SetUp(rotMtx.GetUp());
			mLocalOffsetMatrix.SetRight(rotMtx.GetRight());
			mLocalOffsetMatrix.SetForward(rotMtx.GetForward());

			#if defined ION_OGRE
			UpdateOgreMtx();
			#endif
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

			#if defined ION_OGRE
			UpdateOgreMtx();
			#endif
		}

		void Bone::Rotate(const Quaternion& rotationDelta)
		{
			Matrix4 rotMtx = rotationDelta.ToMatrix();
			mLocalOffsetMatrix = mLocalOffsetMatrix * rotMtx;

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
			mLocalOffsetMatrix.SetIdentity();

			#if defined ION_OGRE
			mOgreBone->reset();
			#endif
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

		Matrix4 Bone::CalculateWorldBindPoseTransform()
		{
			if(mParent)
			{
				mWorldBindPoseMatrix = mParent->CalculateWorldBindPoseTransform() * mLocalBindPoseMatrix;
			}
			else
			{
				mWorldBindPoseMatrix = mLocalBindPoseMatrix;
			}

			return mWorldBindPoseMatrix;
		}

		Matrix4 Bone::CalculateLocalBindPoseTransform()
		{
			if(mParent)
			{
				mLocalBindPoseMatrix = mWorldBindPoseMatrix * mParent->GetBindPoseWorldTransform().GetInverse();
			}
			else
			{
				mLocalBindPoseMatrix = mWorldBindPoseMatrix;
			}

			return mLocalBindPoseMatrix;
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

		#if defined ION_OGRE
		void Bone::UpdateOgreMtx()
		{
			Matrix4 objectMtx = mLocalBindPoseMatrix * mLocalOffsetMatrix;
			Vector3 position = objectMtx.GetTranslation();
			Quaternion rotation;
			rotation.FromMatrix(objectMtx);
			mOgreBone->setPosition(position.x, position.y, position.z);
			mOgreBone->setOrientation(rotation.w, rotation.x, rotation.y, rotation.z);
		}
		#endif

		Skeleton::Skeleton()
		{
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
			#if defined ION_OGRE
			mOgreSkeleton->setBindingPose();
			#endif

			for(std::map<std::string, Bone>::iterator it = mBones.begin(), end = mBones.end(); it != end; ++it)
			{
				it->second.FixBindingPose();
			}
		}

		void Skeleton::SetBindingPose()
		{
			#if defined ION_OGRE
			mOgreSkeleton->reset();
			#endif

			for(std::map<std::string, Bone>::iterator it = mBones.begin(), end = mBones.end(); it != end; ++it)
			{
				it->second.SetBindingPose();
			}
		}

		void Skeleton::Finalise()
		{
			Bone* rootBone = NULL;

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
					rootBone = &it->second;
				}
			}

			#if defined ION_PLUGIN
			for(std::map<std::string, Bone>::iterator it = mBones.begin(), end = mBones.end(); it != end; ++it)
			{
				//it->second.CalculateLocalBindPoseTransform();
				//it->second.CalculateWorldBindPoseTransform();
			}
			#endif

			#if defined ION_OGRE
			BuildOgreSkeleton(rootBone);
			#endif

			//Fix binding pose
			FixBindingPose();
		}

		#if defined ION_OGRE
		void Skeleton::BuildOgreSkeleton(Bone* rootBone)
		{
			//Create Ogre skeleton
			std::stringstream skeletonName;
			skeletonName << "skeleton_" << sSkeletonIndex++;
			mOgreSkeleton = Ogre::SkeletonManager::getSingleton().create(skeletonName.str(), "General", true);
			mOgreSkeleton->load();

			//Create root Ogre bone
			Ogre::Bone* ogreRootBone = mOgreSkeleton->createBone("Root");

			//Manually controlled
			ogreRootBone->setManuallyControlled(true);

			//Set Ogre root bone
			rootBone->SetOgreBone(ogreRootBone);

			//Update matrix
			rootBone->UpdateOgreMtx();

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
						Ogre::Bone* ogreParent = it->second.GetParent()->GetOgreBone();
						
						if(ogreParent)
						{
							//Create child bone
							ogreBone = ogreParent->createChild(ogreBoneId++);

							//Manually controlled
							ogreBone->setManuallyControlled(true);

							//Set Ogre bone
							it->second.SetOgreBone(ogreBone);

							//Update Ogre matrix
							it->second.UpdateOgreMtx();

							numOgreBonesCreated--;
						}
					}
				}
			}
		}
		#endif

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