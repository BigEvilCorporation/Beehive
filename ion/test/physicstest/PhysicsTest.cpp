#include "PhysicsTest.h"

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
	mRenderer = ion::render::Renderer::Create(windowTitle.str(), 1280, 720, false);

	//Set scene clear colour
	mRenderer->SetClearColour(ion::Colour(0.3f, 0.3f, 0.3f));

	//Create camera
	mCamera = new ion::render::Camera();

	//Create input handlers
	mKeyboard = new ion::input::Keyboard();
	mMouse = new ion::input::Mouse();
	mGamepad = new ion::input::Gamepad();

	//Set input cooperative level with renderer window
	mKeyboard->SetCooperativeWindow(ion::input::Keyboard::Exclusive);
	mMouse->SetCooperativeWindow(ion::input::Mouse::Exclusive);

	//Create resource manager
	mResourceManager = new ion::io::ResourceManager();

	//Set resource directories
	mResourceManager->SetResourceDirectory<ion::render::Shader>("../shaders");
	mResourceManager->SetResourceDirectory<ion::render::Texture>("../textures");
	mResourceManager->SetResourceDirectory<ion::render::Material>("../materials");

	//Create floor quad
	mQuad = new ion::render::Quad(ion::render::Quad::xz, ion::Vector2(15.0f, 10.0f));

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
			//Create physics box
			mPhysicsBoxes[x][y] = new ion::physics::Body(ion::physics::Body::Box, ion::Vector3(0.5f, 0.5f, 0.5f));
			mPhysicsWorld->AddBody(*mPhysicsBoxes[x][y]);

			//Set initial transform
			ion::Matrix4 boxTransform;
			boxTransform.SetTranslation(ion::Vector3((float)x - (sBoxStackWidth / 2), (float)y + 0.5f, 0.0f));
			mPhysicsBoxes[x][y]->SetTransform(boxTransform);
		}
	}

	//Create stack box
	mCube = new ion::render::Box(ion::Vector3(0.5f, 0.5f, 0.5f));

	//Create projectile box
	mProjectile = new ion::render::Box(ion::Vector3(0.5f, 0.5f, 0.5f), ion::Vector3());

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
	mCharacter = new ion::render::Box(ion::Vector3(characterDimensions.x, characterDimensions.y, characterDimensions.x), ion::Vector3());

	//Create physics character
	mPhysicsCharacter = new ion::physics::Character(characterDimensions);
	mPhysicsWorld->AddCharacter(*mPhysicsCharacter);

	//Set initial transform
	ion::Matrix4 characterTransform;
	characterTransform.SetTranslation(ion::Vector3(0.0f, characterDimensions.y / 2.0f, 5.0f));
	mPhysicsCharacter->SetTransform(characterTransform);

	//Set max jump height
	mPhysicsCharacter->SetMaxJumpHeight(characterDimensions.y);

	//Set intial camera position
	mCamera->SetPosition(ion::Vector3(0.0f, 0.0f, 15.0f));

	//Load test material
	mMaterial = mResourceManager->GetResource<ion::render::Material>("ship.ion.material");

	while(mResourceManager->GetNumResourcesWaiting() > 0)
	{
	}
	
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

	if(mCamera)
		delete mCamera;

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

	//Update renderer
	exit |= !mRenderer->Update(0.0f);

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
		mRenderer->GetWindow()->SetTitle(text.str().c_str());

		//Reset timer
		mStartTicks = ion::time::GetSystemTicks();
	}

	return !exit;
}

void PhysicsTest::Render()
{
	mRenderer->BeginFrame();

	mRenderer->ClearColour();
	mRenderer->ClearDepth();

	if(mMaterial)
	{
		mMaterial->Bind(mPhysicsFloor->GetTransform(), mCamera->GetTransform().GetInverse(), mRenderer->GetProjectionMatrix());
		mRenderer->DrawVertexBuffer(mQuad->GetVertexBuffer(), mQuad->GetIndexBuffer());
		mMaterial->Unbind();

		mMaterial->Bind(mPhysicsProjectile->GetTransform(), mCamera->GetTransform().GetInverse(), mRenderer->GetProjectionMatrix());
		mRenderer->DrawVertexBuffer(mProjectile->GetVertexBuffer(), mProjectile->GetIndexBuffer());
		mMaterial->Unbind();

		mMaterial->Bind(mPhysicsCharacter->GetTransform(), mCamera->GetTransform().GetInverse(), mRenderer->GetProjectionMatrix());
		mRenderer->DrawVertexBuffer(mCharacter->GetVertexBuffer(), mCharacter->GetIndexBuffer());
		mMaterial->Unbind();

		for(int x = 0; x < sBoxStackWidth; x++)
		{
			for(int y = 0; y < sBoxStackHeight; y++)
			{
				mMaterial->Bind(mPhysicsBoxes[x][y]->GetTransform(), mCamera->GetTransform().GetInverse(), mRenderer->GetProjectionMatrix());
				mRenderer->DrawVertexBuffer(mCube->GetVertexBuffer(), mCube->GetIndexBuffer());
				mMaterial->Unbind();
			}
		}
	}

	mRenderer->SwapBuffers();

	mRenderer->EndFrame();
}