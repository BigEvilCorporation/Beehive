///////////////////////////////////////////////////
// File:		Body.h
// Date:		6th December 2012
// Authors:		Matt Phillips
// Description:	Physics body
///////////////////////////////////////////////////

#pragma once

#include "maths/Vector.h"
#include "maths/Matrix.h"
#include "maths/Quaternion.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

namespace ion
{
	namespace physics
	{
		class Body
		{
		public:
			enum Shape { InfinitePlane, Box, Sphere };

			Body(Shape shape, const Vector3& extents);

			void SetTransform(const ion::Matrix4& transform);
			void SetMass(float mass);
			void SetFriction(float friction);
			void SetRestitution(float restitution);

			//Velocity, force and torque
			void SetLinearVelocity(const ion::Vector3& velocity);
			void SetAngularVelocity(const ion::Vector3& velocity);
			void ApplyLinearForce(const ion::Vector3& force, const ion::Vector3& relativePosition);
			void ApplyAngularForce(const ion::Vector3& force);
			void ApplyLinearImpulse(const ion::Vector3& impulse, const ion::Vector3& relativePosition);
			void ApplyAngularImpulse(const ion::Vector3& impulse);

			const ion::Matrix4& GetTransform() const;
			float GetMass() const;
			float GetFriction() const;
			float GetRestitution() const;

			btRigidBody* GetBulletBody() const { return mBulletBody; }

		private:

			class BulletMotionState : public btMotionState
			{
			public:
				virtual void getWorldTransform(btTransform& worldTrans) const
				{
					btMatrix3x3 rotMtx(
						mMatrix.Get(0,0), mMatrix.Get(0,1), mMatrix.Get(0,2), 
						mMatrix.Get(1,0), mMatrix.Get(1,1), mMatrix.Get(1,2), 
						mMatrix.Get(2,0), mMatrix.Get(2,1), mMatrix.Get(2,2) );

					ion::Vector3 translation = mMatrix.GetTranslation();
					worldTrans.setOrigin(btVector3(translation.x, translation.y, translation.z));
					worldTrans.setBasis(rotMtx);
				}

				virtual void setWorldTransform(const btTransform& worldTrans)
				{
					const btMatrix3x3& rotMtx = worldTrans.getBasis();
					const btVector3& row0 = rotMtx.getRow(0);
					const btVector3& row1 = rotMtx.getRow(1);
					const btVector3& row2 = rotMtx.getRow(2);
					const btVector3& trans = worldTrans.getOrigin();
					mMatrix.SetRight(ion::Vector3(row0.x(), row0.y(), row0.z()));
					mMatrix.SetUp(ion::Vector3(row1.x(), row1.y(), row1.z()));
					mMatrix.SetForward(ion::Vector3(row2.x(), row2.y(), row2.z()));
					mMatrix.SetTranslation(ion::Vector3(trans.x(), trans.y(), trans.z()));
				}

				ion::Matrix4 mMatrix;
			};

			btRigidBody* mBulletBody;
			btCollisionShape* mBulletShape;
			BulletMotionState* mBulletMotionState;
			
			float mMass;
		};
	}
}