///////////////////////////////////////////////////
// File:		Constraint.h
// Date:		10th February 2014
// Authors:		Matt Phillips
// Description:	Physics constraints
///////////////////////////////////////////////////

#pragma once

#include "maths/Vector.h"
#include "maths/Matrix.h"
#include "maths/Quaternion.h"

#include "physics/Body.h"

namespace ion
{
	namespace physics
	{
		class Constraint
		{
		public:
			virtual btTypedConstraint* GetBulletConstraint() const = 0;
		};

		class Hinge : public Constraint
		{
		public:
			Hinge(const Body& body, const Vector3& pivot, const Vector3& axis);
			~Hinge();

			virtual btTypedConstraint* GetBulletConstraint() const { return mBulletHinge; }

		protected:
			btHingeConstraint* mBulletHinge;
		};

		class BallSocket : public Constraint
		{
		public:
			BallSocket(const Body& body, const Vector3& pivot);
			~BallSocket();

			virtual btTypedConstraint* GetBulletConstraint() const { return mBulletP2P; }

		protected:
			btPoint2PointConstraint* mBulletP2P;
		};
	}
}