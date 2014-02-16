#pragma once

#include <ion/gamekit/StateManager.h>
#include <ion/io/ResourceManager.h>
#include <ion/renderer/Renderer.h>
#include <ion/renderer/Camera.h>
#include <ion/renderer/Sprite.h>

class LoadingScreenGlobal : public ion::gamekit::State
{
public:
	LoadingScreenGlobal(ion::gamekit::StateManager& stateManager, ion::io::ResourceManager& resourceManager, ion::gamekit::State& nextState);
	virtual ~LoadingScreenGlobal();

	virtual void OnEnterState();
	virtual void OnLeaveState();
	virtual void OnPauseState();
	virtual void OnResumeState();

	virtual void Update(float deltaTime);
	virtual void Render(ion::render::Renderer& renderer, ion::render::Camera& camera);

private:
	ion::gamekit::State& mNextState;
	ion::render::Sprite* mSpriteBackground;

	float mLoadTime;

	//To prevent loading screen flashing quickly if loading from a fast disk
	float mMinLoadTime;
};