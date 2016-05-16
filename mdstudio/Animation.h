///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
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
	AnimationActor(GameObjectId gameObjectId);

	GameObjectId GetGameObjectId() const;

	AnimTrackPosition m_trackPosition;

private:
	GameObjectId m_gameObjectId;
};

class Animation : public ion::render::Animation
{
public:
	void SetName(const std::string& name);
	const std::string& GetName() const;

	void SetSpeed(int speed);
	int GetSpeed() const;

	void Serialise(ion::io::Archive& archive);

private:
	std::string m_name;
	int m_speed;
};
