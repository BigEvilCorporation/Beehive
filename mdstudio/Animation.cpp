///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "Animation.h"

void Animation::Serialise(ion::io::Archive& archive)
{

}

const ion::Vector2i AnimTrackPosition::GetValue(float time) const
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

void AnimTrackPosition::Export(std::stringstream& stream) const
{

}

void AnimTrackPosition::Export(ion::io::File& file) const
{

}