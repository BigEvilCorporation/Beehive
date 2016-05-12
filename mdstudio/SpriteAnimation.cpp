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
