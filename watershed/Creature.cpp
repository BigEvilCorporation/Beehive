///////////////////////////////////////////////////
// File:		Character.cpp
// Date:		4th January 2013
// Authors:		Matt Phillips
// Description:	Watershed - native Creature
///////////////////////////////////////////////////

#include "Creature.h"

namespace ws
{
	Creature::Creature(ion::physics::World& physicsWorld, ion::renderer::Scene& scene)
	{
		//Defaults
		mAcceleration = 0.4f;
		mDeceleration = 0.8f;
		mCurrSpeed = 0.0f;
		mTopSpeed = 0.15f;

		//Create material
		mMaterial = new ion::renderer::Material();
		mMaterial->SetAmbientColour(ion::Colour(1.0f, 0.3f, 0.3f));
		mMaterial->SetDiffuseColour(ion::Colour(1.0f, 0.3f, 0.3f));
		mMaterial->SetSpecularColour(ion::Colour(1.0f, 0.6f, 0.6f));
		mMaterial->SetReceiveShadows(false);

		//Create character box
		const ion::Vector2 characterDimensions(0.5f, 1.0f);
		mMesh = new ion::renderer::Primitive(scene, ion::renderer::Primitive::Proj3D);
		mMesh->AddBox(mMaterial, ion::Vector3(characterDimensions.x, characterDimensions.y, characterDimensions.x), ion::Vector3());
		mSceneNode = new ion::renderer::SceneNode(scene);
		mSceneNode->Attach(*mMesh);

		//Create physics controller
		mPhysicsController = new ion::physics::Character(characterDimensions);
		physicsWorld.AddCharacter(*mPhysicsController);

		//Set initial transform
		ion::Matrix4 characterTransform;
		characterTransform.SetTranslation(ion::Vector3(0.0f, characterDimensions.y / 2.0f, 0.0f));
		mPhysicsController->SetTransform(characterTransform);

		//Set max jump height
		mPhysicsController->SetMaxJumpHeight(characterDimensions.y);
	}

	void Creature::Jump(float jumpForce)
	{
		mPhysicsController->Jump(jumpForce);
	}

	void Creature::UpdateMovement(float deltaTime, ControlState controlState)
	{
		if(controlState == Left)
		{
			if(mCurrSpeed > 0.0f)
			{
				//Decelerate
				mCurrSpeed -= mDeceleration * deltaTime;
			}
			else
			{
				//Accelerate
				mCurrSpeed -= mAcceleration * deltaTime;
			}
		}
		else if(controlState == Right)
		{
			if(mCurrSpeed < 0.0f)
			{
				//Decelerate
				mCurrSpeed += mDeceleration * deltaTime;
			}
			else
			{
				//Accelerate
				mCurrSpeed += mAcceleration * deltaTime;
			}
		}
		else if(controlState == Idle)
		{
			//Not being controlled, decelerate towards zero
			float decelerationStep = mDeceleration * deltaTime;

			if(mCurrSpeed > 0.0f && mCurrSpeed > decelerationStep)
			{
				mCurrSpeed -= decelerationStep;
			}
			else if(mCurrSpeed < 0.0f && mCurrSpeed < decelerationStep)
			{
				mCurrSpeed += decelerationStep;
			}
			else
			{
				mCurrSpeed = 0.0f;
			}
		}

		//Clamp to top speed
		mCurrSpeed = ion::maths::Clamp(mCurrSpeed, -mTopSpeed, mTopSpeed);
		
		//Set movement vector
		ion::Vector3 moveVector(mCurrSpeed, 0.0f, 0.0f);
		mPhysicsController->SetMoveVector(moveVector);
	}

	void Creature::UpdateAnimation(float deltaTime)
	{
		//Update character graphics from physics simulation
		mSceneNode->SetTransform(mPhysicsController->GetTransform());
	}

	const ion::Matrix4& Creature::GetTransform()
	{
		return mPhysicsController->GetTransform();
	}
}