#include <ion/core/bootstrap/Application.h>
#include <ion/core/Version.h>
#include <ion/core/debug/Debug.h>
#include <ion/core/thread/Thread.h>
#include <ion/core/time/Time.h>
#include <ion/io/fileSystem.h>
#include <ion/maths/Vector.h>
#include <ion/maths/Matrix.h>
#include <ion/io/Resource.h>
#include <ion/io/ResourceManager.h>
#include <ion/io/File.h>
#include <ion/input/Keyboard.h>
#include <ion/input/Mouse.h>
#include <ion/input/Gamepad.h>
#include <ion/physics/World.h>
#include <ion/physics/Body.h>
#include <ion/physics/Character.h>
#include <ion/renderer/Renderer.h>
#include <ion/renderer/Colour.h>
#include <ion/renderer/Shader.h>
#include <ion/renderer/Camera.h>
#include <ion/renderer/Primitive.h>
#include <ion/renderer/Material.h>
#include <ion/renderer/Texture.h>
#include <ion/renderer/Sprite.h>
#include <ion/renderer/Window.h>

class PhysicsTest : public ion::framework::Application
{
public:
	PhysicsTest();
	~PhysicsTest();

	bool Initialise();
	void Shutdown();
	bool Update(float deltaTime);
	void Render();
	
	ion::render::Renderer* mRenderer;
	ion::render::Camera* mCamera;
	ion::input::Keyboard* mKeyboard;
	ion::input::Mouse* mMouse;
	ion::input::Gamepad* mGamepad;
	ion::io::ResourceManager* mResourceManager;

	static const int sBoxStackWidth = 5;
	static const int sBoxStackHeight = 5;

	ion::render::Primitive* mQuad;
	ion::render::Primitive* mCube;
	ion::render::Primitive* mProjectile;
	ion::render::Primitive* mCharacter;

	ion::physics::World* mPhysicsWorld;
	ion::physics::Body* mPhysicsFloor;
	ion::physics::Body* mPhysicsBoxes[sBoxStackWidth][sBoxStackHeight];
	ion::physics::Body* mPhysicsProjectile;
	ion::physics::Character* mPhysicsCharacter;

	ion::io::ResourceHandle<ion::render::Material> mMaterial;

	bool mProjectileTriggered;

	float mCameraPitch;
	float mCameraYaw;
	float mCameraSpeed;
	float mMouseSensitivity;

	float mApplicationTime;

	u64 mStartTicks;
	u32 mFrameCount;
};