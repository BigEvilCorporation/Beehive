#include "Watershed.h"

#include <sstream>

Watershed::Watershed()
	: ion::framework::Application("Watershed")
{
	mRenderer = NULL;

	mKeyboard = NULL;
	mMouse = NULL;
	mGamepad = NULL;

	mMouseSensitivity = 0.005f;

	mCameraType = ThirdPerson;
	mThirdPersonCameraHeadOffset.y = 2.5f;
	mThirdPersonCameraHeadOffset.z = 15.0f;
	mFirstPersonCameraMoveSpeed = 10.0f;

	mCharacterAcceleration = 0.4f;
	mCharacterDeceleration = 0.8f;
	mCharacterCurrSpeed = 0.0f;
	mCharacterTopSpeed = 0.15f;

	mFrameCount = 0;
}

Watershed::~Watershed()
{
}

bool Watershed::Initialise()
{
	std::stringstream windowTitle;
	windowTitle << "ion::engine - build " << ion::sVersion.Major << "." << ion::sVersion.Minor << "." << ion::sVersion.Build;

	//Create renderer, scene, camera and viewport
	mRenderer = new ion::renderer::Renderer(windowTitle.str().c_str(), 1024, 768, false, 8);
	mScene = new ion::renderer::Scene();
	mCamera = new ion::renderer::Camera(*mScene);
	mViewport = new ion::renderer::Viewport(*mRenderer, *mCamera);

	//Create camera controllers
	mCameraThirdPerson = new ion::gamekit::CameraThirdPerson();
	mCameraThirdPerson->SetMoveSpeed(5.0f);

	//Create input handlers
	mKeyboard = new ion::input::Keyboard();
	mMouse = new ion::input::Mouse();
	mGamepad = new ion::input::Gamepad();

	//Set input cooperative level with renderer window
	mKeyboard->SetCooperativeWindow(mRenderer->GetWindowHandle(), ion::input::Keyboard::Exclusive);
	mMouse->SetCooperativeWindow(mRenderer->GetWindowHandle(), ion::input::Mouse::Exclusive);

	//Add post effects
	mPostEffectBloom = new ion::renderer::PostEffectBloom;
	mPostEffectBloom->AssignToViewport(*mViewport);
	mPostEffectBloom->SetBlurWidth(0.01f);
	mPostEffectBloom->SetBlendAlpha(0.15f);

	//Set default ambient light and window background colour
	mScene->SetAmbientLight(ion::ColourRGB(0.5f, 0.5f, 0.5f));
	mViewport->SetBackgroundColour(ion::Colour(0.2f, 0.2f, 0.2f));

	//Create directional light
	mDirectionalLight = new ion::renderer::Light(ion::renderer::Light::Directional, *mScene);
	mDirectionalLight->SetDiffuse(ion::ColourRGB(0.8f, 0.8f, 0.8f));
	mDirectionalLight->SetSpecular(ion::ColourRGB(0.9f, 0.9f, 0.9f));
	mDirectionalLight->SetDirection(ion::Vector3(1.0f, -1.0f, 1.0f));

	//Set default camera position and direction
	mCamera->SetPosition(ion::Vector3(0.0f, 2.0f, 10.0f));
	mCamera->LookAt(ion::Vector3(0.0f, 0.0f, 0.0f));

	//Create floor quad
	mFloorQuad = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mFloorQuad->AddQuad(NULL, 15.0f, 10.0f, ion::renderer::Primitive::xz, ion::Vector3());
	mFloorNode = new ion::renderer::SceneNode(*mScene);
	mFloorNode->Attach(*mFloorQuad);

	//Create physics world
	mPhysicsWorld = new ion::physics::World;
	mPhysicsWorld->SetGravity(ion::Vector3(0.0f, -9.8f, 0.0f));

	//Create physics floor
	mFloorBody = new ion::physics::Body(ion::physics::Body::InfinitePlane, ion::Vector3(0.0f, 1.0f, 0.0f));
	mPhysicsWorld->AddBody(*mFloorBody);

	//Ensure it's immovable
	mFloorBody->SetMass(0.0f);

	//Create character material
	mCharacterMaterial = new ion::renderer::Material();
	mCharacterMaterial->SetAmbientColour(ion::Colour(1.0f, 0.3f, 0.3f));
	mCharacterMaterial->SetDiffuseColour(ion::Colour(1.0f, 0.3f, 0.3f));
	mCharacterMaterial->SetSpecularColour(ion::Colour(1.0f, 0.6f, 0.6f));
	mCharacterMaterial->SetReceiveShadows(false);

	//Create character box
	const ion::Vector2 characterDimensions(0.5f, 1.0f);
	mCharacter = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mCharacter->AddBox(mCharacterMaterial, ion::Vector3(characterDimensions.x, characterDimensions.y, characterDimensions.x), ion::Vector3());
	mCharacterNode = new ion::renderer::SceneNode(*mScene);
	mCharacterNode->Attach(*mCharacter);

	//Create physics character
	mCharacterBody = new ion::physics::Character(characterDimensions);
	mPhysicsWorld->AddCharacter(*mCharacterBody);

	//Set initial transform
	ion::Matrix4 characterTransform;
	characterTransform.SetTranslation(ion::Vector3(0.0f, characterDimensions.y / 2.0f, 0.0f));
	mCharacterBody->SetTransform(characterTransform);

	//Set max jump height
	mCharacterBody->SetMaxJumpHeight(characterDimensions.y);
	
	//Initialise FPS timer
	mStartTicks = ion::time::GetSystemTicks();
	
	return true;
}

void Watershed::Shutdown()
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

bool Watershed::Update(float deltaTime)
{
	mKeyboard->Update();
	mMouse->Update();
	mGamepad->Update();

	//Get state of escape key and gamepad back/select button, for exit
	bool exit = mKeyboard->KeyDown(DIK_ESCAPE) || mGamepad->ButtonDown(ion::input::Gamepad::SELECT);

	//Toggle camera type with TAB key
	if(mKeyboard->KeyPressedThisFrame(DIK_TAB))
	{
		mCameraType = (CameraType)(((int)mCameraType + 1) % MaxCameraTypes);
	}

	//First person (cutscene/debug) camera, no character control
	if(mCameraType == FirstPerson)
	{
		//Un-register third person camera controller
		mCameraThirdPerson->SetCurrentCamera(mCamera);

		//Create camera move vector from WASD state
		ion::Vector3 cameraMoveVector;
		if(mKeyboard->KeyDown(DIK_W))
			cameraMoveVector.z -= mFirstPersonCameraMoveSpeed * deltaTime;
		if(mKeyboard->KeyDown(DIK_S))
			cameraMoveVector.z += mFirstPersonCameraMoveSpeed * deltaTime;
		if(mKeyboard->KeyDown(DIK_A))
			cameraMoveVector.x -= mFirstPersonCameraMoveSpeed * deltaTime;
		if(mKeyboard->KeyDown(DIK_D))
			cameraMoveVector.x += mFirstPersonCameraMoveSpeed * deltaTime;

		//Get mouse deltas
		float mouseDeltaX = (float)mMouse->GetDeltaX();
		float mouseDeltaY = (float)mMouse->GetDeltaY();

		//Move, pitch and yaw camera
		mCamera->Move(cameraMoveVector);
		mCamera->Pitch(-mouseDeltaY * mMouseSensitivity);
		mCamera->Yaw(-mouseDeltaX * mMouseSensitivity);

		//Reset character speed
		mCharacterBody->SetMoveVector(ion::Vector3(0.0f, 0.0f, 0.0f));
	}
	else if(mCameraType == ThirdPerson)
	{
		//Set third person camera
		mCameraThirdPerson->SetCurrentCamera(mCamera);

		//Move character
		const float characterJumpForce(10.0f);

		if(mKeyboard->KeyDown(DIK_A))
		{
			if(mCharacterCurrSpeed > 0.0f)
			{
				//Decelerate
				mCharacterCurrSpeed -= mCharacterDeceleration * deltaTime;
			}
			else
			{
				//Accelerate
				mCharacterCurrSpeed -= mCharacterAcceleration * deltaTime;
			}
		}
		else if(mKeyboard->KeyDown(DIK_D))
		{
			if(mCharacterCurrSpeed < 0.0f)
			{
				//Decelerate
				mCharacterCurrSpeed += mCharacterDeceleration * deltaTime;
			}
			else
			{
				//Accelerate
				mCharacterCurrSpeed += mCharacterAcceleration * deltaTime;
			}
		}
		else
		{
			//Character not being controlled, decelerate towards zero
			float decelerationStep = mCharacterDeceleration * deltaTime;

			if(mCharacterCurrSpeed > 0.0f && mCharacterCurrSpeed > decelerationStep)
			{
				mCharacterCurrSpeed -= decelerationStep;
			}
			else if(mCharacterCurrSpeed < 0.0f && mCharacterCurrSpeed < decelerationStep)
			{
				mCharacterCurrSpeed += decelerationStep;
			}
			else
			{
				mCharacterCurrSpeed = 0.0f;
			}
		}

		if(mKeyboard->KeyPressedThisFrame(DIK_SPACE))
		{
			//Jump
			mCharacterBody->Jump(characterJumpForce);
		}

		//Clamp to top speed
		mCharacterCurrSpeed = ion::maths::Clamp(mCharacterCurrSpeed, -mCharacterTopSpeed, mCharacterTopSpeed);
		
		//Set movement vector
		ion::Vector3 characterMoveVector(mCharacterCurrSpeed, 0.0f, 0.0f);
		mCharacterBody->SetMoveVector(characterMoveVector);

		//Set camera focus target position as character position
		const ion::Matrix4 characterMtx = mCharacterBody->GetTransform();
		mCameraThirdPerson->SetTargetFocusPosition(characterMtx.GetTranslation());

		//Set camera head target position as character position plus offset
		mCameraThirdPerson->SetTargetHeadPosition(characterMtx.GetTranslation() + mThirdPersonCameraHeadOffset);

		//Update camera
		mCameraThirdPerson->Update(deltaTime);
	}

	//Update physics world using 10 substeps
	mPhysicsWorld->Step(deltaTime, 10);

	//Update character graphics from physics simulation
	mCharacterNode->SetTransform(mCharacterBody->GetTransform());

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

		//TODO: Slow! Draw to screen instead
		//mRenderer->SetWindowTitle(text.str().c_str());

		//Reset timer
		mStartTicks = ion::time::GetSystemTicks();
	}

	return !exit;
}

void Watershed::Render()
{
}