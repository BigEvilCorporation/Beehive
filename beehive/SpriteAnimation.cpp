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

AnimTrackSpritePosition::AnimTrackSpritePosition()
{
	//Linear mode by default
	SetBlendMode(eLinear);
}

const ion::Vector2i AnimTrackSpritePosition::GetValue(float time) const
{
	ion::Vector2i result;

	const AnimKeyframePosition* keyframeA = GetPrevKeyframe(time);
	const AnimKeyframePosition* keyframeB = GetNextKeyframe(time);

	if(keyframeA && keyframeB)
	{
		if(GetBlendMode() == eSnap)
		{
			result = keyframeA->GetValue();
		}
		else
		{
			float timeA = keyframeA->GetTime();
			float timeB = keyframeB->GetTime();

			float lerpTime = ion::maths::UnLerp(timeA, timeB, time);

			const ion::Vector2i& vectorA = keyframeA->GetValue();
			const ion::Vector2i& vectorB = keyframeB->GetValue();

			result = vectorA.Lerp(vectorB, lerpTime);
		}
	}

	return result;
}

void AnimTrackSpritePosition::ExportX(std::stringstream& stream) const
{
	if(GetNumKeyframes() > 0)
	{
		stream << "\tdc.b ";

		for(int i = 0; i < GetNumKeyframes(); i++)
		{
			u32 value = GetKeyframe(i).GetValue().x;
			stream << "0x" << value;

			if(i < GetNumKeyframes() - 1)
				stream << ", ";
		}
	}

	stream << std::endl;
}

void AnimTrackSpritePosition::ExportY(std::stringstream& stream) const
{
	if(GetNumKeyframes() > 0)
	{
		stream << "\tdc.b ";

		for(int i = 0; i < GetNumKeyframes(); i++)
		{
			u32 value = GetKeyframe(i).GetValue().y;
			stream << "0x" << value;

			if(i < GetNumKeyframes() - 1)
				stream << ", ";
		}
	}

	stream << std::endl;
}

void AnimTrackSpritePosition::ExportX(ion::io::File& file) const
{

}

void AnimTrackSpritePosition::ExportY(ion::io::File& file) const
{

}
