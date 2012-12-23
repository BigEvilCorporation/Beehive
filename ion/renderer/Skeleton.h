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

#if defined ION_OGRE
#include <OgreBone.h>
#include <OgreSkeleton.h>
#include <OgreSkeletonManager.h>
#endif

#include <list>

namespace ion
{
	namespace renderer
	{
		class Bone
		{
		public:
			Bone(const char* name);

			#if defined ION_OGRE
			Bone(const char* name, Ogre::Bone* ogreBone);
			#endif

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

			#if defined ION_OGRE
			Ogre::Bone* GetOgreBone() const { return mOgreBone; }
			#endif

		protected:
			Matrix4 mLocalMatrix;

			#if defined ION_OGRE
			void UpdateOgreMtx();
			Ogre::Bone* mOgreBone;
			#endif
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

			#if defined ION_OGRE
			Ogre::Skeleton* GetOgreSkeleton() const { return mOgreSkeleton.get(); }
			#endif

		protected:
			static int sSkeletonIndex;
			Bone* mRootBone;
			std::list<Bone*> mBones;

			#if defined ION_OGRE
			Ogre::SkeletonPtr mOgreSkeleton;
			#endif
		};
	}
}