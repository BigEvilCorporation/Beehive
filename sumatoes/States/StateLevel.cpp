#include "StateLevel.h"
#include "../GlobalResources.h"

#include <ion/input/Keyboard.h>
#include <ion/input/Mouse.h>
#include <ion/input/Gamepad.h>

StateLevel::StateLevel(ion::gamekit::StateManager& stateManager, ion::io::ResourceManager& resourceManager)
	: State(stateManager, resourceManager)
{
	mSpriteBackground = NULL;
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
}

void StateLevel::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	mSpriteBackground->Render(renderer, camera);
}

void StateLevel::LoadResources(const std::string& levelName, const std::string& character1, const std::string& character2)
{
	std::string filename = "../levels/";
	filename += levelName;
	filename += ".xml";

	ion::io::XML xml;
	if(xml.Load(filename))
	{
		ion::io::XML* texturesNode = xml.FindChild("Textures");
		if(texturesNode)
		{
			std::string textureName;

			texturesNode->GetAttribute("background", textureName);
			mSpriteBackground = new ion::render::Sprite(ion::render::Sprite::Render2D, ion::Vector2(1.0f, 1.0f), 0.001f, 1, 1, textureName, mResourceManager);
		}
	}
	else
	{
		ion::debug::Error((std::string("Could not load ") + filename).c_str());
	}
}