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

#include <sstream>

typedef ion::UUID64 SpriteAnimId;
static const SpriteAnimId InvalidSpriteAnimId = 0;

//Keyframes
typedef ion::render::Keyframe<u32> AnimKeyframeSpriteFrame;
typedef ion::render::Keyframe<ion::Vector2i> AnimKeyframePosition;

//Tracks
class AnimTrackSpriteFrame : public ion::render::AnimationTrack<u32>
{
public:
	const u32 AnimTrackSpriteFrame::GetValue(float time) const
	{
		u32 intValue = 0;

		if(const ion::render::Keyframe<u32>* keyframeA = GetPrevKeyframe(time))
		{
			intValue = keyframeA->GetValue();
		}

		return intValue;
	}

	void Export(std::stringstream& stream) const
	{
		stream << "\tdc.b ";

		for(int i = 0; i < GetNumKeyframes(); i++)
		{
			u32 value = GetKeyframe(i).GetValue();
			stream << "0x" << value;

			if(i < GetNumKeyframes() - 1)
				stream << ", ";
		}

		stream << std::endl;
	}

	void Export(ion::io::File& file) const
	{

	}
};

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

	void Export(std::stringstream& stream) const
	{

	}

	void Export(ion::io::File& file) const
	{

	}
};

class SpriteAnimation : public ion::render::Animation
{
public:
	SpriteAnimation();

	void SetName(const std::string& name);
	const std::string& GetName() const;

	void SetSpeed(int speed);
	int GetSpeed() const;

	void Serialise(ion::io::Archive& archive);

	AnimTrackSpriteFrame m_trackSpriteFrame;
	AnimTrackPosition m_trackPosition;

private:
	std::string m_name;
	int m_speed;
};