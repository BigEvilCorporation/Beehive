///////////////////////////////////////////////////
// File:		World.h
// Date:		6th December 2012
// Authors:		Matt Phillips
// Description:	Physics world
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
		//Forward declaration
		class Body;
		class Character;

		class World
		{
		public:
			World();

			void SetGravity(const ion::Vector3& gravity);

			void AddBody(Body& body);
			void RemoveBody(Body& body);

			void AddCharacter(Character& character);
			void RemoveCharacter(Character& character);

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