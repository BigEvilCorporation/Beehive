#include "Ship.h"

Ship::Ship(float sceneCylinderRadius, float sceneCylinderHeight)
	: mSceneCylinderRadius(sceneCylinderRadius),
	mSceneCylinderHeight(sceneCylinderHeight)
{
	mMoveSpeedX = 0.5f;
	mMoveSpeedY = 10.0f;

	//Set initial position
	SetPosition(ion::Vector3(0.0f, sceneCylinderHeight / 2.0f, -sceneCylinderRadius));

	mBoxPrimitive = new ion::render::Box(NULL, ion::Vector3(0.2f, 0.1f, 0.2f), ion::Vector3());

	mMaterial = new ion::render::Material();

	ion::render::Shader* vertexShader = ion::render::Shader::Create();
	ion::render::Shader* pixelShader = ion::render::Shader::Create();
	vertexShader->Load("../shaders/default.cgfx", "VertexProgram", ion::render::Shader::Vertex);
	pixelShader->Load("../shaders/default.cgfx", "FragmentProgram", ion::render::Shader::Fragment);

	ion::render::Texture* texture = ion::render::Texture::Create();
	texture->Load("../textures/placeholder256.png");

	mMaterial->SetVertexShader(vertexShader);
	mMaterial->SetPixelShader(pixelShader);
	mMaterial->AddDiffuseMap(texture);
}

Ship::~Ship()
{
	delete mBoxPrimitive;
}

void Ship::Update(float deltaTime)
{

}

void Ship::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	mMaterial->Bind(GetTransform(), camera.GetTransform().GetInverse(), renderer.GetProjectionMatrix());
	renderer.DrawVertexBuffer(mBoxPrimitive->GetVertexBuffer(), mBoxPrimitive->GetIndexBuffer());
	mMaterial->Unbind();
}

void Ship::Move(MoveDirection direction, float deltaTime)
{
	float moveX = 0.0f;
	float moveY = 0.0f;

	if(direction & Up)
		moveY += mMoveSpeedY * deltaTime;
	if(direction & Down)
		moveY -= mMoveSpeedY * deltaTime;
	if(direction & Left)
		moveX -= mMoveSpeedX * deltaTime;
	if(direction & Right)
		moveX += mMoveSpeedX * deltaTime;

	//Move to centre
	ion::Vector3 position = GetTransform().GetTranslation();
	ion::Vector3 directionToCentre = ion::Vector3(0.0f, position.y, 0.0f) - position;
	Entity::Move(directionToCentre);

	//Rotate around Y
	ion::Quaternion quat;
	quat.FromAxis(moveX, ion::Vector3(0.0f, 1.0f, 0.0f));
	ion::Matrix4 matrix = quat.ToMatrix() * GetTransform();
	matrix.SetTranslation(ion::Vector3(0.0f, position.y, 0.0f));

	//Set transform
	SetTransform(matrix);

	//Move back
	Entity::Move(ion::Vector3(0.0f, 0.0f, 0.0f) - directionToCentre);

	//Set new Y
	SetPosition(GetTransform().GetTranslation() + ion::Vector3(0.0f, moveY, 0.0f));
}

void Ship::Shoot(ShootType shootType)
{

}