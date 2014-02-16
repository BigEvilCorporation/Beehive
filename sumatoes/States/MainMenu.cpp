#include "MainMenu.h"
#include "../GlobalResources.h"

MainMenu::MainMenu(ion::gamekit::StateManager& stateManager, ion::io::ResourceManager& resourceManager)
	: State(stateManager, resourceManager)
{
	mSpriteBackground = NULL;
}

MainMenu::~MainMenu()
{
	if(mSpriteBackground)
		delete mSpriteBackground;
}

void MainMenu::OnEnterState()
{
	mSpriteBackground = new ion::render::Sprite(ion::render::Sprite::Render2D, ion::Vector2(1.0f, 1.0f), 0.001f, 1, 1, resources::Textures::sMainMenuBg, mResourceManager);
}

void MainMenu::OnLeaveState()
{
}

void MainMenu::OnPauseState()
{
}

void MainMenu::OnResumeState()
{
}

void MainMenu::Update(float deltaTime)
{
}

void MainMenu::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	mSpriteBackground->Render(renderer, camera);
}
