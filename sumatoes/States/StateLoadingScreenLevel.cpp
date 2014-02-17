#include "StateLoadingScreenLevel.h"
#include "../GlobalResources.h"

StateLoadingScreenLevel::StateLoadingScreenLevel(ion::gamekit::StateManager& stateManager, ion::io::ResourceManager& resourceManager, ion::gamekit::State& levelState)
	: State(stateManager, resourceManager)
	, mLevelState(levelState)
{
	mLoadTime = 0.0f;
	mMinLoadTime = 3.0f;
	mSpriteBackground = NULL;
}

StateLoadingScreenLevel::~StateLoadingScreenLevel()
{
	if(mSpriteBackground)
		delete mSpriteBackground;
}

void StateLoadingScreenLevel::OnEnterState()
{
	//Create loading background sprite
	mSpriteBackground = new ion::render::Sprite(ion::render::Sprite::Render2D, ion::Vector2(1.0f, 1.0f), 0.001f, 1, 1, resources::Textures::sLoadingScreenGlobalBg, mResourceManager);
}

void StateLoadingScreenLevel::OnLeaveState()
{
}

void StateLoadingScreenLevel::OnPauseState()
{
}

void StateLoadingScreenLevel::OnResumeState()
{
}

void StateLoadingScreenLevel::Update(float deltaTime, ion::input::Keyboard* keyboard, ion::input::Mouse* mouse, ion::input::Gamepad* gamepad)
{
	mLoadTime += deltaTime;

	if(mResourceManager.GetNumResourcesWaiting() == 0 && mLoadTime >= mMinLoadTime)
	{
		//Finished loading, switch to level state
		mStateManager.SwapState(mLevelState);
	}
}

void StateLoadingScreenLevel::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	mSpriteBackground->Render(renderer, camera);
}
