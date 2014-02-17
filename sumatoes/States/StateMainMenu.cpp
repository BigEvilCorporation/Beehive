#include "StateMainMenu.h"
#include "../GlobalResources.h"

#include <ion/input/Keyboard.h>
#include <ion/input/Mouse.h>
#include <ion/input/Gamepad.h>

StateMainMenu::StateMainMenu(ion::gamekit::StateManager& stateManager, ion::io::ResourceManager& resourceManager)
	: State(stateManager, resourceManager)
{
	mSpriteBackground = NULL;
	mLevelState = NULL;
	mLevelLoadState = NULL;
}

StateMainMenu::~StateMainMenu()
{
	if(mSpriteBackground)
		delete mSpriteBackground;
}

void StateMainMenu::OnEnterState()
{
	if(!mSpriteBackground)
		mSpriteBackground = new ion::render::Sprite(ion::render::Sprite::Render2D, ion::Vector2(1.0f, 1.0f), 0.001f, 1, 1, resources::Textures::sMainMenuBg, mResourceManager);

	if(mLevelState)
	{
		//Back from level state, delete it
		delete mLevelState;
		mLevelState = NULL;
	}
	
	if(mLevelLoadState)
	{
		//Back from loading screen/level, delete it
		delete mLevelLoadState;
		mLevelLoadState = NULL;
	}
}

void StateMainMenu::OnLeaveState()
{
}

void StateMainMenu::OnPauseState()
{
}

void StateMainMenu::OnResumeState()
{
}

void StateMainMenu::Update(float deltaTime, ion::input::Keyboard* keyboard, ion::input::Mouse* mouse, ion::input::Gamepad* gamepad)
{
	if(keyboard->KeyPressedThisFrame(DIK_SPACE))
	{
		//Create level state
		mLevelState = new StateLevel(mStateManager, mResourceManager);

		//Create loading screen state
		mLevelLoadState = new StateLoadingScreenLevel(mStateManager, mResourceManager, *mLevelState);

		//Begin resource load
		mLevelState->LoadResources(resources::Levels::sDemoLevel, resources::Levels::sDemoPlayer1, resources::Levels::sDemoPlayer2);

		//Push loading screen state
		mStateManager.PushState(*mLevelLoadState);
	}
}

void StateMainMenu::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	mSpriteBackground->Render(renderer, camera);
}
