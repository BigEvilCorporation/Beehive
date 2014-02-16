#pragma once

#include <ion/gamekit/StateManager.h>
#include <ion/io/ResourceManager.h>
#include <ion/renderer/Renderer.h>
#include <ion/renderer/Camera.h>
#include <ion/renderer/Sprite.h>

class MainMenu : public ion::gamekit::State
{
public:
	MainMenu(ion::gamekit::StateManager& stateManager, ion::io::ResourceManager& resourceManager);
	virtual ~MainMenu();

	virtual void OnEnterState();
	virtual void OnLeaveState();
	virtual void OnPauseState();
	virtual void OnResumeState();

	virtual void Update(float deltaTime);
	virtual void Render(ion::render::Renderer& renderer, ion::render::Camera& camera);

private:
	ion::render::Sprite* mSpriteBackground;
};