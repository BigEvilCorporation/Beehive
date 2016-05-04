///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <ion/core/Types.h>
#include <ion/maths/Vector.h>
#include <ion/core/cryptography/UUID.h>
#include <ion/renderer/Animation.h>

typedef ion::UUID64 SpriteAnimId;

//Keyframes
typedef ion::render::Keyframe<int> AnimKeyframeSpriteFrame;
typedef ion::render::Keyframe<ion::Vector2i> AnimKeyframePosition;

//Tracks
typedef ion::render::AnimationTrackInt AnimTrackSpriteFrame;

class AnimTrackPosition : public ion::render::AnimationTrack<ion::Vector2i>
{
public:
	virtual const ion::Vector2i GetValue(float time) const
	{
		ion::Vector2i result;

		const ion::render::Keyframe<ion::Vector2i>* keyframeA = GetPrevKeyframe(time);
		const ion::render::Keyframe<ion::Vector2i>* keyframeB = GetNextKeyframe(time);

		if(keyframeA && keyframeB)
		{
			float timeA = keyframeA->GetTime();
			float timeB = keyframeB->GetTime();

			float lerpTime = ion::maths::UnLerp(timeA, timeB, time);

			const ion::Vector2i& vectorA = keyframeA->GetValue();
			const ion::Vector2i& vectorB = keyframeB->GetValue();

			result = vectorA.Lerp(vectorB, lerpTime);
		}

		return result;
	}
};

class SpriteAnimation : public ion::render::Animation
{
public:
	SpriteAnimation();

	void SetName(const std::string& name);
	const std::string& GetName() const;

	AnimTrackSpriteFrame m_trackSpriteFrame;
	AnimTrackPosition m_trackPosition;

private:
	std::string m_name;
};