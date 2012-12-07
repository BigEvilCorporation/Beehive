///////////////////////////////////////////////////
// File:		Body.h
// Date:		6th December 2012
// Authors:		Matt Phillips
// Description:	Physics body
///////////////////////////////////////////////////

#pragma once

#include "../core/maths/Vector.h"
#include "../core/maths/Matrix.h"
#include "../core/maths/Quaternion.h"

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

			const ion::Matrix4& GetTransform();
			float GetMass() const;
			float GetFriction() const;
			float GetRestitution() const;

			btRigidBody* GetBulletBody() const { return mBulletBody; }

		private:
			btRigidBody* mBulletBody;
			btCollisionShape* mBulletShape;
			btDefaultMotionState* mBulletMotionState;

			ion::Matrix4 mMatrix;
			float mMass;
		};
	}
}