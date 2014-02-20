#include "StateLevel.h"
#include "GlobalResources.h"

#include <ion/input/Keyboard.h>
#include <ion/input/Mouse.h>
#include <ion/input/Gamepad.h>

StateLevel::StateLevel(ion::gamekit::StateManager& stateManager, ion::io::ResourceManager& resourceManager)
	: State(stateManager, resourceManager)
{
	mSpriteBackground = NULL;
	mPlayer1 = NULL;
	mPlayer2 = NULL;
}

StateLevel::~StateLevel()
{
	if(mSpriteBackground)
		delete mSpriteBackground;
}

void StateLevel::OnEnterState()
{
	
}

void StateLevel::OnLeaveState()
{
}

void StateLevel::OnPauseState()
{
}

void StateLevel::OnResumeState()
{
}

void StateLevel::Update(float deltaTime, ion::input::Keyboard* keyboard, ion::input::Mouse* mouse, ion::input::Gamepad* gamepad)
{
	if(keyboard->KeyPressedThisFrame(DIK_Q))
	{
		//Back to main menu
		mStateManager.PopState();
	}

	if(mPlayer1)
		mPlayer1->Update(deltaTime, keyboard, mouse, gamepad);

	if(mPlayer2)
		mPlayer2->Update(deltaTime, NULL, NULL, NULL);
}

void StateLevel::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	mSpriteBackground->Render(renderer, camera);

	if(mPlayer1)
		mPlayer1->Render(renderer, camera);

	if(mPlayer2)
		mPlayer2->Render(renderer, camera);
}

void StateLevel::LoadResources(const std::string& levelName, const std::string& character1, const std::string& character2)
{
	std::string levelFile = "../scripts/levels/";
	levelFile += levelName;
	levelFile += ".xml";

	ion::io::XML levelXml;
	if(levelXml.Load(levelFile))
	{
		ion::io::XML* texturesNode = levelXml.FindChild("Textures");
		if(texturesNode)
		{
			std::string textureName;

			texturesNode->GetAttribute("background", textureName);
			mSpriteBackground = new ion::render::Sprite(ion::render::Sprite::Render2D, ion::Vector2(1.0f, 1.0f), 0.001f, 1, 1, textureName, mResourceManager);
		}

		mPlayer1 = new Character();
		mPlayer1->LoadResources(character1, mResourceManager);
	}
	else
	{
		ion::debug::Error((std::string("Could not load ") + levelFile).c_str());
	}
}