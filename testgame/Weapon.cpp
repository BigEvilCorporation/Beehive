#include "Weapon.h"
#include "Bullet.h"
#include "Ship.h"

Weapon::Weapon(const Ship& owner)
	: mOwner(owner)
{
	mFireRate = 4.0f;
	mBulletVelocity = 0.5f;
	mBulletDamage = 1.0f;
	mBulletRange = 4.0f;
	mAmmo = sUnlimitedAmmo;
	mCoolDown = 0.0f;
}

Weapon::~Weapon()
{
}

bool Weapon::Update(float deltaTime)
{
	mCoolDown -= deltaTime;
	if(mCoolDown < 0.0f)
		mCoolDown = 0.0f;

	for(std::list<Bullet*>::iterator it = mActiveBullets.begin(), end = mActiveBullets.end(); it != end;)
	{
		Bullet* bullet = (*it);
		++it;

		if(!bullet->Update(deltaTime))
		{
			mActiveBullets.remove(bullet);
			delete bullet;
		}
	}

	return true;
}

void Weapon::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	for(std::list<Bullet*>::iterator it = mActiveBullets.begin(), end = mActiveBullets.end(); it != end; ++it)
	{
		(*it)->Render(renderer, camera);
	}
}

bool Weapon::Fire()
{
	if(mCoolDown == 0.0f && (mAmmo > 0 || mAmmo == sUnlimitedAmmo))
	{
		mActiveBullets.push_back(new Bullet(*this, mOwner.GetPositionY(), mOwner.GetRotationY(), mOwner.GetCurrentVelocity().x + mBulletVelocity, mOwner.GetCylinderRadius()));
		mAmmo--;
		mCoolDown = 1.0f / mFireRate;

		return true;
	}

	return false;
}

void Weapon::AddAmmo(u32 ammoCount)
{
	mAmmo += ammoCount;
}
