#include "Watershed.h"
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

	mApplicationTime = 0.0f;
	mProjectileTriggered = false;

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

	//Create physics world
	mPhysicsWorld = new ion::physics::World;
	mPhysicsWorld->SetGravity(ion::Vector3(0.0f, -9.8f, 0.0f));

	//Create physics floor
	mPhysicsFloor = new ion::physics::Body(ion::physics::Body::InfinitePlane, ion::Vector3(0.0f, 1.0f, 0.0f));
	mPhysicsWorld->AddBody(*mPhysicsFloor);

	//Ensure it's immovable
	mPhysicsFloor->SetMass(0.0f);

	//Create box stack
	for(int x = 0; x < sBoxStackWidth; x++)
	{
		for(int y = 0; y < sBoxStackHeight; y++)
		{
			//Create graphical box and scene node
			mCubes[x][y] = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
			mCubes[x][y]->AddBox(NULL, ion::Vector3(0.5f, 0.5f, 0.5f), ion::Vector3());
			mCubeNodes[x][y] = new ion::renderer::SceneNode(*mScene);
			mCubeNodes[x][y]->Attach(*mCubes[x][y]);

			//Create physics box
			mPhysicsBoxes[x][y] = new ion::physics::Body(ion::physics::Body::Box, ion::Vector3(0.5f, 0.5f, 0.5f));
			mPhysicsWorld->AddBody(*mPhysicsBoxes[x][y]);

			//Set initial transform
			ion::Matrix4 boxTransform;
			boxTransform.SetTranslation(ion::Vector3((float)x - (sBoxStackWidth / 2), (float)y + 0.5f, 0.0f));
			mPhysicsBoxes[x][y]->SetTransform(boxTransform);
		}
	}

	//Create projectile box
	mProjectile = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mProjectile->AddBox(NULL, ion::Vector3(0.5f, 0.5f, 0.5f), ion::Vector3());
	mProjectileNode = new ion::renderer::SceneNode(*mScene);
	mProjectileNode->Attach(*mProjectile);

	//Create projectile physics box
	mPhysicsProjectile = new ion::physics::Body(ion::physics::Body::Box, ion::Vector3(0.5f, 0.5f, 0.5f));

	//Set initial transform
	ion::Matrix4 boxTransform;
	boxTransform.SetTranslation(ion::Vector3(0.25f, 5.0f, 10.0f));
	mPhysicsProjectile->SetTransform(boxTransform);

	//Set mass
	mPhysicsProjectile->SetMass(5.0f);

	//Create character box
	const ion::Vector2 characterDimensions(0.5f, 1.0f);
	mCharacter = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mCharacter->AddBox(NULL, ion::Vector3(characterDimensions.x, characterDimensions.y, characterDimensions.x), ion::Vector3());
	mCharacterNode = new ion::renderer::SceneNode(*mScene);
	mCharacterNode->Attach(*mCharacter);

	//Create physics character
	mPhysicsCharacter = new ion::physics::Character(characterDimensions);
	mPhysicsWorld->AddCharacter(*mPhysicsCharacter);

	//Set initial transform
	ion::Matrix4 characterTransform;
	characterTransform.SetTranslation(ion::Vector3(0.0f, characterDimensions.y / 2.0f, 5.0f));
	mPhysicsCharacter->SetTransform(characterTransform);

	//Set max jump height
	mPhysicsCharacter->SetMaxJumpHeight(characterDimensions.y);
	
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
	mApplicationTime += deltaTime;

	mKeyboard->Update();
	mMouse->Update();
	mGamepad->Update();

	//Get state of escape key and gamepad back/select button, for exit
	bool exit = mKeyboard->KeyDown(DIK_ESCAPE) || mGamepad->ButtonDown(ion::input::Gamepad::SELECT);

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

	//Move character
	const float characterMoveSpeed = 0.15f;
	const float characterJumpForce(10.0f);
	ion::Vector3 characterMoveVector;
	
	if(mKeyboard->KeyDown(DIK_UP))
		characterMoveVector.z -= characterMoveSpeed;
	if(mKeyboard->KeyDown(DIK_DOWN))
		characterMoveVector.z += characterMoveSpeed;
	if(mKeyboard->KeyDown(DIK_LEFT))
		characterMoveVector.x -= characterMoveSpeed;
	if(mKeyboard->KeyDown(DIK_RIGHT))
		characterMoveVector.x += characterMoveSpeed;
	if(mKeyboard->KeyPressedThisFrame(DIK_SPACE))
		mPhysicsCharacter->Jump(characterJumpForce);
	
	mPhysicsCharacter->SetMoveVector(characterMoveVector);

	//Fire projectile after box stack has had time to settle
	if(mApplicationTime > 3.0f && !mProjectileTriggered)
	{
		mPhysicsWorld->AddBody(*mPhysicsProjectile);
		mPhysicsProjectile->SetLinearVelocity(ion::Vector3(0.0f, 0.0f, -20.0f));
		mProjectileTriggered = true;
	}

	//Update physics world using 10 substeps
	mPhysicsWorld->Step(deltaTime, 10);

	//Update box stack graphics from physics simulation
	for(int x = 0; x < sBoxStackWidth; x++)
	{
		for(int y = 0; y < sBoxStackHeight; y++)
		{
			mCubeNodes[x][y]->SetTransform(mPhysicsBoxes[x][y]->GetTransform());
		}
	}

	//Update projectile graphics from physics simulation
	mProjectileNode->SetTransform(mPhysicsProjectile->GetTransform());
	mCharacterNode->SetTransform(mPhysicsCharacter->GetTransform());

	//Update renderer
	mRenderer->Update(deltaTime);

	//Update FPS display
	if(mFrameCount++ % 100 == 0)
	{
		//Get 100-frame end time and diff
		u64 endTicks = ion::time::GetSystemTicks();
		u64 diffTicks = endTicks - mStartTicks;

		//Calc frame time and frames per second
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