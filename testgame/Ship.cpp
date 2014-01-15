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

	mBoxPrimitive = new ion::render::Box(NULL, ion::Vector3(0.2f, 0.1f, 0.2f));
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
	/*
	//Rotate around Y
	ion::Quaternion quat;
	quat.FromAxis(mRotationY, ion::Vector3(0.0f, 1.0f, 0.0f));
	ion::Matrix4 matrix = quat.ToMatrix();
	SetTransform(matrix);

	//Move to Y and Z
	Entity::Move(ion::Vector3(0.0f, mPositionY, mSceneCylinderRadius));
	*/

	SetTransform(utils::CalculateCylinderTransform(mRotationY, mPositionY, mSceneCylinderRadius));
}

void Ship::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	mMaterial->Bind(GetTransform(), camera.GetTransform().GetInverse(), renderer.GetProjectionMatrix());
	renderer.DrawVertexBuffer(mBoxPrimitive->GetVertexBuffer(), mBoxPrimitive->GetIndexBuffer());
	mMaterial->Unbind();
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

void Ship::Shoot(ShootType shootType)
{

}