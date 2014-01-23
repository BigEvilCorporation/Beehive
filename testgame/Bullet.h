#pragma once

#include <ion/maths/Vector.h>
#include <ion/renderer/Entity.h>
#include <ion/renderer/Renderer.h>
#include <ion/renderer/Camera.h>
#include <ion/renderer/Primitive.h>
#include <ion/renderer/Material.h>

#include "Utils.h"

class Weapon;

class Bullet : public ion::render::Entity
{
public:
	Bullet(Weapon& weapon, float initialPositionY, float initialRotationY, float velocity, float cylinderRadius);
	virtual ~Bullet();

	virtual bool Update(float deltaTime);
	virtual void Render(ion::render::Renderer& renderer, ion::render::Camera& camera);

protected:
	float mLifeRemaining;
	float mPositionY;
	float mRotationY;
	float mVelocity;
	float mSceneCylinderRadius;

	const Weapon& mWeapon;

	//Temp until mesh is done
	static ion::render::Primitive* sPrimitive;
};
