///////////////////////////////////////////////////
// File:		SkeletonOgre.h
// Date:		30th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d skeleton implementation
///////////////////////////////////////////////////

#pragma once

#include <Ogre/OgreRoot.h>
#include <OgreBone.h>
#include <OgreSkeleton.h>
#include <OgreSkeletonManager.h>

#include "renderer/Skeleton.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			class BoneOgre : public Bone
			{
			public:
				BoneOgre();
				~BoneOgre();

				virtual void SetOffsetTransform(const Matrix4& transform);
				virtual void SetOffsetTranslation(const Vector3& translation);
				virtual void SetOffsetRotation(const Quaternion& rotation);
				virtual void Translate(const Vector3& positionDelta);
				virtual void Rotate(const Quaternion& rotationDelta);
				virtual void FixBindingPose();
				virtual void SetBindingPose();

				void UpdateOgreMtx();

				Ogre::Bone* mOgreBone;
			};

			class SkeletonOgre : public Skeleton
			{
			public:
				SkeletonOgre();
				~SkeletonOgre();

				virtual void FixBindingPose();
				virtual void SetBindingPose();
				virtual void Finalise();

				Ogre::SkeletonPtr mOgreSkeleton;
			};
		}

		typedef ogre::BoneOgre BoneImpl;
		typedef ogre::SkeletonOgre SkeletonImpl;
	}
}