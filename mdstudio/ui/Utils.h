#pragma once

#include <ion/beehive/Types.h>
#include <ion/beehive/Project.h>
#include <ion/beehive/GameObject.h>

void FindGameObjectSprite(
	Project& project,
	const GameObjectType& gameObjectType,
	const GameObject* gameObject,
	const GameObjectType::PrefabChild* prefabChild,
	ActorId*& spriteActorId,
	SpriteSheetId*& spriteSheetId,
	SpriteAnimId*& spriteAnimId,
	Actor*& actor,
	SpriteSheet*& spriteSheet,
	SpriteAnimation*& spriteAnim);