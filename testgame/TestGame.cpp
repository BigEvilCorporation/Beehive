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

	//Load temp materials
	temp::Materials::LoadTempMaterials(*mResourceManager);

	//Create sprites
	mSprite = new ion::render::Sprite(ion::render::Sprite::Render2D, 1.0f, 1.0f, 1, 1, "placeholder256.ion.texture", *mResourceManager);

	while(mResourceManager->GetNumResourcesWaiting() > 0)
	{
	}

	/*
	ion::io::File file("../shaders/sprite_p.ion.shader", ion::io::File::OpenWrite);
	if(file.IsOpen())
	{
		ion::render::Shader* shader = ion::render::Shader::Create();
		shader->SetProgram("../shaders/programs/cg/sprite.cgfx", "FragmentProgram", ion::render::Shader::Fragment);
		ion::io::Archive archive(file, ion::io::Archive::Out);
		ion::render::Shader::RegisterSerialiseType(archive);
		archive.Serialise(shader);
		file.Close();
	}
	*/

	//Set up scene
	mCylinderPrimitive = new ion::render::Cylinder(mSceneCylinderRadius - 5.0f, mSceneCylinderHeight, 16, ion::Vector3(0.0f, mSceneCylinderHeight / 2.0f, 0.0f));

	//Set up player ship
	mPlayer = new Ship(mSceneCylinderRadius, mSceneCylinderHeight);
	mPlayer->SetMaterial(temp::Materials::sDefault.Get());

	//Set up enemy ships
	WaveParams waveParams;
	waveParams.spawnPattern = WaveParams::Line;
	waveParams.spawnLine = ion::Vector2(0.5f, 0.5f);
	waveParams.minNumUnits = 5;
	waveParams.maxNumUnits = 10;

	mEnemyWaves.push_back(new EnemyWave(waveParams, 5, mSceneCylinderRadius, mSceneCylinderHeight));

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
	mRenderer->BeginFrame();

	mRenderer->ClearColour();
	mRenderer->ClearDepth();

	//Render scene
	temp::Materials::sDefault.Get()->Bind(ion::Matrix4(), mCamera->GetTransform().GetInverse(), mRenderer->GetProjectionMatrix());
	mRenderer->DrawVertexBuffer(mCylinderPrimitive->GetVertexBuffer(), mCylinderPrimitive->GetIndexBuffer());
	temp::Materials::sDefault.Get()->Unbind();

	//Render player
	mPlayer->Render(*mRenderer, *mCamera);

	//Render enemies
	for(unsigned int i = 0; i < mEnemyWaves.size(); i++)
	{
		mEnemyWaves[i]->Render(*mRenderer, *mCamera);
	}

	//Render sprites
	mSprite->Render(*mRenderer, *mCamera);

	mRenderer->SwapBuffers();

	mRenderer->EndFrame();
}