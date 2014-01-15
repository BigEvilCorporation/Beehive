#include "Bullet.h"
#include "Weapon.h"

ion::render::Sphere* Bullet::sSpherePrimitive = NULL;

Bullet::Bullet(Weapon& weapon, float initialPositionY, float initialRotationY, float cylinderRadius)
	: mWeapon(weapon)
{
	mLifeRemaining = weapon.GetBulletRange() / weapon.GetBulletSpeed();
	mPositionY = initialPositionY;
	mRotationY = initialRotationY;
	mSceneCylinderRadius = cylinderRadius;

	if(!sSpherePrimitive)
	{
		sSpherePrimitive = new ion::render::Sphere(0.03f, 6, 6);
	}
}

Bullet::~Bullet()
{
}

bool Bullet::Update(float deltaTime)
{
	mLifeRemaining -= deltaTime;
	if(mLifeRemaining > 0.0f)
	{
		mRotationY += mWeapon.GetBulletSpeed() * deltaTime;
		SetTransform(utils::CalculateCylinderTransform(mRotationY, mPositionY, mSceneCylinderRadius));
	}

	return mLifeRemaining > 0.0f;
}

void Bullet::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	temp::Materials::sDefault.Get()->Bind(GetTransform(), camera.GetTransform().GetInverse(), renderer.GetProjectionMatrix());
	renderer.DrawVertexBuffer(sSpherePrimitive->GetVertexBuffer(), sSpherePrimitive->GetIndexBuffer());
	temp::Materials::sDefault.Get()->Unbind();
}