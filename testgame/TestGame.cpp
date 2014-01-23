#include "TestGame.h"

#include <sstream>

TestGame::TestGame()
	: ion::framework::Application("TestGame"),
	mSceneCylinderRadius(30.0f),
	mSceneCylinderHeight(6.0f),
	mCameraDistance(10.0f)
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

	//Set scene clear colour
	mRenderer->SetClearColour(ion::Colour(0.3f, 0.3f, 0.3f));

	//ion::render::Material* material = new ion::render::Material();

	//Load temp materials
	temp::Materials::LoadTempMaterials(*mResourceManager);

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

	//Set up scene
	mCylinderPrimitive = new ion::render::Cylinder(mSceneCylinderRadius - 5.0f, mSceneCylinderHeight, 16, ion::Vector3(0.0f, mSceneCylinderHeight / 2.0f, 0.0f));

	//Set up player ship
	mPlayer = new Ship(mSceneCylinderRadius, mSceneCylinderHeight);
	mPlayer->SetMaterial(temp::Materials::sDefault.Get());

	/*
	//Set up enemy ships
	SpawnPattern* spawnPattern = new SpawnLine(mSceneCylinderRadius, mSceneCylinderHeight, ion::Vector2(5.0f, 5.0f));

	const int numEnemies = 10;
	for(int i = 0; i < numEnemies; i++)
	{
		float time = (float)numEnemies / (float)i;
		ion::Matrix4 spawnTransform = spawnPattern->GetSpawnTransform(time, ion::Vector2(0.0f, 0.0f));
		Enemy* enemy = new Enemy(mSceneCylinderRadius, mSceneCylinderHeight);
		enemy->SetTransform(spawnTransform);
		mEnemies.push_back(enemy);
	}
	*/

	//Initial camera position
	mCamera->SetPosition(ion::Vector3(0.0f, mSceneCylinderHeight / 2.0f, 1.0f));
	
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

	//Create move vector from left stick
	ion::Vector2 moveVector = mGamepad->GetLeftStick();

	//Create move vector from WASD state
	if(mKeyboard->KeyDown(DIK_W))
		moveVector.y = 1.0f;
	if(mKeyboard->KeyDown(DIK_S))
		moveVector.y = -1.0f;
	if(mKeyboard->KeyDown(DIK_A))
		moveVector.x = -1.0f;
	if(mKeyboard->KeyDown(DIK_D))
		moveVector.x = 1.0f;

	mPlayer->Move(moveVector);

	//Get mouse deltas
	float mouseDeltaX = (float)mMouse->GetDeltaX();
	float mouseDeltaY = (float)mMouse->GetDeltaY();

	//Update player
	mPlayer->Update(deltaTime);

	//Position camera behind ship, half-way up the cylinder
	mCamera->SetTransform(utils::CalculateCylinderTransform(mPlayer->GetRotationY(), mSceneCylinderHeight / 2.0f, mSceneCylinderRadius + mCameraDistance));

	//Check fire buttons
	if(mGamepad->ButtonDown(ion::input::Gamepad::BUTTON_A) || mKeyboard->KeyDown(DIK_SPACE))
		mPlayer->Fire(Ship::Primary);

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

void TestGame::Render()
{
	mRenderer->ClearColour();
	mRenderer->ClearDepth();

	//Render scene
	temp::Materials::sDefault.Get()->Bind(ion::Matrix4(), mCamera->GetTransform().GetInverse(), mRenderer->GetProjectionMatrix());
	mRenderer->DrawVertexBuffer(mCylinderPrimitive->GetVertexBuffer(), mCylinderPrimitive->GetIndexBuffer());
	temp::Materials::sDefault.Get()->Unbind();

	//Render player
	mPlayer->Render(*mRenderer, *mCamera);

	//Render enemies
	for(int i = 0; i < mEnemies.size(); i++)
	{
		mEnemies[i]->Render(*mRenderer, *mCamera);
	}

	mRenderer->SwapBuffers();
}