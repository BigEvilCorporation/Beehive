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

			//Set local transform
			void SetLocalTransform(const Matrix4& transform);
			void SetLocalTranslation(const Vector3& translation);
			void SetLocalRotation(const Quaternion& rotation);

			//Get local transform
			const Matrix4& GetLocalTransform() const;

			//Calculate world transform, traversing up to root
			Matrix4 CalculateWorldTransform() const;

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

		protected:
			Matrix4 mLocalMatrix;
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