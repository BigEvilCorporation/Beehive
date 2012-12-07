///////////////////////////////////////////////////
// File:		Body.cpp
// Date:		6th December 2012
// Authors:		Matt Phillips
// Description:	Physics body
///////////////////////////////////////////////////

#include "Body.h"

namespace ion
{
	namespace physics
	{
		Body::Body(Shape shape, const Vector3& extents)
		{
			//Defaults
			mMass = 1.0f;
			float initialFriction = 0.8f;
			float initialRestitution = 0.2f;

			//Create Bullet shape
			switch(shape)
			{
			case InfinitePlane:
				mBulletShape = new btStaticPlaneShape(btVector3(extents.x, extents.y, extents.z), 0.0f);
				break;

			case Box:
				mBulletShape = new btBoxShape(btVector3(extents.x, extents.y, extents.z));
				break;

			case Sphere:
				mBulletShape = new btSphereShape(extents.x);
				break;
			};
			
			//Create Bullet motion state
			mBulletMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, 0.0f, 0.0f)));

			//Calculate initial inertia
			btVector3 initialInertia(0,0,0);
			mBulletShape->calculateLocalInertia(mMass, initialInertia);

			//Create Bullet body construction object
			btRigidBody::btRigidBodyConstructionInfo bulletBodyInfo(mMass, mBulletMotionState, mBulletShape, initialInertia);
			bulletBodyInfo.m_restitution = initialRestitution;
			bulletBodyInfo.m_friction = initialFriction;

			//Create bullet body
			mBulletBody = new btRigidBody(bulletBodyInfo);
		}

		void Body::SetTransform(const ion::Matrix4& transform)
		{
			mMatrix = transform;
			btTransform bulletTransform;
			bulletTransform.setFromOpenGLMatrix(transform.GetAsFloatArray());
			mBulletMotionState->setWorldTransform(bulletTransform);
		}

		void Body::SetMass(float mass)
		{
			btVector3 inertia(0,0,0);
			mBulletShape->calculateLocalInertia(mass, inertia);
			mBulletBody->setMassProps(mass, inertia);
		}

		void Body::SetFriction(float friction)
		{
			mBulletBody->setFriction(friction);
		}

		void Body::SetRestitution(float restitution)
		{
			mBulletBody->setRestitution(restitution);
		}

		const ion::Matrix4& Body::GetTransform()
		{
			btTransform transform;
			mBulletMotionState->getWorldTransform(transform);
			float mtx44[16] = { 0.0f };
			transform.getOpenGLMatrix(mtx44);
			mMatrix = Matrix4(mtx44);
			return mMatrix;
		}

		float Body::GetMass() const
		{
			return mMass;
		}

		float Body::GetFriction() const
		{
			return mBulletBody->getFriction();
		}

		float Body::GetRestitution() const
		{
			return mBulletBody->getRestitution();
		}
	}
}