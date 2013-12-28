#include "renderer/Camera.h"
#include "renderer/Entity.h"
#include "renderer/Primitive.h"
#include "renderer/Renderer.h"
#include "renderer/Material.h"
#include "renderer/Shader.h"

class Ship : public ion::render::Entity
{
public:
	enum MoveDirection { Up = 1, Down = 2, Left = 4, Right = 8 };
	enum ShootType { Primary, Secondary, Special };

	Ship(float sceneCylinderRadius, float sceneCylinderHeight);
	virtual ~Ship();

	virtual void Update(float deltaTime);
	virtual void Render(ion::render::Renderer& renderer, ion::render::Camera& camera);

	virtual void Move(MoveDirection direction, float deltaTime);
	virtual void Shoot(ShootType shootType);

protected:
	int mHealth;
	int mMaxHealth;
	int mShield;
	int mMaxShield;
	float mMoveSpeedX;
	float mMoveSpeedY;

	const float mSceneCylinderRadius;
	const float mSceneCylinderHeight;

	ion::render::Box* mBoxPrimitive;
	ion::render::Material* mMaterial;
};