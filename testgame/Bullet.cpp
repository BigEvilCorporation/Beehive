#include "Bullet.h"
#include "Weapon.h"

ion::render::Primitive* Bullet::sPrimitive = NULL;

Bullet::Bullet(Weapon& weapon, float initialPositionY, float initialRotationY, float velocity, float cylinderRadius)
	: mWeapon(weapon)
{
	mLifeRemaining = weapon.GetBulletRange() * ion::maths::Abs(velocity);
	mPositionY = initialPositionY;
	mRotationY = initialRotationY;
	mVelocity = velocity;
	mSceneCylinderRadius = cylinderRadius;

	if(!sPrimitive)
	{
		sPrimitive = new ion::render::Box(ion::Vector3(0.05f, 0.05f, 0.05f)); //ion::render::Sphere(0.05f, 6, 6);
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
		mRotationY += mVelocity * deltaTime;
		SetTransform(utils::CalculateCylinderTransform(mRotationY, mPositionY, mSceneCylinderRadius));
	}

	return mLifeRemaining > 0.0f;
}

void Bullet::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	temp::Materials::sDefault.Get()->Bind(GetTransform(), camera.GetTransform().GetInverse(), renderer.GetProjectionMatrix());
	renderer.DrawVertexBuffer(sPrimitive->GetVertexBuffer(), sPrimitive->GetIndexBuffer());
	temp::Materials::sDefault.Get()->Unbind();
}