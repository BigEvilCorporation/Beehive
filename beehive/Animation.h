///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#pragma once

#include "Types.h"

#include <ion/core/Types.h>
#include <ion/maths/Vector.h>
#include <ion/renderer/Animation.h>

//Keyframes
typedef ion::render::Keyframe<ion::Vector2i> AnimKeyframePosition;

enum AnimationTracks
{
	eTrackPosition,

	eTrackCount
};

class AnimTrackPosition : public ion::render::AnimationTrack<ion::Vector2i>
{
public:
	virtual const ion::Vector2i GetValue(float time) const;
	void Export(std::stringstream& stream) const;
	void Export(ion::io::File& file) const;
};

class AnimationActor
{
public:
	AnimationActor();
	AnimationActor(GameObjectId gameObjectId);

	GameObjectId GetGameObjectId() const;

	void Serialise(ion::io::Archive& archive);

	AnimTrackPosition m_trackPosition;

private:
	GameObjectId m_gameObjectId;
};

typedef std::map<GameObjectId, AnimationActor> TAnimActorMap;

class Animation : public ion::render::Animation
{
public:
	Animation();

	void SetName(const std::string& name);
	const std::string& GetName() const;

	void AddActor(GameObjectId actorId);
	void RemoveActor(GameObjectId actorId);
	AnimationActor* GetActor(GameObjectId actorId);
	const AnimationActor* GetActor(GameObjectId actorId) const;
	TAnimActorMap::iterator ActorsBegin();
	TAnimActorMap::iterator ActorsEnd();
	int GetActorCount() const;

	void Serialise(ion::io::Archive& archive);

private:
	std::string m_name;
	TAnimActorMap m_actors;
};
