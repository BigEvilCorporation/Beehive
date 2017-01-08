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

#include <ion/core/Types.h>
#include <ion/maths/Vector.h>
#include <ion/core/cryptography/UUID.h>
#include <ion/renderer/Animation.h>

#include <sstream>

#include "Types.h"
#include "Animation.h"

//Keyframes
typedef ion::render::Keyframe<u32> AnimKeyframeSpriteFrame;
typedef ion::render::Keyframe<ion::Vector2i> AnimKeyframeSpritePosition;

//Tracks
class AnimTrackSpriteFrame : public ion::render::AnimationTrack<u32>
{
public:
	const u32 AnimTrackSpriteFrame::GetValue(float time) const;
	void Export(std::stringstream& stream) const;
	void Export(ion::io::File& file) const;
};

class AnimTrackSpritePosition : public ion::render::AnimationTrack<ion::Vector2i>
{
public:
	AnimTrackSpritePosition();
	virtual const ion::Vector2i GetValue(float time) const;
	void ExportX(std::stringstream& stream) const;
	void ExportY(std::stringstream& stream) const;
	void ExportX(ion::io::File& file) const;
	void ExportY(ion::io::File& file) const;
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
	AnimTrackSpritePosition m_trackPosition;

private:
	std::string m_name;
	int m_speed;
};