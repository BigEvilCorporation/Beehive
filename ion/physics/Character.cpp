///////////////////////////////////////////////////
// File:		Character.cpp
// Date:		8th December 2012
// Authors:		Matt Phillips
// Description:	Kinematic character controller
///////////////////////////////////////////////////

#include "physics/Character.h"

namespace ion
{
	namespace physics
	{
		const float Character::sDefaultStepHeight = 0.35f;

		Character::Character(const ion::Vector2& capsuleDimensions)
		{
			//Create Bullet capsule collision shape
			mBulletCapsule = new btCapsuleShape(capsuleDimensions.x, capsuleDimensions.y);

			//Create Bullet ghost object
			mBulletGhost = new btPairCachingGhostObject();
			mBulletGhost->setCollisionShape(mBulletCapsule);
			mBulletGhost->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

			//Create Bullet character controller
			mBulletController = new btKinematicCharacterController(mBulletGhost, mBulletCapsule, sDefaultStepHeight);
		}

		void Character::SetTransform(const ion::Matrix4& transform)
		{
			btMatrix3x3 rotMtx( transform.Get(0,0), transform.Get(0,1), transform.Get(0,2), 
								transform.Get(1,0), transform.Get(1,1), transform.Get(1,2), 
								transform.Get(2,0), transform.Get(2,1), transform.Get(2,2) );

			ion::Vector3 translation = transform.GetTranslation();
			btTransform worldTrans;
			worldTrans.setOrigin(btVector3(translation.x, translation.y, translation.z));
			worldTrans.setBasis(rotMtx);
			mBulletGhost->setWorldTransform(worldTrans);
			mMatrix = transform;
		}

		const ion::Matrix4& Character::GetTransform()
		{
			const btTransform& worldTrans = mBulletGhost->getWorldTransform();
			const btMatrix3x3& rotMtx = worldTrans.getBasis();
			const btVector3& row0 = rotMtx.getRow(0);
			const btVector3& row1 = rotMtx.getRow(1);
			const btVector3& row2 = rotMtx.getRow(2);
			const btVector3& trans = worldTrans.getOrigin();
			mMatrix.SetRight(ion::Vector3(row0.x(), row0.y(), row0.z()));
			mMatrix.SetUp(ion::Vector3(row1.x(), row1.y(), row1.z()));
			mMatrix.SetForward(ion::Vector3(row2.x(), row2.y(), row2.z()));
			mMatrix.SetTranslation(ion::Vector3(trans.x(), trans.y(), trans.z()));
			return mMatrix;
		}

		void Character::SetMaxJumpHeight(float height)
		{
			mBulletController->setMaxJumpHeight(height);
		}

		void Character::SetMaxStepHeight(float height)
		{
		}

		void Character::SetMaxSlopeAngle(float degrees)
		{
			mBulletController->setMaxSlope(maths::DegreesToRadians(degrees));
		}

		void Character::SetMoveVector(const ion::Vector3& moveVector)
		{
			mBulletController->setWalkDirection(btVector3(moveVector.x, moveVector.y, moveVector.z));
		}

		void Character::Jump(float force)
		{
			mBulletController->setJumpSpeed(force);
			mBulletController->jump();
		}
	}
}