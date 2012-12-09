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
			float initialFriction = 1.0f;
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
			mBulletMotionState = new BulletMotionState();

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
			btTransform worldTrans;
			worldTrans.setFromOpenGLMatrix(transform.GetAsFloatArray());
			mBulletMotionState->mMatrix = transform;
			mBulletBody->setWorldTransform(worldTrans);
		}

		void Body::SetMass(float mass)
		{
			btVector3 inertia(0,0,0);

			if(mass > 0.0f)
			{
				mBulletShape->calculateLocalInertia(mass, inertia);
			}

			mBulletBody->setMassProps(mass, inertia);
			mMass = mass;
		}

		void Body::SetFriction(float friction)
		{
			mBulletBody->setFriction(friction);
		}

		void Body::SetRestitution(float restitution)
		{
			mBulletBody->setRestitution(restitution);
		}

		void Body::SetLinearVelocity(const ion::Vector3& velocity)
		{
			mBulletBody->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
		}

		void Body::SetAngularVelocity(const ion::Vector3& velocity)
		{
			mBulletBody->setAngularVelocity(btVector3(velocity.x, velocity.y, velocity.z));
		}

		void Body::ApplyLinearForce(const ion::Vector3& force, const ion::Vector3& relativePosition)
		{
			mBulletBody->applyForce(btVector3(force.x, force.y, force.z), btVector3(relativePosition.x, relativePosition.y, relativePosition.z));
		}

		void Body::ApplyAngularForce(const ion::Vector3& force)
		{
			mBulletBody->applyTorque(btVector3(force.x, force.y, force.z));
		}

		void Body::ApplyLinearImpulse(const ion::Vector3& impulse, const ion::Vector3& relativePosition)
		{
			mBulletBody->applyImpulse(btVector3(impulse.x, impulse.y, impulse.z), btVector3(relativePosition.x, relativePosition.y, relativePosition.z));
		}

		void Body::ApplyAngularImpulse(const ion::Vector3& impulse)
		{
			mBulletBody->applyTorqueImpulse(btVector3(impulse.x, impulse.y, impulse.z));
		}

		const ion::Matrix4& Body::GetTransform()
		{
			return mBulletMotionState->mMatrix;
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