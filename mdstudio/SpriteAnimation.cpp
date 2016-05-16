///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation()
{
	m_speed = 1;
}

void SpriteAnimation::SetName(const std::string& name)
{
	m_name = name;
}

const std::string& SpriteAnimation::GetName() const
{
	return m_name;
}

void SpriteAnimation::SetSpeed(int speed)
{
	m_speed = speed;
}

int SpriteAnimation::GetSpeed() const
{
	return m_speed;
}

void SpriteAnimation::Serialise(ion::io::Archive& archive)
{
	ion::render::Animation::Serialise(archive);

	archive.Serialise(m_name, "name");
	archive.Serialise(m_speed, "speed");
	archive.Serialise(m_trackSpriteFrame, "trackSpriteFrame");
	archive.Serialise(m_trackPosition, "trackPosition");
}

const u32 AnimTrackSpriteFrame::GetValue(float time) const
{
	u32 intValue = 0;

	if(const ion::render::Keyframe<u32>* keyframeA = GetPrevKeyframe(time))
	{
		intValue = keyframeA->GetValue();
	}

	return intValue;
}

void AnimTrackSpriteFrame::Export(std::stringstream& stream) const
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

void AnimTrackSpriteFrame::Export(ion::io::File& file) const
{

}

const ion::Vector2i AnimTrackSpritePosition::GetValue(float time) const
{
	ion::Vector2i result;

	const AnimKeyframePosition* keyframeA = GetPrevKeyframe(time);
	const AnimKeyframePosition* keyframeB = GetNextKeyframe(time);

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

void AnimTrackSpritePosition::Export(std::stringstream& stream) const
{

}

void AnimTrackSpritePosition::Export(ion::io::File& file) const
{

}