#include "Character.h"

#include <ion/core/string/String.h>

Character::Character()
{
	mSpriteSheet = NULL;
	mCurrentAnimation = NULL;
	mCurrentAction = Idle;
}

Character::~Character()
{
	if(mSpriteSheet)
	{
		delete mSpriteSheet;
	}
}

void Character::LoadResources(const std::string& characterName, ion::io::ResourceManager& resourceManager)
{
	std::string filename = "../scripts/characters/";
	filename += characterName;
	filename += ".xml";

	ion::io::XML xml;
	if(xml.Load(filename))
	{
		const ion::io::XML* statsNode = xml.FindChild("Stats");
		if(statsNode)
		{
			mStats.Read(*statsNode);
		}

		const ion::io::XML* spriteSheetNode = xml.FindChild("SpriteSheet");
		if(spriteSheetNode)
		{
			int width = 1;
			int height = 1;
			spriteSheetNode->GetAttribute("numCellsX", width);
			spriteSheetNode->GetAttribute("numCellsY", height);

			std::string textureFile;
			spriteSheetNode->GetAttribute("texture", textureFile);
			mSpriteSheet = new ion::render::Sprite(ion::render::Sprite::Render2D, mStats.size, 0.001f, width, height, textureFile, resourceManager);
		}

		const ion::io::XML* spriteAnimationNode = xml.FindChild("SpriteAnimations");
		if(spriteAnimationNode)
		{
			int numAnimations = spriteAnimationNode->GetNumChildren();
			mSpriteAnimations.reserve(numAnimations);

			for(int i = 0; i < numAnimations; i++)
			{
				ParseAnimation(*spriteAnimationNode->GetChild(i));
			}
		}

		//Data ready, begin idle action
		PerformAction(Idle);
	}
	else
	{
		ion::debug::Error((std::string("Could not load character ") + filename).c_str());
	}
}

void Character::ParseAnimation(const ion::io::XML& node)
{
	ion::render::SpriteAnimation* animation = new ion::render::SpriteAnimation(*mSpriteSheet);
	ion::render::AnimationTrackInt* animTrack = new ion::render::AnimationTrackInt();
	animation->SetAnimationTrack(*animTrack);
	mSpriteAnimations.push_back(animation);

	float duration = 1.0f;
	node.GetAttribute("Duration", duration);
	animation->SetPlaybackSpeed(1.0f / duration);

	bool loop = false;
	node.GetAttribute("Loop", loop);
	animation->SetPlaybackBehaviour(loop ? ion::render::Animation::Loop : ion::render::Animation::PlayOnce);

	std::vector<std::string> frames;
	int numFrames = ion::string::Tokenise(node.GetData(), frames, ',');

	for(int i = 0; i < numFrames; i++)
	{
		animTrack->AddKeyframe(ion::render::Keyframe<int>(ion::maths::UnLerp(0.0f, (float)numFrames-1.0f, (float)i), atoi(frames[i].c_str())));
	}
}

void Character::Update(float deltaTime, ion::input::Keyboard* keyboard, ion::input::Mouse* mouse, ion::input::Gamepad* gamepad)
{
	if(keyboard)
	{
		if(keyboard->KeyPressedThisFrame(DIK_SPACE) && mCurrentAction != Jump)
		{
			PerformAction(Jump);
		}
	}

	if(mCurrentAnimation)
	{
		//Update animation
		mCurrentAnimation->Update(deltaTime);

		//If anim finished, revert to idle
		if(mCurrentAnimation->GetState() == ion::render::Animation::Stopped)
		{
			PerformAction(Idle);
		}
	}
}

void Character::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	if(mSpriteSheet)
		mSpriteSheet->Render(renderer, camera);
}

void Character::TakeDamage(Character& character, int damage)
{
}

int Character::GetHealth() const
{
	return mHealth;
}

void Character::PerformAction(Actions action)
{
	if(mCurrentAnimation)
	{
		mCurrentAnimation->SetState(ion::render::Animation::Stopped);
		mCurrentAnimation->SetFrame(0.0f);
	}

	mCurrentAnimation = mSpriteAnimations[action];
	mCurrentAnimation->SetFrame(0.0f);
	mCurrentAnimation->SetState(ion::render::Animation::Playing);
	mCurrentAction = action;
}

void Character::DealDamage(Character& character, int damage) const
{
}

void Character::Stats::Read(const ion::io::XML& xmlNode)
{
	xmlNode.GetAttribute("sizeX", size.x);
	xmlNode.GetAttribute("sizeY", size.y);
	xmlNode.GetAttribute("maxHealth", maxHealth);
	xmlNode.GetAttribute("kickStrength", kickStrength);
	xmlNode.GetAttribute("punchStrength", punchStrength);
	xmlNode.GetAttribute("specialStrength", specialStrength);
	xmlNode.GetAttribute("jumpHeight", jumpHeight);
	xmlNode.GetAttribute("lungeDistance", lungeDistance);
	xmlNode.GetAttribute("stepBackDistance", stepBackDistance);
}