///////////////////////////////////////////////////
// File:		Character.h
// Date:		8th December 2012
// Authors:		Matt Phillips
// Description:	Kinematic character controller
///////////////////////////////////////////////////

#pragma once

#include "physics/Body.h"

#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

namespace ion
{
	namespace physics
	{
		class Character
		{
		public:
			Character(const ion::Vector2& capsuleDimensions);

			void SetTransform(const ion::Matrix4& transform);
			const ion::Matrix4& GetTransform();

			void SetMaxJumpHeight(float height);
			void SetMaxStepHeight(float height);
			void SetMaxSlopeAngle(float degrees);

			void SetMoveVector(const ion::Vector3& moveVector);
			void Jump(float force);

			btKinematicCharacterController* GetBulletCharacter() const { return mBulletController; }
			btPairCachingGhostObject* GetBulletGhost() const { return mBulletGhost; }

		private:
			btConvexShape* mBulletCapsule;
			btPairCachingGhostObject* mBulletGhost;
			btKinematicCharacterController* mBulletController;

			ion::Matrix4 mMatrix;

			static const float sDefaultStepHeight;
		};
	}
}