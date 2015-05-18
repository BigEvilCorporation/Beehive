#pragma once

#include <ion/gamekit/StateManager.h>
#include <ion/io/ResourceManager.h>
#include <ion/renderer/Renderer.h>
#include <ion/renderer/Camera.h>
#include <ion/renderer/Sprite.h>

#include "StateLevel.h"
#include "StateLoadingScreenLevel.h"

class StateMainMenu : public ion::gamekit::State
{
public:
	StateMainMenu(ion::gamekit::StateManager& stateManager, ion::io::ResourceManager& resourceManager);
	virtual ~StateMainMenu();

	virtual void OnEnterState();
	virtual void OnLeaveState();
	virtual void OnPauseState();
	virtual void OnResumeState();

	virtual void Update(float deltaTime, ion::input::Keyboard* keyboard, ion::input::Mouse* mouse, ion::input::Gamepad* gamepad);
	virtual void Render(ion::render::Renderer& renderer, ion::render::Camera& camera);

private:
	ion::render::Sprite* mSpriteBackground;
	StateLevel* mLevelState;
	StateLoadingScreenLevel* mLevelLoadState;
};