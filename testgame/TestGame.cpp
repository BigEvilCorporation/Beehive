#include "TestGame.h"

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

	//Initialise FPS timer
	mStartTicks = ion::time::GetSystemTicks();

	//Set up game
	mPlayer = new Ship(mSceneCylinderRadius, mSceneCylinderHeight);

	ion::render::Material* material = new ion::render::Material();

	mVertexShader = mResourceManager->RequestLoad<ion::render::Shader>("../shaders/default_v.ion.shader");

	mResourceManager->WaitUntilEmpty();

	/*ion::render::Shader* vertexShader = NULL;
	ion::render::Shader* pixelShader = NULL;

	ion::io::File fileV("../shaders/default_v.ion.shader", ion::io::File::OpenRead);
	if(fileV.IsOpen())
	{
		ion::io::Archive archiveIn(fileV, ion::io::Archive::In);
		ion::render::Shader::RegisterSerialiseType(archiveIn);
		archiveIn.Serialise(vertexShader);
		fileV.Close();
	}

	ion::io::File fileP("../shaders/default_p.ion.shader", ion::io::File::OpenRead);
	if(fileP.IsOpen())
	{
		ion::io::Archive archiveIn(fileP, ion::io::Archive::In);
		ion::render::Shader::RegisterSerialiseType(archiveIn);
		archiveIn.Serialise(pixelShader);
		fileP.Close();
	}*/

	ion::render::Texture* texture = ion::render::Texture::Create();
	texture->Load("../textures/placeholder256.png");

	material->SetVertexShader(mVertexShader.Get());
	material->SetPixelShader(mPixelShader.Get());
	material->AddDiffuseMap(texture);

	mPlayer->SetMaterial(material);

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