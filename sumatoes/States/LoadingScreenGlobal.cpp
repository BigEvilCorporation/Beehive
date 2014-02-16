#include "LoadingScreenGlobal.h"
#include "../GlobalResources.h"

LoadingScreenGlobal::LoadingScreenGlobal(ion::gamekit::StateManager& stateManager, ion::io::ResourceManager& resourceManager, ion::gamekit::State& nextState)
	: State(stateManager, resourceManager)
	, mNextState(nextState)
{
	mLoadTime = 0.0f;
	mMinLoadTime = 3.0f;
	mSpriteBackground = NULL;
}

LoadingScreenGlobal::~LoadingScreenGlobal()
{
	if(mSpriteBackground)
		delete mSpriteBackground;
}

void LoadingScreenGlobal::OnEnterState()
{
	resources::Load(mResourceManager);
	mSpriteBackground = new ion::render::Sprite(ion::render::Sprite::Render2D, ion::Vector2(1.0f, 1.0f), 0.001f, 1, 1, resources::Textures::sLoadingScreenBg, mResourceManager);
}

void LoadingScreenGlobal::OnLeaveState()
{
}

void LoadingScreenGlobal::OnPauseState()
{
}

void LoadingScreenGlobal::OnResumeState()
{
}

void LoadingScreenGlobal::Update(float deltaTime)
{
	mLoadTime += deltaTime;

	if(mResourceManager.GetNumResourcesWaiting() == 0 && mLoadTime >= mMinLoadTime)
	{
		//Finished loading, push next state
		mStateManager.SwapState(mNextState);
	}
}

void LoadingScreenGlobal::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	mSpriteBackground->Render(renderer, camera);
}
