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

#include "Types.h"
#include "Tile.h"

#include <ion/renderer/Animation.h>

class Project;

//Keyframes
typedef ion::render::Keyframe<u32> AnimKeyframeTileFrame;

//Tracks
class AnimTrackTileFrame : public ion::render::AnimationTrack<u32>
{
public:
	const u32 AnimTrackTileFrame::GetValue(float time) const;
	void Export(std::stringstream& stream) const;
	void Export(ion::io::File& file) const;
};

class TileFrame
{
public:
	TileFrame();
	TileFrame(const TileFrame& rhs);
	TileFrame(const std::vector<std::pair<TileId, Tile*>>& tiles, int width, int height);

	bool Import(const std::string filename, Project& project);
	void Serialise(ion::io::Archive& archive);

	std::vector<std::pair<TileId, Tile>> m_tiles;
	int m_width;
	int m_height;
};

class StampAnimation : public ion::render::Animation
{
public:
	StampAnimation();
	StampAnimation(const std::string& name);

	void SetName(const std::string& name);
	const std::string& GetName() const;

	void AddTileFrame(const TileFrame& frame);
	const TileFrame& GetTileFrame(int index);
	int GetTileFrameCount() const;

	void SetSpeed(int speed);
	int GetSpeed() const;

	void Serialise(ion::io::Archive& archive);

	AnimTrackTileFrame m_trackTileFrame;

private:
	std::string m_name;
	std::vector<TileFrame> m_tileFrames;
	int m_speed;
};