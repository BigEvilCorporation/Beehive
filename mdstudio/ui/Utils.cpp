#include "Utils.h"

void FindGameObjectSprite(
	Project& project,
	const GameObjectType& gameObjectType,
	const GameObject* gameObject,
	const GameObjectType::PrefabChild* prefabChild,
	ActorId& spriteActorId,
	SpriteSheetId& spriteSheetId,
	SpriteAnimId& spriteAnimId,
	Actor*& spriteActor,
	SpriteSheet*& spriteSheet,
	SpriteAnimation*& spriteAnim)
{
	//Get sprite actor from prefab
	if (prefabChild)
	{
		spriteActorId = prefabChild->spriteActorId;
		spriteActor = project.GetActor(spriteActorId);
	}

	if (!spriteActor)
	{
		//Find sprite actor from game object
		spriteActorId = gameObject ? gameObject->GetSpriteActorId() : InvalidActorId;
		spriteActor = project.GetActor(spriteActorId);
	}

	if (!spriteActor)
	{
		//Find sprite actor from game object type
		spriteActorId = gameObjectType.GetSpriteActorId();
		spriteActor = project.GetActor(spriteActorId);
	}

	if (spriteActor)
	{
		if (prefabChild)
		{
			//Get sprite sheet from prefab
			spriteSheetId = prefabChild->spriteSheetId;
			spriteSheet = spriteActor->GetSpriteSheet(spriteSheetId);
		}

		if (!spriteSheet)
		{
			//Find sprite sheet from editor preview
			spriteSheetId = gameObjectType.GetPreviewSpriteSheetId();
			spriteSheet = spriteActor->GetSpriteSheet(spriteSheetId);
		}

		if (!spriteSheet)
		{
			//Find sprite sheet from game object
			spriteSheetId = gameObject ? gameObject->GetSpriteSheetId() : InvalidSpriteSheetId;
			spriteSheet = spriteActor->GetSpriteSheet(spriteSheetId);
		}

		if (!spriteSheet)
		{
			//Find sprite sheet from game object type
			spriteSheetId = gameObjectType.GetSpriteSheetId();
			spriteSheet = spriteActor->GetSpriteSheet(spriteSheetId);
		}

		//Default to first sprite sheet
		if (!spriteSheet && spriteActor->GetSpriteSheetCount() > 0)
		{
			spriteSheet = &spriteActor->GetSpriteSheets().begin()->second;
			spriteSheetId = spriteActor->GetSpriteSheets().begin()->first;
		}
	}

	if (spriteSheet)
	{
		if (prefabChild)
		{
			//Get sprite anim from prefab
			spriteAnimId = prefabChild->spriteAnimId;
			spriteAnim = spriteSheet->GetAnimation(spriteAnimId);
		}

		if (!spriteAnim)
		{
			//Find sprite anim from game object
			spriteAnimId = gameObject ? gameObject->GetSpriteAnim() : InvalidSpriteAnimId;
			spriteAnim = spriteSheet->GetAnimation(spriteAnimId);
		}

		if (!spriteAnim)
		{
			//Find sprite anim from game object type
			spriteAnimId = gameObjectType.GetSpriteAnim();
			spriteAnim = spriteSheet->GetAnimation(spriteAnimId);
		}
	}
}