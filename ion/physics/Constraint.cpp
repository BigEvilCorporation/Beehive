///////////////////////////////////////////////////
// File:		Constraint.cpp
// Date:		10th February 2014
// Authors:		Matt Phillips
// Description:	Physics constraints
///////////////////////////////////////////////////

#include "physics/Constraint.h"

namespace ion
{
	namespace physics
	{
		Hinge::Hinge(const Body& body, const Vector3& pivot, const Vector3& axis)
		{
			mBulletHinge = new btHingeConstraint(*body.GetBulletBody(), btVector3(pivot.x, pivot.y, pivot.z), btVector3(axis.x, axis.y, axis.z));
		}

		Hinge::~Hinge()
		{
			delete mBulletHinge;
		}

		BallSocket::BallSocket(const Body& body, const Vector3& pivot)
		{
			mBulletP2P = new btPoint2PointConstraint(*body.GetBulletBody(), btVector3(pivot.x, pivot.y, pivot.z));
		}

		BallSocket::~BallSocket()
		{
			delete mBulletP2P;
		}
	}
}