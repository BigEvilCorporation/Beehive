#include "Ship.h"

Ship::Ship(float sceneCylinderRadius, float sceneCylinderHeight)
	: mSceneCylinderRadius(sceneCylinderRadius),
	mSceneCylinderHeight(sceneCylinderHeight)
{
	mMoveSpeedX = 2.0f;
	mMoveSpeedY = 1.5f;

	mPositionY = sceneCylinderHeight / 2.0f;
	mRotationY = 0.0f;

	//Set initial position
	SetPosition(ion::Vector3(0.0f, mPositionY, -sceneCylinderRadius));

	mBoxPrimitive = new ion::render::Box(ion::Vector3(0.2f, 0.1f, 0.2f));

	for(int i = 0; i < MaxWeapons; i++)
	{
		mWeapons[i] = NULL;
	}

	mWeapons[Primary] = new Weapon(*this);
}

void Ship::SetMaterial(ion::render::Material* material)
{
	mMaterial = material;
}

Ship::~Ship()
{
	delete mBoxPrimitive;
}

void Ship::Update(float deltaTime)
{
	SetTransform(utils::CalculateCylinderTransform(mRotationY, mPositionY, mSceneCylinderRadius));

	for(int i = 0; i < MaxWeapons; i++)
	{
		if(mWeapons[i])
		{
			mWeapons[i]->Update(deltaTime);
		}
	}
}

void Ship::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	mMaterial->Bind(GetTransform(), camera.GetTransform().GetInverse(), renderer.GetProjectionMatrix());
	renderer.DrawVertexBuffer(mBoxPrimitive->GetVertexBuffer(), mBoxPrimitive->GetIndexBuffer());
	mMaterial->Unbind();

	for(int i = 0; i < MaxWeapons; i++)
	{
		if(mWeapons[i])
		{
			mWeapons[i]->Render(renderer, camera);
		}
	}
}

void Ship::Move(MoveDirection direction, float deltaTime)
{
	if(direction & Up)
		mPositionY += mMoveSpeedY * deltaTime;
	if(direction & Down)
		mPositionY -= mMoveSpeedY * deltaTime;
	if(direction & Left)
		mRotationY -= mMoveSpeedX * deltaTime;
	if(direction & Right)
		mRotationY += mMoveSpeedX * deltaTime;

	mPositionY = ion::maths::Clamp(mPositionY, 0.0f, mSceneCylinderHeight);
}

void Ship::Fire(ShootType shootType)
{
	Weapon* weapon = mWeapons[shootType];
	if(weapon)
	{
		weapon->Fire();
	}
}