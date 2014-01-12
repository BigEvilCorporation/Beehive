#include "TestGame.h"

#include <ion/renderer/win32/WindowWin32.h>

#include <sstream>

TestGame::TestGame()
	: ion::framework::Application("TestGame"),
	mSceneCylinderRadius(5.0f),
	mSceneCylinderHeight(4.0f),
	mCameraDistance(1.0f)
{
	mRenderer = NULL;

	mKeyboard = NULL;
	mMouse = NULL;
	mGamepad = NULL;

	mMouseSensitivity = 0.005f;

	mFrameCount = 0;
}

TestGame::~TestGame()
{
}

bool TestGame::Initialise()
{
	std::stringstream windowTitle;
	windowTitle << "ion::engine - build " << ion::sVersion.Major << "." << ion::sVersion.Minor << "." << ion::sVersion.Build;

	/*
	ion::render::Window* window = new ion::render::WindowWin32(windowTitle.str(), 1280, 720, false);

	while(window->Update())
	{
	}

	return false;
	*/

	//Create renderer, scene, camera and viewport
	mRenderer = ion::render::Renderer::Create(windowTitle.str(), 1280, 720, false);

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

	//Initialise FPS timer
	mStartTicks = ion::time::GetSystemTicks();

	//Set up game
	mPlayer = new Ship(mSceneCylinderRadius, mSceneCylinderHeight);

	ion::render::Material* material = new ion::render::Material();

	mMaterial = mResourceManager->GetResource<ion::render::Material>("ship.ion.material");

	while(mResourceManager->GetNumResourcesWaiting() > 0)
	{
	}

	/*
	ion::io::File fileV("../materials/ship.ion.material", ion::io::File::OpenWrite);
	if(fileV.IsOpen())
	{
		ion::io::Archive archiveIn(fileV, ion::io::Archive::Out);
		ion::render::Material::RegisterSerialiseType(archiveIn);
		archiveIn.Serialise(material);
		fileV.Close();
	}
	*/

	mPlayer->SetMaterial(mMaterial.Get());

	mBoxPrimitive = new ion::render::Box(NULL, ion::Vector3(0.2f, 3.0f, 0.2f), ion::Vector3());

	mRenderer->SetClearColour(ion::Colour(0.3f, 0.3f, 0.3f));
	
	return true;
}

void TestGame::Shutdown()
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

bool TestGame::Update(float deltaTime)
{
	mKeyboard->Update();
	mMouse->Update();
	mGamepad->Update();

	//Get state of escape key and gamepad back/select button, for exit
	bool exit = mKeyboard->KeyDown(DIK_ESCAPE);
	exit |= mGamepad->ButtonDown(ion::input::Gamepad::SELECT);

	//Create camera move vector from WASD state
	if(mKeyboard->KeyDown(DIK_W))
		mPlayer->Move(Ship::Up, deltaTime);
	if(mKeyboard->KeyDown(DIK_S))
		mPlayer->Move(Ship::Down, deltaTime);
	if(mKeyboard->KeyDown(DIK_A))
		mPlayer->Move(Ship::Left, deltaTime);
	if(mKeyboard->KeyDown(DIK_D))
		mPlayer->Move(Ship::Right, deltaTime);

	//Get mouse deltas
	float mouseDeltaX = (float)mMouse->GetDeltaX();
	float mouseDeltaY = (float)mMouse->GetDeltaY();

	//Update player
	mPlayer->Update(deltaTime);

	//Position camera behind ship
	ion::Vector3 playerPosition = mPlayer->GetTransform().GetTranslation();
	ion::Vector3 lookCentre(0.0f, playerPosition.y, 0.0f);
	ion::Vector3 playerDirectionFromCentre = lookCentre - playerPosition;
	ion::Vector3 cameraPosition = playerDirectionFromCentre.Normalise() * (mSceneCylinderRadius + mCameraDistance);
	cameraPosition.y = playerPosition.y;
	mCamera->SetPosition(cameraPosition);

	//Point at centre
	//mCamera->SetLookAt(lookCentre);

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
		mRenderer->SetWindowTitle(text.str().c_str());

		//Reset timer
		mStartTicks = ion::time::GetSystemTicks();
	}

	return !exit;
}

void TestGame::Render()
{
	mRenderer->ClearColour();
	mRenderer->ClearDepth();

	mRenderer->SetMatrix(mCamera->GetTransform().GetInverse() * mRenderer->GetProjectionMatrix());
	mRenderer->DrawVertexBuffer(mBoxPrimitive->GetVertexBuffer(), mBoxPrimitive->GetIndexBuffer());
	mRenderer->SetMatrix(ion::Matrix4());

	//Render player
	mPlayer->Render(*mRenderer, *mCamera);

	mRenderer->SwapBuffers();
}