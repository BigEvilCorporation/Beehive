#include "PhysicsTest.h"
#include "core/Version.h"
#include "core/Debug.h"
#include "core/Time.h"

#include <sstream>

PhysicsTest::PhysicsTest()
	: ion::framework::Application("PhysicsTest")
{
	mRenderer = NULL;

	mKeyboard = NULL;
	mMouse = NULL;
	mGamepad = NULL;

	mCameraPitch = 0.0f;
	mCameraYaw = 0.0f;
	mCameraSpeed = 10.0f;
	mMouseSensitivity = 0.005f;

	mFrameCount = 0;
}

PhysicsTest::~PhysicsTest()
{
}

bool PhysicsTest::Initialise()
{
	std::stringstream windowTitle;
	windowTitle << "ion::engine - build " << ion::sVersion.Major << "." << ion::sVersion.Minor << "." << ion::sVersion.Build;

	//Create renderer, scene, camera and viewport
	mRenderer = new ion::renderer::Renderer(windowTitle.str().c_str(), 1024, 768, false);
	mScene = new ion::renderer::Scene();
	mCamera = new ion::renderer::Camera(*mScene);
	mViewport = new ion::renderer::Viewport(*mRenderer, *mCamera);

	//Create input handlers
	mKeyboard = new ion::input::Keyboard();
	mMouse = new ion::input::Mouse();
	mGamepad = new ion::input::Gamepad();

	//Set input cooperative level with renderer window
	mKeyboard->SetCooperativeWindow(mRenderer->GetWindowHandle(), ion::input::Keyboard::Exclusive);
	mMouse->SetCooperativeWindow(mRenderer->GetWindowHandle(), ion::input::Mouse::Exclusive);

	//Set default ambient light and window background colour
	mScene->SetAmbientLight(ion::ColourRGB(0.8f, 0.8f, 0.8f));
	mViewport->SetBackgroundColour(ion::Colour(0.2f, 0.2f, 0.2f));

	//Create directional light
	mDirectionalLight = new ion::renderer::Light(ion::renderer::Light::Directional, *mScene);
	mDirectionalLight->SetDirection(ion::Vector3(1.0f, -1.0f, 1.0f));

	//Set default camera position and direction
	mCamera->SetPosition(ion::Vector3(0.0f, 2.0f, 10.0f));
	mCamera->LookAt(ion::Vector3(0.0f, 0.0f, 0.0f));

	//Create floor quad
	mQuad = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mQuad->AddQuad(NULL, 15.0f, 10.0f, ion::renderer::Primitive::xz, ion::Vector3());
	mQuadNode = new ion::renderer::SceneNode(*mScene);
	mQuadNode->Attach(*mQuad);

	//Create cube
	mCube = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mCube->AddBox(NULL, ion::Vector3(0.5f, 0.5f, 0.5f), ion::Vector3());
	mCubeNode = new ion::renderer::SceneNode(*mScene);
	mCubeNode->Attach(*mCube);

	mPhysicsWorld = new ion::physics::World;
	mPhysicsWorld->SetGravity(ion::Vector3(0.0f, -9.8f, 0.0f));

	mPhysicsFloor = new ion::physics::Body(ion::physics::Body::InfinitePlane, ion::Vector3(0.0f, 1.0f, 0.0f));
	mPhysicsWorld->AddBody(*mPhysicsFloor);

	mPhysicsBox = new ion::physics::Body(ion::physics::Body::Box, ion::Vector3(0.5f, 0.5f, 0.5f));

	ion::Matrix4 boxTransform;
	boxTransform.SetTranslation(ion::Vector3(0.0f, 10.0f, 0.0f));
	mPhysicsBox->SetTransform(boxTransform);

	mPhysicsWorld->AddBody(*mPhysicsBox);
	
	//Initialise FPS timer
	mStartTicks = ion::time::GetSystemTicks();
	
	return true;
}

void PhysicsTest::Shutdown()
{
	if(mKeyboard)
		delete mKeyboard;

	if(mMouse)
		delete mMouse;

	if(mGamepad)
		delete mGamepad;

	if(mViewport)
		delete mViewport;

	if(mCamera)
		delete mCamera;

	if(mScene)
		delete mScene;

	if(mRenderer)
		delete mRenderer;
}

bool PhysicsTest::Update(float deltaTime)
{
	mKeyboard->Update();
	mMouse->Update();
	mGamepad->Update();

	//Get state of escape key and gamepad back/select button, for exit
	bool exit = mKeyboard->KeyDown(DIK_ESCAPE);
	if(!exit)
		exit = mGamepad->ButtonDown(ion::input::Gamepad::SELECT);

	//Create camera move vector from WASD state
	ion::Vector3 cameraMoveVector;
	if(mKeyboard->KeyDown(DIK_W))
		cameraMoveVector.z -= mCameraSpeed * deltaTime;
	if(mKeyboard->KeyDown(DIK_S))
		cameraMoveVector.z += mCameraSpeed * deltaTime;
	if(mKeyboard->KeyDown(DIK_A))
		cameraMoveVector.x -= mCameraSpeed * deltaTime;
	if(mKeyboard->KeyDown(DIK_D))
		cameraMoveVector.x += mCameraSpeed * deltaTime;

	//Get mouse deltas
	float mouseDeltaX = (float)mMouse->GetDeltaX();
	float mouseDeltaY = (float)mMouse->GetDeltaY();

	//Move, pitch and yaw camera
	mCamera->Move(cameraMoveVector);
	mCamera->Pitch(-mouseDeltaY * mMouseSensitivity);
	mCamera->Yaw(-mouseDeltaX * mMouseSensitivity);

	//Update physics world using 10 substeps
	mPhysicsWorld->Step(deltaTime, 10);

	//Update graphics from physics simulation
	mCubeNode->SetTransform(mPhysicsBox->GetTransform());

	//Update renderer
	mRenderer->Update(deltaTime);

	//Update FPS display
	if(mFrameCount++ % 100 == 0)
	{
		//Get 100-frame end time and diff
		u64 endTicks = ion::time::GetSystemTicks();
		u64 diffTicks = endTicks - mStartTicks;

		//Calc frame time and frames per secod
		float frameTime = (float)ion::time::TicksToSeconds(diffTicks) / 100.0f;
		float framesPerSecond = 1.0f / frameTime;

		//Set window title
		std::stringstream text;
		text.setf(std::ios::fixed, std::ios::floatfield);
		text.precision(2);
		text << "FPS: " << framesPerSecond;
		mRenderer->SetWindowTitle(text.str().c_str());

		//Reset timer
		mStartTicks = ion::time::GetSystemTicks();
	}

	return !exit;
}

void PhysicsTest::Render()
{
}