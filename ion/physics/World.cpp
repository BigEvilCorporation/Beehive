///////////////////////////////////////////////////
// File:		World.cpp
// Date:		6th December 2012
// Authors:		Matt Phillips
// Description:	Physics world
///////////////////////////////////////////////////

#include "physics/World.h"
#include "physics/Body.h"
#include "physics/Character.h"
#include "physics/Constraint.h"

namespace ion
{
	namespace physics
	{
		World::World()
		{
			mBulletBroadphase = new btDbvtBroadphase();
			mBulletSolver = new btSequentialImpulseConstraintSolver;
			mBulletCollisionConfig = new btDefaultCollisionConfiguration();
			mBulletCollisionDispatcher = new btCollisionDispatcher(mBulletCollisionConfig);
			mBulletDynamicsWorld = new btDiscreteDynamicsWorld(mBulletCollisionDispatcher, mBulletBroadphase, mBulletSolver, mBulletCollisionConfig);

			//Required for character controller
			mBulletBroadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

			SetGravity(ion::Vector3(0.0f, -9.8f, 0.0f));
		}

		void World::SetGravity(const ion::Vector3& gravity)
		{
			mBulletDynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
		}

		void World::AddBody(Body& body)
		{
			mBulletDynamicsWorld->addRigidBody(body.GetBulletBody());
		}

		void World::RemoveBody(Body& body)
		{
			mBulletDynamicsWorld->removeRigidBody(body.GetBulletBody());
		}

		void World::AddCharacter(Character& character)
		{
			mBulletDynamicsWorld->addCollisionObject(character.GetBulletGhost(), btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
			mBulletDynamicsWorld->addAction(character.GetBulletCharacter());
		}

		void World::RemoveCharacter(Character& character)
		{
			mBulletDynamicsWorld->removeCollisionObject(character.GetBulletGhost());
			mBulletDynamicsWorld->removeAction(character.GetBulletCharacter());
		}

		void World::AddConstraint(Constraint& constraint)
		{
			mBulletDynamicsWorld->addConstraint(constraint.GetBulletConstraint());
		}

		void World::RemoveConstraint(Constraint& constraint)
		{
			mBulletDynamicsWorld->removeConstraint(constraint.GetBulletConstraint());
		}

		void World::Step(float deltaTime, int numSubSteps)
		{
			mBulletDynamicsWorld->stepSimulation(deltaTime, numSubSteps);
		}
	}
}