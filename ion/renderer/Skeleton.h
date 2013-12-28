///////////////////////////////////////////////////
// File:		Skeleton.h
// Date:		19th December 2012
// Authors:		Matt Phillips
// Description:	Skeletal animation
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include "core/maths/Matrix.h"
#include "core/maths/Vector.h"
#include "core/maths/Quaternion.h"
#include "core/Archive.h"
#include "renderer/Animation.h"

#include <list>
#include <map>
#include <utility>

namespace ion
{
	namespace renderer
	{
		//Forward declaration
		class MeshInstance;

		class Bone
		{
			friend class Skeleton;

		public:
			Bone();
			Bone(const char* name);

			//Serialise
			void Serialise(serialise::Archive& archive);

			//Set local offset transform
			virtual void SetOffsetTransform(const Matrix4& transform);
			virtual void SetOffsetTranslation(const Vector3& translation);
			virtual void SetOffsetRotation(const Quaternion& rotation);

			//Get world transform
			const Matrix4 GetWorldTransform() const;

			//Get bind pose transform
			const Matrix4& GetBindPoseLocalTransform() const;
			const Matrix4& GetBindPoseWorldTransform() const;

			//Move/rotate
			virtual void Translate(const Vector3& positionDelta);
			virtual void Rotate(const Quaternion& rotationDelta);

			//Fix current position/rotation as binding pose
			virtual void FixBindingPose();

			//Reset to binding pose
			virtual void SetBindingPose();

			//Get/set parent bone
			void SetParent(Bone* parent);
			Bone* GetParent() const;

			//Get name
			const std::string& GetName() const;

			//Get/set parent name (for serialisation)
			const std::string& GetParentName() const;
			void SetParentName(const char* name);

#if defined ION_PLUGIN
			//Set bind pose transforms
			void SetLocalBindPoseTransform(const Matrix4& transform);
			void SetWorldBindPoseTransform(const Matrix4& transform);
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
		};

		class Skeleton
		{
		public:
			static const int sSerialiseVersion;

			Skeleton();

			//Serialise
			void Serialise(serialise::Archive& archive);

			//Create new bone
			Bone* CreateBone(const char* name);

			//Find bone by name
			Bone* FindBone(const char* name) const;

			//Fix current position/rotation as binding pose
			virtual void FixBindingPose();

			//Reset to binding pose
			virtual void SetBindingPose();

			//Finalise - build tree
			virtual void Finalise();

		protected:
			std::map<std::string, Bone> mBones;
			Bone* mRootBone;
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
			std::vector<std::pair<Bone*, const AnimationTrackTransform*>> mTracks;
		};
	}
}