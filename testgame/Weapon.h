#pragma once

#include <ion/maths/Vector.h>
#include <ion/renderer/Renderer.h>
#include <ion/renderer/Camera.h>

#include <list>

#include "Utils.h"

class Ship;
class Bullet;

class Weapon
{
public:
	static const int sUnlimitedAmmo = -1;

	Weapon(const Ship& owner);
	virtual ~Weapon();

	virtual bool Update(float deltaTime);
	virtual void Render(ion::render::Renderer& renderer, ion::render::Camera& camera);

	virtual bool Fire();

	virtual void AddAmmo(u32 ammoCount);
	u32 GetAmmo() const { return mAmmo; }

	const Ship& GetOwner() const { return mOwner; }

	float GetFireRate() const { return mFireRate; }
	float GetBulletVelocity() const { return mBulletVelocity; }
	float GetBulletDamage() const { return mBulletDamage; }
	float GetBulletRange() const { return mBulletRange; }

protected:

	//Stats
	float mFireRate;
	float mBulletVelocity;
	float mBulletDamage;
	float mBulletRange;

	//Bullets remaining
	u32 mAmmo;

	//Cooldown from last fire
	float mCoolDown;

	//Owner
	const Ship& mOwner;

	//Active bullets
	std::list<Bullet*> mActiveBullets;
};