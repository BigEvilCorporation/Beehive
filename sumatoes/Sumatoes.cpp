#include "Sumatoes.h"

Sumatoes::Sumatoes()
	: ion::framework::Application("Sumatoes")
{
	mRenderer = NULL;

	mKeyboard = NULL;
	mMouse = NULL;
	mGamepad = NULL;

	mFrameCount = 0;

	mSpriteTimer = 0.0f;
	mSpriteSpeed = 0.05f;
}

Sumatoes::~Sumatoes()
{
}

bool Sumatoes::Initialise()
{
	mWindowTitle << "Sumatoes - ion::engine - build " << ion::sVersion.Major << "." << ion::sVersion.Minor << "." << ion::sVersion.Build;

	//Create renderer
	mRenderer = ion::render::Renderer::Create(mWindowTitle.str(), 1136, 640, false);

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
	mRenderer->SetClearColour(ion::Colour(0.3f, 0.8f, 0.7f));

	//Set blend mode
	mRenderer->SetAlphaBlending(ion::render::Renderer::Translucent);

	float aspectX = 1280.0f / 720.0f;

	//Create sprites
	mSprite = new ion::render::Sprite(ion::render::Sprite::Render2D, 0.2f, 0.2f * aspectX, 4, 4, "placeholder256.ion.texture", *mResourceManager);
	mSprite->SetPosition(ion::Vector3(0.0f, 0.0f, 0.0f));

	while(mResourceManager->GetNumResourcesWaiting() > 0)
	{
	}
	
	return true;
}

void Sumatoes::Shutdown()
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

bool Sumatoes::Update(float deltaTime)
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

	//Get mouse deltas
	float mouseDeltaX = (float)mMouse->GetDeltaX();
	float mouseDeltaY = (float)mMouse->GetDeltaY();

	mSpriteTimer += deltaTime;
	if(mSpriteTimer > mSpriteSpeed)
	{
		mSpriteTimer = 0.0f;
		mSprite->SetFrame((mSprite->GetFrame() + 1) % 16);
	}

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
		text << mWindowTitle.str().c_str() << " - FPS: " << framesPerSecond;
		mRenderer->GetWindow()->SetTitle(text.str().c_str());

		//Reset timer
		mStartTicks = ion::time::GetSystemTicks();
	}

	return !exit;
}

void Sumatoes::Render()
{
	mRenderer->BeginFrame();

	mRenderer->ClearColour();
	mRenderer->ClearDepth();

	//Render sprites
	mSprite->Render(*mRenderer, *mCamera);

	mRenderer->SwapBuffers();

	mRenderer->EndFrame();
}