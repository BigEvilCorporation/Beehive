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

	ion::renderer::Primitive* mQuad;
	ion::renderer::Primitive* mCube;
	ion::renderer::SceneNode* mQuadNode;
	ion::renderer::SceneNode* mCubeNode;

	float mCameraPitch;
	float mCameraYaw;
	float mCameraSpeed;
	float mMouseSensitivity;

	u64 mStartTicks;
	u32 mFrameCount;
};