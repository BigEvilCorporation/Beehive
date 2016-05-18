///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <ion/core/cryptography/UUID.h>

typedef u32 GameObjectTypeId;
typedef u32 GameObjectId;
typedef ion::UUID64 ActorId;
typedef ion::UUID64 AnimationId;
typedef ion::UUID64 SpriteAnimId;

static const GameObjectTypeId InvalidGameObjectTypeId = 0;
static const GameObjectId InvalidGameObjectId = 0;
static const ActorId InvalidActorId = 0;
static const AnimationId InvalidAnimationId = 0;
static const SpriteAnimId InvalidSpriteAnimId = 0;