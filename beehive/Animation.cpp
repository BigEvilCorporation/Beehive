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

#include "Animation.h"

Animation::Animation()
{

}

void Animation::SetName(const std::string& name)
{
	m_name = name;
}

const std::string& Animation::GetName() const
{
	return m_name;
}

void Animation::AddActor(GameObjectId actorId)
{
	m_actors.insert(std::make_pair(actorId, AnimationActor(actorId)));
}

void Animation::RemoveActor(GameObjectId actorId)
{
	std::map<GameObjectId, AnimationActor>::iterator it = m_actors.find(actorId);
	if(it != m_actors.end())
	{
		m_actors.erase(it);
	}
}

AnimationActor* Animation::GetActor(GameObjectId actorId)
{
	AnimationActor* actor = NULL;

	std::map<GameObjectId, AnimationActor>::iterator it = m_actors.find(actorId);
	if(it != m_actors.end())
	{
		actor = &it->second;
	}

	return actor;
}

const AnimationActor* Animation::GetActor(GameObjectId actorId) const
{
	const AnimationActor* actor = NULL;

	std::map<GameObjectId, AnimationActor>::const_iterator it = m_actors.find(actorId);
	if(it != m_actors.end())
	{
		actor = &it->second;
	}

	return actor;
}

TAnimActorMap::iterator Animation::ActorsBegin()
{
	return m_actors.begin();
}

TAnimActorMap::iterator Animation::ActorsEnd()
{
	return m_actors.end();
}

int Animation::GetActorCount() const
{
	return m_actors.size();
}

void Animation::Serialise(ion::io::Archive& archive)
{
	ion::render::Animation::Serialise(archive);

	archive.Serialise(m_name);
	archive.Serialise(m_actors);
}

AnimationActor::AnimationActor()
{

}

AnimationActor::AnimationActor(GameObjectId gameObjectId)
{
	m_gameObjectId = gameObjectId;
}

GameObjectId AnimationActor::GetGameObjectId() const
{
	return m_gameObjectId;
}

void AnimationActor::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_gameObjectId);
	archive.Serialise(m_trackPosition);
}

const ion::Vector2i AnimTrackPosition::GetValue(float time) const
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

void AnimTrackPosition::Export(std::stringstream& stream) const
{

}

void AnimTrackPosition::Export(ion::io::File& file) const
{

}