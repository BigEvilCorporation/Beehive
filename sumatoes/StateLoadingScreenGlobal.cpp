#include "StateLoadingScreenGlobal.h"
#include "GlobalResources.h"

StateLoadingScreenGlobal::StateLoadingScreenGlobal(ion::gamekit::StateManager& stateManager, ion::io::ResourceManager& resourceManager, ion::gamekit::State& nextState)
	: State(stateManager, resourceManager)
	, mNextState(nextState)
{
	mLoadTime = 0.0f;
	mMinLoadTime = 3.0f;
	mSpriteBackground = NULL;
}

StateLoadingScreenGlobal::~StateLoadingScreenGlobal()
{
	if(mSpriteBackground)
		delete mSpriteBackground;
}

void StateLoadingScreenGlobal::OnEnterState()
{
	resources::Load(mResourceManager);
	mSpriteBackground = new ion::render::Sprite(ion::render::Sprite::Render2D, ion::Vector2(1.0f, 1.0f), 0.001f, 1, 1, resources::Textures::sLoadingScreenGlobalBg, mResourceManager);
}

void StateLoadingScreenGlobal::OnLeaveState()
{
}

void StateLoadingScreenGlobal::OnPauseState()
{
}

void StateLoadingScreenGlobal::OnResumeState()
{
}

void StateLoadingScreenGlobal::Update(float deltaTime, ion::input::Keyboard* keyboard, ion::input::Mouse* mouse, ion::input::Gamepad* gamepad)
{
	mLoadTime += deltaTime;

	if(mResourceManager.GetNumResourcesWaiting() == 0 && mLoadTime >= mMinLoadTime)
	{
		//Finished loading, push next state
		mStateManager.SwapState(mNextState);
	}
}

void StateLoadingScreenGlobal::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	mSpriteBackground->Render(renderer, camera);
}
