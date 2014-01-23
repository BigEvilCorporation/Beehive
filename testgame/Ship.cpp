#include "Ship.h"

Ship::Ship(float sceneCylinderRadius, float sceneCylinderHeight)
	: mSceneCylinderRadius(sceneCylinderRadius),
	mSceneCylinderHeight(sceneCylinderHeight)
{
	mMaxVelocity.x = 0.3f;
	mMaxVelocity.y = 8.0f;

	mAcceleration.x = 3.0f;
	mAcceleration.y = 8.0f;

	mRotationSpeed = 2.0f;
	mRotationTime = 0.0f;
	mFireRotationThreshold = 0.9f;

	mPositionY = sceneCylinderHeight / 2.0f;
	mRotationY = 0.0f;
	mTargetDirection.x = 1.0f;

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
	//Lerp from current velocity towards target velocity
	ion::Vector2 targetVelocity = mMoveVector * mMaxVelocity;

	mVelocity.x = ion::maths::Lerp(mVelocity.x, targetVelocity.x, mAcceleration.x * deltaTime);
	mVelocity.y = ion::maths::Lerp(mVelocity.y, targetVelocity.y, mAcceleration.y * deltaTime);

	//Apply velocity
	mRotationY += mVelocity.x * deltaTime;
	mPositionY += mVelocity.y * deltaTime;
	
	//Clamp Y
	mPositionY = ion::maths::Clamp(mPositionY, 0.0f, mSceneCylinderHeight);

	//If Y has hit limits, zero Y velocity
	if(mPositionY == 0.0f || mPositionY == mSceneCylinderHeight)
	{
		mVelocity.y = 0.0f;
	}

	//Slerp rotation
	ion::Quaternion currentRotation;
	ion::Quaternion targetRotation;
	targetRotation.FromAxis(ion::maths::DegreesToRadians(180.0f), ion::Vector3(0.0f, 1.0f, 0.0f));

	if(mTargetDirection.x < 0.0f)
	{
		mRotationTime = ion::maths::Clamp(mRotationTime - mRotationSpeed * deltaTime, 0.0f, 1.0f);
	}
	else
	{
		mRotationTime = ion::maths::Clamp(mRotationTime + mRotationSpeed * deltaTime, 0.0f, 1.0f);
	}

	ion::Quaternion newRotation;
	newRotation.Slerp(currentRotation, targetRotation, mRotationTime);

	ion::Matrix4 newTransform = newRotation.ToMatrix() * utils::CalculateCylinderTransform(mRotationY, mPositionY, mSceneCylinderRadius);

	//Apply transform
	SetTransform(newTransform);

	//Reset move vector
	mMoveVector = ion::Vector2();

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

void Ship::Move(const ion::Vector2& moveVector)
{
	mMoveVector = moveVector;

	if(moveVector.x != 0.0f)
	{
		mTargetDirection.x = mMoveVector.x;
		mTargetDirection = mTargetDirection.Normalise();
	}
}

void Ship::Fire(ShootType shootType)
{
	//Don't fire if rotating
	if(mRotationTime > mFireRotationThreshold || mRotationTime < (1.0f - mFireRotationThreshold))
	{
		Weapon* weapon = mWeapons[shootType];
		if(weapon)
		{
			weapon->Fire();
		}
	}
}