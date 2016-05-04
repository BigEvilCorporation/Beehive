///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include "Tile.h"
#include "Palette.h"
#include "SpriteAnimation.h"

#include <ion/core/cryptography/UUID.h>

typedef ion::UUID64 SpriteId;
typedef std::vector<Tile> SpriteFrame;
typedef std::map<SpriteAnimId, SpriteAnimation> TSpriteAnimMap;

static const SpriteId InvalidSpriteId = 0;

class Sprite
{
public:
	Sprite();

	bool ImportBitmap(const std::string& filename, const std::string& name, int widthCells, int heightCells, int maxCells);

	const std::string& GetName() const;
	const SpriteFrame& GetFrame(int index) const;
	int GetNumFrames() const;

	SpriteAnimId AddAnimation();
	SpriteAnimation* GetAnimation(SpriteAnimId animId);
	int GetNumAnimations() const;
	TSpriteAnimMap::const_iterator AnimationsBegin() const;
	TSpriteAnimMap::const_iterator AnimationsEnd() const;

	u8 GetWidthTiles() const;
	u8 GetHeightTiles() const;
	const Palette& GetPalette() const;

	//Serialise
	void Serialise(ion::io::Archive& archive);

private:
	std::string m_name;
	std::vector<SpriteFrame> m_frames;
	TSpriteAnimMap m_animations;
	Palette m_palette;
	u8 m_widthTiles;
	u8 m_heightTiles;
};