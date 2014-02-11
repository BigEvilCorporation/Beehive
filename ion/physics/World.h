///////////////////////////////////////////////////
// File:		World.h
// Date:		6th December 2012
// Authors:		Matt Phillips
// Description:	Physics world
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
		//Forward declaration
		class Body;
		class Character;
		class Constraint;

		class World
		{
		public:
			World();

			void SetGravity(const ion::Vector3& gravity);

			void AddBody(Body& body);
			void RemoveBody(Body& body);

			void AddCharacter(Character& character);
			void RemoveCharacter(Character& character);

			void AddConstraint(Constraint& constraint);
			void RemoveConstraint(Constraint& constraint);

			void Step(float deltaTime, int numSubSteps);

		private:
			btBroadphaseInterface* mBulletBroadphase;
			btSequentialImpulseConstraintSolver* mBulletSolver;
			btDefaultCollisionConfiguration* mBulletCollisionConfig;
			btCollisionDispatcher* mBulletCollisionDispatcher;
			btDiscreteDynamicsWorld* mBulletDynamicsWorld;
		};
	}
}