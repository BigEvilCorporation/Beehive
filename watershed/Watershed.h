#include "core/Application.h"
#include "core/maths/Vector.h"
#include "core/maths/Matrix.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "input/Gamepad.h"
#include "renderer/Renderer.h"
#include "renderer/Scene.h"
#include "renderer/SceneNode.h"
#include "renderer/Camera.h"
#include "renderer/Viewport.h"
#include "renderer/Primitive.h"
#include "renderer/Light.h"
#include "physics/World.h"
#include "physics/Body.h"
#include "physics/Character.h"

class PhysicsTest : public ion::framework::Application
{
public:
	PhysicsTest();
	~PhysicsTest();

	bool Initialise();
	void Shutdown();
	bool Update(float deltaTime);
	void Render();
	
	ion::renderer::Renderer* mRenderer;
	ion::renderer::Scene* mScene;
	ion::renderer::Camera* mCamera;
	ion::renderer::Viewport* mViewport;
	ion::input::Keyboard* mKeyboard;
	ion::input::Mouse* mMouse;
	ion::input::Gamepad* mGamepad;

	ion::renderer::Light* mDirectionalLight;

	static const int sBoxStackWidth = 5;
	static const int sBoxStackHeight = 5;

	ion::renderer::Primitive* mQuad;
	ion::renderer::Primitive* mCubes[sBoxStackWidth][sBoxStackHeight];
	ion::renderer::Primitive* mProjectile;
	ion::renderer::Primitive* mCharacter;
	ion::renderer::SceneNode* mQuadNode;
	ion::renderer::SceneNode* mCubeNodes[sBoxStackWidth][sBoxStackHeight];
	ion::renderer::SceneNode* mProjectileNode;
	ion::renderer::SceneNode* mCharacterNode;

	ion::physics::World* mPhysicsWorld;
	ion::physics::Body* mPhysicsFloor;
	ion::physics::Body* mPhysicsBoxes[sBoxStackWidth][sBoxStackHeight];
	ion::physics::Body* mPhysicsProjectile;
	ion::physics::Character* mPhysicsCharacter;

	bool mProjectileTriggered;

	float mCameraPitch;
	float mCameraYaw;
	float mCameraSpeed;
	float mMouseSensitivity;

	float mApplicationTime;

	u64 mStartTicks;
	u32 mFrameCount;
};