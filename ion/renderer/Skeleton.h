///////////////////////////////////////////////////
// File:		Skeleton.h
// Date:		19th December 2012
// Authors:		Matt Phillips
// Description:	Skeletal animation
///////////////////////////////////////////////////

#pragma once

#include "../core/maths/Matrix.h"
#include "../core/maths/Vector.h"
#include "../core/maths/Quaternion.h"

#include <OgreBone.h>
#include <OgreSkeleton.h>
#include <OgreSkeletonManager.h>

#include <list>

namespace ion
{
	namespace renderer
	{
		class Bone
		{
		public:
			Bone(const char* name, Ogre::Bone* ogreBone);

			//Set local transform
			void SetLocalTransform(const Matrix4& transform);
			void SetLocalTranslation(const Vector3& translation);
			void SetLocalRotation(const Quaternion& rotation);

			//Move/rotate
			void Translate(const Vector3& positionDelta);
			void Rotate(const Quaternion& rotationDelta);

			//Fix current position/rotation as binding pose
			void FixBindingPose();

			//Reset to binding pose
			void SetBindingPose();

			const Matrix4& GetLocalTransform();

			Ogre::Bone* GetOgreBone() const { return mOgreBone; }

		protected:
			void UpdateOgreMtx();

			Matrix4 mLocalMatrix;
			Ogre::Bone* mOgreBone;
		};

		class Skeleton
		{
		public:
			Skeleton();

			//Create new bone
			Bone* CreateBone(const char* name);

			//Fix current position/rotation as binding pose
			void FixBindingPose();

			//Reset to binding pose
			void SetBindingPose();

			Ogre::Skeleton* GetOgreSkeleton() const { return mOgreSkeleton.get(); }

		protected:
			static int sSkeletonIndex;
			Bone* mRootBone;
			std::list<Bone*> mBones;
			Ogre::SkeletonPtr mOgreSkeleton;
		};
	}
}