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
					worldTrans.setFromOpenGLMatrix(mMatrix.GetAsFloatArray());
				}

				virtual void setWorldTransform(const btTransform& worldTrans)
				{
					float glMatrix[16];
					worldTrans.getOpenGLMatrix(glMatrix);
					mMatrix = ion::Matrix4(glMatrix);
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