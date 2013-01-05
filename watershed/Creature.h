///////////////////////////////////////////////////
// File:		Character.h
// Date:		4th January 2013
// Authors:		Matt Phillips
// Description:	Watershed - native Creature
///////////////////////////////////////////////////

#pragma once

#include "renderer/Primitive.h"
#include "renderer/Material.h"
#include "renderer/Scene.h"
#include "renderer/SceneNode.h"
#include "physics/World.h"
#include "physics/Character.h"

namespace ws
{
	class Creature
	{
	public:
		enum ControlState { Idle, Left, Right };

		Creature(ion::physics::World& physicsWorld, ion::renderer::Scene& scene);

		void Jump(float jumpForce);

		void UpdateMovement(float deltaTime, ControlState controlState);
		void UpdateAnimation(float deltaTime);

		const ion::Matrix4& GetTransform();

	private:
		ion::renderer::Primitive* mMesh;
		ion::renderer::Material* mMaterial;
		ion::renderer::SceneNode* mSceneNode;
		ion::physics::Character* mPhysicsController;

		float mAcceleration;
		float mDeceleration;
		float mCurrSpeed;
		float mTopSpeed;
	};
}