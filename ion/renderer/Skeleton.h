///////////////////////////////////////////////////
// File:		Skeleton.h
// Date:		19th December 2012
// Authors:		Matt Phillips
// Description:	Skeletal animation
///////////////////////////////////////////////////

#pragma once

#include "../core/Types.h"
#include "../core/maths/Matrix.h"
#include "../core/maths/Vector.h"
#include "../core/maths/Quaternion.h"
#include "../core/Serialise.h"
#include "../core/Archive.h"

#include "Animation.h"

#if defined ION_OGRE
#include <OgreBone.h>
#include <OgreSkeleton.h>
#include <OgreSkeletonManager.h>
#endif

#include <list>
#include <map>
#include <utility>

namespace ion
{
	namespace renderer
	{
		//Forward declaration
		class MeshInstance;

		class Bone : public serialise::Serialisable
		{
			friend class Skeleton;

		public:
			Bone();
			Bone(const char* name);

			//Serialise
			virtual void Serialise(serialise::Archive& archive);

			#if defined ION_OGRE
			Bone(const char* name, Ogre::Bone* ogreBone);
			#endif

			//Set local offset transform
			void SetOffsetTransform(const Matrix4& transform);
			void SetOffsetTranslation(const Vector3& translation);
			void SetOffsetRotation(const Quaternion& rotation);

			//Get world transform
			const Matrix4 GetWorldTransform() const;

			//Get bind pose transform
			const Matrix4& GetBindPoseLocalTransform() const;
			const Matrix4& GetBindPoseWorldTransform() const;

			//Move/rotate
			void Translate(const Vector3& positionDelta);
			void Rotate(const Quaternion& rotationDelta);

			//Fix current position/rotation as binding pose
			void FixBindingPose();

			//Reset to binding pose
			void SetBindingPose();

			//Get/set parent bone
			void SetParent(Bone* parent);
			Bone* GetParent() const;

			//Get name
			const std::string& GetName() const;

			//Get/set parent name (for serialisation)
			const std::string& GetParentName() const;
			void SetParentName(const char* name);

			#if defined ION_OGRE
			void SetOgreBone(Ogre::Bone* ogreBone) { mOgreBone = ogreBone; }
			Ogre::Bone* GetOgreBone() const { return mOgreBone; }
			#endif

#if defined ION_PLUGIN
			//Set bind pose transforms
			void SetLocalBindPoseTransform(const Matrix4& transform);
			void SetWorldBindPoseTransform(const Matrix4& transform);

			//Calculate world transform from object transforms, traversing up to root
			Matrix4 CalculateWorldBindPoseTransform();

			//Calculate object transform from world transforms, traversing up to root
			Matrix4 CalculateLocalBindPoseTransform();
#endif

		protected:
			//Bind pose
			Matrix4 mLocalBindPoseMatrix;
			Matrix4 mWorldBindPoseMatrix;

			//Current offset
			Matrix4 mLocalOffsetMatrix;

			std::string mName;
			std::string mParentName;
			Bone* mParent;

			#if defined ION_OGRE
			void UpdateOgreMtx();
			Ogre::Bone* mOgreBone;
			#endif
		};

		class Skeleton : public serialise::Serialisable
		{
		public:
			static const int sSerialiseVersion;

			Skeleton();

			//Serialise
			virtual void Serialise(serialise::Archive& archive);

			//Create new bone
			Bone* CreateBone(const char* name);

			//Find bone by name
			Bone* FindBone(const char* name) const;

			//Fix current position/rotation as binding pose
			void FixBindingPose();

			//Reset to binding pose
			void SetBindingPose();

			//Finalise - build tree
			void Finalise();

			#if defined ION_OGRE
			Ogre::Skeleton* GetOgreSkeleton() const { return mOgreSkeleton.get(); }
			#endif

		protected:

			static int sSkeletonIndex;
			std::map<std::string, Bone> mBones;

			#if defined ION_OGRE
			void BuildOgreSkeleton(Bone* rootBone);
			Ogre::SkeletonPtr mOgreSkeleton;
			#endif
		};

		class SkeletalAnimation : public Animation
		{
		public:
			SkeletalAnimation(MeshInstance& meshInstance);

			void AddAnimationTrack(Bone& bone, const AnimationTrackTransform& animationTrack);

		protected:
			virtual void ApplyFrame(float frame);

		private:
			MeshInstance& mMeshInstance;
			std::vector< std::pair<Bone*, const AnimationTrackTransform*> > mTracks;
		};
	}
}