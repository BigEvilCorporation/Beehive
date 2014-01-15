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
	enum MoveDirection { Up = 1, Down = 2, Left = 4, Right = 8 };
	enum ShootType { Primary, Secondary, Special, MaxWeapons };

	Ship(float sceneCylinderRadius, float sceneCylinderHeight);
	virtual ~Ship();

	virtual void Update(float deltaTime);
	virtual void Render(ion::render::Renderer& renderer, ion::render::Camera& camera);

	virtual void Move(MoveDirection direction, float deltaTime);
	virtual void Fire(ShootType shootType);

	void SetMaterial(ion::render::Material* material);

	float GetPositionY() const { return mPositionY; }
	float GetRotationY() const { return mRotationY; }
	float GetCylinderRadius() const { return mSceneCylinderRadius; }

	const ion::Vector3& GetWeaponOffset() const { return mWeaponOffset; }

protected:

	//Health and shield
	int mHealth;
	int mMaxHealth;
	int mShield;
	int mMaxShield;

	//Movement speed
	float mMoveSpeedX;
	float mMoveSpeedY;

	//Vertical position up cylinder
	float mPositionY;

	//Rotation around cylinder
	float mRotationY;

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