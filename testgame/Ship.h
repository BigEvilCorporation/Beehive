#pragma once

#include <ion/maths/Maths.h>
#include <ion/renderer/Camera.h>
#include <ion/renderer/Entity.h>
#include <ion/renderer/Primitive.h>
#include <ion/renderer/Renderer.h>
#include <ion/renderer/Material.h>

#include "Utils.h"
#include "Weapon.h"

class Ship : public ion::render::Entity
{
public:
	enum ShootType { Primary, Secondary, Special, MaxWeapons };

	Ship(float sceneCylinderRadius, float sceneCylinderHeight);
	virtual ~Ship();

	virtual void Update(float deltaTime);
	virtual void Render(ion::render::Renderer& renderer, ion::render::Camera& camera);

	virtual void Move(const ion::Vector2& moveVector);
	virtual void Fire(ShootType shootType);

	void SetMaterial(ion::render::Material* material);

	float GetPositionY() const { return mPositionY; }
	float GetRotationY() const { return mRotationY; }
	float GetCylinderRadius() const { return mSceneCylinderRadius; }

	const ion::Vector2& GetCurrentVelocity() const { return mVelocity; }
	const ion::Vector3& GetTargetDirection() const { return mTargetDirection; }

	const ion::Vector3& GetWeaponOffset() const { return mWeaponOffset; }

protected:

	//Health and shield
	int mHealth;
	int mMaxHealth;
	int mShield;
	int mMaxShield;

	//Max velocity
	ion::Vector2 mMaxVelocity;

	//Velocity
	ion::Vector2 mVelocity;

	//Acceleration
	ion::Vector2 mAcceleration;

	//Rotation speed
	float mRotationSpeed;

	//Rotation time
	float mRotationTime;

	//Vertical position up cylinder
	float mPositionY;

	//Rotation around cylinder
	float mRotationY;

	//Target direction
	ion::Vector3 mTargetDirection;

	//Current move state
	ion::Vector2 mMoveVector;

	//Threshold to fire weapon while rotating
	float mFireRotationThreshold;

	//Weapons
	Weapon* mWeapons[MaxWeapons];

	//Weapon offset
	ion::Vector3 mWeaponOffset;

	//Scene dimensions
	const float mSceneCylinderRadius;
	const float mSceneCylinderHeight;

	ion::render::Box* mBoxPrimitive;
	ion::render::Material* mMaterial;
};