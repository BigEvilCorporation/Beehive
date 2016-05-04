///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation()
{
	
}

void SpriteAnimation::SetName(const std::string& name)
{
	m_name = name;
}

const std::string& SpriteAnimation::GetName() const
{
	return m_name;
}

void SpriteAnimation::Serialise(ion::io::Archive& archive)
{
	ion::render::Animation::Serialise(archive);

	archive.Serialise(m_name, "name");
	archive.Serialise(m_trackSpriteFrame, "trackSpriteFrame");
	archive.Serialise(m_trackPosition, "trackPosition");
}