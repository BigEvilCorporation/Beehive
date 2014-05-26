#pragma once

#include <ion/gamekit/StateManager.h>
#include <ion/io/ResourceManager.h>
#include <ion/io/XML.h>
#include <ion/input/Keyboard.h>
#include <ion/renderer/Renderer.h>
#include <ion/renderer/Camera.h>
#include <ion/renderer/Sprite.h>
#include <ion/renderer/Animation.h>

class Character
{
public:
	enum Actions
	{
		Idle,
		LungeForward,
		StepBackward,
		Jump,
		Punch,
		Kick,
		Block,
		Special,

		NumActions
	};

	Character();
	~Character();

	void LoadResources(const std::string& characterName, ion::io::ResourceManager& resourceManager);

	void Update(float deltaTime, ion::input::Keyboard* keyboard, ion::input::Mouse* mouse, ion::input::Gamepad* gamepad);
	void Render(ion::render::Renderer& renderer, ion::render::Camera& camera);

	void TakeDamage(Character& character, int damage);

	int GetHealth() const;

protected:
	void ParseAnimation(const ion::io::XML& node);
	void PerformAction(Actions action);
	void DealDamage(Character& character, int damage) const;

	struct Stats
	{
		ion::Vector2 size;

		int maxHealth;
		int kickStrength;
		int punchStrength;
		int specialStrength;
		float jumpHeight;
		float lungeDistance;
		float stepBackDistance;

		void Read(const ion::io::XML& xmlNode);
	} mStats;

	struct Action
	{
		int spriteAnimationId;
		int moveAnimationId;
	};

	int mHealth;

	//Current position
	ion::Vector2 mPosition;

	//Target position for current action
	ion::Vector2 mTargetPosition;

	//Position to return to after current action
	ion::Vector2 mOriginPosition;

	//Speed towards target position
	float mCurrentLerpSpeed;

	ion::render::Sprite* mSpriteSheet;
	std::vector<ion::render::SpriteAnimation*> mSpriteAnimations;
	std::vector<ion::render::Animation*> mMoveAnimations;
	ion::render::SpriteAnimation* mCurrentAnimation;
};